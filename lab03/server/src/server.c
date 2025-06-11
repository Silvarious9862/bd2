#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "server.h"
#include "db_utils.h"
#include "db_rows.h" // Подключаем новый модуль для выборки строк
#include "response.h"
#include "db_apply_changes.h"
#include <jansson.h>

// Обрабатываем JSON-запрос и отправляем ответ
void process_request(int client_fd, const char *request_json)
{
    json_error_t error;
    json_t *json_request = json_loads(request_json, 0, &error);
    if (!json_request)
    {
        fprintf(stderr, "Ошибка парсинга входящего JSON: %s\n", error.text);
        // Формируем JSON-ответ об ошибке
        json_t *error_obj = json_object();
        json_object_set_new(error_obj, "status", json_string("error"));
        json_object_set_new(error_obj, "message", json_string("Неверный JSON"));
        char *error_str = json_dumps(error_obj, JSON_COMPACT);
        send_response(client_fd, error_str);
        free(error_str);
        json_decref(error_obj);
        return;
    }

    json_t *action_item = json_object_get(json_request, "action");
    if (json_is_string(action_item))
    {
        const char *action = json_string_value(action_item);
        if (strcmp(action, "list_tables") == 0)
        {
            char *tables_json = get_tables_list();
            if (!tables_json)
            {
                json_t *error_obj = json_object();
                json_object_set_new(error_obj, "status", json_string("error"));
                json_object_set_new(error_obj, "message", json_string("Ошибка получения списка таблиц"));
                char *error_str = json_dumps(error_obj, JSON_COMPACT);
                send_response(client_fd, error_str);
                free(error_str);
                json_decref(error_obj);
            }
            else
            {
                json_t *response_obj = json_object();
                json_object_set_new(response_obj, "status", json_string("ok"));
                // Парсим строку с JSON-массивом, полученную из get_tables_list()
                json_error_t dummy_error;
                json_t *data_array = json_loads(tables_json, 0, &dummy_error);
                if (data_array)
                {
                    json_object_set_new(response_obj, "data", data_array);
                }
                else
                {
                    json_object_set_new(response_obj, "data", json_null());
                }
                json_object_set_new(response_obj, "message", json_string("Список таблиц получен"));

                char *response_str = json_dumps(response_obj, JSON_COMPACT);
                send_response(client_fd, response_str);

                free(response_str);
                json_decref(response_obj);
                free(tables_json);
            }
        }
        else if (strcmp(action, "get_table_rows") == 0)
        {
            // Новый запрос на получение строк из таблицы
            json_t *table_item = json_object_get(json_request, "table");
            if (!json_is_string(table_item))
            {
                json_t *error_obj = json_object();
                json_object_set_new(error_obj, "status", json_string("error"));
                json_object_set_new(error_obj, "message", json_string("Отсутствует или неверный параметр 'table'"));
                char *error_str = json_dumps(error_obj, JSON_COMPACT);
                send_response(client_fd, error_str);
                free(error_str);
                json_decref(error_obj);
                json_decref(json_request);
                return;
            }
            const char *table = json_string_value(table_item);
            int limit = 10; // Значение по умолчанию
            int offset = 0; // Значение по умолчанию

            json_t *limit_item = json_object_get(json_request, "limit");
            json_t *offset_item = json_object_get(json_request, "offset");
            if (json_is_integer(limit_item))
                limit = (int)json_integer_value(limit_item);
            if (json_is_integer(offset_item))
                offset = (int)json_integer_value(offset_item);

            char *rows_json = get_table_rows(table, limit, offset);
            if (!rows_json)
            {
                json_t *error_obj = json_object();
                json_object_set_new(error_obj, "status", json_string("error"));
                json_object_set_new(error_obj, "message", json_string("Ошибка получения записей таблицы"));
                char *error_str = json_dumps(error_obj, JSON_COMPACT);
                send_response(client_fd, error_str);
                free(error_str);
                json_decref(error_obj);
            }
            else
            {
                json_t *response_obj = json_object();
                json_object_set_new(response_obj, "status", json_string("ok"));
                // Преобразуем полученную строку с JSON-массивом в объект
                json_error_t dummy_error;
                json_t *data_array = json_loads(rows_json, 0, &dummy_error);
                if (data_array)
                    json_object_set_new(response_obj, "data", data_array);
                else
                    json_object_set_new(response_obj, "data", json_null());
                json_object_set_new(response_obj, "message", json_string("Записи получены"));

                char *response_str = json_dumps(response_obj, JSON_COMPACT);
                send_response(client_fd, response_str);

                free(response_str);
                json_decref(response_obj);
                free(rows_json);
            }
        }
        else if (strcmp(action, "get_lookup") == 0)
        {
            json_t *table_item = json_object_get(json_request, "table");
            if (!json_is_string(table_item))
            {
                json_t *error_obj = json_object();
                json_object_set_new(error_obj, "status", json_string("error"));
                json_object_set_new(error_obj, "message", json_string("Некорректный параметр 'table'"));
                char *error_str = json_dumps(error_obj, JSON_COMPACT);
                send_response(client_fd, error_str);
                free(error_str);
                json_decref(error_obj);
                json_decref(json_request);
                return;
            }

            const char *table = json_string_value(table_item);
            char *lookup_json = get_lookup_table(table);
            if (!lookup_json)
            {
                json_t *error_obj = json_object();
                json_object_set_new(error_obj, "status", json_string("error"));
                json_object_set_new(error_obj, "message", json_string("Ошибка получения данных"));
                char *error_str = json_dumps(error_obj, JSON_COMPACT);
                send_response(client_fd, error_str);
                free(error_str);
                json_decref(error_obj);
            }
            else
            {
                json_t *response_obj = json_object();
                json_object_set_new(response_obj, "status", json_string("ok"));

                json_error_t dummy_error;
                json_t *data_array = json_loads(lookup_json, 0, &dummy_error);
                if (data_array)
                    json_object_set_new(response_obj, "data", data_array);
                else
                    json_object_set_new(response_obj, "data", json_null());

                char *response_str = json_dumps(response_obj, JSON_COMPACT);
                send_response(client_fd, response_str);

                free(response_str);
                json_decref(response_obj);
                free(lookup_json);
            }
        }
        else if (strcmp(action, "apply_changes") == 0)
        {
            json_t *edited_array = json_object_get(json_request, "edited");
            json_t *added_array = json_object_get(json_request, "added");
            json_t *deleted_array = json_object_get(json_request, "deleted");

            char *pg_err = NULL;
            int rc = apply_changes_transaction(edited_array, added_array, deleted_array, &pg_err);

            json_t *response_obj = json_object();
            if (rc == 0)
            {
                json_object_set_new(response_obj, "status", json_string("ok"));
            }
            else
            {
                json_object_set_new(response_obj, "status", json_string("error"));
                // Формируем сообщение, комбинируя фиксированный текст и ошибку PostgreSQL.
                char message[512];
                snprintf(message, sizeof(message), "Ошибка применения изменений транзакцией. %s", pg_err ? pg_err : "");
                json_object_set_new(response_obj, "message", json_string(message));
                if (pg_err)
                    free(pg_err);
            }
            char *response_str = json_dumps(response_obj, JSON_COMPACT);
            send_response(client_fd, response_str);
            free(response_str);
            json_decref(response_obj);
        }

        else
        {
            json_t *error_obj = json_object();
            json_object_set_new(error_obj, "status", json_string("error"));
            json_object_set_new(error_obj, "message", json_string("Неизвестное действие"));
            char *error_str = json_dumps(error_obj, JSON_COMPACT);
            send_response(client_fd, error_str);
            free(error_str);
            json_decref(error_obj);
        }
    }
    else
    {
        json_t *error_obj = json_object();
        json_object_set_new(error_obj, "status", json_string("error"));
        json_object_set_new(error_obj, "message", json_string("Отсутствует параметр 'action'"));
        char *error_str = json_dumps(error_obj, JSON_COMPACT);
        send_response(client_fd, error_str);
        free(error_str);
        json_decref(error_obj);
    }
    json_decref(json_request);
}

void *client_handler(void *arg)
{
    client_info *info = (client_info *)arg;
    int client_fd = info->client_fd;
    char buffer[MAX_BUFFER_SIZE];
    ssize_t received;

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(info->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("Клиент подключился: %s:%d\n", client_ip, ntohs(info->client_addr.sin_port));

    received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (received < 0)
    {
        perror("Ошибка при чтении данных от клиента");
        close(client_fd);
        free(info);
        return NULL;
    }
    buffer[received] = '\0';
    printf("Получен запрос: %s\n", buffer);

    // Обработка запроса: если это JSON, запускаем обработку
    process_request(client_fd, buffer);

    close(client_fd);
    printf("Клиент отключился: %s:%d\n", client_ip, ntohs(info->client_addr.sin_port));
    free(info);
    return NULL;
}

int server_start(int port)
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Ошибка при создании сокета");
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Ошибка при установке опций сокета");
        close(server_fd);
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Ошибка привязки сокета");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Ошибка прослушивания");
        close(server_fd);
        return -1;
    }
    printf("Сервер запущен и слушает порт %d...\n", port);

    while (1)
    {
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (client_fd < 0)
        {
            perror("Ошибка при принятии соединения");
            continue;
        }
        client_info *info = malloc(sizeof(client_info));
        if (!info)
        {
            perror("Ошибка при выделении памяти");
            close(client_fd);
            continue;
        }
        info->client_fd = client_fd;
        info->client_addr = client_addr;

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, (void *)info) != 0)
        {
            perror("Ошибка при создании потока");
            close(client_fd);
            free(info);
            continue;
        }
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}

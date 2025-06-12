#include "db_rows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <jansson.h>

char* get_table_rows(const char* table, int limit, int offset) {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DBC *cursorp = NULL;
    DBT key, data;
    char prefix[256];
    size_t prefix_len;

    // Формируем префикс для ключей заданной таблицы, например "transport:"
    snprintf(prefix, sizeof(prefix), "%s:", table);
    prefix_len = strlen(prefix);

    // Создаем и открываем окружение BerkeleyDB
    if ((ret = db_env_create(&env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания DB_ENV: %s\n", db_strerror(ret));
        return NULL;
    }
    if ((ret = env->open(env, "/home/silvarious/bd2/lab02/db_env", DB_CREATE | DB_INIT_MPOOL | DB_INIT_TXN, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия окружения BerkeleyDB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }


    // Открываем базу данных, в которой хранятся записи, например, transfer_db.db
    if ((ret = db_create(&dbp, env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания объекта DB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }
    if ((ret = dbp->open(dbp, NULL, "transfer_db.db", NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия базы данных transfer_db.db: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return NULL;
    }

    // Получаем курсор для итерации по записям
    if ((ret = dbp->cursor(dbp, NULL, &cursorp, 0)) != 0) {
        fprintf(stderr, "Ошибка получения курсора: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return NULL;
    }

    // Подготавливаем структуры DBT для ключа и данных
    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    key.flags = DB_DBT_MALLOC;
    data.flags = DB_DBT_MALLOC;

    // Устанавливаем курсор на первую запись с нужным префиксом
    key.data = prefix;
    key.size = prefix_len + 1;

    ret = cursorp->get(cursorp, &key, &data, DB_SET_RANGE);

    json_t *json_arr = json_array();
    int current = 0;  // Счётчик найденных записей с нужным префиксом

    while (ret == 0) {
        // Если ключ не начинается с заданного префикса, нужные записи закончились
        if (key.size < prefix_len || strncmp((char *)key.data, prefix, prefix_len) != 0)
            break;

        // Если счётчик достиг смещения, начинаем собирать записи
        if (current >= offset) {
            // data.data содержит строку вида:
            // "id=90|car_model=Bentley|insurance=2024-07-13|maintenance=2024-12-01|vin=LRXLV450KPW773092|company_id=6"
            char *data_str = strndup((char *)data.data, data.size);
            if (!data_str) {
                fprintf(stderr, "Ошибка выделения памяти для данных\n");
                break;
            }
            json_t *json_obj = json_object();

            // Разбиваем строку по разделителю "|"
            char *saveptr;
            char *token = strtok_r(data_str, "|", &saveptr);
            while (token) {
                // Разделяем каждую пару по символу '='
                char *equal = strchr(token, '=');
                if (equal) {
                    *equal = '\0';
                    char *field = token;
                    char *value = equal + 1;
                    json_object_set_new(json_obj, field, json_string(value));
                }
                token = strtok_r(NULL, "|", &saveptr);
            }
            free(data_str);

            json_array_append_new(json_arr, json_obj);
            // Если лимит достигнут (если лимит > 0), завершаем сбор
            if (limit > 0 && json_array_size(json_arr) >= (size_t)limit) {
                free(key.data);
                free(data.data);
                break;
            }
        }
        current++;

        free(key.data);
        free(data.data);

        ret = cursorp->get(cursorp, &key, &data, DB_NEXT);
    }

    cursorp->close(cursorp);
    dbp->close(dbp, 0);
    env->close(env, 0);

    // Преобразуем JSON-массив в строку в компактном формате
    char *json_str = json_dumps(json_arr, JSON_COMPACT);
    json_decref(json_arr);
    return json_str;  // Вызывающему необходимо освободить json_str через free()
}

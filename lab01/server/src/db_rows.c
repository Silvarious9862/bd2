#include "db_rows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <jansson.h>

char* get_table_rows(const char* table, int limit, int offset) {
    // Параметры подключения – настройте под свои нужды
    const char *conninfo = "host=localhost port=5432 dbname=cargo user=postgres password=12345";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Ошибка подключения к базе: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    // Составляем SQL-запрос.
    // Обратите внимание: имя таблицы рекомендуется валидировать, чтобы избежать SQL-инъекций.
    char query[1024];
    snprintf(query, sizeof(query), "SELECT * FROM \"%s\" LIMIT %d OFFSET %d;", table, limit, offset);
    
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Ошибка выполнения запроса: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }

    // Создаём JSON-массив для хранения строк
    json_t *json_arr = json_array();
    int num_rows = PQntuples(res);
    int num_cols = PQnfields(res);

    for (int i = 0; i < num_rows; i++) {
        json_t *json_row = json_object();
        for (int j = 0; j < num_cols; j++) {
            const char *col_name = PQfname(res, j);
            // Проверяем, является ли поле NULL
            if (PQgetisnull(res, i, j)) {
                json_object_set_new(json_row, col_name, json_null());
            } else {
                const char *value = PQgetvalue(res, i, j);
                json_object_set_new(json_row, col_name, json_string(value));
            }
        }
        json_array_append_new(json_arr, json_row);
    }

    // Преобразуем JSON-массив в компактную строку
    char *json_str = json_dumps(json_arr, JSON_COMPACT);

    // Освобождаем ресурсы
    json_decref(json_arr);
    PQclear(res);
    PQfinish(conn);

    return json_str;
}

#include "db_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <jansson.h>  // Подключаем Jansson

char* get_tables_list() {
    // Настроить параметры подключения к PostgreSQL
    const char *conninfo = "host=localhost port=5432 dbname=cargo user=postgres password=12345";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Ошибка подключения к базе: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    // Запрос получения списка таблиц из схемы public
    PGresult *res = PQexec(conn, "SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname = 'public';");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Ошибка выполнения запроса: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }

    // Создаем JSON-массив с помощью Jansson
    json_t *json_arr = json_array();
    int nRows = PQntuples(res);
    for (int i = 0; i < nRows; i++) {
        char *tablename = PQgetvalue(res, i, 0);
        json_array_append_new(json_arr, json_string(tablename));
    }
    
    // Преобразуем JSON-массив в строку (компактный формат)
    char *json_str = json_dumps(json_arr, JSON_COMPACT);
    json_decref(json_arr);  // Освобождаем JSON-объект

    PQclear(res);
    PQfinish(conn);

    return json_str;  // Вызывающий должен вызвать free() для json_str
}

char* get_lookup_table(const char* table) {
    const char *conninfo = "host=localhost port=5432 dbname=cargo user=postgres password=12345";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Ошибка подключения к базе: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    char query[512];
    snprintf(query, sizeof(query),
             "SELECT id, company_name FROM \"%s\" ORDER BY id;", table);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Ошибка выполнения запроса: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return NULL;
    }

    json_t *json_arr = json_array();
    int num_rows = PQntuples(res);

    for (int i = 0; i < num_rows; i++) {
        json_t *json_obj = json_object();
        json_object_set_new(json_obj, "carrier_id", json_string(PQgetvalue(res, i, 0)));
        json_object_set_new(json_obj, "carrier_name", json_string(PQgetvalue(res, i, 1)));
        json_array_append_new(json_arr, json_obj);
    }

    char *json_str = json_dumps(json_arr, JSON_COMPACT);
    json_decref(json_arr);
    PQclear(res);
    PQfinish(conn);

    return json_str;
}

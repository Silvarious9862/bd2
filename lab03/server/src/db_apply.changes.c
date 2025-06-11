// В начале файла определяем POSIX-стандарт,
// чтобы получить объявление strdup.
#define _POSIX_C_SOURCE 200809L

#include "db_apply_changes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#define TARGET_TABLE "employee"  // Используем новую таблицу

int apply_changes_transaction(json_t *edited_array, json_t *added_array, json_t *deleted_array, char **err_msg) {
    const char *conninfo = "host=localhost port=5432 dbname=cargo user=postgres password=12345";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        *err_msg = strdup(PQerrorMessage(conn));
        PQfinish(conn);
        return -1;
    }

    PGresult *res = PQexec(conn, "BEGIN;");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        *err_msg = strdup(PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return -1;
    }
    PQclear(res);

    /* 1. Редактирование существующих записей */
    if (json_is_array(edited_array)) {
        int num_edits = (int)json_array_size(edited_array);
        for (int i = 0; i < num_edits; i++) {
            json_t *record = json_array_get(edited_array, i);
            if (!json_is_object(record)) continue;

            const char *id         = json_string_value(json_object_get(record, "id"));
            const char *badge      = json_string_value(json_object_get(record, "badge"));
            const char *lastname   = json_string_value(json_object_get(record, "lastname"));
            const char *firstname  = json_string_value(json_object_get(record, "firstname"));
            const char *experience = json_string_value(json_object_get(record, "experience"));
            const char *carrier_id = json_string_value(json_object_get(record, "carrier_id"));

            char query[1024];
            snprintf(query, sizeof(query),
                     "UPDATE %s SET badge='%s', lastname='%s', firstname='%s', experience='%s', carrier_id='%s' WHERE id='%s';",
                     TARGET_TABLE,
                     badge ? badge : "",
                     lastname ? lastname : "",
                     firstname ? firstname : "",
                     experience ? experience : "",
                     carrier_id ? carrier_id : "",
                     id ? id : "");

            res = PQexec(conn, query);
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                *err_msg = strdup(PQerrorMessage(conn));
                PQclear(res);
                PQexec(conn, "ROLLBACK;");
                PQfinish(conn);
                return -1;
            }
            PQclear(res);
        }
    }

    /* 2. Добавление новых записей */
    if (json_is_array(added_array)) {
        int num_added = (int)json_array_size(added_array);
        for (int i = 0; i < num_added; i++) {
            json_t *record = json_array_get(added_array, i);
            if (!json_is_object(record)) continue;

            const char *id         = json_string_value(json_object_get(record, "id"));
            const char *badge      = json_string_value(json_object_get(record, "badge"));
            const char *lastname   = json_string_value(json_object_get(record, "lastname"));
            const char *firstname  = json_string_value(json_object_get(record, "firstname"));
            const char *experience = json_string_value(json_object_get(record, "experience"));
            const char *carrier_id = json_string_value(json_object_get(record, "carrier_id"));

            char query[1024];
            snprintf(query, sizeof(query),
                     "INSERT INTO %s (id, badge, lastname, firstname, experience, carrier_id) "
                     "VALUES ('%s', '%s', '%s', '%s', '%s', '%s');",
                     TARGET_TABLE,
                     id ? id : "",
                     badge ? badge : "",
                     lastname ? lastname : "",
                     firstname ? firstname : "",
                     experience ? experience : "",
                     carrier_id ? carrier_id : "");

            res = PQexec(conn, query);
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                *err_msg = strdup(PQerrorMessage(conn));
                PQclear(res);
                PQexec(conn, "ROLLBACK;");
                PQfinish(conn);
                return -1;
            }
            PQclear(res);
        }
    }

    /* 3. Удаление записей */
    if (json_is_array(deleted_array)) {
        int num_deleted = (int)json_array_size(deleted_array);
        for (int i = 0; i < num_deleted; i++) {
            json_t *id_item = json_array_get(deleted_array, i);
            if (!json_is_integer(id_item) && !json_is_string(id_item)) {
                continue;
            }
            const char *id = NULL;
            char id_buf[32];
            if (json_is_integer(id_item)) {
                snprintf(id_buf, sizeof(id_buf), "%lld", json_integer_value(id_item));
                id = id_buf;
            } else {
                id = json_string_value(id_item);
            }

            char query[256];
            snprintf(query, sizeof(query),
                     "DELETE FROM %s WHERE id='%s';",
                     TARGET_TABLE,
                     id ? id : "");

            res = PQexec(conn, query);
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                *err_msg = strdup(PQerrorMessage(conn));
                PQclear(res);
                PQexec(conn, "ROLLBACK;");
                PQfinish(conn);  // Исправлено: раньше было PQFinish
                return -1;
            }
            PQclear(res);
        }
    }

    res = PQexec(conn, "COMMIT;");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        *err_msg = strdup(PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        PQfinish(conn);
        return -1;
    }
    PQclear(res);
    PQfinish(conn);
    return 0;
}

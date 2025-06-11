#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pg_migration.h"

PGconn *pg_connect(const char *conninfo) {
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Ошибка подключения к PostgreSQL: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }
    return conn;
}

PGTables pg_get_tables(PGconn *conn) {
    PGTables pgTables = { NULL, 0 };
    const char *sql =
        "SELECT table_name FROM information_schema.tables "
        "WHERE table_schema = 'public' AND table_type = 'BASE TABLE';";
    PGresult *res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Ошибка получения списка таблиц: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return pgTables;
    }

    int ntuples = PQntuples(res);
    pgTables.count = ntuples;
    pgTables.table_names = (char **)malloc(sizeof(char *) * ntuples);
    for (int i = 0; i < ntuples; i++) {
        char *tbl = PQgetvalue(res, i, 0);
        pgTables.table_names[i] = strdup(tbl);
    }

    PQclear(res);
    return pgTables;
}

PGresult *pg_get_table_data(PGconn *conn, const char *table_name) {
    char query[256];
    /* Простая сборка запроса – убедитесь, что table_name не содержит нежелательных символов */
    snprintf(query, sizeof(query), "SELECT * FROM \"%s\";", table_name);
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Ошибка выборки данных для таблицы %s: %s\n", table_name,
                PQerrorMessage(conn));
        PQclear(res);
        return NULL;
    }
    return res;
}

void pg_free_tables(PGTables *pgTables) {
    if (pgTables->table_names) {
        for (int i = 0; i < pgTables->count; i++) {
            free(pgTables->table_names[i]);
        }
        free(pgTables->table_names);
        pgTables->table_names = NULL;
        pgTables->count = 0;
    }
}

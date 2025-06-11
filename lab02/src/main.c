#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <libpq-fe.h>
#include "pg_migration.h"
#include "db_migration.h"

#define PG_CONNINFO "host=localhost dbname=cargo user=postgres password=12345"
#define DB_ENV_DIR "./db_env"
#define DB_FILENAME "transfer_db.db"

#define KEY_BUF_SIZE 512
#define DATA_BUF_SIZE 4096

/* Функция для сериализации данных строки из PGresult.
   Для упрощения мы выводим данные в формате:
   "col1=value1|col2=value2|..." */
void serialize_row(PGresult *res, int row, char *buffer, size_t buf_size) {
    int nfields = PQnfields(res);
    buffer[0] = '\0';
    for (int i = 0; i < nfields; i++) {
        const char *fieldname = PQfname(res, i);
        const char *value = PQgetvalue(res, row, i);
        strncat(buffer, fieldname, buf_size - strlen(buffer) - 1);
        strncat(buffer, "=", buf_size - strlen(buffer) - 1);
        strncat(buffer, value, buf_size - strlen(buffer) - 1);
        if (i < nfields - 1) {
            strncat(buffer, "|", buf_size - strlen(buffer) - 1);
        }
    }
}

int main(void) {
    int ret;

    /* Подключаемся к PostgreSQL */
    PGconn *pg_conn = pg_connect(PG_CONNINFO);
    if (!pg_conn) {
        exit(EXIT_FAILURE);
    }

    /* Получаем список таблиц */
    PGTables pgTables = pg_get_tables(pg_conn);
    if (pgTables.count == 0) {
        fprintf(stderr, "Нет таблиц для переноса.\n");
        PQfinish(pg_conn);
        exit(EXIT_FAILURE);
    }
    printf("Найдено %d таблиц для переноса.\n", pgTables.count);

    /* Инициализируем Berkeley DB */
    DBHandler dbHandler;
    ret = db_init(&dbHandler, DB_ENV_DIR, DB_FILENAME);
    if (ret != 0) {
        pg_free_tables(&pgTables);
        PQfinish(pg_conn);
        exit(EXIT_FAILURE);
    }

    /* Начинаем транзакцию в Berkeley DB */
    DB_TXN *txn = NULL;
    ret = dbHandler.env->txn_begin(dbHandler.env, NULL, &txn, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка начала транзакции: %s\n", db_strerror(ret));
        db_close(&dbHandler);
        pg_free_tables(&pgTables);
        PQfinish(pg_conn);
        exit(EXIT_FAILURE);
    }

    /* Перебираем таблицы */
    for (int i = 0; i < pgTables.count; i++) {
        const char *table_name = pgTables.table_names[i];
        printf("Перенос данных из таблицы %s...\n", table_name);

        PGresult *table_res = pg_get_table_data(pg_conn, table_name);
        if (!table_res) {
            txn->abort(txn);
            db_close(&dbHandler);
            pg_free_tables(&pgTables);
            PQfinish(pg_conn);
            exit(EXIT_FAILURE);
        }
        int nrows = PQntuples(table_res);
        int nfields = PQnfields(table_res);

        /* Для каждой строки создаем ключ вида "table_name:row_index" и сериализуем данные */
        for (int row = 0; row < nrows; row++) {
            char key[KEY_BUF_SIZE];
            char data[DATA_BUF_SIZE];
            snprintf(key, sizeof(key), "%s:%d", table_name, row);
            serialize_row(table_res, row, data, sizeof(data));

            ret = db_put_record(&dbHandler, txn, key, data);
            if (ret != 0) {
                txn->abort(txn);
                PQclear(table_res);
                db_close(&dbHandler);
                pg_free_tables(&pgTables);
                PQfinish(pg_conn);
                exit(EXIT_FAILURE);
            }
        }
        PQclear(table_res);
    }

    /* Фиксируем транзакцию */
    ret = txn->commit(txn, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка фиксации транзакции: %s\n", db_strerror(ret));
        db_close(&dbHandler);
        pg_free_tables(&pgTables);
        PQfinish(pg_conn);
        exit(EXIT_FAILURE);
    }

    printf("Весь перенос выполнен успешно!\n");

    /* Очистка ресурсов */
    db_close(&dbHandler);
    pg_free_tables(&pgTables);
    PQfinish(pg_conn);

    return EXIT_SUCCESS;
}

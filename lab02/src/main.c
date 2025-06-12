#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <libpq-fe.h>
#include "pg_migration.h"
#include "db_migration.h"

#define PG_CONNINFO "host=localhost dbname=cargo user=postgres password=12345"
#define KEY_BUF_SIZE 512
#define DATA_BUF_SIZE 4096

/**
 * Функция serialize_row_excluding_id:
 * Сериализует строку результата PostgreSQL (PGresult),
 * пропуская первый столбец, который содержит id.
 * Результат записывается в буфер buffer заданного размера.
 */
void serialize_row_excluding_id(PGresult *res, int row, char *buffer, size_t buf_size) {
    int nfields = PQnfields(res);
    buffer[0] = '\0';
    
    // Пропускаем первый столбец (id) и сериализуем остальные поля.
    for (int i = 1; i < nfields; i++) {
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
        fprintf(stderr, "Ошибка подключения к PostgreSQL.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Получаем список таблиц (уникальный список) */
    PGTables pgTables = pg_get_tables(pg_conn);
    if (pgTables.count == 0) {
        fprintf(stderr, "Нет таблиц для переноса.\n");
        PQfinish(pg_conn);
        exit(EXIT_FAILURE);
    }
    printf("Найдено %d таблиц для переноса.\n", pgTables.count);
    
    /* Инициализируем Berkeley DB */
    DBHandler dbHandler;
    ret = db_init(&dbHandler, "./db_env", "transfer_db.db");
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
    
    /* Перебираем список таблиц (каждая таблица обрабатывается один раз) */
    for (int t = 0; t < pgTables.count; t++) {
        const char *table_name = pgTables.table_names[t];
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
        /* Обрабатываем каждую строку таблицы */
        for (int row = 0; row < nrows; row++) {
            /* Берем значение id (предполагаем, что это первый столбец) */
            const char *id_value = PQgetvalue(table_res, row, 0);
            char key[KEY_BUF_SIZE];
            char data[DATA_BUF_SIZE];
            
            /* Формируем ключ как "tablename:id_value" */
            snprintf(key, sizeof(key), "%s:%s", table_name, id_value);
            
            /* Сериализуем данные для строки, исключая поле id */
            serialize_row_excluding_id(table_res, row, data, sizeof(data));
            
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
    
    /* Освобождаем ресурсы */
    db_close(&dbHandler);
    pg_free_tables(&pgTables);
    PQfinish(pg_conn);
    
    return EXIT_SUCCESS;
}

#ifndef PG_MIGRATION_H
#define PG_MIGRATION_H

#include <libpq-fe.h>

/* Структура для хранения списка таблиц из PostgreSQL */
typedef struct {
    char **table_names; /* динамический массив строк с именами таблиц */
    int count;
} PGTables;

/* Подключение к PostgreSQL.
   Возвращает указатель на соединение или NULL в случае ошибки. */
PGconn *pg_connect(const char *conninfo);

/* Получение списка всех таблиц в публичной схеме */
PGTables pg_get_tables(PGconn *conn);

/* Получение данных для конкретной таблицы.
   Функция формирует запрос вида "SELECT * FROM <table_name>".
   Возвращает PGresult*, который необходимо освободить вызывающему. */
PGresult *pg_get_table_data(PGconn *conn, const char *table_name);

/* Освобождение памяти, выделенной для списка таблиц */
void pg_free_tables(PGTables *pgTables);

#endif /* PG_MIGRATION_H */

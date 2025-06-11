#ifndef DB_MIGRATION_H
#define DB_MIGRATION_H

#include <db.h>

/* Структура-обёртка для Berkeley DB */
typedef struct {
    DB_ENV *env;
    DB *db;
} DBHandler;

/* Инициализация окружения и открытие новой базы данных.
   env_path – путь к каталогу для файлов окружения;
   db_filename – имя файла базы данных.
   Возвращает 0 при успехе, отличное значение в случае ошибки.
*/
int db_init(DBHandler *handler, const char *env_path, const char *db_filename);

/* Запись пары (ключ, значение) в контексте заданной транзакции.
   Ключ и data должны быть нуль-терминированными строками.
   Возвращает 0 при успехе, иначе код ошибки Berkeley DB. */
int db_put_record(DBHandler *handler, DB_TXN *txn, const char *key, const char *data);

/* Закрытие базы данных и окружения (освобождение ресурсов).
   Возвращает 0 при успехе, иначе код ошибки Berkeley DB. */
int db_close(DBHandler *handler);

#endif /* DB_MIGRATION_H */

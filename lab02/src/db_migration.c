#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "db_migration.h"

int db_init(DBHandler *handler, const char *env_path, const char *db_filename) {
    int ret;

    /* Создаем окружение */
    ret = db_env_create(&handler->env, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка db_env_create: %s\n", db_strerror(ret));
        return ret;
    }

    /* Создаем каталог для окружения, если он не существует */
    if (mkdir(env_path, 0755) && errno != EEXIST) {
        perror("Ошибка создания каталога для окружения");
        return errno;
    }

    ret = handler->env->open(handler->env,
                             env_path,
                             DB_CREATE | DB_INIT_TXN | DB_INIT_LOG | DB_INIT_MPOOL,
                             0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка открытия окружения Berkeley DB: %s\n", db_strerror(ret));
        return ret;
    }

    /* Создаем дескриптор базы данных */
    ret = db_create(&handler->db, handler->env, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка db_create: %s\n", db_strerror(ret));
        return ret;
    }

    /* Для создания транзакционной базы данных,
       начинаем транзакцию и передаем ее в open() */
    DB_TXN *txn = NULL;
    ret = handler->env->txn_begin(handler->env, NULL, &txn, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка начала транзакции в db_init: %s\n", db_strerror(ret));
        return ret;
    }

    ret = handler->db->open(handler->db,
                            txn, /* передаем транзакцию, чтобы создать транзакционную БД */
                            db_filename,
                            NULL,
                            DB_BTREE,
                            DB_CREATE,
                            0664);
    if (ret != 0) {
        fprintf(stderr, "Ошибка открытия Berkeley DB: %s\n", db_strerror(ret));
        txn->abort(txn);
        return ret;
    }

    /* Фиксируем транзакцию, в рамках которой создавалась база */
    ret = txn->commit(txn, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка фиксации транзакции при создании БД: %s\n", db_strerror(ret));
        return ret;
    }

    return 0;
}
int db_put_record(DBHandler *handler, DB_TXN *txn, const char *key, const char *data) {
    int ret;
    DBT db_key, db_data;
    memset(&db_key, 0, sizeof(db_key));
    memset(&db_data, 0, sizeof(db_data));

    /* Подставляем ключ и данные; учитываем нуль-терминированную строку */
    db_key.data = (void *)key;
    db_key.size = strlen(key) + 1;
    db_data.data = (void *)data;
    db_data.size = strlen(data) + 1;

    ret = handler->db->put(handler->db, txn, &db_key, &db_data, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка записи ключа '%s': %s\n", key, db_strerror(ret));
    }
    return ret;
}

int db_close(DBHandler *handler) {
    int ret = 0;
    if (handler->db) {
        ret = handler->db->close(handler->db, 0);
        if (ret != 0) {
            fprintf(stderr, "Ошибка закрытия базы данных: %s\n", db_strerror(ret));
        }
    }
    if (handler->env) {
        ret = handler->env->close(handler->env, 0);
        if (ret != 0) {
            fprintf(stderr, "Ошибка закрытия окружения: %s\n", db_strerror(ret));
        }
    }
    return ret;
}

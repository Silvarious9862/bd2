#define _POSIX_C_SOURCE 200809L

#include "db_apply_changes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <jansson.h>

#define TARGET_TABLE "employee"  // Имя таблицы для операций

int apply_changes_transaction(json_t *edited_array, 
                              json_t *added_array, 
                              json_t *deleted_array, 
                              char **err_msg) {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DB_TXN *txn = NULL;

    /* Создаем и открываем окружение BerkeleyDB с поддержкой транзакций и журнала */
    ret = db_env_create(&env, 0);
    if (ret != 0) {
        *err_msg = strdup("Ошибка создания DB_ENV");
        return -1;
    }
    ret = env->open(env, "/home/silvarious/bd2/lab02/db_env",
                    DB_CREATE | DB_INIT_TXN | DB_INIT_LOG | DB_INIT_MPOOL,
                    0);
    if (ret != 0) {
        *err_msg = strdup("Ошибка открытия окружения Berkeley DB");
        env->close(env, 0);
        return -1;
    }

    /* Создаем дескриптор базы данных */
    ret = db_create(&dbp, env, 0);
    if (ret != 0) {
        *err_msg = strdup("Ошибка создания объекта DB");
        env->close(env, 0);
        return -1;
    }

    /* Начинаем транзакцию, чтобы создать (или открыть) базу как транзакционную */
    ret = env->txn_begin(env, NULL, &txn, 0);
    if (ret != 0) {
        *err_msg = strdup("Ошибка начала транзакции");
        dbp->close(dbp, 0);
        env->close(env, 0);
        return -1;
    }

    ret = dbp->open(dbp,
                    txn,          /* Передаем транзакцию, чтобы создать транзакционную базу */
                    "transfer_db.db",
                    NULL,
                    DB_BTREE,
                    DB_CREATE,
                    0664);
    if (ret != 0) {
        *err_msg = strdup("Ошибка открытия базы данных transfer_db.db");
        txn->abort(txn);
        dbp->close(dbp, 0);
        env->close(env, 0);
        return -1;
    }

    /* Фиксируем транзакцию создания базы */
    ret = txn->commit(txn, 0);
    if (ret != 0) {
        *err_msg = strdup("Ошибка фиксации транзакции при создании БД");
        dbp->close(dbp, 0);
        env->close(env, 0);
        return -1;
    }

    /* Далее выполняем операции apply_changes в рамках транзакции */
    /* 1. Редактирование существующих записей */
    if (json_is_array(edited_array)) {
        int num_edits = (int)json_array_size(edited_array);
        for (int i = 0; i < num_edits; i++) {
            json_t *record = json_array_get(edited_array, i);
            if (!json_is_object(record))
                continue;
            
            /* Значение id теперь используется напрямую для построения key */
            const char *id         = json_string_value(json_object_get(record, "id"));
            const char *badge      = json_string_value(json_object_get(record, "badge"));
            const char *lastname   = json_string_value(json_object_get(record, "lastname"));
            const char *firstname  = json_string_value(json_object_get(record, "firstname"));
            const char *experience = json_string_value(json_object_get(record, "experience"));
            const char *carrier_id = json_string_value(json_object_get(record, "carrier_id"));

            char key_str[256];
            snprintf(key_str, sizeof(key_str), "%s:%s", TARGET_TABLE, id ? id : "");

            /* Формируем строку данных без поля id */
            char value_str[1024];
            snprintf(value_str, sizeof(value_str),
                     "badge=%s|lastname=%s|firstname=%s|experience=%s|carrier_id=%s",
                     badge ? badge : "",
                     lastname ? lastname : "",
                     firstname ? firstname : "",
                     experience ? experience : "",
                     carrier_id ? carrier_id : "");

            DBT db_key = {0}, db_data = {0};
            db_key.data = key_str;
            db_key.size = strlen(key_str) + 1;
            db_data.data = value_str;
            db_data.size = strlen(value_str) + 1;

            ret = dbp->put(dbp, NULL, &db_key, &db_data, 0);
            if (ret != 0) {
                *err_msg = strdup(db_strerror(ret));
                dbp->close(dbp, 0);
                env->close(env, 0);
                return -1;
            }
        }
    }

    /* 2. Добавление новых записей */
    if (json_is_array(added_array)) {
        int num_added = (int)json_array_size(added_array);
        for (int i = 0; i < num_added; i++) {
            json_t *record = json_array_get(added_array, i);
            if (!json_is_object(record))
                continue;

            const char *id         = json_string_value(json_object_get(record, "id"));
            const char *badge      = json_string_value(json_object_get(record, "badge"));
            const char *lastname   = json_string_value(json_object_get(record, "lastname"));
            const char *firstname  = json_string_value(json_object_get(record, "firstname"));
            const char *experience = json_string_value(json_object_get(record, "experience"));
            const char *carrier_id = json_string_value(json_object_get(record, "carrier_id"));

            char key_str[256];
            snprintf(key_str, sizeof(key_str), "%s:%s", TARGET_TABLE, id ? id : "");

            char value_str[1024];
            snprintf(value_str, sizeof(value_str),
                     "badge=%s|lastname=%s|firstname=%s|experience=%s|carrier_id=%s",
                     badge ? badge : "",
                     lastname ? lastname : "",
                     firstname ? firstname : "",
                     experience ? experience : "",
                     carrier_id ? carrier_id : "");

            DBT db_key = {0}, db_data = {0};
            db_key.data = key_str;
            db_key.size = strlen(key_str) + 1;
            db_data.data = value_str;
            db_data.size = strlen(value_str) + 1;

            ret = dbp->put(dbp, NULL, &db_key, &db_data, 0);
            if (ret != 0) {
                *err_msg = strdup(db_strerror(ret));
                dbp->close(dbp, 0);
                env->close(env, 0);
                return -1;
            }
        }
    }

    /* 3. Удаление записей */
    if (json_is_array(deleted_array)) {
        int num_deleted = (int)json_array_size(deleted_array);
        for (int i = 0; i < num_deleted; i++) {
            json_t *id_item = json_array_get(deleted_array, i);
            const char *id = NULL;
            char id_buf[32] = {0};  // <-- Объявляем буфер здесь, в начале итерации
            if (json_is_integer(id_item)) {
                snprintf(id_buf, sizeof(id_buf), "%lld", json_integer_value(id_item));
                id = id_buf;
            } else if (json_is_string(id_item)) {
                id = json_string_value(id_item);
            } else {
                continue;
            }

            char key_str[256];
            snprintf(key_str, sizeof(key_str), "%s:%s", TARGET_TABLE, id ? id : "");

            DBT db_key = {0};
            db_key.data = key_str;
            db_key.size = strlen(key_str) + 1;

            ret = dbp->del(dbp, NULL, &db_key, 0);
            if (ret != 0) {
                *err_msg = strdup(db_strerror(ret));
                dbp->close(dbp, 0);
                env->close(env, 0);
                return -1;
            }
        }
    }

    /* Закрываем базу и окружение после выполнения всех операций */
    dbp->close(dbp, 0);
    env->close(env, 0);
    return 0;
}

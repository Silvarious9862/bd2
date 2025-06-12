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
            
            const char *id_str       = json_string_value(json_object_get(record, "id"));
            const char *badge        = json_string_value(json_object_get(record, "badge"));
            const char *lastname     = json_string_value(json_object_get(record, "lastname"));
            const char *firstname    = json_string_value(json_object_get(record, "firstname"));
            const char *experience   = json_string_value(json_object_get(record, "experience"));
            const char *carrier_id   = json_string_value(json_object_get(record, "carrier_id"));

            int id_val = id_str ? atoi(id_str) : 0;
            int key_index = id_val - 1; // ключ должен ссылаться на индекс, считающий с нуля

            char key_str[256];
            snprintf(key_str, sizeof(key_str), "%s:%d", TARGET_TABLE, key_index);

            char value_str[1024];
            snprintf(value_str, sizeof(value_str),
                     "id=%s|badge=%s|lastname=%s|firstname=%s|experience=%s|carrier_id=%s",
                     id_str ? id_str : "",
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
                /* Здесь можно выполнить откат всей операции, если требуется */
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

            const char *id_str       = json_string_value(json_object_get(record, "id"));
            const char *badge        = json_string_value(json_object_get(record, "badge"));
            const char *lastname     = json_string_value(json_object_get(record, "lastname"));
            const char *firstname    = json_string_value(json_object_get(record, "firstname"));
            const char *experience   = json_string_value(json_object_get(record, "experience"));
            const char *carrier_id   = json_string_value(json_object_get(record, "carrier_id"));

            int id_val = id_str ? atoi(id_str) : 0;
            int key_index = id_val - 1; // ключ = data id - 1

            char key_str[256];
            snprintf(key_str, sizeof(key_str), "%s:%d", TARGET_TABLE, key_index);

            char value_str[1024];
            snprintf(value_str, sizeof(value_str),
                     "id=%s|badge=%s|lastname=%s|firstname=%s|experience=%s|carrier_id=%s",
                     id_str ? id_str : "",
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
            int id_val = 0;
            char id_buf[32];
            if (json_is_integer(id_item)) {
                id_val = (int)json_integer_value(id_item);
            } else if (json_is_string(id_item)) {
                id_val = atoi(json_string_value(id_item));
            } else {
                continue;
            }
            int key_index = id_val - 1; // вычитаем 1 чтобы получить ключ
            snprintf(id_buf, sizeof(id_buf), "%d", key_index);

            char key_str[256];
            snprintf(key_str, sizeof(key_str), "%s:%s", TARGET_TABLE, id_buf);

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

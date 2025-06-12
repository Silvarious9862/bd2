#include "db_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <jansson.h>

static int table_already_added(json_t *json_arr, const char *table_name) {
    size_t index;
    json_t *value;
    json_array_foreach(json_arr, index, value) {
        if (strcmp(json_string_value(value), table_name) == 0)
            return 1;
    }
    return 0;
}

char* get_tables_list() {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DBC *cursorp = NULL;
    DBT key, data;

    // Создаем и открываем окружение BerkeleyDB
    if ((ret = db_env_create(&env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания DB_ENV: %s\n", db_strerror(ret));
        return NULL;
    }
    if ((ret = env->open(env, "/home/silvarious/bd2/lab02/db_env", DB_CREATE | DB_INIT_MPOOL | DB_INIT_TXN, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия окружения BerkeleyDB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }


    // Открываем базу данных с основными данными (например, transfer_db.db)
    if ((ret = db_create(&dbp, env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания объекта DB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }
    if ((ret = dbp->open(dbp, NULL, "transfer_db.db", NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия базы данных transfer_db.db: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return NULL;
    }

    // Получаем курсор для итерации по записям
    if ((ret = dbp->cursor(dbp, NULL, &cursorp, 0)) != 0) {
        fprintf(stderr, "Ошибка получения курсора: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return NULL;
    }

    json_t *json_arr = json_array();
    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    key.flags = DB_DBT_MALLOC;

    // Проходим по всем записям и извлекаем уникальные имена таблиц
    while ((ret = cursorp->get(cursorp, &key, &data, DB_NEXT)) == 0) {
        char *key_str = (char *) key.data;
        // Находим разделитель – двоеточие
        char *colon = strchr(key_str, ':');
        if (colon) {
            size_t len = colon - key_str;
            char table_name[256] = {0};
            if (len < sizeof(table_name)) {
                strncpy(table_name, key_str, len);
                table_name[len] = '\0';
                if (!table_already_added(json_arr, table_name)) {
                    json_array_append_new(json_arr, json_string(table_name));
                }
            }
        }
        free(key.data); // освобождаем память, выделенную BerkeleyDB
        free(data.data);
    }
    
    cursorp->close(cursorp);
    dbp->close(dbp, 0);
    env->close(env, 0);

    char *json_str = json_dumps(json_arr, JSON_COMPACT);
    json_decref(json_arr);
    return json_str;  // Вызывающему необходимо free(json_str)
}

static char* extract_field_value(const char *data_str, const char *field_name) {
    // Ищем подстроку вида "field_name="
    size_t field_len = strlen(field_name);
    char search_str[128];
    snprintf(search_str, sizeof(search_str), "%s=", field_name);
    char *found = strstr(data_str, search_str);
    if (!found)
        return NULL;
    found += field_len + 1; // переход к значению

    // Копируем значение до символа | или конца строки
    char *end = strchr(found, '|');
    size_t value_len = end ? (size_t)(end - found) : strlen(found);
    char *value = malloc(value_len + 1);
    if (value) {
        strncpy(value, found, value_len);
        value[value_len] = '\0';
    }
    return value;
}

char* get_lookup_table(const char* table) {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DBC *cursorp = NULL;
    DBT key, data;
    
    // Создаем и открываем окружение BerkeleyDB
    if ((ret = db_env_create(&env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания DB_ENV: %s\n", db_strerror(ret));
        return NULL;
    }
    if ((ret = env->open(env, "/home/silvarious/bd2/lab02/db_env", DB_CREATE | DB_INIT_MPOOL | DB_INIT_TXN, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия окружения BerkeleyDB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }


    // Открываем базу данных lookup (например, lookup.db)
    if ((ret = db_create(&dbp, env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания объекта DB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }
    if ((ret = dbp->open(dbp, NULL, "lookup.db", NULL, DB_BTREE, DB_CREATE, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия базы lookup: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return NULL;
    }

    // Формируем префикс для выборки: "lookup:<table>:"
    char prefix[256];
    snprintf(prefix, sizeof(prefix), "lookup:%s:", table);
    size_t prefix_len = strlen(prefix);

    if ((ret = dbp->cursor(dbp, NULL, &cursorp, 0)) != 0) {
        fprintf(stderr, "Ошибка получения курсора: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return NULL;
    }

    json_t *json_arr = json_array();
    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    key.flags = DB_DBT_MALLOC;
    data.flags = DB_DBT_MALLOC;

    // Устанавливаем курсор на нужный префикс
    key.data = prefix;
    key.size = prefix_len + 1;

    ret = cursorp->get(cursorp, &key, &data, DB_SET_RANGE);
    while (ret == 0) {
        // Если ключ не начинается с нужного префикса, прекращаем итерацию
        if (key.size < prefix_len || strncmp((char *)key.data, prefix, prefix_len) != 0)
            break;

        // Ключ имеет вид "lookup:<table>:<id>"
        char *id = (char *)key.data + prefix_len;

        // В data.data ожидается строка вида "id=...|company_name=...|..."
        char *data_str = (char *)data.data;
        char *company_name = extract_field_value(data_str, "company_name");

        json_t *json_obj = json_object();
        json_object_set_new(json_obj, "carrier_id", json_string(id));
        if (company_name) {
            json_object_set_new(json_obj, "carrier_name", json_string(company_name));
            free(company_name);
        } else {
            json_object_set_new(json_obj, "carrier_name", json_null());
        }
        json_array_append_new(json_arr, json_obj);

        free(key.data);
        free(data.data);

        ret = cursorp->get(cursorp, &key, &data, DB_NEXT);
    }

    cursorp->close(cursorp);
    dbp->close(dbp, 0);
    env->close(env, 0);

    char *json_str = json_dumps(json_arr, JSON_COMPACT);
    json_decref(json_arr);
    return json_str;  // Вызывающему необходимо free(json_str)
}

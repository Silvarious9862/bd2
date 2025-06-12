#include "db_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <jansson.h>

/* Функция проверяет, добавлено ли уже имя таблицы в массив JSON */
static int table_already_added(json_t *json_arr, const char *table_name) {
    size_t index;
    json_t *value;
    json_array_foreach(json_arr, index, value) {
        if (strcmp(json_string_value(value), table_name) == 0)
            return 1;
    }
    return 0;
}

/* Функция для формирования списка таблиц по ключам из базы */
char* get_tables_list() {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DBC *cursorp = NULL;
    DBT key, data;

    /* Создаем и открываем окружение BerkeleyDB */
    if ((ret = db_env_create(&env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания DB_ENV: %s\n", db_strerror(ret));
        return NULL;
    }
    if ((ret = env->open(env, "/home/silvarious/bd2/lab02/db_env", 
                         DB_CREATE | DB_INIT_MPOOL | DB_INIT_TXN, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия окружения BerkeleyDB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }

    /* Открываем базу данных с основными данными (например, transfer_db.db) */
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

    /* Получаем курсор для итерации по записям */
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

    /* Проходим по всем записям и извлекаем уникальные имена таблиц (до двоеточия) */
    while ((ret = cursorp->get(cursorp, &key, &data, DB_NEXT)) == 0) {
        char *key_str = (char *) key.data;
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
        free(key.data);   // освобождаем память, выделенную BerkeleyDB
        free(data.data);
    }
    
    cursorp->close(cursorp);
    dbp->close(dbp, 0);
    env->close(env, 0);

    char *json_str = json_dumps(json_arr, JSON_COMPACT);
    json_decref(json_arr);
    return json_str;  // Вызывающему необходимо free(json_str)
}

/* Функция для извлечения значения поля из data-строки.
   Например, если data_str равна:
     "company_name=Hammes Group|inn=26550868|..."
   то extract_field_value(data_str, "company_name") вернет "Hammes Group". */
static char* extract_field_value(const char *data_str, const char *field_name) {
    size_t field_len = strlen(field_name);
    char search_str[128];
    snprintf(search_str, sizeof(search_str), "%s=", field_name);
    char *found = strstr(data_str, search_str);
    if (!found)
        return NULL;
    found += strlen(search_str);  // переход к значению
    char *end = strchr(found, '|');
    size_t value_len = end ? (size_t)(end - found) : strlen(found);
    char *value = malloc(value_len + 1);
    if (value) {
        strncpy(value, found, value_len);
        value[value_len] = '\0';
    }
    return value;
}

/* Функция для сравнения двух JSON-объектов по полю "carrier_id" (из данных) */
static int compare_json_objects(const void *a, const void *b) {
    const json_t *objA = *(const json_t **)a;
    const json_t *objB = *(const json_t **)b;

    const char* idA_str = json_string_value(json_object_get(objA, "carrier_id"));
    const char* idB_str = json_string_value(json_object_get(objB, "carrier_id"));
    long long idA = idA_str ? strtoll(idA_str, NULL, 10) : 0;
    long long idB = idB_str ? strtoll(idB_str, NULL, 10) : 0;

    if (idA < idB)
        return -1;
    else if (idA > idB)
        return 1;
    else
        return 0;
}

/* Функция для получения lookup-таблицы из основной базы.
   Теперь lookup-записи ищутся в transfer_db.db по ключам вида "carriers:<id>".
   Из ключа извлекается id записи, а из data – значение company_name. */
char* get_lookup_table(const char* table) {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DBC *cursorp = NULL;
    DBT key, data;

    /* Открываем окружение */
    if ((ret = db_env_create(&env, 0)) != 0) {
        fprintf(stderr, "Ошибка создания DB_ENV: %s\n", db_strerror(ret));
        return NULL;
    }
    if ((ret = env->open(env, "/home/silvarious/bd2/lab02/db_env",
                         DB_CREATE | DB_INIT_MPOOL | DB_INIT_TXN, 0)) != 0) {
        fprintf(stderr, "Ошибка открытия окружения BerkeleyDB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return NULL;
    }

    /* Открываем основную базу данных transfer_db.db */
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

    /* Формируем префикс для выборки. Например, если table = "carriers",
       ищем ключи вида "carriers:" */
    char prefix[256];
    snprintf(prefix, sizeof(prefix), "%s:", table);
    size_t prefix_len = strlen(prefix);

    if ((ret = dbp->cursor(dbp, NULL, &cursorp, 0)) != 0) {
        fprintf(stderr, "Ошибка получения курсора: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return NULL;
    }

    json_t *unsorted_arr = json_array();
    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    key.flags = DB_DBT_MALLOC;
    data.flags = DB_DBT_MALLOC;

    /* Устанавливаем курсор на первую запись, удовлетворяющую префиксу */
    key.data = prefix;
    key.size = prefix_len + 1;

    ret = cursorp->get(cursorp, &key, &data, DB_SET_RANGE);
    while (ret == 0) {
        if (key.size < prefix_len || strncmp((char *)key.data, prefix, prefix_len) != 0)
            break;

        /* Теперь id записи определяется из ключа, которая имеет вид "table:<id>".
           Извлекаем id после двоеточия */
        char *key_str = (char *) key.data;
        char *id_from_key = key_str + prefix_len;

        /* data.data содержит строку без поля id, например:
           "company_name=Hammes Group|inn=26550868|..."
           Извлекаем company_name */
        char *data_str = (char *)data.data;
        char *company_name = extract_field_value(data_str, "company_name");

        json_t *json_obj = json_object();
        json_object_set_new(json_obj, "carrier_id", json_string(id_from_key));
        if (company_name) {
            json_object_set_new(json_obj, "carrier_name", json_string(company_name));
            free(company_name);
        } else {
            json_object_set_new(json_obj, "carrier_name", json_null());
        }
        json_array_append_new(unsorted_arr, json_obj);

        free(key.data);
        free(data.data);
        ret = cursorp->get(cursorp, &key, &data, DB_NEXT);
    }

    cursorp->close(cursorp);
    dbp->close(dbp, 0);
    env->close(env, 0);

    /* Сортировка массива по значениям поля "carrier_id" */
    size_t arr_size = json_array_size(unsorted_arr);
    json_t **obj_array = malloc(arr_size * sizeof(json_t *));
    if (!obj_array) {
        json_decref(unsorted_arr);
        return NULL;
    }
    for (size_t i = 0; i < arr_size; i++) {
        obj_array[i] = json_array_get(unsorted_arr, i);
    }
    qsort(obj_array, arr_size, sizeof(json_t *), compare_json_objects);

    json_t *sorted_arr = json_array();
    for (size_t i = 0; i < arr_size; i++) {
        json_array_append(sorted_arr, obj_array[i]);
    }
    free(obj_array);

    char *json_str = json_dumps(sorted_arr, JSON_COMPACT);
    json_decref(sorted_arr);
    json_decref(unsorted_arr);
    return json_str;  // Вызывающему необходимо free(json_str)
}

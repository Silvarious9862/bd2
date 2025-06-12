#include "db_rows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>
#include <jansson.h>

/* Функция для сравнения двух JSON-объектов по полю "id" */
static int compare_json_rows(const void *a, const void *b) {
    const json_t *objA = *(const json_t **)a;
    const json_t *objB = *(const json_t **)b;

    const char *idA_str = json_string_value(json_object_get(objA, "id"));
    const char *idB_str = json_string_value(json_object_get(objB, "id"));

    long long idA = idA_str ? strtoll(idA_str, NULL, 10) : 0;
    long long idB = idB_str ? strtoll(idB_str, NULL, 10) : 0;

    if (idA < idB)
        return -1;
    else if (idA > idB)
        return 1;
    else
        return 0;
}

char* get_table_rows(const char* table, int limit, int offset) {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DBC *cursorp = NULL;
    DBT key, data;
    char prefix[256];
    size_t prefix_len;

    // Формируем префикс для ключей заданной таблицы, например "transport:"
    snprintf(prefix, sizeof(prefix), "%s:", table);
    prefix_len = strlen(prefix);

    // Создаем и открываем окружение BerkeleyDB
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

    // Открываем базу данных, в которой хранятся записи, например, transfer_db.db
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

    // Подготавливаем структуры DBT для ключа и данных
    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    key.flags = DB_DBT_MALLOC;
    data.flags = DB_DBT_MALLOC;

    // Устанавливаем курсор на первую запись с нужным префиксом
    key.data = prefix;
    key.size = prefix_len + 1;

    ret = cursorp->get(cursorp, &key, &data, DB_SET_RANGE);

    // Собираем все записи, удовлетворяющие префиксу, в JSON-массив
    json_t *unsorted_arr = json_array();

    while (ret == 0) {
        // Если ключ не начинается с заданного префикса, записи закончились
        if (key.size < prefix_len || strncmp((char *)key.data, prefix, prefix_len) != 0)
            break;

        // Извлекаем идентификатор из ключа: часть после префикса
        char *key_str = (char *)key.data;
        char *id_in_key = key_str + prefix_len; // например, "90"

        // data.data содержит строку вида:
        // "car_model=Bentley|insurance=2024-07-13|maintenance=2024-12-01|vin=LRXLV450KPW773092|company_id=6"
        char *data_str = strndup((char *)data.data, data.size);
        if (!data_str) {
            fprintf(stderr, "Ошибка выделения памяти для данных\n");
            break;
        }
        json_t *json_obj = json_object();

        // Добавляем id, извлеченное из ключа, как поле "id"
        json_object_set_new(json_obj, "id", json_string(id_in_key));

        // Парсим остаток data_str: разбиваем по разделителю "|" для формирования остальных пар
        char *saveptr;
        char *token = strtok_r(data_str, "|", &saveptr);
        while (token) {
            char *equal = strchr(token, '=');
            if (equal) {
                *equal = '\0';
                char *field = token;
                char *value = equal + 1;
                json_object_set_new(json_obj, field, json_string(value));
            }
            token = strtok_r(NULL, "|", &saveptr);
        }
        free(data_str);

        json_array_append_new(unsorted_arr, json_obj);

        free(key.data);
        free(data.data);

        ret = cursorp->get(cursorp, &key, &data, DB_NEXT);
    }

    cursorp->close(cursorp);
    dbp->close(dbp, 0);
    env->close(env, 0);

    // Сортировка по полю "id"
    size_t arr_size = json_array_size(unsorted_arr);
    json_t **obj_array = malloc(arr_size * sizeof(json_t *));
    if (!obj_array) {
        json_decref(unsorted_arr);
        return NULL;
    }
    for (size_t i = 0; i < arr_size; i++) {
        obj_array[i] = json_array_get(unsorted_arr, i);
    }
    qsort(obj_array, arr_size, sizeof(json_t *), compare_json_rows);

    // Формируем новый JSON-массив, применяя offset и limit.
    json_t *sorted_arr = json_array();
    for (size_t i = offset; i < arr_size; i++) {
        if (limit > 0 && (i - offset) >= (size_t)limit)
            break;
        json_array_append_new(sorted_arr, json_deep_copy(obj_array[i]));
    }
    free(obj_array);

    char *json_str = json_dumps(sorted_arr, JSON_COMPACT);
    json_decref(sorted_arr);
    json_decref(unsorted_arr);
    return json_str;  // Вызывающему необходимо освободить json_str через free()
}

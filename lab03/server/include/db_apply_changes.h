#ifndef DB_APPLY_CHANGES_H
#define DB_APPLY_CHANGES_H

#include <jansson.h>

/*
 * Функция применяет изменения (редактирование, добавление и удаление) в рамках одной транзакции.
 * При успехе – возвращает 0, при ошибке – -1.
 * Если возникает ошибка, функция через секонд-аргумент (err_msg) возвращает текст ошибки от Postgres.
 * Вызывающий обязан освободить err_msg (free), если функция вернула ошибку.
 */
int apply_changes_transaction(json_t *edited_array, json_t *added_array, json_t *deleted_array, char **err_msg);

#endif // DB_APPLY_CHANGES_H

#ifndef DB_UTILS_H
#define DB_UTILS_H

// Функция для получения списка таблиц в базе данных в виде JSON-строки.
// Возвращает динамически выделенную строку (вызывающему необходимо вызвать free).
char* get_tables_list();

// Функция для получения lookup-данных для заданной таблицы в виде JSON-строки.
// Возвращает динамически выделенную строку (вызывающему необходимо вызвать free).
char* get_lookup_table(const char* table);

#endif // DB_UTILS_H

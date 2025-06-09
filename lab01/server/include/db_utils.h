#ifndef DB_UTILS_H
#define DB_UTILS_H

// Функция для получения списка таблиц в базе данных в виде JSON-строки.
// Возвращает динамически выделенную строку (которая должна быть освобождена вызывающим).
char* get_tables_list();
char* get_lookup_table(const char* table);

#endif // DB_UTILS_H

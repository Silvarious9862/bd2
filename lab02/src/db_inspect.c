#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>

int main(void) {
    int ret;
    DB *dbp;
    DBC *cursorp;
    DBT key, data;

    // Создаем дескриптор базы данных
    ret = db_create(&dbp, NULL, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка db_create: %s\n", db_strerror(ret));
        exit(EXIT_FAILURE);
    }

    // Открываем БД в режиме только для чтения
    ret = dbp->open(dbp, NULL, "db_env/transfer_db.db", NULL, DB_BTREE, DB_RDONLY, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка открытия БД: %s\n", db_strerror(ret));
        exit(EXIT_FAILURE);
    }

    // Инициализируем структуры для ключа и данных
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    // Создаем курсор для перебора записей
    ret = dbp->cursor(dbp, NULL, &cursorp, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания курсора: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        exit(EXIT_FAILURE);
    }

    // Проходим по всем записям
    while ((ret = cursorp->get(cursorp, &key, &data, DB_NEXT)) == 0) {
        printf("Key: %s\nData: %s\n\n", (char *) key.data, (char *) data.data);
    }

    if (ret != DB_NOTFOUND) { // Окончание перебора должно вернуть DB_NOTFOUND
        fprintf(stderr, "Ошибка при переборе записей: %s\n", db_strerror(ret));
    }

    // Закрываем курсор и базу
    cursorp->close(cursorp);
    dbp->close(dbp, 0);

    return EXIT_SUCCESS;
}

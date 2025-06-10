#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <db.h>
#include <jansson.h>

#include "transfer.h"

int main(void) {
    // 1. Подключаемся к PostgreSQL
    PGconn *conn = PQconnectdb("dbname=your_database user=your_username password=your_password");
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Ошибка подключения к PostgreSQL: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }
    printf("Соединение с PostgreSQL установлено.\n");

    // 2. Создаем и открываем Berkeley DB
    DB *db;
    int ret = db_create(&db, NULL, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания Berkeley DB: %s\n", db_strerror(ret));
        PQfinish(conn);
        return EXIT_FAILURE;
    }
    ret = db->open(db, NULL, "berkeley.db", NULL, DB_BTREE, DB_CREATE, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка открытия Berkeley DB: %s\n", db_strerror(ret));
        db->close(db, 0);
        PQfinish(conn);
        return EXIT_FAILURE;
    }
    printf("База Berkeley DB успешно открыта.\n");

    // 3. Передаем данные (пока что заглушка; реализацию переноса допишем позже)
    printf("Начинается перенос данных из PostgreSQL в Berkeley DB...\n");
    transfer_data(conn, db);

    // 4. Очистка ресурсов
    db->close(db, 0);
    PQfinish(conn);
    printf("Утилита завершила работу.\n");
    return EXIT_SUCCESS;
}

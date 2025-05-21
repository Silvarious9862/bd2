#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

int main() {
    PGconn *conn = PQconnectdb("dbname=cargo user=postgres password=12345");

    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Ошибка подключения: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    printf("Успешное подключение к базе данных!\n");
    PQfinish(conn);
    return 0;
}

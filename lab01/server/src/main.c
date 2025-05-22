#include "server.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int port = 8080;  // Можно изменить порт по необходимости
    if (server_start(port) != 0) {
        fprintf(stderr, "Не удалось запустить сервер на порту %d\n", port);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

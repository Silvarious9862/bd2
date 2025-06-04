#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "response.h"

// Формируем минимальный HTTP-ответ с заголовками.
// Если сервер работает по протоколу HTTP, этот вариант подойдёт.
int send_response(int client_fd, const char* response_json) {
    char response_header[256];
    int content_length = strlen(response_json);

    // Формируем HTTP-ответ: статус 200, JSON и длина контента
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n"
             "\r\n", content_length);
    
    if (send(client_fd, response_header, strlen(response_header), 0) < 0) {
        perror("Ошибка отправки заголовка");
        return -1;
    }
    if (send(client_fd, response_json, content_length, 0) < 0) {
        perror("Ошибка отправки JSON-ответа");
        return -1;
    }
    return 0;
}

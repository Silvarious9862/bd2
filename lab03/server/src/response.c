#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include "response.h"

// Функция, которая гарантированно отправляет все байты данных
ssize_t send_all(int sockfd, const char *buffer, size_t length) {
    size_t total_sent = 0;
    while (total_sent < length) {
        ssize_t sent = send(sockfd, buffer + total_sent, length - total_sent, 0);
        if (sent < 0) {
            if (errno == EINTR)
                continue;  // если прерван, повторяем
            return -1;     // при другой ошибке
        }
        total_sent += sent;
    }
    return total_sent;
}

// Функция для логирования ответа в файл
static void log_response_to_file(const char *response_json) {
    FILE *fp = fopen("server_response.log", "a");
    if (!fp) {
        perror("Ошибка открытия файла для логирования");
        return;
    }
    // Добавим временную метку для удобства
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(fp, "[%s] Отправляем ответ:\n%s\n\n", time_str, response_json);
    fclose(fp);
}

int send_response(int client_fd, const char* response_json) {
    char response_header[256];
    int content_length = strlen(response_json);

    // Запишем формируемый JSON в лог перед отправкой
    log_response_to_file(response_json);

    // Формируем HTTP-ответ: статус 200, тип содержимого и длина ответа
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n"
             "\r\n", content_length);

    // Отправляем заголовок полностью
    if (send_all(client_fd, response_header, strlen(response_header)) < 0) {
        perror("Ошибка отправки заголовка");
        return -1;
    }
    // Отправляем JSON-тело полностью
    if (send_all(client_fd, response_json, content_length) < 0) {
        perror("Ошибка отправки JSON-ответа");
        return -1;
    }
    return 0;
}

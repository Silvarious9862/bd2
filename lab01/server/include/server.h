#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define MAX_BUFFER_SIZE 4096

// Структура для хранения информации о клиентском соединении
typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
} client_info;

// Функция для старта сервера на заданном порту
int server_start(int port);

// Функция-обработчик клиентского соединения (запускается в отдельном потоке)
void *client_handler(void *arg);

// Обработчик запроса
void process_request(int client_fd, const char* request_json);

#endif // SERVER_H

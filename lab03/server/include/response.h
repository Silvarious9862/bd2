#ifndef RESPONSE_H
#define RESPONSE_H

// Функция для отправки ответа по сокету client_fd.
// response_json – строка с JSON-ответом.
ssize_t send_all(int sockfd, const char *buffer, size_t length);
int send_response(int client_fd, const char* response_json);
static void log_response_to_file(const char *response_json);

#endif // RESPONSE_H

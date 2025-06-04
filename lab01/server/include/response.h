#ifndef RESPONSE_H
#define RESPONSE_H

// Функция для отправки ответа по сокету client_fd.
// response_json – строка с JSON-ответом.
int send_response(int client_fd, const char* response_json);

#endif // RESPONSE_H

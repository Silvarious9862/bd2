#ifndef TRANSFER_H
#define TRANSFER_H

#include <libpq-fe.h>
#include <db.h>

// Прототип: функция для переноса данных из PostgreSQL в Berkeley DB
void transfer_data(PGconn *conn, DB *db);

#endif // TRANSFER_H

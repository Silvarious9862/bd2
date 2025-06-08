#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QJsonObject>

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = nullptr);
    ~NetworkClient();

    /**
     * Отправляет JSON-запрос на сервер и возвращает JSON-ответ.
     *
     * @param request   Объект JSON с запросом
     * @param serverIp  IP-адрес сервера
     * @param serverPort Порт сервера
     * @return JSON-объект с ответом сервера, либо пустой объект при ошибке
     */
    QJsonObject sendRequest(const QJsonObject &request, const QString &serverIp, quint16 serverPort);
};

#endif // NETWORKCLIENT_H

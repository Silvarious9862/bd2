#include "networkclient.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
}

NetworkClient::~NetworkClient()
{
}

QJsonObject NetworkClient::sendRequest(const QJsonObject &request, const QString &serverIp, quint16 serverPort)
{
    QTcpSocket socket;
    socket.connectToHost(serverIp, serverPort);
    if (!socket.waitForConnected(3000)) {
        qWarning() << "Не удалось подключиться к серверу.";
        return QJsonObject();
    }

    // Преобразуем запрос в JSON-формат
    QJsonDocument doc(request);
    QByteArray requestData = doc.toJson(QJsonDocument::Compact);

    socket.write(requestData);
    if (!socket.waitForBytesWritten(3000)) {
        qWarning() << "Ошибка отправки запроса.";
        return QJsonObject();
    }

    // Накапливаем данные ответа
    QByteArray responseData;
    while (socket.waitForReadyRead(3000)) {
        responseData.append(socket.readAll());
    }

    if (responseData.isEmpty()) {
        qWarning() << "Нет ответа от сервера.";
        return QJsonObject();
    }

    // Записываем ответ для отладки (опционально)
    QFile file("response.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(responseData);
        file.close();
        qDebug() << "JSON-ответ успешно сохранен в файл response.json";
    } else {
        qWarning() << "Не удалось открыть файл для записи";
    }

    // Удаляем возможные заголовки (ищем первую '{')
    QString responseString(responseData);
    int jsonStart = responseString.indexOf('{');
    if (jsonStart == -1) {
        qWarning() << "Ответ сервера не содержит JSON.";
        return QJsonObject();
    }
    QString jsonBody = responseString.mid(jsonStart);
    QByteArray cleanJson = jsonBody.toUtf8();

    // Парсим JSON
    QJsonParseError parseError;
    QJsonDocument responseDoc = QJsonDocument::fromJson(cleanJson, &parseError);
    if (responseDoc.isNull()) {
        qWarning() << "Ошибка парсинга JSON:" << parseError.errorString();
        return QJsonObject();
    }

    return responseDoc.object();
}

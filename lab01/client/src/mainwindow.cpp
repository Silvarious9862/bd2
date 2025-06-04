#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>

#define SERVER_IP "192.168.12.121"
#define SERVER_PORT 8080

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Сразу при запуске делаем запрос к серверу и заполняем QTableWidget данными таблицы employee
    requestEmployeeTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestEmployeeTable()
{
    // Создаём сокет и подключаемся к серверу
    QTcpSocket socket;
    socket.connectToHost("192.168.12.121", 8080);
    if (!socket.waitForConnected(3000)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверу.");
        return;
    }

    // Формируем JSON-запрос для получения данных таблицы employee
    QJsonObject requestObj;
    requestObj["request_id"] = 1;
    requestObj["action"] = "get_table_rows";
    requestObj["table"] = "employee";
    requestObj["limit"] = 1000; // Задаём большое число для получения всех строк (либо, можно убрать пагинацию, если сервер поддерживает)
    requestObj["offset"] = 0;
    QJsonDocument doc(requestObj);
    QByteArray requestData = doc.toJson(QJsonDocument::Compact);

    socket.write(requestData);
    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::warning(this, "Ошибка", "Ошибка отправки запроса.");
        return;
    }
    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::warning(this, "Ошибка", "Нет ответа от сервера.");
        return;
    }

    QByteArray responseData = socket.readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    if (responseDoc.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Ошибка парсинга ответа.");
        return;
    }
    QJsonObject responseObj = responseDoc.object();
    if (responseObj.value("status").toString() != "ok") {
        QMessageBox::warning(this, "Ошибка", "Сервер вернул ошибку: " + responseObj.value("message").toString());
        return;
    }

    // Подразумеваем, что данные содержатся в массиве JSON-объектов в поле "data"
    QJsonArray dataArray = responseObj.value("data").toArray();

    ui->listTable->clear();

    if (dataArray.isEmpty()) {
        QMessageBox::information(this, "Информация", "В таблице нет данных.");
        ui->listTable->setRowCount(0);
        ui->listTable->setColumnCount(0);
        return;
    }

    // Используем первую строку для получения заголовков столбцов
    QJsonObject firstRow = dataArray.first().toObject();
    QStringList headers = firstRow.keys();
    ui->listTable->setColumnCount(headers.size());
    ui->listTable->setHorizontalHeaderLabels(headers);
    ui->listTable->setRowCount(dataArray.size());

    // Заполняем таблицу
    int row = 0;
    for (const QJsonValue &value : dataArray) {
        QJsonObject rowObj = value.toObject();
        for (int col = 0; col < headers.size(); ++col) {
            QString key = headers.at(col);
            QString cellText = rowObj.value(key).toString();
            ui->listTable->setItem(row, col, new QTableWidgetItem(cellText));
        }
        ++row;
    }
}

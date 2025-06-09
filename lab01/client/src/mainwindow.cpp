#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>

#define SERVER_IP "192.168.100.10"
#define SERVER_PORT 8080

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем экземпляр NetworkClient
    networkClient = new NetworkClient(this);

    // Сначала заполним lookup таблицу перевозчиков
    populateCarrierLookup();

    // После этого можно делать запрос данных для основной таблицы
    requestEmployeeTable();
    ui->listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateCarrierLookup()
{
    // Формируем JSON-запрос для получения справочной таблицы перевозчиков
    QJsonObject requestObj;
    requestObj["request_id"] = 2;  // Уникальный идентификатор запроса
    requestObj["action"] = "get_lookup";
    requestObj["table"] = "carriers";  // Название таблицы справки

    // Отправляем запрос через наш сетевой клиент
    QJsonObject responseObj = networkClient->sendRequest(requestObj, SERVER_IP, SERVER_PORT);

    // Проверяем, получили ли мы валидный ответ и статус "ok"
    if (responseObj.isEmpty() || responseObj.value("status").toString() != "ok") {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить lookup-таблицу для перевозчиков.");
        return;
    }

    // Ожидается, что сервер вернёт JSON-объект, содержащий массив данных в поле "data"
    // Каждая запись должна быть объектом с полями "carrier_id" и "carrier_name"
    QJsonArray dataArray = responseObj.value("data").toArray();
    m_carrierLookup.clear();

    // Например, сервер может вернуть данные так:
    // {
    //    "status": "ok",
    //    "data": [
    //         { "carrier_id": "13", "carrier_name": "Carrier A" },
    //         { "carrier_id": "14", "carrier_name": "Carrier B" },
    //         ...
    //    ]
    // }
    for (const QJsonValue &value : dataArray) {
        QJsonObject rowObj = value.toObject();
        QString id = rowObj.value("carrier_id").toString();
        QString name = rowObj.value("carrier_name").toString();
        if (!id.isEmpty()) {
            m_carrierLookup.insert(id, name);
        }
    }
}

void MainWindow::requestEmployeeTable()
{
    // Формируем JSON-запрос для получения данных таблицы employee
    QJsonObject requestObj;
    requestObj["request_id"] = 1;
    requestObj["action"] = "get_table_rows";
    requestObj["table"] = "employee";
    requestObj["limit"] = 1000;
    requestObj["offset"] = 0;

    // Получаем ответ через наш сетевой клиент
    QJsonObject responseObj = networkClient->sendRequest(requestObj, SERVER_IP, SERVER_PORT);
    if (responseObj.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Ошибка получения данных.");
        return;
    }

    if (responseObj.value("status").toString() != "ok") {
        QString errorMessage = responseObj.value("message").toString();
        QMessageBox::warning(this, "Ошибка", "Сервер вернул ошибку: " + errorMessage);
        return;
    }

    QJsonArray dataArray = responseObj.value("data").toArray();

    ui->listTable->clear();

    if (dataArray.isEmpty()) {
        QMessageBox::information(this, "Информация", "В таблице нет данных.");
        ui->listTable->setRowCount(0);
        ui->listTable->setColumnCount(0);
        return;
    }

    // Жестко задаем порядок столбцов
    QStringList headers;
    headers << "badge" << "lastname" << "firstname" << "experience" << "carrier_id";

    ui->listTable->setColumnCount(headers.size());
    ui->listTable->setHorizontalHeaderLabels(headers);
    ui->listTable->setRowCount(dataArray.size());

    int row = 0;
    for (const QJsonValue &value : dataArray) {
        QJsonObject rowObj = value.toObject();
        for (int col = 0; col < headers.size(); ++col) {
            QString key = headers.at(col);
            QString cellText;
            // Если это поле carrier_id, подменяем значение описанием из lookup таблицы
            if (key == "carrier_id") {
                QString rawCarrierId = rowObj.value(key).toString();
                cellText = m_carrierLookup.contains(rawCarrierId) ? m_carrierLookup.value(rawCarrierId) : rawCarrierId;
            } else {
                cellText = rowObj.contains(key) ? rowObj.value(key).toString() : "";
            }
            ui->listTable->setItem(row, col, new QTableWidgetItem(cellText));
        }
        ++row;
    }
}

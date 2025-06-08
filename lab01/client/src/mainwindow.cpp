#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkclient.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>

#define SERVER_IP "192.168.100.10"
#define SERVER_PORT 8080

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем экземпляр NetworkClient с передачей текущего окна в качестве родителя
    networkClient = new NetworkClient(this);

    // Сразу при запуске запрашиваем данные таблицы employee
    requestEmployeeTable();
}

MainWindow::~MainWindow()
{
    delete ui;
    // NetworkClient удалится автоматически, если его передали в конструктор QObject-родителя
}

void MainWindow::requestEmployeeTable()
{
    // Формируем JSON-запрос для получения данных таблицы employee
    QJsonObject requestObj;
    requestObj["request_id"] = 1;
    requestObj["action"] = "get_table_rows";
    requestObj["table"] = "employee";
    requestObj["limit"] = 1000; // Получаем до 1000 записей
    requestObj["offset"] = 0;

    // Отправляем запрос и получаем ответ
    QJsonObject responseObj = networkClient->sendRequest(requestObj, SERVER_IP, SERVER_PORT);
    if (responseObj.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Произошла ошибка при получении ответа от сервера.");
        return;
    }

    if (responseObj.value("status").toString() != "ok") {
        QString errorMessage = responseObj.value("message").toString();
        QMessageBox::warning(this, "Ошибка", "Сервер вернул ошибку: " + errorMessage);
        return;
    }

    // Обработка полученных данных из массива JSON
    QJsonArray dataArray = responseObj.value("data").toArray();

    ui->listTable->clear();

    if (dataArray.isEmpty()) {
        QMessageBox::information(this, "Информация", "В таблице нет данных.");
        ui->listTable->setRowCount(0);
        ui->listTable->setColumnCount(0);
        return;
    }

    // Используем первую запись для заголовков столбцов
    QJsonObject firstRow = dataArray.first().toObject();
    QStringList headers = firstRow.keys();
    ui->listTable->setColumnCount(headers.size());
    ui->listTable->setHorizontalHeaderLabels(headers);
    ui->listTable->setRowCount(dataArray.size());

    // Заполнение таблицы данными
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

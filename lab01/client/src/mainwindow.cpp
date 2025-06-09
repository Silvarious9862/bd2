#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>
#include "RecordEditDialog.h"
#include "RecordViewDialog.h"

#define SERVER_IP "localhost"
#define SERVER_PORT 8080

// Обновлённый конструктор с параметром режима работы
MainWindow::MainWindow(FormMode mode, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_mode(mode)
{
    ui->setupUi(this);

    // Формируем заголовок окна в зависимости от режима
    QString title = "employee - ";
    if (m_mode == ViewEdit)
        title += "Просмотр с редактированием";
    else if (m_mode == ViewReadOnly)
        title += "Просмотр без редактирования";
    else if (m_mode == Selection)
        title += "Выбор записи";
    setWindowTitle(title);

    // Создаем экземпляр NetworkClient
    networkClient = new NetworkClient(this);

    // Сначала заполним lookup-таблицу перевозчиков
    populateCarrierLookup();

    // После этого запрашиваем данные таблицы employee и заполняем таблицу
    requestEmployeeTable();
    // Настраиваем таблицу в зависимости от режима работы
    if (m_mode == ViewEdit) {
        ui->listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listTable->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    } else if (m_mode == ViewReadOnly) {
        ui->listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    } else if (m_mode == Selection) {
        ui->listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listTable->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
    // Устанавливаем для всех столбцов, кроме последнего, режим подгонки под содержимое,
    // а для последнего столбца режим растягивания.
    for (int i = 0; i < ui->listTable->columnCount() - 1; ++i) {
        ui->listTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
    ui->listTable->horizontalHeader()->setSectionResizeMode(ui->listTable->columnCount() - 1, QHeaderView::Stretch);

    updateButtonStates();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateButtonStates()
{
    switch(m_mode) {
    case ViewEdit:
        // В режиме редактирования все кнопки доступны. По умолчанию кнопку "Применить" можно сделать неактивной,
        // если изменений ещё нет.
        ui->btnView->setEnabled(true);
        ui->btnEdit->setEnabled(true);
        ui->btnAdd->setEnabled(true);
        ui->btnDelete->setEnabled(true);
        ui->btnExit->setEnabled(true);
        ui->btnApply->setEnabled(false);
        break;
    case ViewReadOnly:
        // В режиме просмотра доступны только кнопка Выход.
        ui->btnView->setEnabled(false);
        ui->btnEdit->setEnabled(false);
        ui->btnAdd->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnExit->setEnabled(true);
        ui->btnApply->setEnabled(false);
        break;
    case Selection:
        // В режиме выбора записи доступны Просмотр и Выход.
        ui->btnView->setEnabled(true);
        ui->btnEdit->setEnabled(false);
        ui->btnAdd->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnExit->setEnabled(true);
        ui->btnApply->setEnabled(false);
        break;
    }
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

    // Ожидается, что сервер вернёт JSON с массивом данных в поле "data"
    // Каждая запись должна содержать поля "carrier_id" и "carrier_name"
    QJsonArray dataArray = responseObj.value("data").toArray();
    m_carrierLookup.clear();

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

    // Жестко задаем порядок столбцов (без колонки "id")
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
            // При заполнении проверяем, если это поле carrier_id, подменяем его значением из lookup-таблицы
            if (key == "carrier_id") {
                QString rawCarrierId = rowObj.value(key).toString();
                // Получаем отображаемое имя из lookup‑таблицы
                QString displayText = m_carrierLookup.contains(rawCarrierId) ? m_carrierLookup.value(rawCarrierId) : rawCarrierId;
                QTableWidgetItem *item = new QTableWidgetItem(displayText);
                // Сохраняем настоящий id в пользовательских данных ячейки
                item->setData(Qt::UserRole, rawCarrierId);
                ui->listTable->setItem(row, col, item);
            } else {
                QString cellText = rowObj.contains(key) ? rowObj.value(key).toString() : "";
                ui->listTable->setItem(row, col, new QTableWidgetItem(cellText));
            }
        }
        ++row;
    }
}

void MainWindow::on_btnBack_clicked()
{
    // При нажатии на кнопку Back посылаем сигнал и закрываем окно
    emit backRequested();
    close();
}


void MainWindow::on_btnEdit_clicked()
{
    int row = ui->listTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Редактирование", "Пожалуйста, выберите запись для редактирования.");
        return;
    }

    Record record;
    record.badge     = ui->listTable->item(row, 0)->text();
    record.lastName  = ui->listTable->item(row, 1)->text();
    record.firstName = ui->listTable->item(row, 2)->text();
    record.experience= ui->listTable->item(row, 3)->text();
    record.carrierName = ui->listTable->item(row, 4)->text();
    record.carrierId = ui->listTable->item(row, 4)->data(Qt::UserRole).toString();

    RecordEditDialog dlg(record, m_carrierLookup, this);
    if (dlg.exec() == QDialog::Accepted) {
        // При обновлении строки в таблице:
        Record newRecord = dlg.getEditedRecord();

        ui->listTable->item(row, 0)->setText(newRecord.badge);
        ui->listTable->item(row, 1)->setText(newRecord.lastName);
        ui->listTable->item(row, 2)->setText(newRecord.firstName);
        ui->listTable->item(row, 3)->setText(newRecord.experience);
        // Отображаем только название перевозчика
        ui->listTable->item(row, 4)->setText(newRecord.carrierName);

        // Включаем кнопку Применить, если были изменения:
        ui->btnApply->setEnabled(true);


        ui->btnApply->setEnabled(true);
    }
}


void MainWindow::on_btnView_clicked()
{
    int row = ui->listTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Просмотр",
                                 "Пожалуйста, выберите запись для просмотра.");
        return;
    }

    // Формируем объект Record
    Record record;
    record.badge      = ui->listTable->item(row, 0)->text();
    record.lastName   = ui->listTable->item(row, 1)->text();
    record.firstName  = ui->listTable->item(row, 2)->text();
    record.experience = ui->listTable->item(row, 3)->text();
    record.carrierName= ui->listTable->item(row, 4)->text();
    record.carrierId  = ui->listTable->item(row, 4)->data(Qt::UserRole).toString();

    // Создаем и запускаем диалог просмотра
    RecordViewDialog dlg(record, m_carrierLookup, this);
    dlg.exec();
}


void MainWindow::on_btnExit_clicked()
{
    QApplication::exit();
}


void MainWindow::on_btnAdd_clicked()
{
    // Создаем пустую запись
    Record record;
    record.id = 0; // временно, новый id вычислим ниже
    record.badge = "";
    record.lastName = "";
    record.firstName = "";
    record.experience = "";
    record.carrierId = "";
    record.carrierName = "";

    // Открываем диалог редактирования (с пустыми полями)
    RecordEditDialog dlg(record, m_carrierLookup, this);
    if (dlg.exec() == QDialog::Accepted) {
        Record newRecord = dlg.getEditedRecord();
        // Проверяем, что все необходимые поля заполнены
        if (newRecord.badge.isEmpty() ||
            newRecord.lastName.isEmpty() ||
            newRecord.firstName.isEmpty() ||
            newRecord.experience.isEmpty() ||
            newRecord.carrierId.isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены.");
            return;
        }

        // Находим максимальное значение id в таблице (предполагаем, что id хранится в userData первого столбца)
        int maxId = 0;
        for (int i = 0; i < ui->listTable->rowCount(); ++i) {
            int curId = ui->listTable->item(i, 0)->data(Qt::UserRole).toInt();
            if (curId > maxId)
                maxId = curId;
        }
        // Выполняем явное преобразование: новый id – символ, равный maxId+1
        newRecord.id = QChar(maxId + 1);

        // Добавляем новую строку в конец таблицы
        int row = ui->listTable->rowCount();
        ui->listTable->insertRow(row);

        QTableWidgetItem *itemBadge = new QTableWidgetItem(newRecord.badge);
        itemBadge->setData(Qt::UserRole, newRecord.id); // сохраняем id в userData

        QTableWidgetItem *itemLastName  = new QTableWidgetItem(newRecord.lastName);
        QTableWidgetItem *itemFirstName = new QTableWidgetItem(newRecord.firstName);
        QTableWidgetItem *itemExperience= new QTableWidgetItem(newRecord.experience);

        QString displayCarrier = m_carrierLookup.contains(newRecord.carrierId) ?
                                     m_carrierLookup.value(newRecord.carrierId) :
                                     newRecord.carrierId;
        QTableWidgetItem *itemCarrier = new QTableWidgetItem(displayCarrier);
        itemCarrier->setData(Qt::UserRole, newRecord.carrierId);

        ui->listTable->setItem(row, 0, itemBadge);
        ui->listTable->setItem(row, 1, itemLastName);
        ui->listTable->setItem(row, 2, itemFirstName);
        ui->listTable->setItem(row, 3, itemExperience);
        ui->listTable->setItem(row, 4, itemCarrier);

        ui->btnApply->setEnabled(true);

        // Выбираем добавленную строку
        ui->listTable->selectRow(row);
        // Прокручиваем таблицу до только что добавленной записи
        ui->listTable->scrollToItem(ui->listTable->item(row, 0), QAbstractItemView::PositionAtBottom);
    }
}

void MainWindow::on_btnDelete_clicked()
{
    int row = ui->listTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Удаление", "Пожалуйста, выберите запись для удаления.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Подтверждение удаления",
                                                              "Вы уверены, что хотите удалить запись?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->listTable->removeRow(row);
        // Если предполагается, то включаем кнопку "Применить", чтобы пользователь мог синхронизировать изменения.
        ui->btnApply->setEnabled(true);
    }
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QMetaObject>
#include "record.h"
#include "RecordEditDialog.h"
#include "RecordViewDialog.h"

#define SERVER_IP "localhost"

MainWindow::MainWindow(FormMode mode, const QString &dbType, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_mode(mode),
    m_dbType(dbType)  // например, "postgres" или "berkleydb"
{
    ui->setupUi(this);

    loadingLabel = new QLabel(this);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->setStyleSheet("background: transparent;");
    loadingLabel->setGeometry(width() / 2 - 50, height() / 2 - 50, 100, 100);

    loadingAnimation = new QMovie(":/icons/loading.gif");
    loadingLabel->setMovie(loadingAnimation);
    loadingLabel->hide();

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

    // Записываем выбранный тип базы в JSON как дополнительное поле
    // (Это будет сделано при формировании запроса ниже.)

    // Сначала заполним lookup-таблицу перевозчиков
    populateCarrierLookup();

    // Запрос данных таблицы employee
    requestEmployeeTable();

    // Настройка таблицы в зависимости от режима работы
    if (m_mode == ViewEdit) {
        ui->listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listTable->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
    else if (m_mode == ViewReadOnly) {
        ui->listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
    else if (m_mode == Selection) {
        ui->listTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listTable->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

    int colCount = ui->listTable->columnCount();
    if (colCount > 0) {
        for (int i = 0; i < colCount - 1; ++i)
            ui->listTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        ui->listTable->horizontalHeader()->setSectionResizeMode(colCount - 1, QHeaderView::Stretch);
    }

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
    QJsonObject requestObj;
    requestObj["request_id"] = 2;
    requestObj["action"] = "get_lookup";
    requestObj["table"] = "carriers";

    // Используем выбранный порт
    QJsonObject responseObj = networkClient->sendRequest(requestObj, SERVER_IP, serverPort());

    if (responseObj.isEmpty() || responseObj.value("status").toString() != "ok") {
        //QMessageBox::warning(this, "Ошибка", "Не удалось получить lookup-таблицу для перевозчиков.");
        return;
    }

    QJsonArray dataArray = responseObj.value("data").toArray();
    m_carrierLookup.clear();

    for (const QJsonValue &value : dataArray) {
        QJsonObject rowObj = value.toObject();
        QString id = rowObj.value("carrier_id").toString();
        QString name = rowObj.value("carrier_name").toString();
        if (!id.isEmpty())
            m_carrierLookup.insert(id, name);
    }
}

void MainWindow::requestEmployeeTable()
{
    QJsonObject requestObj;
    requestObj["request_id"] = 1;
    requestObj["action"] = "get_table_rows";
    requestObj["table"] = "employee";
    requestObj["limit"] = 1000;
    requestObj["offset"] = 0;
    // (Если вы не передаёте тип базы в JSON, этот параметр можно не добавлять.)
    // requestObj["database_type"] = m_dbType;

    QJsonObject responseObj = networkClient->sendRequest(requestObj, SERVER_IP, serverPort());
    if (responseObj.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Ошибка получения данных. Не удалось установить соединение с сервером.");
        // Запланировать возврат с emit'ом сигнала после завершения обработки текущего события
        QMetaObject::invokeMethod(this, "handleConnectionError", Qt::QueuedConnection);
        return;
    }
    if (responseObj.value("status").toString() != "ok") {
        QString errorMessage = responseObj.value("message").toString();
        QMessageBox::warning(this, "Ошибка", "Сервер вернул ошибку: " + errorMessage);
        QMetaObject::invokeMethod(this, "handleConnectionError", Qt::QueuedConnection);
        return;
    }

    // Если данные успешно получены, продолжаем заполнять таблицу.
    QJsonArray dataArray = responseObj.value("data").toArray();
    ui->listTable->clear();

    if (dataArray.isEmpty()) {
        QMessageBox::information(this, "Информация", "В таблице нет данных.");
        ui->listTable->setRowCount(0);
        ui->listTable->setColumnCount(0);
        return;
    }

    QStringList headers;
    headers << "id" << "badge" << "lastname" << "firstname" << "experience" << "carrier_id";

    ui->listTable->setColumnCount(headers.size());
    ui->listTable->setHorizontalHeaderLabels(headers);
    ui->listTable->setColumnHidden(0, true);
    ui->listTable->setRowCount(dataArray.size());

    int row = 0;
    for (const QJsonValue &value : dataArray) {
        QJsonObject rowObj = value.toObject();
        for (int col = 0; col < headers.size(); ++col) {
            QString key = headers.at(col);
            QTableWidgetItem *item = nullptr;

            if (key == "carrier_id") {
                QString rawCarrierId = rowObj.value(key).toString();
                QString displayText = m_carrierLookup.contains(rawCarrierId) ? m_carrierLookup.value(rawCarrierId) : rawCarrierId;
                item = new QTableWidgetItem(displayText);
                item->setData(Qt::UserRole, rawCarrierId);
            }
            else if (key == "id") {
                QString cellText = rowObj.contains(key) ? rowObj.value(key).toString() : "";
                item = new QTableWidgetItem(cellText);
                item->setData(Qt::UserRole, cellText);
            }
            else {
                QString cellText = rowObj.contains(key) ? rowObj.value(key).toString() : "";
                item = new QTableWidgetItem(cellText);
            }

            ui->listTable->setItem(row, col, item);
        }
        ++row;
    }
}

void MainWindow::on_btnBack_clicked()
{
    // При нажатии на кнопку Back посылаем сигнал и закрываем окно
    emit backRequested(m_dbType);
    close();
}


void MainWindow::on_btnEdit_clicked()
{
    int row = ui->listTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Редактирование",
                                 "Пожалуйста, выберите запись для редактирования.");
        return;
    }

    Record origRecord;
    // Извлекаем данные из таблицы (с учётом структуры: [0]=id, [1]=badge, [2]=lastname, [3]=firstname, [4]=experience, [5]=carrier)
    origRecord.id = ui->listTable->item(row, 0)->data(Qt::UserRole).toString();
    origRecord.badge = ui->listTable->item(row, 1)->text();
    origRecord.lastName = ui->listTable->item(row, 2)->text();
    origRecord.firstName = ui->listTable->item(row, 3)->text();
    origRecord.experience = ui->listTable->item(row, 4)->text();
    origRecord.carrierName = ui->listTable->item(row, 5)->text();
    origRecord.carrierId = ui->listTable->item(row, 5)->data(Qt::UserRole).toString();

    // Открываем диалог редактирования и передаём исходный Record
    RecordEditDialog dlg(origRecord, m_carrierLookup, this);
    if (dlg.exec() == QDialog::Accepted) {
        Record newRecord = dlg.getEditedRecord();

        // Если пользователь не изменил запись, выходим (ничего не обновляем)
        if (origRecord.badge == newRecord.badge &&
            origRecord.lastName == newRecord.lastName &&
            origRecord.firstName == newRecord.firstName &&
            origRecord.experience == newRecord.experience &&
            origRecord.carrierId == newRecord.carrierId &&
            origRecord.carrierName == newRecord.carrierName)
        {
            // Можно также вывести информационное сообщение
            qDebug() << "Запись не изменилась, сохранение изменений не требуется.";
            return;
        }

        // Если изменения обнаружены, обновляем данные в таблице
        ui->listTable->item(row, 1)->setText(newRecord.badge);
        ui->listTable->item(row, 2)->setText(newRecord.lastName);
        ui->listTable->item(row, 3)->setText(newRecord.firstName);
        ui->listTable->item(row, 4)->setText(newRecord.experience);
        ui->listTable->item(row, 5)->setText(newRecord.carrierName);
        // Сохраняем id в скрытой колонке без изменений
        ui->listTable->item(row, 0)->setData(Qt::UserRole, newRecord.id);

        // Обновляем списки изменений:
        // Если запись уже находится в списке добавленных, обновляем её там.
        bool foundInAdded = false;
        for (int i = 0; i < m_addedRecords.size(); ++i) {
            if (m_addedRecords[i].id == newRecord.id) {
                m_addedRecords[i] = newRecord;
                foundInAdded = true;
                break;
            }
        }
        // Если не найдена, ищем в списке редактированных
        if (!foundInAdded) {
            bool foundInEdited = false;
            for (int i = 0; i < m_editedRecords.size(); ++i) {
                if (m_editedRecords[i].id == newRecord.id) {
                    m_editedRecords[i] = newRecord;
                    foundInEdited = true;
                    break;
                }
            }
            if (!foundInEdited)
                m_editedRecords.append(newRecord);
        }
        ui->btnApply->setEnabled(true);
        debugWriteChangeLists(); // Записываем состояние списков для отладки
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
    record.id = ""; // пока пусто, новый id будет просчитан ниже
    record.badge = "";
    record.lastName = "";
    record.firstName = "";
    record.experience = "";
    record.carrierId = "";
    record.carrierName = "";

    RecordEditDialog dlg(record, m_carrierLookup, this);
    if (dlg.exec() == QDialog::Accepted) {
        Record newRecord = dlg.getEditedRecord();
        if (newRecord.badge.isEmpty() ||
            newRecord.lastName.isEmpty() ||
            newRecord.firstName.isEmpty() ||
            newRecord.experience.isEmpty() ||
            newRecord.carrierId.isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены.");
            return;
        }

        // Вычисляем максимальный id среди записей в таблице
        int maxId = 0;
        for (int i = 0; i < ui->listTable->rowCount(); ++i) {
            int curId = ui->listTable->item(i, 0)->data(Qt::UserRole).toString().toInt();
            if (curId > maxId)
                maxId = curId;
        }
        // Назначаем новый id как строку
        newRecord.id = QString::number(maxId + 1);

        // Добавляем новую строку в таблицу
        int row = ui->listTable->rowCount();
        ui->listTable->insertRow(row);

        QTableWidgetItem *itemId = new QTableWidgetItem(newRecord.id);
        itemId->setData(Qt::UserRole, newRecord.id);
        QTableWidgetItem *itemBadge = new QTableWidgetItem(newRecord.badge);
        QTableWidgetItem *itemLastName  = new QTableWidgetItem(newRecord.lastName);
        QTableWidgetItem *itemFirstName = new QTableWidgetItem(newRecord.firstName);
        QTableWidgetItem *itemExperience= new QTableWidgetItem(newRecord.experience);

        // Для перевозчика:
        QString displayCarrier = m_carrierLookup.contains(newRecord.carrierId) ?
                                     m_carrierLookup.value(newRecord.carrierId) :
                                     newRecord.carrierId;
        QTableWidgetItem *itemCarrier = new QTableWidgetItem(displayCarrier);
        itemCarrier->setData(Qt::UserRole, newRecord.carrierId);


        ui->listTable->setItem(row, 0, itemId);
        ui->listTable->setColumnHidden(0, true);
        ui->listTable->setItem(row, 1, itemBadge);
        ui->listTable->setItem(row, 2, itemLastName);
        ui->listTable->setItem(row, 3, itemFirstName);
        ui->listTable->setItem(row, 4, itemExperience);
        ui->listTable->setItem(row, 5, itemCarrier);


        ui->btnApply->setEnabled(true);
        ui->listTable->selectRow(row);
        ui->listTable->scrollToItem(ui->listTable->item(row, 0), QAbstractItemView::PositionAtBottom);

        // Сохраняем новую запись в массиве добавленных записей
        m_addedRecords.append(newRecord);

        debugWriteChangeLists(); // записываем состояние списков
    }
}

void MainWindow::on_btnDelete_clicked()
{
    int row = ui->listTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Удаление",
                                 "Пожалуйста, выберите запись для удаления.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Подтверждение удаления",
                                                              "Вы уверены, что хотите удалить запись?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Извлекаем id записи как строку
        QString id = ui->listTable->item(row, 0)->data(Qt::UserRole).toString();

        bool foundInAdded = false;
        // Если запись принадлежит m_addedRecords, удаляем её оттуда
        for (int i = 0; i < m_addedRecords.size(); ++i) {
            if (m_addedRecords[i].id == id) {
                m_addedRecords.removeAt(i);
                foundInAdded = true;
                break;
            }
        }
        // Если запись не новая, то удаляем её из m_editedRecords (если есть)
        // и добавляем ее id в список удаленных записей
        if (!foundInAdded) {
            for (int i = 0; i < m_editedRecords.size(); ++i) {
                if (m_editedRecords[i].id == id) {
                    m_editedRecords.removeAt(i);
                    break;
                }
            }
            // Добавляем id в виде int (при необходимости, id.toInt())
            m_deletedRecordIDs.append(id.toInt());
        }

        ui->listTable->removeRow(row);
        ui->btnApply->setEnabled(true);

        debugWriteChangeLists(); // записываем состояние списков
    }
}

void MainWindow::on_btnApply_clicked()
{
    showLoadingSpinner();

    QJsonObject requestObj;
    requestObj["action"] = "apply_changes";
    // Добавляем выбранный тип базы
    requestObj["database_type"] = m_dbType;

    QJsonArray editedArray;
    for (const Record &rec : m_editedRecords) {
        QJsonObject obj;
        obj["id"] = rec.id;
        obj["badge"] = rec.badge;
        obj["lastname"] = rec.lastName;
        obj["firstname"] = rec.firstName;
        obj["experience"] = rec.experience;
        obj["carrier_id"] = rec.carrierId;
        editedArray.append(obj);
    }
    requestObj["edited"] = editedArray;

    QJsonArray deletedArray;
    for (int recId : m_deletedRecordIDs)
        deletedArray.append(recId);
    requestObj["deleted"] = deletedArray;

    QJsonArray addedArray;
    for (const Record &rec : m_addedRecords) {
        QJsonObject obj;
        obj["id"] = rec.id;
        obj["badge"] = rec.badge;
        obj["lastname"] = rec.lastName;
        obj["firstname"] = rec.firstName;
        obj["experience"] = rec.experience;
        obj["carrier_id"] = rec.carrierId;
        addedArray.append(obj);
    }
    requestObj["added"] = addedArray;

    requestObj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Отправляем запрос на сервер через NetworkClient с выбранным портом
    QJsonObject responseObj = networkClient->sendRequest(requestObj, SERVER_IP, serverPort());

    hideLoadingSpinner();

    if (responseObj.value("status").toString() == "ok") {
        m_editedRecords.clear();
        m_addedRecords.clear();
        m_deletedRecordIDs.clear();
        QMessageBox::information(this, "Изменения применены", "Все изменения успешно внесены в БД.");
    } else {
        QString errorDetails = responseObj.value("message").toString();
        showErrorDialog("Ошибка", "Не удалось применить изменения на сервере.", errorDetails);
    }
}

void MainWindow::showLoadingSpinner()
{
    loadingLabel->show();
    loadingAnimation->start();

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    loadingLabel->setGraphicsEffect(effect);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::hideLoadingSpinner()
{
    QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect*>(loadingLabel->graphicsEffect());
    if (effect) {
        QPropertyAnimation *fadeOut = new QPropertyAnimation(effect, "opacity");
        fadeOut->setDuration(300);
        fadeOut->setStartValue(1);
        fadeOut->setEndValue(0);
        connect(fadeOut, &QPropertyAnimation::finished, loadingLabel, &QLabel::hide);
        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    }
    loadingAnimation->stop();
}

// Функция для записи отладочной информации о списках изменений в файл
void MainWindow::debugWriteChangeLists()
{
    // Открываем (или создаем) файл для записи в режиме добавления
    QFile file("debug_changes.log");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Невозможно открыть debug_changes.log для записи:" << file.errorString();
        return;
    }

    QTextStream out(&file);

    // Записываем текущую дату и время для отметки момента записи
    out << "DEBUG LOG: " << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n";

    // Записываем содержимое списка изменённых записей
    out << "Edited Records:" << "\n";
    for (const Record &rec : m_editedRecords) {
        out << "  ID: " << rec.id
            << ", badge: " << rec.badge
            << ", lastName: " << rec.lastName
            << ", firstName: " << rec.firstName
            << ", experience: " << rec.experience
            << ", carrier_id: " << rec.carrierId
            << ", carrier_name: " << rec.carrierName << "\n";
    }

    // Записываем список добавленных записей
    out << "Added Records:" << "\n";
    for (const Record &rec : m_addedRecords) {
        out << "  ID: " << rec.id
            << ", badge: " << rec.badge
            << ", lastName: " << rec.lastName
            << ", firstName: " << rec.firstName
            << ", experience: " << rec.experience
            << ", carrier_id: " << rec.carrierId
            << ", carrier_name: " << rec.carrierName << "\n";
    }

    // Записываем список удалённых идентификаторов
    out << "Deleted Record IDs:" << "\n";
    for (int id : m_deletedRecordIDs) {
        out << "  " << id << "\n";
    }

    out << "---------------------------------------" << "\n";
    file.close();
}

// Функция для вывода диалога с раскрывающимися подробностями ошибки.
void MainWindow::showErrorDialog(const QString &title, const QString &shortMessage, const QString &detailedMessage)
{
    // Создаем новый диалог
    QDialog dialog(this);
    dialog.setWindowTitle(title);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Основное короткое сообщение
    QLabel *label = new QLabel(shortMessage, &dialog);
    label->setWordWrap(true);
    layout->addWidget(label);

    // Кнопка для переключения видимости подробного текста (спойлер)
    QToolButton *toggleButton = new QToolButton(&dialog);
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setArrowType(Qt::RightArrow);
    toggleButton->setText("Показать детали...");
    toggleButton->setCheckable(true);
    layout->addWidget(toggleButton);

    // Текстовое поле с подробностями
    QTextEdit *textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(detailedMessage);
    textEdit->setVisible(false);  // по умолчанию скрыто
    layout->addWidget(textEdit);

    // При переключении кнопки скрываем или показываем детализацию
    connect(toggleButton, &QToolButton::toggled, textEdit, &QTextEdit::setVisible);
    connect(toggleButton, &QToolButton::toggled, [toggleButton, &dialog](bool checked){
        toggleButton->setArrowType(checked ? Qt::DownArrow : Qt::RightArrow);
        toggleButton->setText(checked ? "Скрыть детали" : "Показать детали...");
        dialog.adjustSize();
    });

    // Стандартные кнопки, например, только ОК
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, &dialog);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    dialog.exec();
}

int MainWindow::serverPort() const {
    if(m_dbType.toLower() == "postgres")
        return 8080;
    else if(m_dbType.toLower() == "berkleydb")
        return 8081;
    return 8080; // порт по умолчанию
}

void MainWindow::handleConnectionError()
{
    emit backRequested(m_dbType);   // передаём значение типа базы
    if (!this->isHidden())
        this->close();
}

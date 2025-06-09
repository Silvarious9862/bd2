#include "RecordViewDialog.h"
#include "ui_RecordViewDialog.h"

#include <QTableWidgetItem>
#include <QPushButton>

RecordViewDialog::RecordViewDialog(const Record &record,
                                   const QMap<QString, QString> &carrierLookup,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordViewDialog)
{
    ui->setupUi(this);

    // Настраиваем таблицу для просмотра (5 строк, 2 столбца)
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers << "Поле" << "Значение";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Растягиваем второй столбец на оставшееся пространство
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Отключаем возможность редактирования таблицы
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Заполняем первую колонку названиями полей (снимаем редактирование)
    QStringList fieldLabels = { "Badge", "Last Name", "First Name", "Experience", "Carrier" };
    for (int i = 0; i < fieldLabels.size(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem(fieldLabels.at(i));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 0, item);
    }

    // Заполняем вторую колонку значениями из объекта record
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(record.badge));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem(record.lastName));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem(record.firstName));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(record.experience));

    // Для поля Carrier: если в записи есть carrierName - используем его,
    // иначе подставляем наименование из lookup по carrierId.
    QString carrierDisplay = !record.carrierName.isEmpty() ?
                                 record.carrierName :
                                 carrierLookup.value(record.carrierId, record.carrierId);
    QTableWidgetItem *carrierItem = new QTableWidgetItem(carrierDisplay);
    carrierItem->setFlags(carrierItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(4, 1, carrierItem);

    // Настраиваем кнопки: очищаем существующий набор кнопок и оставляем только кнопку "Закрыть"
    ui->buttonBox->clear();
    QPushButton *closeBtn = ui->buttonBox->addButton(QDialogButtonBox::Close);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);
}

RecordViewDialog::~RecordViewDialog()
{
    delete ui;
}

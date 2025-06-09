#include "RecordEditDialog.h"
#include <QTableWidgetItem>

RecordEditDialog::RecordEditDialog(const Record &record, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordEditDialog),  // ui теперь имеет полный тип, определённый в ui_RecordEditDialog.h
    m_record(record)
{
    ui->setupUi(this);

    // Предположим, что в записи 5 полей, поэтому создаём таблицу с 5 строками и 2 столбцами.
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers << "Поле" << "Значение";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Заполняем первую колонку названиями полей
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Badge"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Last Name"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("First Name"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("Experience"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("Carrier ID"));

    // Заполняем вторую колонку значениями текущей записи
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(record.badge));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem(record.lastName));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem(record.firstName));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(record.experience));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem(record.carrierId));

    // Делаем ячейки второй колонки редактируемыми
    for (int row = 0; row < 5; ++row)
    {
        if (QTableWidgetItem *item = ui->tableWidget->item(row, 1)) {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }
    }

    // Подключаем стандартные сигналы для QDialogButtonBox (Ok/Cancel)
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &RecordEditDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &RecordEditDialog::reject);
}

RecordEditDialog::~RecordEditDialog()
{
    delete ui;
}

Record RecordEditDialog::getEditedRecord() const
{
    Record r;
    if (ui->tableWidget->item(0, 1))
        r.badge = ui->tableWidget->item(0, 1)->text();
    if (ui->tableWidget->item(1, 1))
        r.lastName = ui->tableWidget->item(1, 1)->text();
    if (ui->tableWidget->item(2, 1))
        r.firstName = ui->tableWidget->item(2, 1)->text();
    if (ui->tableWidget->item(3, 1))
        r.experience = ui->tableWidget->item(3, 1)->text();
    if (ui->tableWidget->item(4, 1))
        r.carrierId = ui->tableWidget->item(4, 1)->text();
    return r;
}

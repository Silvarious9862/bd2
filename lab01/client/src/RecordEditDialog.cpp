#include "RecordEditDialog.h"
#include "ui_RecordEditDialog.h"
#include <QTableWidgetItem>
#include <QColor>
#include <QMessageBox>
#include <QComboBox>

RecordEditDialog::RecordEditDialog(const Record &record,
                                   const QMap<QString, QString> &carrierLookup,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordEditDialog),
    m_record(record)
{
    ui->setupUi(this);

    // Настраиваем таблицу для редактирования (5 строк, 2 столбца)
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(2);
    QStringList headers = {"Поле", "Значение"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Растягиваем второй столбец на оставшееся пространство
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Заполняем первую колонку (названия полей) и блокируем изменение
    QStringList fieldLabels = {"Badge", "Last Name", "First Name", "Experience", "Carrier"};
    for (int i = 0; i < fieldLabels.size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(fieldLabels.at(i));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 0, item);
    }

    // Заполняем строки 0–3 данными из record (это поля, которые можно редактировать)
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(record.badge));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem(record.lastName));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem(record.firstName));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(record.experience));
    m_defaultBackground.resize(4);
    for (int row = 0; row < 4; ++row) {
        QTableWidgetItem *item = ui->tableWidget->item(row, 1);
        if (item)
            m_defaultBackground[row] = item->background();
    }
    // Для строки 4 (Carrier) создаем выпадающий список
    QComboBox *combo = new QComboBox(ui->tableWidget);
    for (auto it = carrierLookup.constBegin(); it != carrierLookup.constEnd(); ++it) {
        combo->addItem(it.value(), it.key());
    }
    int index = combo->findData(record.carrierId);
    if (index != -1)
        combo->setCurrentIndex(index);
    else
        combo->setCurrentIndex(0);
    ui->tableWidget->setCellWidget(4, 1, combo);

    // Подключаем сигнал cellChanged для обновления подсветки ячеек
    connect(ui->tableWidget, &QTableWidget::cellChanged,
            this, [this](int row, int col) {
                // Проверяем только редактируемые поля (строки 0–3, колонка 1)
                if (col == 1 && row < 4) {
                    QTableWidgetItem *item = ui->tableWidget->item(row, col);
                    if (item) {
                        if (item->text().trimmed().isEmpty())
                            item->setBackground(QColor(255, 200, 200)); // светло-красный фон
                        else
                            // Восстанавливаем исходную заливку, которую сохранили при создании
                            item->setBackground(m_defaultBackground.at(row));
                    }
                }
            });

    // Подключаем стандартные сигналы для кнопок Ok/Cancel
    //connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &RecordEditDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &RecordEditDialog::reject);
}

void RecordEditDialog::accept()
{
    bool hasEmpty = false;
    // Проверяем строки 0–3 (только редактируемые текстовые поля)
    for (int row = 0; row < 4; ++row) {
        QTableWidgetItem *item = ui->tableWidget->item(row, 1);
        if (!item || item->text().trimmed().isEmpty()) {
            hasEmpty = true;
            if (item)
                item->setBackground(QColor(255, 200, 200)); // подсвечиваем незаполненное поле
        }
        else {
            // Восстанавливаем исходный фон для ячейки
            item->setBackground(m_defaultBackground.at(row));
        }
    }

    if (hasEmpty) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены.");
        // Диалог не закрывается – вводимые данные остаются в ячейках
        return;
    }
    QDialog::accept();
}

Record RecordEditDialog::getEditedRecord() const
{
    Record r;
    // Сохраняем исходный id из переданного в конструкторе record
    r.id = m_record.id;  // ← добавляем, чтобы id не потерялся
    // Считываем значения из редактируемых ячеек
    if (ui->tableWidget->item(0, 1))
        r.badge = ui->tableWidget->item(0, 1)->text();
    if (ui->tableWidget->item(1, 1))
        r.lastName = ui->tableWidget->item(1, 1)->text();
    if (ui->tableWidget->item(2, 1))
        r.firstName = ui->tableWidget->item(2, 1)->text();
    if (ui->tableWidget->item(3, 1))
        r.experience = ui->tableWidget->item(3, 1)->text();

    // Для перевозчика используем QComboBox
    QComboBox *combo = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(4, 1));
    if (combo) {
        r.carrierId = combo->currentData().toString();
        r.carrierName = combo->currentText();
    }
    return r;
}


RecordEditDialog::~RecordEditDialog()
{
    delete ui;
}

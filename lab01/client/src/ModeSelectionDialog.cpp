#include "ModeSelectionDialog.h"
// Здесь включать ui_ModeSelectionDialog.h повторно не обязательно, т.к. он уже включён в заголовке

ModeSelectionDialog::ModeSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModeSelectionDialog)  // Теперь компилятор знает полный тип
{
    ui->setupUi(this);

    // По умолчанию выбираем режим "Просмотр без редактирования"
    ui->radioReadOnly->setChecked(true);

    ui->btnSwitchDb->setCheckable(true);
    ui->btnSwitchDb->setText("Postgres");
    ui->verticalLayout->setAlignment(ui->btnSwitchDb, Qt::AlignHCenter);

    // Подключаем сигнал toggled к лямбда-функции для обновления состояния кнопки
    connect(ui->btnSwitchDb, &QToolButton::toggled,
            this, [this](bool checked) {
                if (checked) {
                    // Если кнопка в состоянии "checked" – считаем, что выбран BerkeleyDB
                    ui->btnSwitchDb->setText("BerkeleyDB");
                    // Здесь можно также установить другую иконку, если требуется:
                    // ui->btnSwitchDb->setIcon(QIcon(":/icons/berkleydb.png"));
                } else {
                    // Если кнопка не нажата – выбран Postgres
                    ui->btnSwitchDb->setText("Postgres");
                    // ui->btnSwitchDb->setIcon(QIcon(":/icons/postgres.png"));
                }
            });
}

ModeSelectionDialog::~ModeSelectionDialog()
{
    delete ui;
}

FormMode ModeSelectionDialog::selectedMode() const
{
    if (ui->radioEdit->isChecked())
        return ViewEdit;
    else if (ui->radioReadOnly->isChecked())
        return ViewReadOnly;
    else if (ui->radioSelection->isChecked())
        return Selection;

    return ViewReadOnly;
}

QString ModeSelectionDialog::selectedDbType() const
{
    // Если toggle-кнопка в состоянии checked, значит выбран BerkeleyDB, иначе Postgres
    return ui->btnSwitchDb->isChecked() ? "berkleydb" : "postgres";
}

void ModeSelectionDialog::setInitialDbType(const QString &dbType)
{
    // Если выбран BerkeleyDB, ставим кнопку в состояние checked, иначе – unchecked
    if (dbType.toLower() == "berkleydb")
        ui->btnSwitchDb->setChecked(true);
    else
        ui->btnSwitchDb->setChecked(false);
}

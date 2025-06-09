#include "ModeSelectionDialog.h"
// Здесь включать ui_ModeSelectionDialog.h повторно не обязательно, т.к. он уже включён в заголовке

ModeSelectionDialog::ModeSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModeSelectionDialog)  // Теперь компилятор знает полный тип
{
    ui->setupUi(this);

    // По умолчанию выбираем режим "Просмотр без редактирования"
    ui->radioReadOnly->setChecked(true);
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

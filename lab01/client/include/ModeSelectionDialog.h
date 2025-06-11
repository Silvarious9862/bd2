#ifndef MODESELECTIONDIALOG_H
#define MODESELECTIONDIALOG_H

#include <QDialog>
#include "formmode.h"  // Файл с объявлением перечисления FormMode
#include "ui_ModeSelectionDialog.h"  // Включаем полный сгенерированный заголовочный файл

// Больше не нужна форвард-декларация: класс Ui::ModeSelectionDialog уже определён

class ModeSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModeSelectionDialog(QWidget *parent = nullptr);
    ~ModeSelectionDialog();

    // Метод, возвращающий выбранный режим
    FormMode selectedMode() const;
    QString selectedDbType() const;
    void setInitialDbType(const QString &dbType);


private:
    Ui::ModeSelectionDialog *ui;
};

#endif // MODESELECTIONDIALOG_H

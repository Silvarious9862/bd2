#ifndef RECORDEDITDIALOG_H
#define RECORDEDITDIALOG_H

#include <QDialog>
#include "record.h"
#include "ui_RecordEditDialog.h"  // Полное определение интерфейса, сгенерированного uic

class RecordEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordEditDialog(const Record &record, QWidget *parent = nullptr);
    ~RecordEditDialog();

    // Возвращает изменённую запись
    Record getEditedRecord() const;

private:
    Ui::RecordEditDialog *ui;
    Record m_record;
};

#endif // RECORDEDITDIALOG_H

#ifndef RECORDEDITDIALOG_H
#define RECORDEDITDIALOG_H

#include <QDialog>
#include "record.h"
#include <QMap>
#include "ui_RecordEditDialog.h"  // Полное определение интерфейса

class RecordEditDialog : public QDialog
{
    Q_OBJECT

public:
    // Добавляем carrierLookup для заполнения комбобокса
    explicit RecordEditDialog(const Record &record,
                              const QMap<QString, QString> &carrierLookup,
                              QWidget *parent = nullptr);
    ~RecordEditDialog();

    // Возвращает изменённую запись (обновляются оба поля)
    Record getEditedRecord() const;

protected:
    void accept() override;

private:
    Ui::RecordEditDialog *ui;
    Record m_record;
    QVector<QBrush> m_defaultBackground;

};

#endif // RECORDEDITDIALOG_H

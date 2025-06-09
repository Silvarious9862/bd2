#ifndef RECORDVIEWDIALOG_H
#define RECORDVIEWDIALOG_H

#include <QDialog>
#include "record.h"
#include <QMap>

namespace Ui {
class RecordViewDialog;
}

class RecordViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordViewDialog(const Record &record,
                              const QMap<QString, QString> &carrierLookup,
                              QWidget *parent = nullptr);
    ~RecordViewDialog();

private:
    Ui::RecordViewDialog *ui;
};

#endif // RECORDVIEWDIALOG_H

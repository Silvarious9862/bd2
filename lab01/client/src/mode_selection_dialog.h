#ifndef MODE_SELECTION_DIALOG_H
#define MODE_SELECTION_DIALOG_H

#include <QDialog>
#include "mainwindow.h"  // Для использования MainWindow::FormMode

namespace Ui {
class ModeSelectionDialog;
}

class ModeSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModeSelectionDialog(QWidget *parent = nullptr);
    ~ModeSelectionDialog();

    // Метод возвращает выбранный режим
    MainWindow::FormMode selectedMode() const;

private:
    Ui::ModeSelectionDialog *ui;
};

#endif // MODE_SELECTION_DIALOG_H

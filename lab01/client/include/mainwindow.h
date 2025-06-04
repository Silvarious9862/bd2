#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /// Запрашивает данные таблицы employee с сервера и заполняет QTableWidget
    void requestEmployeeTable();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

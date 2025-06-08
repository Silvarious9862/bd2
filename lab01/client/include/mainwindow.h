#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class NetworkClient;  // Предварительное объявление класса для разделения слоев

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /// Запрашивает данные таблицы employee и заполняет QTableWidget
    void requestEmployeeTable();

    Ui::MainWindow *ui;
    NetworkClient *networkClient; // Объект для работы с сервером
};

#endif // MAINWINDOW_H

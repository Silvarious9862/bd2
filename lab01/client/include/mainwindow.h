#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class NetworkClient;  // Предварительное объявление

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /// Запрашивает данные таблицы employee и заполняет QTableWidget
    void requestEmployeeTable();

    /// Выполняет запрос к серверу для получения lookup-таблицы для перевозчиков
    void populateCarrierLookup();

    Ui::MainWindow *ui;
    NetworkClient *networkClient;

    // Локальная lookup таблица <carrier_id, carrier_name>
    QMap<QString, QString> m_carrierLookup;
};

#endif // MAINWINDOW_H

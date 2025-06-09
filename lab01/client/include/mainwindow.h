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
    // Перечисление режимов работы формы
    enum FormMode {
        ViewEdit,      // Просмотр с редактированием
        ViewReadOnly,  // Просмотр без редактирования
        Selection      // Выбор записи
    };

    // Обновлённый конструктор, принимающий режим работы (по умолчанию — ViewReadOnly)
    explicit MainWindow(FormMode mode = ViewReadOnly, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    // Сигнал, который будет посылаться при нажатии кнопки Back
    void backRequested();

private slots:
    void on_btnBack_clicked();

    void on_btnEdit_clicked();

    void on_btnView_clicked();

    void on_btnExit_clicked();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

private:
    /// Запрашивает данные таблицы employee и заполняет QTableWidget
    void requestEmployeeTable();

    /// Выполняет запрос к серверу для получения lookup-таблицы для перевозчиков
    void populateCarrierLookup();

    ///метод для установки состояний кнопок в зависимости от режима
    void updateButtonStates();

    Ui::MainWindow *ui;
    NetworkClient *networkClient;

    // Локальная lookup таблица <carrier_id, carrier_name>
    QMap<QString, QString> m_carrierLookup;

    // Режим работы окна
    FormMode m_mode;
};

#endif // MAINWINDOW_H

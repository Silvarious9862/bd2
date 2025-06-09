#include <QApplication>
#include "mainwindow.h"
#include "ModeSelectionDialog.h"
#include "formmode.h" // Если используется глобальное перечисление, но здесь мы используем внутреннее MainWindow::FormMode

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Флаг, который будет устанавливаться, если пользователь нажал Back
    bool restartMainWindow = false;

    do {
        // Показываем диалог выбора режима
        ModeSelectionDialog modeDialog;
        if (modeDialog.exec() != QDialog::Accepted) {
            // Если пользователь отменил выбор, завершаем приложение
            break;
        }

        // Создаем главное окно с выбранным режимом
        MainWindow *w = new MainWindow(static_cast<MainWindow::FormMode>(modeDialog.selectedMode()));
        // При соединении сигнала backRequested завершаем текущую event loop
        QObject::connect(w, &MainWindow::backRequested, [&]() {
            restartMainWindow = true;
            app.quit();
        });

        w->show();

        // Запускаем event loop до закрытия главного окна
        int ret = app.exec();

        // Удаляем окно
        delete w;

        // Если приложение завершилось не по нажатию "Back", выходим из цикла
        if (!restartMainWindow)
            break;

        // Сбрасываем флаг перед следующим циклом
        restartMainWindow = false;
    } while (true);

    return 0;
}

#include <QApplication>
#include <QString>
#include "mainwindow.h"
#include "ModeSelectionDialog.h"
#include "formmode.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    bool restartMainWindow = false;
    QString previousDbType = "";
    do {
        ModeSelectionDialog modeDialog;

        if (!previousDbType.isEmpty()) {
            modeDialog.setInitialDbType(previousDbType);
        }

        if (modeDialog.exec() != QDialog::Accepted) {
            break;
        }

        // Получаем выбранный режим и тип базы данных
        MainWindow::FormMode mode = static_cast<MainWindow::FormMode>(modeDialog.selectedMode());
        QString dbType = modeDialog.selectedDbType();

        // Создаем главное окно, передавая оба параметра
        MainWindow *w = new MainWindow(mode, dbType);
        QObject::connect(w, &MainWindow::backRequested, [&](const QString &returnedDbType) {
            restartMainWindow = true;
            previousDbType = returnedDbType;
        });

        w->show();
        int ret = app.exec();
        delete w;

        if (!restartMainWindow)
            break;

        restartMainWindow = false;
    } while (true);

    return 0;
}

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Инициализация Qt-приложения

    QMessageBox msgBox;
    msgBox.setText("Hello, World!");
    msgBox.exec(); // Показываем всплывающее окно

    return app.exec();
}

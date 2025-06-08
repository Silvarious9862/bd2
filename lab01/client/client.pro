QT += core gui widgets network

CONFIG += c++11

TARGET = Client
TEMPLATE = app

# Пути к заголовочным файлам
INCLUDEPATH += include

# Исходные файлы проекта
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/networkclient.cpp

# Заголовочные файлы
HEADERS += \
    include/mainwindow.h \
    include/networkclient.h

# UI-файлы (если они находятся в папке src, иначе поправьте путь)
FORMS += \
    src/mainwindow.ui

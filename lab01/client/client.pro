QT += core gui widgets network

CONFIG += c++11

TARGET = Client
TEMPLATE = app

# Пути к заголовочным файлам
INCLUDEPATH += include

# Исходные файлы проекта
SOURCES += \
    src/ModeSelectionDialog.cpp \
    src/RecordEditDialog.cpp \
    src/RecordViewDialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/networkclient.cpp

# Заголовочные файлы
HEADERS += \
    include/ModeSelectionDialog.h \
    include/RecordEditDialog.h \
    include/RecordViewDialog.h \
    include/formmode.h \
    include/mainwindow.h \
    include/networkclient.h \
    include/record.h

# UI-файлы
FORMS += \
    ModeSelectionDialog.ui \
    src/RecordEditDialog.ui \
    src/RecordViewDialog.ui \
    src/mainwindow.ui

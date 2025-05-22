QT += core gui widgets

CONFIG += c++17

INCLUDEPATH += include   # Добавляем путь к заголовочным файлам

SOURCES += src/main.cpp src/mainwindow.cpp
HEADERS += include/mainwindow.h  # Указываем заголовочный файл явно

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    client_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

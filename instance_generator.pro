QT       += core gui widgets

TARGET = instance_generator
TEMPLATE = app

FORMS    += \
    ui/histogram.ui \
    ui/mainwindow.ui \

HEADERS  += \
    src/histogram.h \
    src/mainwindow.h \

SOURCES += \
    src/histogram.cpp \
    src/main.cpp \
    src/mainwindow.cpp \

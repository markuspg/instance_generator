#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T15:02:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = instance_generator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    histogram.cpp

HEADERS  += mainwindow.h \
    histogram.h

FORMS    += mainwindow.ui \
    histogram.ui

QMAKE_CXXFLAGS += -std=c++11

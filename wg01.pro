#-------------------------------------------------
#
# Project created by QtCreator 2015-07-18T22:29:22
#
#-------------------------------------------------

QT       += core gui
QT += webenginewidgets
QT += webkitwidgets
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wg01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    systray.qrc

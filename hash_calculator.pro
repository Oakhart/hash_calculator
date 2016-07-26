#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T09:29:19
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hash_calculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hashcalculator.cpp \
    pleasewaitdialog.cpp \
    resultdialog.cpp

HEADERS  += mainwindow.h \
    hashcalculator.h \
    pleasewaitdialog.h \
    resultdialog.h

FORMS    += mainwindow.ui \
    pleasewaitdialog.ui \
    resultdialog.ui

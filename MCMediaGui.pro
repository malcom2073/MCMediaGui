#-------------------------------------------------
#
# Project created by QtCreator 2018-05-19T17:12:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MCMediaGui
TEMPLATE = app

include(../MCIPC/MCIPC.pri)
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

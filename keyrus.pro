#-------------------------------------------------
#
# Project created by QtCreator 2014-11-21T02:07:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = keyrus
TEMPLATE = app


SOURCES += main.cpp\
    overlay.cpp

HEADERS  += overlay.h
LIBS += -lX11

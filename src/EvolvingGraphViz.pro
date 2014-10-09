#-------------------------------------------------
#
# Project created by QtCreator 2014-07-18T11:13:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EvolvingGraphViz
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    binaryevolvinggraphreader.cpp \
    graphviewer.cpp \
    agent.cpp

HEADERS  += mainwindow.h \
    binaryevolvinggraphreader.h \
    graphviewer.h \
    agent.h

FORMS    += mainwindow.ui

#-------------------------------------------------
#
# Project created by QtCreator 2014-07-18T11:13:52
#
#-------------------------------------------------

QT       += core gui positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EvolvingGraphViz
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.9


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

#-------------------------------------------------
#
# Project created by QtCreator 2016-10-17T21:54:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BCrypt
TEMPLATE = app

INCLUDEPATH += /usr/local/include/botan-1.11
LIBS += -L/usr/local/include/botan-1.11/botan -LBotan

SOURCES += main.cpp\
    mainwindow.cpp \
    password.cpp

HEADERS  += mainwindow.h \
    password.h

FORMS    += mainwindow.ui

DISTFILES +=

QMAKE_CXXFLAGS += -std=c++11

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lbotan-1.11

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libbotan-1.11.a

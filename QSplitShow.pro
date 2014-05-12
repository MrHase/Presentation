#-------------------------------------------------
#
# Project created by QtCreator 2014-04-23T11:14:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSplitShow
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    pdfrenderer.cpp \
    presentation.cpp \
    splitpresentation.cpp

HEADERS  += mainwindow.h \
    pdfrenderer.h \
    presentation.h \
    splitpresentation.h

FORMS    += mainwindow.ui

unix|win32: LIBS += -lpoppler-qt5

INCLUDEPATH += \
    /usr/local/include

QMAKE_LIBDIR += \
    /usr/local/lib

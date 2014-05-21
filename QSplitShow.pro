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
    fullscreenpresentation.cpp \
    eventhandler.cpp \
    helperscreenpresentation.cpp \
    exception.cpp

HEADERS  += mainwindow.h \
    pdfrenderer.h \
    presentation.h \
    fullscreenpresentation.h \
    eventhandler.h \
    helperscreenpresentation.h \
    exception.h

FORMS    += mainwindow.ui \
    fullscreenpresentation.ui \
    helperscreenpresentation.ui

unix|win32: LIBS += -lpoppler-qt5

INCLUDEPATH += \
    /usr/local/include

QMAKE_LIBDIR += \
    /usr/local/lib

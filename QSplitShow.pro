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
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    presentation.cpp \
    fullscreenpresentation.cpp \
    eventhandler.cpp \
    exception.cpp \
    dynamicpdfpagecache.cpp \
    lecturerscreen.cpp \
    cache.cpp

HEADERS  += mainwindow.h \
    presentation.h \
    fullscreenpresentation.h \
    eventhandler.h \
    exception.h \
    dynamicpdfpagecache.h \
    lecturerscreen.h \
    cache.h

FORMS    += mainwindow.ui \
    fullscreenpresentation.ui \
    lecturerscreen.ui

unix|win32: LIBS += -lpoppler-qt5

INCLUDEPATH += \
    /usr/local/include

QMAKE_LIBDIR += \
    /usr/local/lib

macx: {
    QMAKE_INFO_PLIST = QSplitShow.plist
}

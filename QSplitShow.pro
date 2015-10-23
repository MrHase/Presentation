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
#QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    presentation.cpp \
    fullscreenpresentation.cpp \
    eventhandler.cpp \
    exception.cpp \
    dynamicpdfpagecache.cpp \
    lecturerscreen.cpp \
    cache.cpp \
    filecache.cpp

HEADERS  += mainwindow.h \
    presentation.h \
    fullscreenpresentation.h \
    eventhandler.h \
    exception.h \
    dynamicpdfpagecache.h \
    lecturerscreen.h \
    cache.h \
    filecache.h

FORMS    += mainwindow.ui \
    fullscreenpresentation.ui \
    lecturerscreen.ui

win32: {
    INCLUDEPATH += \
        D:/DEV/Workspaces/workspace_qtcreator/Presentation/windows_x64/lib/poppler/include
    LIBS += \
        D:/DEV/Workspaces/workspace_qtcreator/Presentation/windows_x64/lib/poppler/bin/Release/poppler-qt5.lib
}

unix: {
    INCLUDEPATH += \
        /usr/local/include
    LIBS += \
        -lpoppler-qt5
}

QMAKE_LIBDIR += \
    /usr/local/lib

macx: {
    QMAKE_INFO_PLIST = QSplitShow.plist
}

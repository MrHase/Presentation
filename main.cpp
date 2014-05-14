#include "mainwindow.h"
#include <QApplication>
#include "eventhandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   
    MainWindow w;
    EventHandler e(&w);

    a.installEventFilter(&e);


    w.show();


    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include "eventhandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   
    MainWindow w;
    EventHandler e(&w);

    //HighRes on MAC?
//    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.installEventFilter(&e);


    w.show();


    return a.exec();
}

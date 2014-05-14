#include "eventhandler.h"
#include <QKeyEvent>

EventHandler::EventHandler(MainWindow *main):
    mainWindow(main)
{

}

bool EventHandler::eventFilter (QObject * o, QEvent * event)
{
    if ((event->type() == QEvent::KeyRelease) || (event->type() == QEvent::KeyPress) )
    {
        if(event->type()==QEvent::KeyRelease){
            QKeyEvent *key=static_cast<QKeyEvent*>(event);
            switch(key->key()){
            case Qt::Key_Up:
                qDebug()<<"UP";
                mainWindow->presentation.previousPage();
                mainWindow->updatePresentation();
                break;
            case Qt::Key_Down:
                qDebug()<<"Down";
                mainWindow->presentation.nextPage();
                break;
            case Qt::Key_Space:
                qDebug()<<"Space";
                mainWindow->presentation.nextPage();
                break;
            case Qt::Key_F5:
                qDebug()<<"F5";
                mainWindow->togglePresentation();
                break;
            case Qt::Key_Escape:
                qDebug()<<"F5";
                mainWindow->stopPresentation();
                break;
            }
        }
        qDebug()<<"Event handled...." ;
        mainWindow->updatePresentation();
        return true;
    }
    return false;
}

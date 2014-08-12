#ifndef HELPERSCREENPRESENTATION_H
#define HELPERSCREENPRESENTATION_H

#include <QWidget>
#include <QGraphicsScene>
#include <memory>
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsItem>
#include <QTimer>
#include <QTime>
#include <QResizeEvent>
#include <QShowEvent>

#include <memory>
#include <vector>
#include "presentation.h"


using namespace std;

namespace Ui {
class lecturerscreen;
}

using namespace std;

class LecturerScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LecturerScreen(QWidget *parent = 0, Presentation *aPresentation = nullptr);
    ~LecturerScreen();



    //!UGLY!!! maybe inheritance useful?
    //!                      /-LecturerScreen
    //! PresentationWidget <-
    //!                      \-FullScreen
    //! ???????????????????????????????????
    void setImage(QImage image);


    void updatePresentation(); //! evtl als slot?

    virtual void resizeEvent(QResizeEvent * event)
    {
        event->accept();
        updatePresentation();
    }
    virtual void showEvent(QShowEvent * event)
    {
        event->accept();
        updatePresentation();
    }

    QSize getPresentationWidgetSize();
    void setHelperScreenScene(const QGraphicsScene &value);

private slots:
    void on_pushButton_clicked();
    void updateClockWidget();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::lecturerscreen *ui;
    QGraphicsScene helperScreenScene;
    QImage image;

    bool timerIsRunning;
    QTimer timer;
    int seconds = 0;
    int minutes = 0;

    Presentation *presentation;

};

#endif // HELPERSCREENPRESENTATION_H

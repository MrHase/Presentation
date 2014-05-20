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

namespace Ui {
class helperscreenpresentation;
}

class HelperScreenPresentation : public QWidget
{
    Q_OBJECT

public:
    explicit HelperScreenPresentation(QWidget *parent = 0);
    ~HelperScreenPresentation();



    //!UGLY!!! maybe inheritance useful?
    //!                      /-HelperScreen
    //! PresentationWidget <-
    //!                      \-FullScreen
    //! ???????????????????????????????????
    void setImage(QImage image);


    void updatePresentation(); //! evtl als slot?

    virtual void resizeEvent(QResizeEvent * event){updatePresentation();}
    virtual void showEvent(QShowEvent *){updatePresentation();}



private slots:
    void on_pushButton_clicked();
    void updateClockWidget();

private:
    Ui::helperscreenpresentation *ui;
    QGraphicsScene scene;
    QImage image;

    bool timerIsRunning;
    QTimer timer;
    int seconds = 0;
    int minutes = 0;

};

#endif // HELPERSCREENPRESENTATION_H

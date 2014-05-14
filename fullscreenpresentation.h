#ifndef FULLSCREENPRESENTATION_H
#define FULLSCREENPRESENTATION_H

#include <QWidget>
#include <QGraphicsScene>
#include <memory>
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsItem>

using namespace std;


namespace Ui {
class FullScreenPresentation;
}

class FullScreenPresentation : public QWidget
{
    Q_OBJECT

public:
    explicit FullScreenPresentation(QWidget *parent = 0);
    ~FullScreenPresentation();



    void setImage(QImage image);


    void updatePresentation(); //! evtl als slot?

    virtual void resizeEvent(QResizeEvent * event){updatePresentation();}
    virtual void showEvent(QShowEvent *){updatePresentation();}

private:
    Ui::FullScreenPresentation *ui;
    QGraphicsScene fullscreenScene;
    QImage image;
};

#endif // FULLSCREENPRESENTATION_H

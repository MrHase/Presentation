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

    void setScene(QGraphicsScene *scene);
    void setImageToWidgetSize();

    void setImageToWidget(QImage image);

private:
    Ui::FullScreenPresentation *ui;
    QGraphicsScene fullscreenScene;
};

#endif // FULLSCREENPRESENTATION_H

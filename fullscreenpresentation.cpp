#include "fullscreenpresentation.h"
#include "ui_fullscreenpresentation.h"

FullScreenPresentation::FullScreenPresentation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullScreenPresentation)
{
    ui->setupUi(this);
}

FullScreenPresentation::~FullScreenPresentation()
{
    delete ui;
}

void FullScreenPresentation::setScene(QGraphicsScene *scene)
{
    ui->graphicsView->setScene(scene);
}

void FullScreenPresentation::setImageToWidgetSize()
{

}

void FullScreenPresentation::setImageToWidget(QImage image)
{
    fullscreenScene.clear();

    QSize widgetSize = this->size();
    QImage newImage = image.scaled(widgetSize, Qt::KeepAspectRatio);

    qDebug()<<"Widgetsize "<< widgetSize.width() << "     .... " << widgetSize.height();

    fullscreenScene.addPixmap(QPixmap::fromImage(newImage));
    fullscreenScene.setSceneRect(0,0,newImage.size().width(),newImage.size().height());
    ui->graphicsView->setScene(&fullscreenScene);
}

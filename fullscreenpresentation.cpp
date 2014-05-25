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


void FullScreenPresentation::setImage(QImage image)
{
    this->image=image;
    updatePresentation();
}



void FullScreenPresentation::updatePresentation()
{
    fullscreenScene.clear();
//    we need to set the Background to black... first try here
//    fullscreenScene.setBackgroundBrush(QBrush(Qt::black));

    QSize widgetSize = this->size();
    QImage newImage = image.scaled(widgetSize, Qt::KeepAspectRatio);

//    newImage.setDevicePixelRatio(2.0);

    qDebug()<<"Widgetsize "<< widgetSize.width() << "     .... " << widgetSize.height();

    QPixmap pixMap = QPixmap::fromImage(newImage);
//    map.setDevicePixelRatio(2.0);

    fullscreenScene.addPixmap(pixMap);
    fullscreenScene.setSceneRect(0,0,newImage.size().width(),newImage.size().height());
    ui->graphicsView->setScene(&fullscreenScene);
}

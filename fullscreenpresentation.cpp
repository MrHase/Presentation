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

    image.setDevicePixelRatio(this->devicePixelRatio());

    QPixmap pixMap = QPixmap::fromImage(image);


    QRect rect = image.rect();
    rect.setHeight(rect.height()/this->devicePixelRatio());
    rect.setWidth(rect.width()/this->devicePixelRatio());

    fullscreenScene.addPixmap(pixMap);

    fullscreenScene.setSceneRect(rect);


//    fullscreenScene.setSceneRect(0,0,this->size().width(),this->size().height());

    ui->graphicsView->setScene(&fullscreenScene);
//    ui->graphicsView->fitInView(item);
}

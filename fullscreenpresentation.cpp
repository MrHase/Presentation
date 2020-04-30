#include "fullscreenpresentation.h"
#include "ui_fullscreenpresentation.h"

FullScreenPresentation::FullScreenPresentation(QWidget* parent) : QWidget(parent), ui(new Ui::FullScreenPresentation)
{
	ui->setupUi(this);
}

FullScreenPresentation::~FullScreenPresentation()
{
	delete ui;
}

void FullScreenPresentation::setImage(QImage image)
{
	this->image = image;
	updatePresentation();
}

void FullScreenPresentation::updatePresentation()
{
	fullscreenScene.clear();

	image.setDevicePixelRatio(this->devicePixelRatio());

	QPixmap pixMap = QPixmap::fromImage(image);

	// to be able to use high-dpi displays we need to adjust the rectangle...
	QRect rect = image.rect();
	rect.setHeight(rect.height() / devicePixelRatio());
	rect.setWidth(rect.width() / devicePixelRatio());

	fullscreenScene.addPixmap(pixMap);
	fullscreenScene.setSceneRect(rect);

	ui->graphicsView->setScene(&fullscreenScene);
}

QSize FullScreenPresentation::getPresentationWidgetSize()
{
	return ui->graphicsView->size();
}

#ifndef FULLSCREENPRESENTATION_H
#define FULLSCREENPRESENTATION_H

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWidget>
#include <memory>
using namespace std;

namespace Ui {
class FullScreenPresentation;
}

class FullScreenPresentation : public QWidget
{
	Q_OBJECT

public:
	explicit FullScreenPresentation(QWidget* parent = 0);
	~FullScreenPresentation();

	void setImage(QImage image);

	void updatePresentation();  //! evtl als slot?

	virtual void resizeEvent(QResizeEvent* event)
	{
		event->accept();
		updatePresentation();
	}
	virtual void showEvent(QShowEvent* event)
	{
		event->accept();
		updatePresentation();
	}

	QSize getPresentationWidgetSize();

private:
	Ui::FullScreenPresentation* ui;
	QGraphicsScene fullscreenScene;
	QImage image;
};

#endif  // FULLSCREENPRESENTATION_H

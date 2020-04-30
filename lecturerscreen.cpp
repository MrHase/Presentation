#include "lecturerscreen.h"
#include "ui_lecturerscreen.h"

LecturerScreen::LecturerScreen(QWidget* parent, Presentation* aPresentation, QTime aTargetPresetationTime)
    : QWidget(parent)
    , ui(new Ui::lecturerscreen)
    , timerIsRunning(false)
    , timer(this)
    , presentation(aPresentation)
    , targetPresentationTime(aTargetPresetationTime)
    , currentPresentationTime(QTime(0, 0, 0))
{
	ui->setupUi(this);
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateClockAndTimeProgressWidget()));

	if (presentation == nullptr)
	{
		qDebug() << "Error..... presentation was null... should never happen";
		exit(EXIT_FAILURE);
	}

	// set thumbnails to the preview list
	vector<QImage> thumbs = presentation->GetThumbnails();
	for (uint8_t i = 0; i < thumbs.size(); i++)
	{
		QString s             = tr("Slide") + " " + QString::number(i);
		QListWidgetItem* item = new QListWidgetItem(s, ui->pageList);
		item->setData(Qt::DecorationRole, QPixmap::fromImage(thumbs[i]));
	}

	ui->pageList->setCurrentRow(presentation->CurrentPage());
	ui->pageProgress->setValue(presentation->Progress());

	//    connect(presentation,&Presentation::pageChanged,[&](int index)
	//    {
	//        cout<<"New Index: "<<index<<endl;
	//        ui->pageList->setCurrentRow(index);
	//        ui->pageProgress->setValue(presentation->Progress());

	//    });
	// we do not use the lambda function because it does not get disconnected when the lecturerScreen is closed
	connect(presentation, SIGNAL(pageChanged(int)), this, SLOT(slot_pageChanged(int)));

	//    QTime time (0,minutes,seconds);
	QString text = currentPresentationTime.toString("mm:ss");
	ui->lcdNumber->display(text);
}

void LecturerScreen::slot_pageChanged(int index)
{
	ui->pageList->setCurrentRow(index);
	ui->pageProgress->setValue(presentation->Progress());
}

LecturerScreen::~LecturerScreen()
{
	// disconnect()
	delete ui;
}

void LecturerScreen::setImage(QImage image)
{
	this->image = image;
	updatePresentation();
}

void LecturerScreen::updatePresentation()
{
	helperScreenScene.clear();

	image.setDevicePixelRatio(this->devicePixelRatio());

	QPixmap pixMap = QPixmap::fromImage(image);

	// to be able to use high-dpi displays we need to adjust the rectangle...
	QRect rect = image.rect();
	rect.setHeight(rect.height() / devicePixelRatio());
	rect.setWidth(rect.width() / devicePixelRatio());

	helperScreenScene.addPixmap(pixMap);
	helperScreenScene.setSceneRect(rect);
	ui->graphicsView->setScene(&helperScreenScene);
}

QSize LecturerScreen::getPresentationWidgetSize()
{
	return ui->graphicsView->size();
}

void LecturerScreen::on_pushButton_clicked()
{
	if (!timerIsRunning)
	{
		timerIsRunning = true;
		ui->pushButton->setText("Stop Timer");

		// run every second
		timer.start(1000);
	}
	else
	{
		timer.stop();
		timerIsRunning = false;
		ui->pushButton->setText("Start Timer");

		// rest timer views
		currentPresentationTime.setHMS(0, 0, 0);
		ui->progressBar_2->setValue(0);
		ui->lcdNumber->setStyleSheet("color: rgb(128, 255, 0);");
	}
}

void LecturerScreen::updateClockAndTimeProgressWidget()
{
	currentPresentationTime = currentPresentationTime.addSecs(1);

	QString text = currentPresentationTime.toString("mm:ss");
	if ((currentPresentationTime.second() % 2) == 0)
	{
		text[2] = ' ';
	}

	// now update the widgets
	ui->lcdNumber->display(text);
	ui->progressBar_2->setValue(calculateTimeProgressInPercent());

	// set color of lcd to a error red if we've run out of time
	if (currentPresentationTime >= targetPresentationTime)
	{
		ui->lcdNumber->setStyleSheet("color: rgb(255, 58, 32)");
	}
}

void LecturerScreen::on_pageList_clicked(const QModelIndex& index)
{
	presentation->goToPage(index.row());
}

int LecturerScreen::calculateTimeProgressInPercent()
{
	int currentSeconds = currentPresentationTime.hour() * 3600 + currentPresentationTime.minute() * 60 +
	                     currentPresentationTime.second();
	int targetSeconds =
	    targetPresentationTime.hour() * 3600 + targetPresentationTime.minute() * 60 + targetPresentationTime.second();
	//    int targetSeconds = QTime(0, 0, 0).secsTo(targetPresentationTime);
	int ret = ((float)currentSeconds / (float)targetSeconds) * 100.0;
	qDebug() << "TimeProgress in Percent: " << ret;
	return ret;
}

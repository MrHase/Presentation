#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <poppler/qt5/poppler-qt5.h>

//#include <QDesktopWidget>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QScreen>
#include "exception.h"

constexpr auto NO_OUTPUT = "No output";

QString Orientation(Qt::ScreenOrientation orientation)
{
	switch (orientation)
	{
		case Qt::PrimaryOrientation:
			return "Primary";
		case Qt::LandscapeOrientation:
			return "Landscape";
		case Qt::PortraitOrientation:
			return "Portrait";
		case Qt::InvertedLandscapeOrientation:
			return "Inverted landscape";
		case Qt::InvertedPortraitOrientation:
			return "Inverted portrait";
		default:
			return "Unknown";
	}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), split(false), presentationRunning(false), presentation()
//    helperScreen(0)
//    scene(new QGraphicsScene(this)),
//    scene_left(new QGraphicsScene(this)),
//    scene_right(new QGraphicsScene(this))
{
	ui->setupUi(this);

	qDebug() << "Number of screens:" << QGuiApplication::screens().size();

	qDebug() << "Primary screen: " << QGuiApplication::primaryScreen()->name();

	//    foreach (QScreen *screen, QGuiApplication::screens()) {
	//        qDebug() << "Information for screen:" << screen->name();
	//        qDebug() << "  Available geometry:" << screen->availableGeometry().x() << screen->availableGeometry().y()
	//        << screen->availableGeometry().width() << "x" << screen->availableGeometry().height(); qDebug() << "
	//        Available size:" << screen->availableSize().width() << "x" << screen->availableSize().height(); qDebug()
	//        << "  Available virtual geometry:" << screen->availableVirtualGeometry().x() <<
	//        screen->availableVirtualGeometry().y() << screen->availableVirtualGeometry().width() << "x" <<
	//        screen->availableVirtualGeometry().height(); qDebug() << "  Available virtual size:" <<
	//        screen->availableVirtualSize().width() << "x" << screen->availableVirtualSize().height(); qDebug() << "
	//        Depth:" << screen->depth() << "bits"; qDebug() << "  Geometry:" << screen->geometry().x() <<
	//        screen->geometry().y() << screen->geometry().width() << "x" << screen->geometry().height(); qDebug() << "
	//        Logical DPI:" << screen->logicalDotsPerInch(); qDebug() << "  Logical DPI X:" <<
	//        screen->logicalDotsPerInchX(); qDebug() << "  Logical DPI Y:" << screen->logicalDotsPerInchY(); qDebug()
	//        << "  Orientation:" << Orientation(screen->orientation()); qDebug() << "  Physical DPI:" <<
	//        screen->physicalDotsPerInch(); qDebug() << "  Physical DPI X:" << screen->physicalDotsPerInchX(); qDebug()
	//        << "  Physical DPI Y:" << screen->physicalDotsPerInchY(); qDebug() << "  Physical size:" <<
	//        screen->physicalSize().width() << "x" << screen->physicalSize().height() << "mm"; qDebug() << "  Primary
	//        orientation:" << Orientation(screen->primaryOrientation()); qDebug() << "  Refresh rate:" <<
	//        screen->refreshRate() << "Hz"; qDebug() << "  Size:" << screen->size().width() << "x" <<
	//        screen->size().height(); qDebug() << "  Virtual geometry:" << screen->virtualGeometry().x() <<
	//        screen->virtualGeometry().y() << screen->virtualGeometry().width() << "x" <<
	//        screen->virtualGeometry().height(); qDebug() << "  Virtual size:" << screen->virtualSize().width() << "x"
	//        << screen->virtualSize().height();
	//    }

	updateOutputLists();
	ui->outputList_HelperScreen->setCurrentText(
	    QGuiApplication::primaryScreen()->name());  // set the rer output to the primary screen

	//    ui->outputList_HelperScreen->setCurrentText("This should not change anything");

	connect(&presentation, &Presentation::pageChanged, [&](int index) {
		qDebug() << "PAGE CHANGED!";
		ui->pageList->setCurrentRow(index);
		updatePresentation();
	});

	connect((const QGuiApplication*)QGuiApplication::instance(), &QGuiApplication::screenAdded, [&](QScreen* screen) {
		qDebug() << "QScreen added: " << screen->name();
		// std::cout<<"QScreen added: "<<screen->name()<<endl;
		//! refresh the output list!
	});
	// screenAdded(QScreen * screen)

	Reset();
}

MainWindow::~MainWindow()
{
	delete ui;
}

// void MainWindow::keyReleaseEvent(QKeyEvent *event)
//{

//    //updatePresentation();
//}

void MainWindow::updatePresentation()
{
	scene_left.clear();
	scene_right.clear();

	//    QImage leftSide, rightSide;

	//    if (split)
	//    {
	//        leftSide = presentation.getLeftSideOfPreviewPage();
	//        rightSide = presentation.getRightSideOfPreviewPage();
	//    }
	//    else
	//    {
	//        leftSide=presentation.preview_getCurrentPage();
	//        rightSide=presentation.preview_getCurrentPage();
	//}

	QImage leftSide  = (split) ? presentation.getLeftSideOfPreviewPage() : presentation.preview_getCurrentPage();
	QImage rightSide = (split) ? presentation.getRightSideOfPreviewPage() : presentation.preview_getCurrentPage();

	leftSide.setDevicePixelRatio(this->devicePixelRatio());
	rightSide.setDevicePixelRatio(this->devicePixelRatio());

	scene_left.addPixmap(QPixmap::fromImage(leftSide));
	scene_right.addPixmap(QPixmap::fromImage(rightSide));

	ui->graphicsView_left->setScene(&scene_left);
	ui->graphicsView_right->setScene(&scene_right);

	//! maybe we should do this in mainScreenPresentation. We could just listen to the pageChanged signal
	//! ugly?
	if (mainScreenPresentation)
	{
		if (split)
		{
			mainScreenPresentation->setImage(presentation.getLeftSideOfMainScreen());
		}
		else
		{
			mainScreenPresentation->setImage(presentation.getCurrentMainScreen());
			//! TBD
			//            mainScreenPresentation->setImage(presentation.get());
		}
	}

	//! ugly?
	if (lecturerScreen)
	{
		// if split is active we need to figure out, which side of the page we need to put in to the windo
		if (split)
		{
			lecturerScreen->setImage(presentation.getRightSideOfLecturerScreen());
		}
		else
		{
			lecturerScreen->setImage(presentation.getCurrentLectureScreen());
		}
	}
}

void MainWindow::updateOutputLists()
{
	//! wir müssen uns hier die ausgewählten bildschirme merken bevor wir die liste refreshen um sie danach wieder
	//! setzten zu können

	//! ausserdem muss beim ersten aufruf der helper screen auf den screen gesetzt werden wo das programm ausgeführt
	//! wurde Presentation muss beim ersten aufruf auf no_output gesetzt werden es sei denn es gibt einen weiteren
	//! screen....

	ui->outputList_MainScreen->addItem(NO_OUTPUT);
	ui->outputList_HelperScreen->addItem(NO_OUTPUT);

	for (auto* screen : QGuiApplication::screens())
	{
		ui->outputList_MainScreen->addItem(screen->name());
		ui->outputList_HelperScreen->addItem(screen->name());
	}
}

void MainWindow::Reset()
{
	//! thumbnail list clear

	//    if(ui->cb_splitPDF->isChecked())
	//        ui->cb_splitPDF->toggle();

	if (ui->chkBoxSplitScreen->isChecked())
		ui->chkBoxSplitScreen->toggle();
	ui->graphicsView_right->setVisible(false);
}

QScreen* MainWindow::getMainPresentationScreen()
{
	//! also save the pointer in the outputLists?
	QString name = ui->outputList_MainScreen->currentText();

	if (name == NO_OUTPUT)
		return nullptr;

	for (auto* screen : QGuiApplication::screens())
	{
		if (name == screen->name())
		{
			return screen;
		}
	}

	throw InconsitentScreenException();
	return nullptr;
}

QScreen* MainWindow::getHelperScreen()
{
	QString name = ui->outputList_HelperScreen->currentText();

	if (name == NO_OUTPUT)
		return nullptr;

	for (auto* screen : QGuiApplication::screens())
	{
		if (name == screen->name())
		{
			return screen;
		}
	}

	throw InconsitentScreenException();
	return nullptr;
}

void MainWindow::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Files(*.pdf)"));

	if (filename == "")
	{
		return;
	}

	Reset();

	// set the document file, for the presentation
	presentation.setDocumentFile(filename);

	// sets the rectangle for the preview in order to render the document for the right size
	presentation.setPreview_size(ui->graphicsView_left->size());

	// sets the previewDocument

	//! the cache is initialized here... not very intuitiv
	//! no, not really -.-
	presentation.setPreviewDocument(this->devicePixelRatio(), (ui->chkBoxSplitScreen->isChecked()) ? 2 : 1);

	// updates the presentation
	//    vector<QImage> thumbs;

	//    //!muss auch irgendwie schöner gehen, oder??
	//    while(presentation.getThumbnailsFromDocument().size() == 0){
	//        usleep(50000);
	//    }
	//    thumbs = presentation.getThumbnailsFromDocument();

	vector<QImage> thumbs = presentation.GetThumbnails();
	for (uint8_t i = 0; i < thumbs.size(); i++)
	{
		qDebug() << "adding thumbnail " << i;
		QString s             = tr("Slide") + " " + QString::number(i);
		QListWidgetItem* item = new QListWidgetItem(s, ui->pageList);
		item->setData(Qt::DecorationRole, QPixmap::fromImage(thumbs[i]));
	}
	ui->pageList->setCurrentRow(0);

	//! TODO fileLoaded must be set to false at some point, if we can open files twice :)
	fileLoaded = true;
	ui->chkBoxSplitScreen->setEnabled(true);

	updatePresentation();
}

// void MainWindow::on_actionStart_Presentation_F5_triggered()
//{
//    if (!presentationRunning)
//    {
//        presentationRunning = true;
//        ui->menuPresentation->t
//        startPresentation();
//    }
//}

void MainWindow::startPresentation()
{
	qDebug() << "Start Presentation";

	// Check if everthing is ready to start the presentation:

	//! presentation and document was set before?

	QScreen* screen_main   = nullptr;
	QScreen* screen_helper = nullptr;
	try
	{
		screen_main   = getMainPresentationScreen();
		screen_helper = getHelperScreen();
	}
	catch (exception& e)
	{
		QMessageBox msgBox;
		std::cerr << e.what() << std::endl;
		msgBox.setText(
		    "The output device list is inconsistent and will be refreshed now. Please select the right output device "
		    "and try again.");
		msgBox.exec();
		updateOutputLists();
		return;
	}

	if (!screen_main && !screen_helper)
	{
		QMessageBox msgBox;
		msgBox.setText("No output screen is selected.");
		msgBox.exec();
		return;
	}

	if (screen_main == screen_helper)
	{  //! pointervergleich, ist das ok so? sollte eig...
		QMessageBox msgBox;
		msgBox.setText("Please choose different output devices for the presentation windows");
		msgBox.exec();
		return;
	}

	// Everthing seems fine! lets go!
	presentationRunning = true;
	ui->actionToggle_Presentation_F5->setText("Stop Presentation F5");

	presentation.goToPage(ui->pageList->currentRow());

	if (screen_main)
	{
		mainScreenPresentation = new FullScreenPresentation(0);
		moveWidgetToScreenAndShowFullScreen(mainScreenPresentation, screen_main);
		qDebug() << "main screen resulution: h:" << mainScreenPresentation->getPresentationWidgetSize().height()
		         << "w: " << mainScreenPresentation->getPresentationWidgetSize().width()
		         << "dpri: " << mainScreenPresentation->devicePixelRatio();

		// set the sizes of the presentation attributes
		presentation.setMain_size(mainScreenPresentation->getPresentationWidgetSize());
		presentation.setMainScreenDocument(mainScreenPresentation->devicePixelRatio(),
		                                   (ui->chkBoxSplitScreen->isChecked()) ? 2 : 1);
		mainScreenPresentation->setImage(presentation.getLeftSideOfMainScreen());
	}

	if (screen_helper)
	{
		QTime targetPresentationTime = this->ui->timeEdit_targetTime->dateTime().time();
		lecturerScreen               = new LecturerScreen(0, &presentation, targetPresentationTime);

		moveWidgetToScreenAndShowFullScreen(lecturerScreen, screen_helper);
		qDebug() << "helper screen resolution: h:" << lecturerScreen->getPresentationWidgetSize().height()
		         << "w: " << lecturerScreen->getPresentationWidgetSize().width()
		         << "dpri: " << lecturerScreen->devicePixelRatio();

		// set the sizes of the presentation attributes
		presentation.setHelper_size(lecturerScreen->getPresentationWidgetSize());
		presentation.setHelperScreenDocument(lecturerScreen->devicePixelRatio(),
		                                     (ui->chkBoxSplitScreen->isChecked()) ? 2 : 1);
		lecturerScreen->setImage(presentation.getRightSideOfLecturerScreen());
		// updatePresentation();
	}
}

void MainWindow::stopPresentation()
{
	qDebug() << "Stop Presentation";

	presentationRunning = false;
	ui->actionToggle_Presentation_F5->setText("Start Presenation F5");

	if (mainScreenPresentation)
	{
		mainScreenPresentation->showNormal();
		mainScreenPresentation->close();
		delete mainScreenPresentation;
		mainScreenPresentation = nullptr;
	}

	if (lecturerScreen)
	{
		lecturerScreen->showNormal();
		lecturerScreen->close();
		delete lecturerScreen;
		lecturerScreen = nullptr;
	}
}

void MainWindow::moveWidgetToScreenAndShowFullScreen(QWidget* widget, QScreen* screen)
{
	QRect rect = screen->availableGeometry();

	qDebug() << "screeen::::: " << screen->name();
	qDebug() << "rect:::::: w:" << rect.width() << "  h: " << rect.height() << "  x: " << rect.x()
	         << "   y: " << rect.y();

	// create a new point at the beginning of the screen an move the widget to that point
	widget->setGeometry(rect);
	widget->move(QPoint(rect.x(), rect.y()));
	widget->showFullScreen();
}

void MainWindow::next()
{
	presentation.nextPage();
}

void MainWindow::prev()
{
	presentation.previousPage();
}

void MainWindow::on_actionToggle_Presentation_F5_triggered()
{
	togglePresentation();
}

void MainWindow::togglePresentation()
{
	if (!presentationRunning)
	{
		startPresentation();
	}
	else
	{
		stopPresentation();
	}
}

void MainWindow::on_cb_splitPDF_toggled(bool checked)
{
	qDebug() << "Checked: " << checked;
	if (fileLoaded)
	{
		split = checked;

		scene_left.clear();
		scene_right.clear();

		scene_left.setSceneRect(presentation.getRectOfImage(checked));
		scene_right.setSceneRect(presentation.getRectOfImage(checked));

		if (checked)
		{
			ui->graphicsView_right->setVisible(true);
		}
		else
		{
			ui->graphicsView_right->setVisible(false);
		}

		updatePresentation();
	}
	else
	{
		//        ui->cb_splitPDF->setEnabled(false);
	}
}

void MainWindow::on_pageList_clicked(const QModelIndex& index)
{
	presentation.goToPage(index.row());
	updatePresentation();
}

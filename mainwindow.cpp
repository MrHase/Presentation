#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <poppler/qt5/poppler-qt5.h>

//#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>

QString Orientation(Qt::ScreenOrientation orientation)
{
    switch (orientation) {
        case Qt::PrimaryOrientation           : return "Primary";
        case Qt::LandscapeOrientation         : return "Landscape";
        case Qt::PortraitOrientation          : return "Portrait";
        case Qt::InvertedLandscapeOrientation : return "Inverted landscape";
        case Qt::InvertedPortraitOrientation  : return "Inverted portrait";
        default                               : return "Unknown";
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    split(false),
    presentationRunning(false) //,
//    helperScreen(0)
//    scene(new QGraphicsScene(this)),
//    scene_left(new QGraphicsScene(this)),
//    scene_right(new QGraphicsScene(this))
{
    ui->setupUi(this);

    qDebug() << "Number of screens:" << QGuiApplication::screens().size();

    qDebug() << "Primary screen: " << QGuiApplication::primaryScreen()->name();

    foreach (QScreen *screen, QGuiApplication::screens()) {
        qDebug() << "Information for screen:" << screen->name();
        qDebug() << "  Available geometry:" << screen->availableGeometry().x() << screen->availableGeometry().y() << screen->availableGeometry().width() << "x" << screen->availableGeometry().height();
        qDebug() << "  Available size:" << screen->availableSize().width() << "x" << screen->availableSize().height();
        qDebug() << "  Available virtual geometry:" << screen->availableVirtualGeometry().x() << screen->availableVirtualGeometry().y() << screen->availableVirtualGeometry().width() << "x" << screen->availableVirtualGeometry().height();
        qDebug() << "  Available virtual size:" << screen->availableVirtualSize().width() << "x" << screen->availableVirtualSize().height();
        qDebug() << "  Depth:" << screen->depth() << "bits";
        qDebug() << "  Geometry:" << screen->geometry().x() << screen->geometry().y() << screen->geometry().width() << "x" << screen->geometry().height();
        qDebug() << "  Logical DPI:" << screen->logicalDotsPerInch();
        qDebug() << "  Logical DPI X:" << screen->logicalDotsPerInchX();
        qDebug() << "  Logical DPI Y:" << screen->logicalDotsPerInchY();
        qDebug() << "  Orientation:" << Orientation(screen->orientation());
        qDebug() << "  Physical DPI:" << screen->physicalDotsPerInch();
        qDebug() << "  Physical DPI X:" << screen->physicalDotsPerInchX();
        qDebug() << "  Physical DPI Y:" << screen->physicalDotsPerInchY();
        qDebug() << "  Physical size:" << screen->physicalSize().width() << "x" << screen->physicalSize().height() << "mm";
        qDebug() << "  Primary orientation:" << Orientation(screen->primaryOrientation());
        qDebug() << "  Refresh rate:" << screen->refreshRate() << "Hz";
        qDebug() << "  Size:" << screen->size().width() << "x" << screen->size().height();
        qDebug() << "  Virtual geometry:" << screen->virtualGeometry().x() << screen->virtualGeometry().y() << screen->virtualGeometry().width() << "x" << screen->virtualGeometry().height();
        qDebug() << "  Virtual size:" << screen->virtualSize().width() << "x" << screen->virtualSize().height();
    }

    updateOutputLists();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyReleaseEvent(QKeyEvent *event)
{

    //updatePresentation();
}





void MainWindow::updatePresentation()
{

    QImage leftSide, rightSide;

    scene_left.clear();
    scene_right.clear();

    if (split)
    {
        leftSide = presentation.getLeftSideOfPage();
        rightSide = presentation.getRightSideOfPage();
    }
    else
    {
        leftSide=presentation.getCurrentPage();
        rightSide=presentation.getCurrentPage();
    }

    scene_left.addPixmap(QPixmap::fromImage(leftSide));
    scene_right.addPixmap(QPixmap::fromImage(rightSide));

    ui->graphicsView_left->setScene(&scene_left);
    ui->graphicsView_right->setScene(&scene_right);

    //! ugly?
    if (mainScreenPresentation != NULL)
    {
        mainScreenPresentation->setImage(leftSide);
    }

    //! ugly?
    if (helperScreen != NULL)
    {
        helperScreen->setImage(rightSide);
    }
}

void MainWindow::updateOutputLists()
{

    for (auto *screen: QGuiApplication::screens())
    {
        ui->outputList_MainScreen->addItem(screen->name());
        ui->outputList_HelperScreen->addItem(screen->name());
    }

}

QScreen *MainWindow::getMainPresentationScreen()
{
    for (auto *screen: QGuiApplication::screens())
    {
        if(ui->outputList_MainScreen->currentText()==screen->name()){
            return screen;
        }
    }
    return NULL;
}

QScreen *MainWindow::getHelperScreen()
{
    for (auto *screen: QGuiApplication::screens())
    {
        if(ui->outputList_HelperScreen->currentText()==screen->name()){
            return screen;
        }
    }
    return NULL;
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::homePath(),tr("Files(*.pdf)"));

    if(filename==""){
        return;
    }

    presentation.setDocument(filename.toStdString()); //! ugly!

    updatePresentation();

}


//void MainWindow::on_actionStart_Presentation_F5_triggered()
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
    qDebug()<<"Start Presentation";

    //! presentation and document was set before?

    presentationRunning = true;
    ui->actionToggle_Presentation_F5->setText("Stop Presentation F5");

    //create a new Window
    mainScreenPresentation = new FullScreenPresentation(0);
    helperScreen = new HelperScreenPresentation(0);


    if ( getMainPresentationScreen() == NULL)
    {
        qDebug()<<"A selected screen does not exist!";
        //! error msg
        updateOutputLists();
        return;
    }
    else
    {
        moveWidgetToScreenAndShowFullScreen(mainScreenPresentation,getMainPresentationScreen());
    }

    if (getHelperScreen() == NULL)
    {
        qDebug()<<"A selected screen does not exist!";
        //! error msg
        updateOutputLists();
        return;
    }
    else
    {
        moveWidgetToScreenAndShowFullScreen(helperScreen,getHelperScreen());
    }

}

void MainWindow::stopPresentation()
{
    qDebug()<<"Stop Presentation";

    presentationRunning = false;
    ui->actionToggle_Presentation_F5->setText("Start Presenation F5");

    mainScreenPresentation->showNormal();
//    fullScreenPresentation->hide();

    mainScreenPresentation->close();

    helperScreen->showNormal();
    helperScreen->close();

    delete mainScreenPresentation;
    delete helperScreen;
    mainScreenPresentation = NULL;
    helperScreen = NULL;
}

void MainWindow::moveWidgetToScreenAndShowFullScreen(QWidget *widget, QScreen *screen)
{
    QRect rect = screen->availableGeometry();

    qDebug()<<"screeen::::: " << getHelperScreen()->name();
    qDebug()<<"rect:::::: w:" << rect.width() << "  h: " << rect.height() << "  x: " << rect.x() << "   y: " << rect.y();

    //create a new point at the beginning of the screen an move the widget to that point
    widget->setGeometry(rect);
    widget->move(QPoint(rect.x(),rect.y()));
    widget->showFullScreen();
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
    qDebug()<<"Checked: "<<checked;

    split=checked;

    scene_left.clear();
    scene_right.clear();

    updatePresentation();
}

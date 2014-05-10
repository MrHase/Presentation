#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <poppler/qt5/poppler-qt5.h>

//#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>

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
    toggleFullsreen(false),
    pdfRenderer(new PdfRenderer()),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    qDebug() << "Number of screens:" << QGuiApplication::screens().size();

    qDebug() << "Primary screen:" << QGuiApplication::primaryScreen()->name();


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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    ui->widget_right->setParent(0);
//    ui->widget_right->showFullScreen();

//    qDebug()<<"Clicked Right";

}

void MainWindow::on_pushButton_2_clicked()
{
//    ui->widget_left->setParent(0);
//    ui->widget_left->showFullScreen();
//    qDebug()<<"Clicked Left";
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::homePath(),tr("Files(*.pdf)"));

    if(filename==""){
        return;
    }


    pdfRenderer->setDocument(filename);
    QImage rederedPage = pdfRenderer->getRenderedImage(0);

//    QGraphicsScene *scene = new QGraphicsScene(this);

    scene->addPixmap(QPixmap::fromImage(rederedPage));
    ui->graphicsView->setScene(scene.get());


//    delete scene;



//    Poppler::Document *doc = Poppler::Document::load(filename);



//    // Access page of the PDF file
//    Poppler::Page* pdfPage = doc->page(0);  // Document starts at page 0
//    if (pdfPage == 0)
//    {
//        qDebug() << "Error....." ;
//    }

//    QImage image = pdfPage->renderToImage();
//    if (image.isNull()) {
//        qDebug() << "Error....." ;
//    }
//    QGraphicsScene *scene = new QGraphicsScene(this);

//    scene->addPixmap(QPixmap::fromImage(image));
//    ui->graphicsView->setScene(scene);

//    qDebug()<<filename;
}


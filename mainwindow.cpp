#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <poppler/qt5/poppler-qt5.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    toggleFullsreen(false)
{
    ui->setupUi(this);
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
    QString homedirectory = QDir::homePath();
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),homedirectory,tr("Files(*.*)"));

    if(filename==""){
        return;
    }

    Poppler::Document *doc = Poppler::Document::load(filename);



    // Access page of the PDF file
    Poppler::Page* pdfPage = doc->page(0);  // Document starts at page 0
    if (pdfPage == 0)
    {
        qDebug() << "Error....." ;
    }

    QImage image = pdfPage->renderToImage();
    if (image.isNull()) {
        qDebug() << "Error....." ;
    }
    QGraphicsScene *scene = new QGraphicsScene(this);

    scene->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(scene);

    qDebug()<<filename;
}


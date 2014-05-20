#include "helperscreenpresentation.h"
#include "ui_helperscreenpresentation.h"

HelperScreenPresentation::HelperScreenPresentation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helperscreenpresentation),
    timerIsRunning(false),
    timer(this)
{
    ui->setupUi(this);
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateClockWidget()));
}

HelperScreenPresentation::~HelperScreenPresentation()
{
    delete ui;
}

void HelperScreenPresentation::setImage(QImage image)
{
    this->image = image;
    updatePresentation();
}

void HelperScreenPresentation::updatePresentation()
{
    scene.clear();

    QSize widgetSize = this->size();
    QImage newImage = image.scaled(widgetSize, Qt::KeepAspectRatio);

    scene.addPixmap(QPixmap::fromImage(newImage));
    scene.setSceneRect(0,0,newImage.size().width(),newImage.size().height());
    ui->graphicsView->setScene(&scene);
}

void HelperScreenPresentation::on_pushButton_clicked()
{
    if (!timerIsRunning)
    {
        timerIsRunning = true;
        ui->pushButton->setText("StopTimer");

        //run every second
        timer.start(1000);
    }
    else
    {
        timer.stop();
        timerIsRunning = false;
        ui->pushButton->setText("StartTimer");
        seconds = 0;
        minutes = 0;
    }

}

void HelperScreenPresentation::updateClockWidget()
{
    seconds ++;
    if (seconds %60 == 0)
    {
        minutes++;
        seconds = 0;
    }

    QTime time (0,minutes,seconds);
    QString text = time.toString("mm:ss");
    if ((time.second() % 2) == 0)
    {
             text[2] = ' ';
    }
    ui->lcdNumber->display(text);
}

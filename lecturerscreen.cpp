#include "lecturerscreen.h"
#include "ui_lecturerscreen.h"

LecturerScreen::LecturerScreen(QWidget *parent, Presentation *aPresentation) :
    QWidget(parent),
    ui(new Ui::lecturerscreen),
    timerIsRunning(false),
    timer(this),
    presentation(aPresentation)
{
    ui->setupUi(this);
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateClockWidget()));

    if (presentation == nullptr)
    {
        qDebug()<<"Error..... presentation was null... should never happen";
        exit(EXIT_FAILURE);
    }

    // set thumbnails to the preview list
    vector<QImage> thumbs = presentation->getThumbnailsFromDocument();
    for (uint8_t i = 0; i < thumbs.size(); i++)
    {
        QString s = tr("Slide") + " " + QString::number(i);
        QListWidgetItem  *item = new QListWidgetItem(s,ui->pageList);
        item->setData(Qt::DecorationRole,QPixmap::fromImage(thumbs[i]));
    }


    ui->pageList->setCurrentRow(presentation->CurrentPage());
    ui->pageProgress->setValue(presentation->Progress());

    connect(presentation,&Presentation::pageChanged,[&](int index)
    {
        ui->pageList->setCurrentRow(index);
        ui->pageProgress->setValue(presentation->Progress());

    });

    QTime time (0,minutes,seconds);
    QString text = time.toString("mm:ss");
    ui->lcdNumber->display(text);

}

LecturerScreen::~LecturerScreen()
{
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

    //to be able to use high-dpi displays we need to adjust the rectangle...
    QRect rect = image.rect();
    rect.setHeight(rect.height()/devicePixelRatio());
    rect.setWidth(rect.width()/devicePixelRatio());

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

void LecturerScreen::updateClockWidget()
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


void LecturerScreen::on_listWidget_clicked(const QModelIndex &index)
{
    //presentation->goToPage(index.row());
    //emit(pageChanged(index.row()));
    presentation->goToPage(index.row());
}

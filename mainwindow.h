#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <memory>
#include <QGraphicsScene>

#include <QDesktopWidget>

#include "presentation.h"


#include "pdfrenderer.h"
#include "fullscreenpresentation.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    
    friend class EventHandler;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



protected:
    virtual void keyReleaseEvent ( QKeyEvent * event );

private slots:
    void on_pushButton_clicked();
    void on_actionOpen_triggered();
    void on_pushButton_2_clicked();
    void on_actionToggle_Presentation_F5_triggered();

private:

    void updatePresentation();
    void togglePresentation();

    void startPresentation();
    void stopPresentation();


    Ui::MainWindow *ui;
    bool split;
    bool presentationRunning;

    shared_ptr<QGraphicsScene> scene;
    shared_ptr<QGraphicsScene> scene_left;
    shared_ptr<QGraphicsScene> scene_right;

    shared_ptr<FullScreenPresentation> fullScreenPresentation;



    Presentation presentation;
};

#endif // MAINWINDOW_H

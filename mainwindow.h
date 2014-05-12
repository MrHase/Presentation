#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <memory>
#include <QGraphicsScene>

#include "presentation.h"

#include "pdfrenderer.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void  keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent ( QKeyEvent * event );

private slots:
    void on_pushButton_clicked();
    void on_actionOpen_triggered();

    void on_pushButton_2_clicked();

private:

    void updatePresentation();

    Ui::MainWindow *ui;
    bool split;

    shared_ptr<QGraphicsScene> scene;
    shared_ptr<QGraphicsScene> scene_left;
    shared_ptr<QGraphicsScene> scene_right;


    Presentation presentation;
};

#endif // MAINWINDOW_H

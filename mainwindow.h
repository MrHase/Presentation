#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QFileDialog>
#include <QDir>
#include <QDebug>


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

private slots:
    void on_pushButton_clicked();
    void on_actionOpen_triggered();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    bool toggleFullsreen;
};

#endif // MAINWINDOW_H

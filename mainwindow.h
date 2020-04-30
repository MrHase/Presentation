#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QMainWindow>
#include <iostream>
#include <memory>

#include <QDesktopWidget>
#include <QWindow>

#include "presentation.h"
#ifndef _WIN64
#include <unistd.h>
#else
#include <io.h>
#endif

#include "fullscreenpresentation.h"
#include "lecturerscreen.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	friend class EventHandler;

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

protected:
	//    virtual void keyReleaseEvent ( QKeyEvent * event );

private slots:

	void on_actionOpen_triggered();
	void on_actionToggle_Presentation_F5_triggered();

	void on_cb_splitPDF_toggled(bool checked);

	void on_pageList_clicked(const QModelIndex& index);

private:
	bool fileLoaded = false;

	void updatePresentation();
	void updateOutputLists();

	void Reset();

	QScreen* getMainPresentationScreen();
	QScreen* getHelperScreen();

	void togglePresentation();
	void startPresentation();
	void stopPresentation();

	void moveWidgetToScreenAndShowFullScreen(QWidget* widget, QScreen* screen);

	void next();
	void prev();

	Ui::MainWindow* ui;
	bool split;
	bool presentationRunning;

	QGraphicsScene scene;
	QGraphicsScene scene_left;
	QGraphicsScene scene_right;

	FullScreenPresentation* mainScreenPresentation = nullptr;
	LecturerScreen* lecturerScreen                 = nullptr;

	Presentation presentation;
};

#endif  // MAINWINDOW_H

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QDebug>
#include <QEvent>
#include <QObject>
#include "mainwindow.h"

class EventHandler : public QObject
{
	Q_OBJECT
	MainWindow* mainWindow;

public:
	EventHandler(MainWindow* main);

	virtual bool eventFilter(QObject* o, QEvent* event);
signals:

public slots:
};

#endif  // EVENTHANDLER_H

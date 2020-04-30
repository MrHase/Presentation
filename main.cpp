#include <QApplication>
#include "eventhandler.h"
#include "mainwindow.h"

#include <thread>
#include "cache.h"
int main(int argc, char* argv[])
{
//    //! remove/*
//    Cache<int,int> test(3);
//    test.Add(1,10);
//    test.Add(2,20);
//    test.Add(3,30);
//    cout<<test.Status()<<endl;
//    test.Touch(1);
//    test.Add(4,40);
//    cout<<test.Status()<<endl;
//    test.Add(5,50);
//    cout<<test.Status()<<endl;
//    cout<<test.Get(1)<<endl;
//    cout<<test.Status()<<endl;
// exit(1);*/

// disable all ox animations
#if __MACH__

#endif

//	mutex test;
//	cout << "TEST" << endl;
//	test.lock();

	QApplication a(argc, argv);
	MainWindow w;
	EventHandler e(&w);

	// HighRes on MAC?
	//    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
	a.installEventFilter(&e);

	w.show();

// enable all ox animations
#if __MACH__

#endif

	return a.exec();
}

#include "filecache.h"
#include <QBuffer>

FileCache::FileCache()
{
	//! test[index]->close(); is missing;
}

FileCache::~FileCache() {}

QImage FileCache::GetPage(int index)
{
	test[index]->seek(0);
	// QByteArray barray=test[index]->readAll();
	// qDebug()<<"SIZE: "<<barray.size();

	// QImage img(538,202,QImage::Format_ARGB32);
	QImage img;
	bool success = img.loadFromData(test[index]->readAll(), "PNG");
	qDebug() << "XXXX " << success << endl;
	return img;
}

void FileCache::Add(int index, QImage img)
{
	mutex.lock();
	bool success = false;

	qDebug() << "READY: " << index << " w: " << img.width() << " h: " << img.height() << " format: " << img.format();

	QTemporaryFile* tmp = new QTemporaryFile();
	test[index]         = tmp;

	// tmp->setFileTemplate(tmp->fileTemplate() + ".png");
	success = tmp->open();

	qDebug() << "Open success: " << success;
	qDebug() << " FILEEEEEEEEEE NAME :" << tmp->fileName();

	// tmp->setAutoRemove(false);

	// qint64 c=tmp->write(ba);
	// qDebug()<<"Writing success: "<<c;

	//! die bilder sind ca. 2,5 MB gross auf meinem MacBook mit Retina Display...
	//! Wie würde sich das auf anderen Computern verhalten?
	success = img.save(tmp, "PNG");
	qDebug() << "Save success" << success;

	success = tmp->flush();
	qDebug() << "Flush success: " << success;
	// tmp->seek(0);

	// qDebug()<<"CHECK "<<tmp->read(100000).size();

	// tmpFiles[index]->open();
	// img.save(tmpFiles[index]);

	mutex.unlock();
}

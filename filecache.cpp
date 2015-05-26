#include "filecache.h"

FileCache::FileCache()
{

}

QImage FileCache::GetPage(int index)
{
    QImage img;
    bool success=img.loadFromData(test[index]->readAll());
    qDebug()<<"XXXX "<<success<<endl;
    return img;
}

void FileCache::Add(int index, QImage img)
{
    mutex.lock();

    qDebug()<<"READY: "<<index<<endl;
    //tmpFiles[index]=new QTemporaryFile(QString ( "qXXXXXX" ) );
    test[index]=new QTemporaryFile();
    bool open_success=test[index]->open();
    qDebug()<<"Open success: "<<open_success<<endl;
    bool success=img.save(test[index]);
    qDebug()<<"Writing success: "<<success<<endl;

    //tmpFiles[index]->open();
    //img.save(tmpFiles[index]);

    mutex.unlock();
}


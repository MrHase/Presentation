#ifndef FILECACHE_H
#define FILECACHE_H


#include <QTemporaryFile>
#include <QImage>
#include <iostream>
#include <QDebug>
#include <array>
#include <QMutex>

using namespace std;

class FileCache
{
public:
    FileCache();
    virtual ~FileCache();

    QImage GetPage(int index);

    void Add(int index,QImage img);


private:

    //array<QTemporaryFile> tmpFiles;
    map<int,QTemporaryFile*> tmpFiles;
    QTemporaryFile* test[1000];
    QMutex mutex;

};

#endif // FILECACHE_H

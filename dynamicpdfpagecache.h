#ifndef DYNAMICPDFPAGECACHE_H
#define DYNAMICPDFPAGECACHE_H

#include <vector>
#include <QImage>
#include <mutex>

#include <poppler/qt5/poppler-qt5.h>


#include <QDebug>
#include <QRect>

#include <thread>
#include <QString>

#include "cache.h"
#include "filecache.h"

#if (__MACH__ || __linux)
#include <pthread.h>
#elif (__WIN32)
#include <windows.h>
#endif


using namespace std;

class DotsPerInch
{
public:
    double dpiWidth;
    double dpiHeight;
};

class DynamicPdfPageCache
{
public:

    DynamicPdfPageCache(int pixelRatio);

    ~DynamicPdfPageCache();

    //void setDocument(QString docFilePath, double splitscreen);

    void setDocument(Poppler::Document *document, double splitscreen);

    QImage getElementFromPos(int pos);

    void initializeCache(); //! remove

    QSize getDisplaySize() const;
    void setDisplaySize(const QSize &value);

    int getPixelRatio() const;
    void setPixelRatio(int value);

    int getSizeOfDocument() const;



    void deleteAndResetCache();





private:

    const uint8_t DISTANCE_TO_CACHE_BORDER = 8;
//    const uint8_t ELEMENTS_IN_CACHE = (DISTANCE_TO_CACHE_BORDER *2) +1;
    const uint8_t ELEMENTS_IN_CACHE = 8;

    const uint8_t THUMBNAIL_HIGHT_IN_PIXEL = 64; //! remove when thumbnails are created in the presentation.cpp
    const double  DPI_CONSTANT = 72.0;  //! global or something

    Poppler::Document *doc;

    vector<QImage*> pageCache;
    Cache<int,QImage> cache;


    double splitscreen = 1.0;


    int sizeOfDocument = 0;

    QSize displaySize;
    int pixelRatio = 1;

    mutex cache_mutex;
    mutex doc_mutex;

    void renderPageAsThread(int pageNum);
    void deletePageFromCache(int pageNum);

    void renderPage(Poppler::Page *page, int i);

    void renderPageToImage(Poppler::Page *page,int i);

    DotsPerInch calculateDPI(QSize size, Poppler::Page *page);

    FileCache fileCache;



};

#endif // DYNAMICPDFPAGECACHE_H

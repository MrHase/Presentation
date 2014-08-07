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

#if (__MACH__ || __linux)
#include <pthread.h>
#elif (__WIN32)
#include <windows.h>
#endif

#include "pdfrenderer.h"

using namespace std;

//class RenderInfo
//{
//public:
//    uint32_t requested_width = 0; // size in points? ich geh jetzt davon aus
//    uint32_t requested_height = 0;
//    bool splitscreen = false; // two slides on one page
//};

class DotsPerInch
{
public:
    double dpiWidth;
    double dpiHeight;
};



class DynamicPdfPageCache
{
public:
    DynamicPdfPageCache(QSize size, int pixelRatio);
    DynamicPdfPageCache(int pixelRatio);

    ~DynamicPdfPageCache();

    void setDocument(QString docFilePath);

    QImage* getElementFromPos(int pos);
    QImage* getNextElement();
    QImage* getPreviousElement();

    void initializeCache();

    QSize getDisplaySize() const;
    void setDisplaySize(const QSize &value);

    int getPixelRatio() const;
    void setPixelRatio(int value);

    int getSizeOfDocument() const;

    bool getIsDocumentSet() const;

private:

    const uint8_t DISTANCE_TO_CACHE_BORDER = 8;
    const uint8_t ELEMENTS_IN_CACHE = (DISTANCE_TO_CACHE_BORDER *2) +1;

    Poppler::Document *doc;
    vector<QImage*> pageCache;

    bool isDocumentSet = false;


    // current position of the cache
    int cacheCurrentPos = 0;
    //the first rendered page in the cache
    int cacheBegin = 0;
    //the last page in cache
    int cacheEnd = 0;


    int sizeOfDocument = 0;

    QSize displaySize;
    int pixelRatio = 1;

    mutex cache_mutex;
    mutex doc_mutex;

    void renderPageAsThread(int pageNum);
    void deletePageFromCache(int pageNum);
    void renderPage(Poppler::Page *page, int i);

    DotsPerInch calculateDPI(QSize size, Poppler::Page *page);
    QImage* getRenderedImageFromCache(int pageNum);

    void fillCacheToCurrentPosAndSetEnd(int pos);
    void fillCacheToCurrentPosAndSetBegin(int pos);

    void fillCacheAndSetNewBorders(int pos);
    void fillCacheToPos(int pos);

    void renderPagesAsThreadsPositiveDirection(int start, int end);
    void renderPagesAsThreadsNegativeDirection(int start, int end);

    void deletePagesFromCachePositiveDirection(int start, int end);
    void deletePagesFromCacheNegativeDirection(int start, int end);

};

#endif // DYNAMICPDFPAGECACHE_H

#include "dynamicpdfpagecache.h"

DynamicPdfPageCache::DynamicPdfPageCache(int pixelRatio):
    pixelRatio(pixelRatio)
{

}



DynamicPdfPageCache::~DynamicPdfPageCache()
{
    delete doc;
}

void DynamicPdfPageCache::setDocument(QString docFilePath,double splitscreen)
{

    //! alle threads anhalten
    //! cache löschen+leeren

    isDocumentSet = true;
    this->splitscreen=splitscreen;

    doc = Poppler::Document::load(docFilePath);

    doc->setRenderBackend(Poppler::Document::SplashBackend);
    doc->setRenderHint(Poppler::Document::Antialiasing, true);
    doc->setRenderHint(Poppler::Document::TextAntialiasing, true);

    sizeOfDocument = doc->numPages();
    pageCache.resize(sizeOfDocument);



}

QImage* DynamicPdfPageCache::getRenderedImageFromCache(int pageNum)
{
    QImage *ret = nullptr;

    cache_mutex.lock();
    if (pageCache[pageNum] != nullptr)
    {
        ret = pageCache[pageNum];
    }
    else
    {
        qDebug()<<"!!!!!!!!!! Cache " << pageNum <<"was nullpointer... should never happen...";
    }
    cache_mutex.unlock();

    return ret;
}

void DynamicPdfPageCache::fillCacheAfterCacheEnd(int pos)
{
    int distanceToCacheEnd = pos - cacheEnd;

    // check if we run out of bounces first
    int end = ((pos + (DISTANCE_TO_CACHE_BORDER) ) > pageCache.size()) ? pageCache.size()-1 : (pos + (DISTANCE_TO_CACHE_BORDER) );

    // cache has to be completely new generated...
    if (distanceToCacheEnd > (DISTANCE_TO_CACHE_BORDER))
    {
        renderPagesAsThreadsNegativeDirection(pos-1, DISTANCE_TO_CACHE_BORDER);
        renderPagesAsThreadsPositiveDirection(pos+1, end);
    }
    //borders lay within already present data
    else
    {
        renderPagesAsThreadsNegativeDirection(pos-1, distanceToCacheEnd);
        renderPagesAsThreadsPositiveDirection(pos+1, end);
    }

    cacheBegin = pos - DISTANCE_TO_CACHE_BORDER;
    cacheEnd = end;

    // delete all pages laying before the cache
    deletePagesFromCachePositiveDirection(0, cacheBegin);

    //delete all pages laying behind the cache
    deletePagesFromCachePositiveDirection(cacheEnd, pageCache.size());
}

void DynamicPdfPageCache::fillCacheBeforeCacheBegin(int pos)
{
    int distanceToCacheBegin = cacheBegin - pos;
    // check if we run out of bounces first
    int end = ((pos - DISTANCE_TO_CACHE_BORDER ) < 0) ? 0 : (pos - DISTANCE_TO_CACHE_BORDER );

    // cache has to be completely new generated
    if (distanceToCacheBegin > DISTANCE_TO_CACHE_BORDER )
    {
        renderPagesAsThreadsNegativeDirection(pos-1,end);
        renderPagesAsThreadsPositiveDirection(pos+1, DISTANCE_TO_CACHE_BORDER);
    }
    else
    {
        renderPagesAsThreadsNegativeDirection(pos-1, end);
        renderPagesAsThreadsPositiveDirection(pos+1, distanceToCacheBegin);
    }

    cacheBegin = end;
    cacheEnd = pos + DISTANCE_TO_CACHE_BORDER;

    // delete all pages laying before the cache
    deletePagesFromCachePositiveDirection(0, cacheBegin);

    //delete all pages laying behind the cache
    deletePagesFromCachePositiveDirection(cacheEnd, pageCache.size());

}



DotsPerInch DynamicPdfPageCache::calculateDPI(QSize size, Poppler::Page *page)
{
    DotsPerInch ret;

//    double splitscreen = (ri.splitscreen)?2:1; //!
//    double splitscreen = 1.0;
    //double splitscreen = 2.0;
    ret.dpiWidth = (size.width() * 72.0) / (page->pageSizeF().width()/splitscreen);
    ret.dpiHeight = (size.height() * 72.0) / page->pageSizeF().height();

    double factor = (page->pageSizeF().width()/splitscreen)/page->pageSizeF().height();
    double fake_w = factor*size.height();

    if(fake_w<size.width())
    {
        ret.dpiWidth = ret.dpiHeight;
    }
    else
    {
        ret.dpiHeight = ret.dpiWidth;
    }

    return ret;
}


void DynamicPdfPageCache::renderPage( Poppler::Page *page,int i)
{

    // setting a higher priority to the current thread
#if (__MACH__ || __linux__ )
    struct sched_param threadparam;
    threadparam.sched_priority = 10;
    pthread_setschedparam(pthread_self(), SCHED_RR, &threadparam);
#elif __WIN32
    SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);
#endif

    DotsPerInch dpiRequest = calculateDPI(displaySize,page);
    QImage image = page->renderToImage(dpiRequest.dpiWidth*pixelRatio, dpiRequest.dpiHeight*pixelRatio);

    qDebug() << "+++++ Image "<<  i <<" rendered..... h: " << image.size().height() << " w: " << image.size().width();

    cache_mutex.lock();
    pageCache[i] = new QImage(image);
    cache_mutex.unlock();
}

void DynamicPdfPageCache::initializeCache()
{
    int end = (sizeOfDocument < ELEMENTS_IN_CACHE) ? sizeOfDocument : ELEMENTS_IN_CACHE;
    for (int i = 0; i < end; i++)
    {
        Poppler::Page *page = doc->page(i);
        //new thread (&DynamicPdfPageCache::renderPage,this,page, i); //! memory leak
        threads.push_back(thread(&DynamicPdfPageCache::renderPage,this,page, i));
    }

    cacheCurrentPos = 0;
    cacheBegin = 0;
    cacheEnd = end -1;
}
QSize DynamicPdfPageCache::getDisplaySize() const
{
    return displaySize;
}

void DynamicPdfPageCache::setDisplaySize(const QSize &value)
{
    displaySize = value;
}

int DynamicPdfPageCache::getPixelRatio() const
{
    return pixelRatio;
}

void DynamicPdfPageCache::setPixelRatio(int value)
{
    pixelRatio = value;
}

int DynamicPdfPageCache::getSizeOfDocument() const
{
    return sizeOfDocument;
}

bool DynamicPdfPageCache::getIsDocumentSet() const
{
    return isDocumentSet;
}

void DynamicPdfPageCache::renderPageAsThread(int pageNum)
{
    Poppler::Page *page = doc->page(pageNum);
    threads.push_back( thread (&DynamicPdfPageCache::renderPage,this,page, pageNum));
}

void DynamicPdfPageCache::deletePageFromCache(int pageNum)
{
    cache_mutex.lock();
    //just to make sure
    if (pageCache[pageNum] != nullptr)
    {
        qDebug() << "-------- removed " << pageNum << "from cache" ;
        delete pageCache[pageNum];
        pageCache[pageNum] = nullptr;
    }
    cache_mutex.unlock();
}

void DynamicPdfPageCache::fillCacheAndSetNewBorders(int pos)
{
    int distanceToCacheEnd = cacheEnd - pos;
    int distanceToCacheBegin = pos - cacheBegin;

    // check in which direction we have to fill the cache and which direction we need to clear elements
    // extend the cache to the memory begin
    if (distanceToCacheBegin < distanceToCacheEnd)
    {
        int changeDistance = DISTANCE_TO_CACHE_BORDER - distanceToCacheBegin;
        int end = 0;

        if ((cacheBegin - changeDistance ) >= 0)
        {
            end = cacheBegin - changeDistance;
            qDebug() << "Extending cache in a negative direction with: " << changeDistance;

            renderPagesAsThreadsNegativeDirection(cacheBegin-1, end);
            deletePagesFromCacheNegativeDirection(cacheEnd, cacheEnd - changeDistance);

            cacheBegin -= changeDistance ;
            cacheEnd -= changeDistance;
        }

    }
    // extend the cache to the memory end
    else
    {
        //check first if we are running out of bounces...
        int changeDistance = DISTANCE_TO_CACHE_BORDER - distanceToCacheEnd;
        int end = 0;

        if ((cacheEnd + changeDistance ) <= pageCache.size()-1){

            end = cacheEnd + changeDistance;
            qDebug() << "Extending cache in a positive direction with: " << changeDistance ;

            renderPagesAsThreadsPositiveDirection(cacheEnd+1, end);
            deletePagesFromCachePositiveDirection(cacheBegin, cacheBegin + changeDistance);

            cacheBegin += changeDistance ;
            cacheEnd += changeDistance;
        }
    }

    qDebug()<<"Accessing Pos: " << pos;
    qDebug()<<"CacheBegin: " << cacheBegin;
    qDebug()<<"CacheEnd: " << cacheEnd;

}




void DynamicPdfPageCache::fillCacheFromPos(int pos)
{
    // cachemiss is behind cache
    if (pos > cacheEnd)
    {
        fillCacheAfterCacheEnd(pos);
    }
    // cachemiss is before cache
    if (pos < cacheBegin)
    {
        fillCacheBeforeCacheBegin(pos);
    }
}

void DynamicPdfPageCache::renderPagesAsThreadsPositiveDirection(int start, int end)
{

    for (int i = start; i <= end; i++)
    {
        renderPageAsThread(i);
    }
}

void DynamicPdfPageCache::renderPagesAsThreadsNegativeDirection(int start, int end)
{
    for (int i = start; i >= end; i--)
    {
        renderPageAsThread(i);
    }
}

void DynamicPdfPageCache::deletePagesFromCachePositiveDirection(int start, int end)
{
    for (int i = start; i < end ; i++)
    {
        deletePageFromCache(i);
    }
}

void DynamicPdfPageCache::deletePagesFromCacheNegativeDirection(int start, int end)
{
    for (int i = start; i > end; i--)
    {
        deletePageFromCache(i);
    }
}

QImage *DynamicPdfPageCache::getElementFromPos(int pos)
{

    // just to make sure
    if (pos > pageCache.size())
    {
        qDebug() << "Accessing page cache out of bounces ";
        return nullptr;
    }

    // check if we have to render a page immediately ->
    // it is a cache miss!
    if ((pos > cacheEnd) || (pos < cacheBegin))
    {
        qDebug()<< "cache miss!!  pos: " << pos ;
        Poppler::Page * page = doc->page(pos);
        renderPage(page,pos);
        fillCacheFromPos(pos);

    }
    // position lay in current cache area
    // we have a cache hit!
    else
    {
        qDebug()<< "cache hit!! pos: " << pos ;
        fillCacheAndSetNewBorders(pos);
    }

    cacheCurrentPos = pos;
    return getRenderedImageFromCache(cacheCurrentPos);
}

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
    int end = ((pos + (DISTANCE_TO_CACHE_BORDER) ) > pageCache.size()) ? pageCache.size()-1 : (pos + DISTANCE_TO_CACHE_BORDER);

    cacheBegin = pos - DISTANCE_TO_CACHE_BORDER;
    cacheEnd = end;

    // cache has to be completely new generated...
    if (distanceToCacheEnd > DISTANCE_TO_CACHE_BORDER)
    {
        qDebug() << __FUNCTION__ << "++++++++++++++++++ Completely generating new cache";
        renderPagesAsThreadsNegativeDirection(pos-1, cacheBegin);
        renderPagesAsThreadsPositiveDirection(pos+1, cacheEnd);
    }
    //borders lay within already present data
    else
    {
        qDebug() << __FUNCTION__ << "++++++++++++++++++ partly generating new cache";
        renderPagesAsThreadsNegativeDirection(pos-1, pos+distanceToCacheEnd);
        renderPagesAsThreadsPositiveDirection(pos+1, cacheEnd);
    }

    // delete all pages laying before the cache
    deletePagesFromCachePositiveDirection(0, cacheBegin);

    //delete all pages laying behind the cache
    deletePagesFromCachePositiveDirection(cacheEnd+1, pageCache.size());
}

void DynamicPdfPageCache::fillCacheBeforeCacheBegin(int pos)
{
    int distanceToCacheBegin = cacheBegin - pos;
    // check if we run out of bounces first
    int end = ((pos - DISTANCE_TO_CACHE_BORDER ) < 0) ? 0 : (pos - DISTANCE_TO_CACHE_BORDER);

    cacheBegin = end;
    cacheEnd = pos + DISTANCE_TO_CACHE_BORDER;

    // cache has to be completely new generated
    if (distanceToCacheBegin > DISTANCE_TO_CACHE_BORDER )
    {
        qDebug() << __FUNCTION__ << "++++++++++++++++++ Completely generating new cache";
        renderPagesAsThreadsNegativeDirection(pos-1, cacheBegin);
        renderPagesAsThreadsPositiveDirection(pos+1, pos + DISTANCE_TO_CACHE_BORDER);
    }
    else
    {
        qDebug() << __FUNCTION__ << "++++++++++++++++++ partly generating new cache";
        renderPagesAsThreadsNegativeDirection(pos-1, cacheBegin);
        renderPagesAsThreadsPositiveDirection(pos+1, pos + distanceToCacheBegin);
    }


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
    ret.dpiWidth = (size.width() * DPI_CONSTANT) / (page->pageSizeF().width()/splitscreen);
    ret.dpiHeight = (size.height() * DPI_CONSTANT) / page->pageSizeF().height();

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

void DynamicPdfPageCache::initializeCache(bool createThumbnails)
{
    int end = (sizeOfDocument < ELEMENTS_IN_CACHE) ? sizeOfDocument : ELEMENTS_IN_CACHE;
    for (int i = 0; i < end; i++)
    {
        Poppler::Page *page = doc->page(i);
        //new thread (&DynamicPdfPageCache::renderPage,this,page, i); //! memory leak
        thread render_thread(&DynamicPdfPageCache::renderPage,this,page, i);
        render_thread.detach();
    }

    cacheCurrentPos = 0;
    cacheBegin = 0;
    cacheEnd = end -1;

    if (createThumbnails)
    {
        thread thumbnailThread(&DynamicPdfPageCache::generateThumbnails,this);
        thumbnailThread.detach();
    }
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

void DynamicPdfPageCache::deleteAndResetCache()
{
    pageCache.clear();
//    for (uint16_t = 0; i < pageCache.size(); i++)
//    {
//        deletePageFromCache(1);
//    }

//    pageCache.resize(0);
}

vector<QImage> DynamicPdfPageCache::getThumbnails() const
{
    if (thumbnailsCreated)
    {
        return thumbnails;
    }
    else
    {
        return vector<QImage>();
    }
}


void DynamicPdfPageCache::renderPageAsThread(int pageNum)
{
    Poppler::Page *page = doc->page(pageNum);
    thread render_thread(&DynamicPdfPageCache::renderPage,this,page, pageNum);
    render_thread.detach();
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

    qDebug() << "########### Distance to cache begin: " << distanceToCacheBegin;
    qDebug() << "########### Distance to cache end: " << distanceToCacheEnd;

    // check in which direction we have to fill the cache and which direction we need to clear elements
    // extend the cache to the memory begin
    if (distanceToCacheBegin < distanceToCacheEnd)
    {
        int changeDistance = DISTANCE_TO_CACHE_BORDER - distanceToCacheBegin;
        int end = 0;

        if ((cacheBegin - changeDistance ) >= 0)
        {
            end = cacheBegin - changeDistance;
            qDebug() << "Extending cache in a negative direction with: " << changeDistance ;

            renderPagesAsThreadsNegativeDirection(cacheBegin-1, end);

            if (distanceToCacheEnd >= DISTANCE_TO_CACHE_BORDER)
            {
                deletePagesFromCacheNegativeDirection(cacheEnd, cacheEnd - changeDistance);
                cacheEnd -= changeDistance;
            }


            cacheBegin -= changeDistance;

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
            qDebug() << "Extending cache in a positive direction with: " << changeDistance;

            renderPagesAsThreadsPositiveDirection(cacheEnd+1, end);

            if (distanceToCacheBegin >= DISTANCE_TO_CACHE_BORDER)
            {
                deletePagesFromCachePositiveDirection(cacheBegin, cacheBegin + changeDistance);
                cacheBegin += changeDistance ;
            }

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
    else if (pos < cacheBegin)
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

void DynamicPdfPageCache::generateThumbnails()
{
    thumbnails.resize(doc->numPages());
    for (uint16_t i = 0; i < doc->numPages(); i++)
    {
        Poppler::Page *p = doc->page(i);

        double dpi = (THUMBNAIL_HIGHT_IN_PIXEL * DPI_CONSTANT) / p->pageSizeF().height();
        QImage thumb = p->renderToImage(dpi,dpi);

        thumbnails[i] = thumb;
    }
    thumbnailsCreated = true;
}

QImage *DynamicPdfPageCache::getElementFromPos(int pos)
{

    // just to make sure
    if ((pos > pageCache.size()) || (pos < 0))
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
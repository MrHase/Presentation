#include "dynamicpdfpagecache.h"

DynamicPdfPageCache::DynamicPdfPageCache(int pixelRatio):
    pixelRatio(pixelRatio)
{
    cache.SetCacheSize(ELEMENTS_IN_CACHE);
}



DynamicPdfPageCache::~DynamicPdfPageCache()
{
    cache.SetCacheSize(ELEMENTS_IN_CACHE);
}

void DynamicPdfPageCache::setDocument(Poppler::Document *document, double splitscreen)
{

    this->splitscreen=splitscreen;

    doc = document;

    sizeOfDocument = doc->numPages();
    pageCache.resize(sizeOfDocument);


}

//void DynamicPdfPageCache::setDocument(QString docFilePath,double splitscreen)
//{

//    //! alle threads anhalten
//    //! cache löschen+leeren

//    isDocumentSet = true;
//    this->splitscreen=splitscreen;

//    doc = Poppler::Document::load(docFilePath);

//    doc->setRenderBackend(Poppler::Document::SplashBackend);
//    doc->setRenderHint(Poppler::Document::Antialiasing, true);
//    doc->setRenderHint(Poppler::Document::TextAntialiasing, true);

//    sizeOfDocument = doc->numPages();
//    pageCache.resize(sizeOfDocument);



//}



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
    //QImage image = page->renderToImage(dpiRequest.dpiWidth*pixelRatio, dpiRequest.dpiHeight*pixelRatio);
    cache.Lock();
    if(!cache.Available(i)){
        cout<<"Adde page "<<i<<" to cache"<<endl;
//        cache.Add(i,img_ptr);
        cache.Add(i,page->renderToImage(dpiRequest.dpiWidth*pixelRatio, dpiRequest.dpiHeight*pixelRatio));

    }
    cache.Unlock();

//    qDebug() << "+++++ Image "<<  i <<" rendered..... h: " << image.size().height() << " w: " << image.size().width();


//    cache_mutex.lock();
//    auto img_ptr=new QImage(image);
//    pageCache[i] = img_ptr;
//    cache_mutex.unlock();

}

void DynamicPdfPageCache::renderPageToImage(Poppler::Page *page, int i)
{
    //return page->renderToImage(dpiRequest.dpiWidth*pixelRatio, dpiRequest.dpiHeight*pixelRatio);
}

void DynamicPdfPageCache::initializeCache()
{


    std::vector<std::thread> workers;
    for (int i = 0; i < sizeOfDocument; i++) {
        workers.push_back(std::thread([=]()
        {
            std::cout << "Adding Image "<<i<<" to File\n";
            Poppler::Page *page = doc->page(i);
            DotsPerInch dpiRequest = calculateDPI(displaySize,page);

            QImage img=page->renderToImage(dpiRequest.dpiWidth*pixelRatio, dpiRequest.dpiHeight*pixelRatio);

            fileCache.Add(i,img);

        }));
    }

    std::for_each(workers.begin(), workers.end(), [](std::thread &t)
    {
        t.join();
    });

    //for(auto w:workers)

    /*

    for (int i = 0; i < sizeOfDocument; i++)
    {
        Poppler::Page *page = doc->page(i);


        std::cout << "main thread\n";

        // Looping every thread via for_each
        // The 3rd argument assigns a task
        // It tells the compiler we're using lambda ([])
        // The lambda function takes its argument as a reference to a thread, t
        // Then, joins one by one, and this works like barrier

    }
    */



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


void DynamicPdfPageCache::deleteAndResetCache()
{
    pageCache.clear();
//    for (uint16_t = 0; i < pageCache.size(); i++)
//    {
//        deletePageFromCache(1);
//    }

//    pageCache.resize(0);
}




void DynamicPdfPageCache::renderPageAsThread(int pageNum)
{
    Poppler::Page *page = doc->page(pageNum);
    thread render_thread(&DynamicPdfPageCache::renderPage,this,page, pageNum);
    render_thread.detach();
}




QImage DynamicPdfPageCache::getElementFromPos(int pos)
{

    //return fileCache.GetPage(pos); // use this for fileCache

    int begin_=pos-(ELEMENTS_IN_CACHE/2);
    const uint32_t begin=(begin_<0)? 0:begin_;

    int end_=pos+(ELEMENTS_IN_CACHE/2);
    const uint32_t end=(end_>=doc->numPages())?doc->numPages():end_;

    cout<<"Getting page: "<<pos<<" begin: "<<begin<<" end: "<<end<<endl;
    cache.Lock();

    if (cache.Available(pos)){
        cout<<"Page found!"<<pos<<endl;


        for(auto i=begin; i<end;i++)
        {
            cache.Touch(i);
        }
        for(auto i=begin; i<end;i++)
        {
            if(!cache.Available(i))
                renderPageAsThread(i);
        }





    }
    else{
        cout<<"Page not found! "<<pos<<endl;

        for(auto i=begin; i<end;i++)
        {
            cache.Touch(i);
        }

        auto page=doc->page(pos);
        DotsPerInch dpiRequest = calculateDPI(displaySize,page);
        cache.Add(pos,page->renderToImage(dpiRequest.dpiWidth*pixelRatio, dpiRequest.dpiHeight*pixelRatio));
//        renderPage(doc->page(pos),pos);


        /*
        for(auto i=begin; i<end;i++)
        {
            if(!cache.Available(i))
                renderPageAsThread(i);
        }
        */
    }

    QImage img= cache.Get(pos);
    cache.Unlock();

    return img;


//    // just to make sure
//    if ((pos > pageCache.size()) || (pos < 0))
//    {
//        qDebug() << "Accessing page cache out of bounces ";
//        return nullptr;
//    }

//    // check if we have to render a page immediately ->
//    // it is a cache miss!
//    if ((pos > cacheEnd) || (pos < cacheBegin))
//    {
//        qDebug()<< "cache miss!!  pos: " << pos ;
//        Poppler::Page * page = doc->page(pos);
//        renderPage(page,pos);
//        fillCacheFromPos(pos);

//    }
//    // position lay in current cache area
//    // we have a cache hit!
//    else
//    {
//        qDebug()<< "cache hit!! pos: " << pos ;
//        fillCacheAndSetNewBorders(pos);
//    }

//    cacheCurrentPos = pos;
//    return getRenderedImageFromCache(cacheCurrentPos);
}

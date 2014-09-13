    #include "presentation.h"
Presentation::Presentation():
    cache_preview(0),
    cache_mainScreen(0),
    cache_lecturerScreen(0)
//    renderer_preview(0),
//    renderer_mainScreen(0),
//    renderer_helperScreen(0)
{
    doc=nullptr;
}




void Presentation::setDocumentFile(QString file_n)
{
    filename = file_n;


    //! it is crucial that no render threads are running, because we delete doc!
    if(doc!=nullptr)
        delete doc;

    doc = Poppler::Document::load(filename);

    doc->setRenderBackend(Poppler::Document::SplashBackend);
    doc->setRenderHint(Poppler::Document::Antialiasing, true);
    doc->setRenderHint(Poppler::Document::TextAntialiasing, true);

    numOfPages=doc->numPages();

    generateThumbnails(); //! this takes too long -> own thread with signal when finished

}

void Presentation::setPreviewDocument(int dpri,double splitscreen)
{
//    setRendererDocument(dpri, renderer_preview, preview_size);

    setCacheDocument(dpri,cache_preview,preview_size,splitscreen);



//    // setup the renderer and the size for rendering
//    renderer_preview.setDocument(filename);
//    renderer_preview.setThisDevicePixelRatio(dpri);

//    qDebug()<< "preview_rect height: " << preview_rect.height();
//    qDebug()<< "preview_rect width: " << preview_rect.width();

//    RenderInfo ri;
//    ri.requested_height = preview_rect.height();
//    ri.requested_width = preview_rect.width();
//    ri.splitscreen = true;

//    try
//    {
//        renderer_preview.renderDocumentIntoCache(ri);
//    }
//    catch (exception& e)
//    {
//        std::cerr << e.what() << std::endl;
//    }
}

QImage Presentation::preview_getCurrentPage()
{
    return getCurrentImageFromCache(cache_preview);
//    return renderer_preview.getRenderedImage(currentPage);
}

QImage Presentation::getRightSideOfPreviewPage()
{
    QImage page = getCurrentImageFromCache(cache_preview);
    return left_rightSideOfPage(false,page);
}

QImage Presentation::getLeftSideOfPreviewPage()
{
    QImage page = getCurrentImageFromCache(cache_preview);
    return left_rightSideOfPage(true,page);
}

QImage Presentation::getRightSideOfMainScreen()
{
    QImage page = getCurrentImageFromCache(cache_mainScreen);
    return left_rightSideOfPage(false, page);
}

QImage Presentation::getLeftSideOfMainScreen()
{
    QImage page = getCurrentImageFromCache(cache_mainScreen);
    return left_rightSideOfPage(true, page);
}

QImage Presentation::getRightSideOfLecturerScreen()
{
    QImage page = getCurrentImageFromCache(cache_lecturerScreen);
    return left_rightSideOfPage(false, page);
}

QImage Presentation::getLeftSideOfLecturerScreen()
{
    QImage page = getCurrentImageFromCache(cache_lecturerScreen);
    return left_rightSideOfPage(true, page);
}

QImage Presentation::getCurrentMainScreen()
{
    return getCurrentImageFromCache(cache_mainScreen);
}

QImage Presentation::getCurrentLectureScreen()
{
    return getCurrentImageFromCache(cache_lecturerScreen);
}



bool Presentation::documentSet() const
{
    return doc!=nullptr;

}



QImage Presentation::left_rightSideOfPage(bool left_right, QImage &image)
{
    QImage ret;

    QSize origSize = image.size();
    if (left_right)
    {
        ret = image.copy(0,0,origSize.width()/2,origSize.height());
    }
    else
    {
        ret = image.copy(origSize.width()/2,0,origSize.width()/2,origSize.height());
    }

    return ret;
}

QImage Presentation::getCurrentImageFromCache(DynamicPdfPageCache &cache)
{
    QImage ret;
    QImage * tmp = cache.getElementFromPos(currentPage);

    if (tmp != nullptr)
    {
        ret = *tmp;
    }

    return ret;
}


void Presentation::setCacheDocument(int dpri, DynamicPdfPageCache &cache, QSize size, double splitscreen)
{
    //! alter cache deleten und neu anlegen?

    cache.setDocument(doc,splitscreen); //!
    cache.setPixelRatio(dpri); //! mit in den SetDocument/Constructur
    cache.setDisplaySize(size); //! constructor

    cache.initializeCache(); //! constructor
}

QSize Presentation::getHelper_size() const
{
    return helper_size;
}

void Presentation::setHelper_size(const QSize &value)
{
    helper_size = value;
}

QRect Presentation::getRectOfImage(bool split)
{
    QRect ret;
    if (split)
    {
        QImage image = getRightSideOfPreviewPage();
        ret = image.rect();
    }
    else
    {
        QImage image = preview_getCurrentPage();
        ret = image.rect();
    }

    return ret;
}

vector<QImage> Presentation::GetThumbnails()
{
    return thumbnails; //! maybe pointer?
}

void Presentation::generateThumbnails()
{
    const uint8_t THUMBNAIL_HIGHT_IN_PIXEL = 64; //! somewhere else
    const double  DPI_CONSTANT = 72.0;  //! global or something

    thumbnails.clear();
    thumbnails.resize(doc->numPages());

    for (uint16_t i = 0; i < doc->numPages(); i++)
    {
        Poppler::Page *p = doc->page(i);

        double dpi = (THUMBNAIL_HIGHT_IN_PIXEL * DPI_CONSTANT) / p->pageSizeF().height();
        QImage thumb = p->renderToImage(dpi,dpi);

        thumbnails[i] = thumb;
    }
}

uint32_t Presentation::CurrentPage()
{
    return currentPage;
}

uint32_t Presentation::NumberOfPages()
{
    return numOfPages;
}

uint32_t Presentation::Progress()
{
    return ((float)(CurrentPage()+1)/(float)NumberOfPages())*100.0;
}

QSize Presentation::getMain_size() const
{
    return main_size;
}

void Presentation::setMain_size(const QSize &value)
{
    main_size = value;
}

QSize Presentation::getPreview_size() const
{
    return preview_size;
}

void Presentation::setPreview_size(const QSize &value)
{
    preview_size = value;
}


void Presentation::nextPage()
{
    if(currentPage < numOfPages-1)
    {
        currentPage++;
        emit(pageChanged(currentPage));
    }
}

void Presentation::previousPage()
{
    if(currentPage > 0)
    {
        currentPage--;
        emit(pageChanged(currentPage));
    }
}

void Presentation::goToPage(uint16_t pageNum)
{
    //just to make sure
    if ( pageNum < numOfPages )
    {
        if(currentPage!=pageNum)
        {
            currentPage = pageNum;
            qDebug() << "set page to " << currentPage;
            emit(pageChanged(currentPage));
        }
    }
}

void Presentation::setHelperScreenDocument(int dpri,double splitscreen)
{
    setCacheDocument(dpri,cache_lecturerScreen,helper_size,splitscreen);
//    setRendererDocument(dpri,renderer_helperScreen,helper_size);
}

void Presentation::setMainScreenDocument(int dpri,double splitscreen)
{
    setCacheDocument(dpri,cache_mainScreen,main_size,splitscreen);
//    setRendererDocument(dpri,renderer_mainScreen,main_size);
}


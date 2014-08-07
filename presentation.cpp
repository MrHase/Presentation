    #include "presentation.h"
Presentation::Presentation():
    cache_preview(0),
    cache_mainScreen(0),
    cache_helperScreen(0)
//    renderer_preview(0),
//    renderer_mainScreen(0),
//    renderer_helperScreen(0)
{
}


Presentation::Presentation(QString file_n):
//    renderer_preview(0),
    cache_preview(0),
    cache_mainScreen(0),
    cache_helperScreen(0),
//    renderer_mainScreen(0),
//    renderer_helperScreen(0),
    filename(file_n)
{

}

void Presentation::setDocumentFile(QString file_n)
{
    filename = file_n;
}

void Presentation::setPreviewDocument(int dpri)
{
//    setRendererDocument(dpri, renderer_preview, preview_size);

    setCacheDocument(dpri,cache_preview,preview_size);

    numOfPages = cache_preview.getSizeOfDocument();

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

QImage Presentation::getRightSideOfHelperScreen()
{
    QImage page = getCurrentImageFromCache(cache_helperScreen);
    return left_rightSideOfPage(false, page);
}

QImage Presentation::getLeftSideOfHelperScreen()
{
    QImage page = getCurrentImageFromCache(cache_helperScreen);
    return left_rightSideOfPage(true, page);
}

bool Presentation::documentSet() const
{
    bool ret = false;
    if (cache_preview.getIsDocumentSet() || cache_mainScreen.getIsDocumentSet() || cache_helperScreen.getIsDocumentSet())
    {
        ret = true;
    }

    return ret;
//    return (cache_helperScreen.getIsDocumentSet()
//            || cache_mainScreen.getIsDocumentSet()
//            || cache_helperScreen.getIsDocumentSet()) ?
//                true: false;
//    return renderer_preview.documentSet();
}


QImage Presentation::left_rightSideOfPage(bool left, PdfRenderer& renderer)
{
    QImage retImage;
    QImage orig = renderer.getRenderedImage(currentPage);
    QSize origSize = orig.size();
    if (left)
    {
        retImage = orig.copy(0,0,origSize.width()/2,origSize.height());
    }
    else
    {
        retImage = orig.copy(origSize.width()/2,0,origSize.width()/2,origSize.height());
    }

    return retImage;
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

void Presentation::setRendererDocument(int dpri, PdfRenderer &renderer, QSize size)
{
    // setup the renderer and the size for rendering
    renderer.setDocument(filename);
    renderer.setThisDevicePixelRatio(dpri);

    qDebug()<< "size height: " << size.height() << "size width : " << size.width();


    RenderInfo ri;
    ri.requested_height = size.height();
    ri.requested_width = size.width();
    ri.splitscreen = true;

    try
    {
        renderer.renderDocumentIntoCache(ri);
    }
    catch (exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Presentation::setCacheDocument(int dpri, DynamicPdfPageCache &cache, QSize size)
{
    cache.setDocument(filename);
    cache.setPixelRatio(dpri);
    cache.setDisplaySize(size);

    cache.initializeCache();
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
    if(currentPage < (numOfPages-1))
        currentPage++;
}

void Presentation::previousPage()
{
    if(currentPage > 0)
        currentPage--;
}

void Presentation::setHelperScreenDocument(int dpri)
{
    setCacheDocument(dpri,cache_helperScreen,helper_size);
//    setRendererDocument(dpri,renderer_helperScreen,helper_size);
}

void Presentation::setMainScreenDocument(int dpri)
{
    setCacheDocument(dpri,cache_mainScreen,main_size);
//    setRendererDocument(dpri,renderer_mainScreen,main_size);
}


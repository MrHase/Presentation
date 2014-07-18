    #include "presentation.h"
Presentation::Presentation(double dpri):
    renderer_preview(0),
    renderer_mainScreen(0),
    renderer_helperScreen(0)
{
}


Presentation::Presentation(QString file_n, double dpri):
    renderer_preview(0),
    renderer_mainScreen(0),
    renderer_helperScreen(0),
    filename(file_n)
{

}

void Presentation::setDocumentFile(QString file_n)
{
    filename = file_n;
}

void Presentation::setPreviewDocument(int dpri)
{

    setRendererDocument(dpri, renderer_preview, preview_size);
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
    return renderer_preview.getRenderedImage(currentPage);
}

QImage Presentation::getRightSideOfPreviewPage()
{
    return left_rightSideOfPage(false,renderer_preview);
}

QImage Presentation::getLeftSideOfPreviewPage()
{
    return left_rightSideOfPage(true,renderer_preview);
}

QImage Presentation::getRightSideOfMainScreen()
{
    return left_rightSideOfPage(false, renderer_mainScreen);
}

QImage Presentation::getLeftSideOfMainScreen()
{
    return left_rightSideOfPage(true, renderer_mainScreen);
}

QImage Presentation::getRightSideOfHelperScreen()
{
    return left_rightSideOfPage(false, renderer_helperScreen);
}

QImage Presentation::getLeftSideOfHelperScreen()
{
    return left_rightSideOfPage(true, renderer_helperScreen);
}

bool Presentation::documentSet() const
{
    return renderer_preview.documentSet();
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
QSize Presentation::getHelper_size() const
{
    return helper_size;
}

void Presentation::setHelper_size(const QSize &value)
{
    helper_size = value;
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

    if(currentPage < (renderer_preview.pages() - 1))
        currentPage++;
}

void Presentation::previousPage()
{
    if(currentPage > 0)
        currentPage--;
}

void Presentation::setHelperScreenDocument(int dpri)
{
    setRendererDocument(dpri,renderer_helperScreen,helper_size);
}

void Presentation::setMainScreenDocument(int dpri)
{
    setRendererDocument(dpri,renderer_mainScreen,main_size);
}


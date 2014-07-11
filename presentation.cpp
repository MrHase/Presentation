    #include "presentation.h"
Presentation::Presentation(double dpri):
    renderer_preview(dpri),
    renderer_mainScreen(dpri),
    renderer_helperScreen(dpri)
{
}


Presentation::Presentation(QString file_n, double dpri):
    renderer_preview(dpri),
    renderer_mainScreen(dpri),
    renderer_helperScreen(dpri),
    filename(file_n)
{

}

void Presentation::setDocumentFile(QString file_n)
{
    filename = file_n;
}

void Presentation::setPreviewDocument()
{
    // setup the renderer and the size for rendering
    renderer_preview.setDocument(filename);

    qDebug()<< "preview_rect height: " << preview_rect.height();
    qDebug()<< "preview_rect width: " << preview_rect.width();

    RenderInfo ri;
    ri.requested_height = preview_rect.height();
    ri.requested_width = preview_rect.width();
    ri.splitscreen = true;

    try
    {
        renderer_preview.renderDocumentIntoCache(ri);
    }
    catch (exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

QImage Presentation::preview_getCurrentPage()
{
    return renderer_preview.getRenderedImage(currentPage);
}

QImage Presentation::getRightSideOfPreviewPage()
{
    return left_rightSideOfPreviewPage(false);
}

QImage Presentation::getLeftSideOfPreviewPage()
{
    return left_rightSideOfPreviewPage(true);
}

bool Presentation::documentSet() const
{
    return renderer_preview.documentSet();
}

QImage Presentation::left_rightSideOfPreviewPage(bool left)
{
    QImage retImage;
    QImage orig = renderer_preview.getRenderedImage(currentPage);
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
QRect Presentation::getHelper_rect() const
{
    return helper_rect;
}

void Presentation::setHelper_rect(const QRect &value)
{
    helper_rect = value;
}

QRect Presentation::getMain_rect() const
{
    return main_rect;
}

void Presentation::setMain_rect(const QRect &value)
{
    main_rect = value;
}

QRect Presentation::getPreview_rect() const
{
    return preview_rect;
}

void Presentation::setPreview_rect(const QRect &value)
{
    preview_rect = value;
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


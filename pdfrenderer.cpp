#include "pdfrenderer.h"

void PdfRenderer::renderDocumentIntoCache()
{
    for (int i = 0; i < doc->numPages(); i++)
    {
        Poppler::Page *page = doc->page(i);
        if (page)
        {
            qDebug() << "p height: " << page->pageSizeF().height();

            // calculation of DPI:
            // desired dpi = ((resolution_of_screen) * 72.0 ) / smallest_side_of_pic
            //!900 needs to be resolved automatically!!
            double dpi = (900 * 72.0) / page->pageSizeF().height();
            qDebug() << "dpi: " << dpi;
            QImage img = page->renderToImage(dpi,dpi);

            qDebug() << "image height: " << img.size().height();

            pageCache.push_back(img);
        }
    }
}

PdfRenderer::PdfRenderer(QString filePath):
    doc(Poppler::Document::load(filePath)),
    isDocumentSet(true)
{
    renderDocumentIntoCache();
}

PdfRenderer::PdfRenderer(double optionDpiXAxis, double optionDpiYAxis):
    isDocumentSet(false),
    renderOptionDpiXAxis(optionDpiXAxis),
    renderOptionDpiYAxis(optionDpiYAxis)
{

}

uint32_t PdfRenderer::pages()
{
    return doc->numPages();
}

PdfRenderer::~PdfRenderer()
{
}

void PdfRenderer::setDocument(QString filePath)
{
    doc = shared_ptr<Poppler::Document>(Poppler::Document::load(filePath));
    isDocumentSet = true;
    doc->setRenderBackend(Poppler::Document::SplashBackend);
    doc->setRenderHint(Poppler::Document::Antialiasing, true);
    doc->setRenderHint(Poppler::Document::TextAntialiasing, true);

    renderDocumentIntoCache();
}

void PdfRenderer::setDocument(string filePath)
{
    setDocument(QString(filePath.c_str()));
}

QImage PdfRenderer::getRenderedImage(int pageNum)
{
    // Old stuff...
//    QImage ret;
//    if(!isDocumentSet)
//    {
//        qDebug() << "error.... Document must be set befor calling" ;
//    }

//    if (pageNum < 0)
//    {
//        qDebug() << "error.... poage num must be greater than 0" ;
//    }

//    // Access page of the PDF file
//    Poppler::Page* pdfPage = doc->page(pageNum);
//    if (!pdfPage)
//    {
//        qDebug() << "Error... no PdfPage";
//    }

//    ret = pdfPage->renderToImage();
//    if (ret.isNull())
//    {
//        qDebug() << "Error... Page couldnt be rendered";
//    }

//    return ret;

    return pageCache[pageNum];
}

bool PdfRenderer::documentSet() const
{
    return isDocumentSet;
}





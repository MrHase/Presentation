#include "pdfrenderer.h"

void PdfRenderer::renderDocumentIntoCache(RenderInfo ri)
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
            //double dpi = (ri.requested_height * 72.0) / page->pageSizeF().height();

            double dpi_x=0,dpi_y=0;
            calculateDPI(ri,page,[&](double calc_dpi_x,double calc_dpi_y){
                dpi_x=calc_dpi_x;
                dpi_y=calc_dpi_y;
            });

            QImage img = page->renderToImage(dpi_x,dpi_y);

            qDebug()<<"i.width: "<<img.size().width() << "i.height: " << img.size().height();

            pageCache.push_back(img);
        }
    }
}

void PdfRenderer::calculateDPI(RenderInfo ri, Poppler::Page *page, std::function<void(double dpi_x, double dpi_y)> use_dpi)
{

    double dpi_x = (ri.requested_width * 72.0) / page->pageSizeF().width();
    double dpi_y = (ri.requested_height * 72.0) / page->pageSizeF().height();

    dpi_x=dpi_y; //! das ist natürlich blödsinn... wir müssen entscheiden ob wir breite oder höhe zum ausrechnen der dpi brauchen

    use_dpi(dpi_x,dpi_y);
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

    RenderInfo ri;
    ri.requested_height=1080;
    //ri.requested_width=1920;
    renderDocumentIntoCache(ri);
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





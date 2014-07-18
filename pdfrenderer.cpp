#include "pdfrenderer.h"


QRect PdfRenderer::getSize() const
{
    return size;
}

void PdfRenderer::setSize(const QRect &value)
{
    size = value;
}
void PdfRenderer::renderDocumentIntoCache(RenderInfo ri)
{
    if (isDocumentSet)
    {
        pageCache.resize(doc->numPages());

        for (int i = 0; i < doc->numPages(); i++)
        {
            Poppler::Page *page = doc->page(i);
            if (page)
            {
                qDebug() << "p height: " << page->pageSizeF().height();

                // calculation of DPI:
                // desired dpi = (resolution_of_screen * 72.0 )[dots] / smallest_side_of_pic[inch]
                //!900 needs to be resolved automatically!!
                //double dpi = (ri.requested_height * 72.0) / page->pageSizeF().height();

    //            double dpi_x=0,dpi_y=0;

                //! ugly imho. lets use a class for that?
    //            calculateDPI(ri,page,[&](double calc_dpi_x,double calc_dpi_y){
    //                dpi_x=calc_dpi_x;
    //                dpi_y=calc_dpi_y;
    //            });

                DpiRequest request = calculateDPI(ri,page);

                //! deletee!!
                thread *t=new thread([=](){
                    //! use unique_lock or

                    qDebug()<<"Test : "<<i;
                    QImage img = page->renderToImage(request.dpiWidth*thisDevicePixelRatio,request.dpiHeight*thisDevicePixelRatio);
                    qDebug()<<"i.width: "<<img.size().width() << "i.height: " << img.size().height() << "dpri: " << thisDevicePixelRatio;

                    //! hier ist doch call by value, oder?
                    //! funktioniert dass dann noch mit dem Mutex??
                    mutex_cache.lock();
                    //pageCache.push_back(img);
                    pageCache[i]=img; //!und wieso funktioniert das?

                    mutex_cache.unlock();
                });
                /*
                QImage img = page->renderToImage(request.dpiWidth*thisDevicePixelRatio,request.dpiHeight*thisDevicePixelRatio);
                qDebug()<<"i.width: "<<img.size().width() << "i.height: " << img.size().height() << "dpri: " << thisDevicePixelRatio;
                pageCache.push_back(img);
                */

            }
        }
    }
    else
    {
//        throw RendererException;
        qDebug()<<"Error Docuemnt was not set";
    }
}


//! imho much more readable :)

double PdfRenderer::getThisDevicePixelRatio() const
{
    return thisDevicePixelRatio;
}

void PdfRenderer::setThisDevicePixelRatio(double value)
{
    thisDevicePixelRatio = value;
}
DpiRequest PdfRenderer::calculateDPI(RenderInfo ri, Poppler::Page *page)
{
    DpiRequest ret;

    double splitscreen=(ri.splitscreen)?2:1;
    ret.dpiWidth = (ri.requested_width * 72.0) / (page->pageSizeF().width()/splitscreen);
    ret.dpiHeight = (ri.requested_height * 72.0) / page->pageSizeF().height();

    double factor=(page->pageSizeF().width()/splitscreen)/page->pageSizeF().height();
    double fake_w=factor*ri.requested_height;

    if(fake_w<ri.requested_width){
        ret.dpiWidth=ret.dpiHeight;
    }else{
        ret.dpiHeight=ret.dpiWidth;
    }



    return ret;
}

PdfRenderer::PdfRenderer(double dpri):
    thisDevicePixelRatio(dpri)
{

}

PdfRenderer::PdfRenderer(QRect size, double dpri):
    size(size),
    thisDevicePixelRatio(dpri)
{

}

//void PdfRenderer::calculateDPI(RenderInfo ri, Poppler::Page *page, std::function<void(double dpi_x, double dpi_y)> use_dpi)
//{

//    double splitscreen=(ri.splitscreen)?2:1;
//    double dpi_x = (ri.requested_width * 72.0) / (page->pageSizeF().width()/splitscreen);
//    double dpi_y = (ri.requested_height * 72.0) / page->pageSizeF().height();


//    double factor=(page->pageSizeF().width()/splitscreen)/page->pageSizeF().height();
//    double fake_w=factor*ri.requested_height;
//    //double fake_h=ri.requested_height; // not necessary

//    if(fake_w<ri.requested_width){
//        dpi_x=dpi_y; //! das ist natürlich blödsinn... wir müssen entscheiden ob wir breite oder höhe zum ausrechnen der dpi brauchen
//    }else{
//        dpi_y=dpi_x;
//    }

//    use_dpi(dpi_x,dpi_y);
//}



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

//    RenderInfo ri;
//    ri.requested_height=1080;
//    ri.requested_width=1920;
//    ri.splitscreen=true;
//    renderDocumentIntoCache(ri);
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

    //! use locker
    QImage img;
    mutex_cache.lock();

    img=pageCache[pageNum];
    mutex_cache.unlock();
    return img;

    return pageCache[pageNum];
}

bool PdfRenderer::documentSet() const
{
    return isDocumentSet;
}





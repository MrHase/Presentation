#include "pdfrenderer.h"

PdfRenderer::PdfRenderer(QString filePath):
    doc(Poppler::Document::load(filePath)),
    isDocumentSet(true)
{

}

PdfRenderer::PdfRenderer():
    isDocumentSet(false)
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
}

void PdfRenderer::setDocument(string filePath)
{
    setDocument(QString(filePath.c_str()));
}

QImage PdfRenderer::getRenderedImage(int pageNum)
{
    QImage ret;
    if(!isDocumentSet)
    {
        qDebug() << "error.... Document must be set befor calling" ;
    }

    if (pageNum < 0)
    {
        qDebug() << "error.... poage num must be greater than 0" ;
    }

    // Access page of the PDF file
    Poppler::Page* pdfPage = doc->page(pageNum);
    if (!pdfPage)
    {
        qDebug() << "Error... no PdfPage";
    }

    ret = pdfPage->renderToImage();
    if (ret.isNull())
    {
        qDebug() << "Error... Page couldnt be rendered";
    }

    return ret;
}





#include "presentation.h"

Presentation::Presentation()
{
}

void Presentation::setDocument(string filename)
{
    pdfRenderer.setDocument(QString(filename.c_str())); //! ugly... no QString in renderer
}

void Presentation::nextPage()
{
    //! check if an pdf document is open

    if(currentPage<(pdfRenderer.pages()-1))
        currentPage++;
}

void Presentation::previousPage()
{
    if(currentPage>0)
        currentPage--;
}

QImage Presentation::getCurrentPage()
{
    return pdfRenderer.getRenderedImage(currentPage);
}

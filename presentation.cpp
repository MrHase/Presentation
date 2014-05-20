#include "presentation.h"

Presentation::Presentation()
{
    qDebug()<<"Erstellt....";
}

void Presentation::setDocument(string filename)
{
//    pdfRenderer.setDocument(QString(filename.c_str())); //! ugly... no QString in renderer
    pdfRenderer.setDocument(filename);
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

QImage Presentation::getRightSideOfPage()
{
    return left_rightSideOfPage(false);
}

QImage Presentation::getLeftSideOfPage()
{
    return left_rightSideOfPage(true);
}

bool Presentation::documentSet() const
{
    return pdfRenderer.documentSet();
}

QImage Presentation::left_rightSideOfPage(bool left)
{
    QImage retImage;
    QImage orig = pdfRenderer.getRenderedImage(currentPage);
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


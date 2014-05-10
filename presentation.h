#ifndef PRESENTATION_H
#define PRESENTATION_H

#include "pdfrenderer.h"
class Presentation
{
public:
    Presentation();

    // Init
    void setDocument(string filename);

    // Navigation:
    void nextPage();
    void previousPage();

    QImage getCurrentPage();
private:

    PdfRenderer pdfRenderer;

    uint32_t currentPage=0;
};

#endif // PRESENTATION_H

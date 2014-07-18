#ifndef PRESENTATION_H
#define PRESENTATION_H

#include "pdfrenderer.h"
#include <iostream>

class Presentation
{
public:
    Presentation(double dpri);
    Presentation(QString file_n, double dpri);

    void setDocumentFile(QString file_n);

    // Init preview
    void setPreviewDocument(int dpri);

    QImage preview_getCurrentPage();

    QImage getRightSideOfPreviewPage();
    QImage getLeftSideOfPreviewPage();

    QImage getRightSideOfMainScreen();
    QImage getLeftSideOfMainScreen();

    QImage getRightSideOfHelperScreen();
    QImage getLeftSideOfHelperScreen();

    bool documentSet() const;

    void nextPage();
    void previousPage();

    void setHelperScreenDocument(int dpri);
    void setMainScreenDocument(int dpri);

    QSize getPreview_size() const;
    void setPreview_size(const QSize &value);

    QSize getMain_size() const;
    void setMain_size(const QSize &value);

    QSize getHelper_size() const;
    void setHelper_size(const QSize &value);

private:
    QImage left_rightSideOfPage(bool left, PdfRenderer &renderer);

    void setRendererDocument(int dpri, PdfRenderer &renderer, QSize size);

    PdfRenderer renderer_preview;
    PdfRenderer renderer_mainScreen;
    PdfRenderer renderer_helperScreen;

    uint32_t currentPage = 0;

    QString filename;



    // rectangles for the supported screens
    // should be okay for now, maybe once a map...
    QSize preview_size;
    QSize main_size;
    QSize helper_size;


};

#endif // PRESENTATION_H

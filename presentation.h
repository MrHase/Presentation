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
    void setPreviewDocument();

    QImage preview_getCurrentPage();

    QImage getRightSideOfPreviewPage();
    QImage getLeftSideOfPreviewPage();

    bool documentSet() const;

    QRect getPreview_rect() const;
    void setPreview_rect(const QRect &value);

    QRect getMain_rect() const;
    void setMain_rect(const QRect &value);

    QRect getHelper_rect() const;
    void setHelper_rect(const QRect &value);



    void nextPage();
    void previousPage();

private:
    QImage left_rightSideOfPreviewPage(bool left);

    PdfRenderer renderer_preview;
    PdfRenderer renderer_mainScreen;
    PdfRenderer renderer_helperScreen;

    uint32_t currentPage = 0;

    QString filename;



    // rectangles for the supported screens
    // should be okay for now, maybe once a map...
    QRect preview_rect;
    QRect main_rect;
    QRect helper_rect;


};

#endif // PRESENTATION_H

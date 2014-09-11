#ifndef PRESENTATION_H
#define PRESENTATION_H

#include "dynamicpdfpagecache.h"
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;


class Presentation:public QObject
{
    Q_OBJECT
public:
    Presentation();
    Presentation(QString file_n);

    void setDocumentFile(QString file_n);

    // Init preview
    void setPreviewDocument(int dpri, double splitscreen);

    QImage preview_getCurrentPage();

    QImage getRightSideOfPreviewPage();
    QImage getLeftSideOfPreviewPage();

    QImage getRightSideOfMainScreen();
    QImage getLeftSideOfMainScreen();

    QImage getRightSideOfLecturerScreen();
    QImage getLeftSideOfLecturerScreen();

    QImage getCurrentMainScreen();
    QImage getCurrentLectureScreen();

    vector<QImage> getThumbnailsFromDocument();


    bool documentSet() const;

    void nextPage();
    void previousPage();

    void goToPage(uint16_t pageNum);

    void setHelperScreenDocument(int dpri, double splitscreen);
    void setMainScreenDocument(int dpri, double splitscreen);

    QSize getPreview_size() const;
    void setPreview_size(const QSize &value);

    QSize getMain_size() const;
    void setMain_size(const QSize &value);

    QSize getHelper_size() const;
    void setHelper_size(const QSize &value);


    QRect getRectOfImage(bool split);

    uint32_t CurrentPage();
    uint32_t NumberOfPages();
    uint32_t Progress();

signals:
    void pageChanged(int newPage);

private:

    QImage left_rightSideOfPage(bool left_right, QImage & image);


    QImage getCurrentImageFromCache(DynamicPdfPageCache & cache);


    void setCacheDocument(int dpri, DynamicPdfPageCache & cache, QSize size, double splitscreen, bool createThumbnails = false);


    DynamicPdfPageCache cache_preview;
    DynamicPdfPageCache cache_mainScreen;
    DynamicPdfPageCache cache_lecturerScreen;

//    PdfRenderer renderer_preview;
//    PdfRenderer renderer_mainScreen;
//    PdfRenderer renderer_helperScreen;

    uint32_t currentPage = 0;
    uint32_t numOfPages = 0;

    QString filename;



    // rectangles for the supported screens
    // should be okay for now, maybe once a map...
    QSize preview_size;
    QSize main_size;
    QSize helper_size;


};

#endif // PRESENTATION_H

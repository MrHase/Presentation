#ifndef PDFRENDERER_H
#define PDFRENDERER_H
#include <QImage>
#include <QString>
#include <poppler/qt5/poppler-qt5.h>
#include <QDebug>

#include <memory>
#include <string>
#include <vector>
#include <functional>

#include <thread>

#include "exception.h"

using namespace std;

class RenderInfo
{
public:
    uint32_t requested_width = 0; // size in points? ich geh jetzt davon aus
    uint32_t requested_height = 0;
    bool splitscreen = false; // two slides on one page
};

class DpiRequest
{
public:
    double dpiWidth;
    double dpiHeight;
};


class PdfRenderer
{
private:
    shared_ptr<Poppler::Document> doc;
    bool isDocumentSet=false;
    vector<QImage> pageCache;

    QRect size;
    double thisDevicePixelRatio;

    mutex mutex_cache;




    /*
    double renderOptionDpiXAxis; //= 144.0;
    double renderOptionDpiYAxis; //= 144.0;
    */


    void calculateDPI(RenderInfo ri, Poppler::Page *page, std::function<void(double dpi_x, double dpi_y)> use_dpi);
    DpiRequest calculateDPI(RenderInfo ri, Poppler::Page *page);


public:
    PdfRenderer(double dpri);
    PdfRenderer(QRect size, double dpri);


    uint32_t pages();
    virtual ~PdfRenderer();


    void setDocument(QString filePath);



    void renderDocumentIntoCache(RenderInfo ri);



    QImage getRenderedImage(int pageNum);
    bool documentSet() const;


    QRect getSize() const;
    void setSize(const QRect &value);
    double getThisDevicePixelRatio() const;
    void setThisDevicePixelRatio(double value);

};

#endif // PDFRENDERER_H

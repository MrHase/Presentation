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


using namespace std;

class RenderInfo
{
public:
    uint32_t requested_width=0;
    uint32_t requested_height=0;
    bool splitscreen=false;
};

class PdfRenderer
{
private:
    shared_ptr<Poppler::Document> doc;
    bool isDocumentSet=false;
    vector<QImage> pageCache;

    /*
    double renderOptionDpiXAxis; //= 144.0;
    double renderOptionDpiYAxis; //= 144.0;
    */
    void renderDocumentIntoCache(RenderInfo ri);

    void calculateDPI(RenderInfo ri, Poppler::Page *page, std::function<void(double dpi_x, double dpi_y)> use_dpi);

public:
    PdfRenderer(){}



    uint32_t pages();
    virtual ~PdfRenderer();


    void setDocument(QString filePath);




    QImage getRenderedImage(int pageNum);
    bool documentSet() const;
};

#endif // PDFRENDERER_H

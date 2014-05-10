#ifndef PDFRENDERER_H
#define PDFRENDERER_H
#include <QImage>
#include <QString>
#include <poppler/qt5/poppler-qt5.h>
#include <QDebug>

#include <memory>

using namespace std;
class PdfRenderer
{
private:
    shared_ptr<Poppler::Document> doc;
    bool isDocumentSet;
public:
    PdfRenderer(QString filePath);
    PdfRenderer();

    uint32_t pages();
    virtual ~PdfRenderer();


    void setDocument(QString filePath);
    QImage getRenderedImage(int pageNum);
};

#endif // PDFRENDERER_H

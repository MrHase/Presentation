#ifndef HELPERSCREENPRESENTATION_H
#define HELPERSCREENPRESENTATION_H

#include <QWidget>

namespace Ui {
class helperscreenpresentation;
}

class HelperScreenPresentation : public QWidget
{
    Q_OBJECT

public:
    explicit HelperScreenPresentation(QWidget *parent = 0);
    ~HelperScreenPresentation();

private:
    Ui::helperscreenpresentation *ui;
};

#endif // HELPERSCREENPRESENTATION_H

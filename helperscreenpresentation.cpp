#include "helperscreenpresentation.h"
#include "ui_helperscreenpresentation.h"

HelperScreenPresentation::HelperScreenPresentation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helperscreenpresentation)
{
    ui->setupUi(this);
}

HelperScreenPresentation::~HelperScreenPresentation()
{
    delete ui;
}

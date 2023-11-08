#include "CCurveStisticalForm.h"
#include "ui_CCurveStisticalForm.h"

CCurveStisticalForm::CCurveStisticalForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCurveStisticalForm)
{
    ui->setupUi(this);
}

CCurveStisticalForm::~CCurveStisticalForm()
{
    delete ui;
}

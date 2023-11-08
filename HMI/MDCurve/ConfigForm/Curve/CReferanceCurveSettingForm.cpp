#include "CReferanceCurveSettingForm.h"
#include "ui_CReferanceCurveSettingForm.h"

CReferanceCurveSettingForm::CReferanceCurveSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CReferanceCurveSettingForm)
{
    ui->setupUi(this);
}

CReferanceCurveSettingForm::~CReferanceCurveSettingForm()
{
    delete ui;
}

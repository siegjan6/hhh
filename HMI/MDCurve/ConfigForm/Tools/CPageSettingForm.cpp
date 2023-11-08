#include "CPageSettingForm.h"
#include "ui_CPageSettingForm.h"

CPageSettingForm::CPageSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPageSettingForm)
{
    ui->setupUi(this);
}

CPageSettingForm::~CPageSettingForm()
{
    delete ui;
}

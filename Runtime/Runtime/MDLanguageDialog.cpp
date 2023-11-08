#include "MDLanguageDialog.h"
#include "ui_MDLanguageDialog.h"
#include "MDMultiLanguage.h"

CMDLanguageDialog::CMDLanguageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDLanguageDialog)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(CMDMultiLanguage::instance()->toolWidget());
}

CMDLanguageDialog::~CMDLanguageDialog()
{
    delete ui;
}

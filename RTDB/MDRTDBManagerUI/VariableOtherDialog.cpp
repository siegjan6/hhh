#include "VariableOtherDialog.h"
#include "ui_VariableOtherDialog.h"
#include "MDMultiLanguage.h"
CVariableOtherDialog::CVariableOtherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariableOtherDialog)
{
    ui->setupUi(this);
    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CVariableOtherDialog::~CVariableOtherDialog()
{
    delete ui;
}

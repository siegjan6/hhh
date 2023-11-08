#include "SetRecipeTemplateDialog.h"
#include "ui_SetRecipeTemplateDialog.h"

CSetRecipeTemplateDialog::CSetRecipeTemplateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSetRecipeTemplateDialog)
{
    ui->setupUi(this);

}

CSetRecipeTemplateDialog::~CSetRecipeTemplateDialog()
{
    delete ui;
}

void CSetRecipeTemplateDialog::on_pushbuttonOk_clicked()
{
    accept();
}

void CSetRecipeTemplateDialog::on_pushbuttonCancel_clicked()
{
    reject();
}

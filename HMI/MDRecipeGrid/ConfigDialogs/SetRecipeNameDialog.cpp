#include "SetRecipeNameDialog.h"
#include "ui_SetRecipeNameDialog.h"
#include "ShareTool.h"

CSetRecipeNameDialog::CSetRecipeNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSetRecipeNameDialog)
{
    ui->setupUi(this);
    CShareTool::installDialogTranslator(this);
}

CSetRecipeNameDialog::~CSetRecipeNameDialog()
{
    delete ui;
}

void CSetRecipeNameDialog::on_pushbuttonOk_clicked()
{
    accept();
}

void CSetRecipeNameDialog::on_pushbuttonCancel_clicked()
{
    reject();
}

void CSetRecipeNameDialog::on_pushbuttonSelectRecipe_clicked()
{

}

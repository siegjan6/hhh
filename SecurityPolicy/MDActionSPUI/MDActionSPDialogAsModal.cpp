#include "MDActionSPDialogAsModal.h"
#include "ui_MDActionSPDialogAsModal.h"

CMDActionSPDialogAsModal::CMDActionSPDialogAsModal(QWidget *parent) :
    CMDActionSPListDialog(parent),
    ui(new Ui::CMDActionSPDialogAsModal)
{
    ui->setupUi(this);
}

CMDActionSPDialogAsModal::~CMDActionSPDialogAsModal()
{
    delete ui;
}

void CMDActionSPDialogAsModal::on_OKButton_clicked()
{
    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDActionSPDialogAsModal::on_CancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject();
}


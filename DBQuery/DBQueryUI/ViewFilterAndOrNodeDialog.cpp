#include "ViewFilterAndOrNodeDialog.h"
#include "ui_ViewFilterAndOrNodeDialog.h"

ViewFilterAndOrNodeDialog::ViewFilterAndOrNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFilterAndOrNodeDialog)
{
    ui->setupUi(this);
    QWidget::setTabOrder(ui->typeComboBox, ui->okButton);
    QWidget::setTabOrder(ui->okButton, ui->cancelButton);
    QWidget::setTabOrder(ui->cancelButton, ui->typeComboBox);
}

ViewFilterAndOrNodeDialog::~ViewFilterAndOrNodeDialog()
{
    delete ui;
}

void ViewFilterAndOrNodeDialog::setType(DBQ::VIEW_FILTER_NODE_TYPE type)
{
    if(DBQ::VIEW_FILTER_NODE_TYPE_AND == type)
    {
        ui->typeComboBox->setCurrentIndex(0);
    }else if(DBQ::VIEW_FILTER_NODE_TYPE_OR == type)
    {
        ui->typeComboBox->setCurrentIndex(1);
    }
    else
    {
        //TODO:log
    }
}

DBQ::VIEW_FILTER_NODE_TYPE ViewFilterAndOrNodeDialog::getType()
{
    if(ui->typeComboBox->currentIndex() == 1)
        return DBQ::VIEW_FILTER_NODE_TYPE_OR;

    return DBQ::VIEW_FILTER_NODE_TYPE_AND;
}

void ViewFilterAndOrNodeDialog::on_okButton_clicked()
{
    accept();
}

void ViewFilterAndOrNodeDialog::on_cancelButton_clicked()
{
    reject();
}

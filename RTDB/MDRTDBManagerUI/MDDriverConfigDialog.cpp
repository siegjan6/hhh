#include "MDDriverConfigDialog.h"
#include "ui_MDDriverConfigDialog.h"

CMDDriverConfigDialog::CMDDriverConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDriverConfigDialog)
{
    ui->setupUi(this);
}

CMDDriverConfigDialog::~CMDDriverConfigDialog()
{
    delete ui;
}

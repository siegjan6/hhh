#include "testdatafilenamedialog.h"
#include "ui_testdatafilenamedialog.h"

TestDataFileNameDialog::TestDataFileNameDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestDataFileNameDialog)
{
    ui->setupUi(this);
}

TestDataFileNameDialog::~TestDataFileNameDialog()
{
    delete ui;
}

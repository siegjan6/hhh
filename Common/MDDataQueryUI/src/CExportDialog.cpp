#include "CExportDialog.h"
#include "ui_CExportDialog.h"

CExportDialog::CExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CExportDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("文件类型选择"));
}

CExportDialog::~CExportDialog()
{
    delete ui;
}

int CExportDialog::exportType() const
{
    if(ui->csvButton->isChecked())
        return (int)exType::Csv;
    else if(ui->xmlButton->isChecked())
        return (int)exType::Xml;
    else if(ui->jsonButton->isChecked())
        return (int)exType::Json;
}

void CExportDialog::on_confirmButton_clicked()
{
    accept();
}

void CExportDialog::on_cancelButton_clicked()
{
    reject();
}

#include "InsertRCDialog.h"
#include "ui_InsertRCDialog.h"
#include "ShareTool.h"

CInsertRCDialog::CInsertRCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CInsertRCDialog)
{
    ui->setupUi(this);

    ui->rowColumnEdit->setText("1");
    ui->rowColumnEdit->setValidator(new QIntValidator(1,10000,this));
    ui->rowButton->setChecked(true);
    m_count = 1;
    m_row = true;

    CShareTool::installDialogTranslator(this);
}

CInsertRCDialog::~CInsertRCDialog()
{
    delete ui;
}

void CInsertRCDialog::on_confirmButton_clicked()
{
    int num = ui->rowColumnEdit->text().toLongLong();
    m_count = num;
    accept();
}

void CInsertRCDialog::on_cancelButton_clicked()
{
    reject();
}

void CInsertRCDialog::on_rowButton_toggled(bool checked)
{
    m_row = checked;
}


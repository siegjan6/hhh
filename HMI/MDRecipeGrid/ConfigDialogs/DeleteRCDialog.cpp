#include "DeleteRCDialog.h"
#include "ui_DeleteRCDialog.h"
#include "ShareTool.h"

CDeleteRCDialog::CDeleteRCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDeleteRCDialog)
{
    ui->setupUi(this);
    ui->rowRadioButton->setChecked(true);
    m_row = true;

    CShareTool::installDialogTranslator(this);
}

CDeleteRCDialog::~CDeleteRCDialog()
{
    delete ui;
}


void CDeleteRCDialog::on_rowRadioButton_toggled(bool checked)
{
    m_row = checked;
}

void CDeleteRCDialog::on_confirmButton_clicked()
{
    accept();
}

void CDeleteRCDialog::on_cancelButton_clicked()
{
    reject();
}

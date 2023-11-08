#include "SetHWDialog.h"
#include "ui_SetHWDialog.h"
#include "ShareTool.h"
CSetHWDialog::CSetHWDialog(quint32 defaultRowHeight, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSetHWDialog)
{
    ui->setupUi(this);

    m_row = true;
    m_number = defaultRowHeight;

    ui->radiobuttonRow->setChecked(true);

    ui->lineeditNumber->setText(QString::number(m_number));
    ui->lineeditNumber->setValidator(new QIntValidator(1,1000,this));

    CShareTool::installDialogTranslator(this);
}

CSetHWDialog::~CSetHWDialog()
{
    delete ui;
}

void CSetHWDialog::accept()
{
    m_row = ui->radiobuttonRow->isChecked();
    bool ok = false;
    quint32 number = ui->lineeditNumber->text().toUInt(&ok);
    if(ok)
        m_number = number;

    QDialog::accept();
}

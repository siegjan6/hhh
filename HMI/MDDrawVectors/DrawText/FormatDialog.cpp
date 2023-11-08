#include "FormatDialog.h"
#include "ui_FormatDialog.h"
#include "MDMultiLanguage.h"

CFormatDialog::CFormatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFormatDialog)
{
    ui->setupUi(this);

    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}
CFormatDialog::~CFormatDialog()
{
    delete ui;
}
void CFormatDialog::on_pushButtonOk_clicked()
{
    saveData();
    accept();
}
void CFormatDialog::on_pushButtonCancel_clicked()
{
    reject();
}
////////////////////////////////////////////////////////////
void CFormatDialog::init(CMDDrawText* obj)
{
    ui->spinBoxDecimal->setValue(obj->decimal());
    ui->lineEditDateFormat->setText(obj->dateFormat());
    ui->spinBoxMaxLength->setValue(obj->maxLength());
}
void CFormatDialog::saveData()
{
    m_value.insert("Decimal", ui->spinBoxDecimal->value());
    m_value.insert("DateFormat", ui->lineEditDateFormat->text().simplified());
    m_value.insert("MaxLength", ui->spinBoxMaxLength->value());
}
////////////////////////////////////////////////////////////

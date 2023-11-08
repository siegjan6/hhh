#include "MDDrawTextEditFormatDialog.h"
#include "ui_MDDrawTextEditFormatDialog.h"
#include "MDMultiLanguage.h"

MDDrawTextEditFormatDialog::MDDrawTextEditFormatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MDDrawTextEditFormatDialog)
{
    ui->setupUi(this);

    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

MDDrawTextEditFormatDialog::~MDDrawTextEditFormatDialog()
{
    delete ui;
}

void MDDrawTextEditFormatDialog::on_pushButtonOk_clicked()
{
    saveData();
    accept();
}

void MDDrawTextEditFormatDialog::on_pushButtonCancel_clicked()
{
    reject();
}

void MDDrawTextEditFormatDialog::init(CMDDrawTextEdit *obj)
{
    ui->spinBoxDecimal->setValue(obj->decimal());
    ui->lineEditDateFormat->setText(obj->dateFormat());
    ui->spinBoxMaxLength->setValue(obj->varMaxLength());
}

void MDDrawTextEditFormatDialog::saveData()
{
    m_value.insert("Decimal", ui->spinBoxDecimal->value());
    m_value.insert("DateFormat", ui->lineEditDateFormat->text().simplified());
    m_value.insert("VarMaxLength", ui->spinBoxMaxLength->value());
}

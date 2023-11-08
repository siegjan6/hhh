#include "NewFieldDialog.h"
#include "ui_NewFieldDialog.h"
#include "QMessageBox"

NewFieldDialog::NewFieldDialog(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFieldDialog),
    m_Field(new DBQ::ViewField),
    m_windowTitle(title)
{
    ui->setupUi(this);
    QWidget::setTabOrder(ui->lineEdit_tableName,ui->lineEdit_field);
    QWidget::setTabOrder(ui->lineEdit_field, ui->lineEdit_alias);
    QWidget::setTabOrder(ui->lineEdit_alias, ui->comboBox_Type);
    QWidget::setTabOrder(ui->comboBox_Type, ui->lineEdit_description);
    QWidget::setTabOrder(ui->lineEdit_description, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_tableName);
}

NewFieldDialog::~NewFieldDialog()
{
    delete ui;

    if(m_Field)
    {
        delete m_Field;
        m_Field = NULL;
    }
}

DBQ::ViewField *NewFieldDialog::getField()
{
    return m_Field;
}

void NewFieldDialog::setOriginalViewFieldParameter(DBQ::ViewField *viewField)
{
    ui->lineEdit_field->setText(viewField->fieldName);
    ui->lineEdit_tableName->setText(viewField->tableName);
    ui->lineEdit_alias->setText(viewField->alias);
    ui->comboBox_Type->setCurrentIndex((int)viewField->type-1);
    ui->lineEdit_description->setText(viewField->description);
}

void NewFieldDialog::on_pbt_Confirm_clicked()
{
    if(ui->lineEdit_field->text().isEmpty())
    {
        QMessageBox::information(NULL,"警告",tr("字段名称为空！"));
        return;
    }

    if(ui->lineEdit_tableName->text().isEmpty())
    {
        QMessageBox::information(NULL,"警告",tr("表名称为空！"));
        return;
    }

    QRegExp fieldaAlias_reg_exp;
    if(ui->lineEdit_alias->text().isEmpty())
    {
        ui->lineEdit_alias->setText(ui->lineEdit_field->text());
    }
    fieldaAlias_reg_exp.setPattern("^^[a-zA-Z]+[a-zA-Z0-9_]{1,30}$");
    QRegExpValidator fieldaAlias_validator(fieldaAlias_reg_exp);
    if(!fieldaAlias_validator.regExp().exactMatch(ui->lineEdit_alias->text()))
    {
        QMessageBox::information(NULL,"警告",tr("字段别名不正确！"));
        return;
    }

    m_Field->tableName = ui->lineEdit_tableName->text();
    m_Field->fieldName = ui->lineEdit_field->text();
    m_Field->alias = ui->lineEdit_alias->text();
    m_Field->type = (DBQ::VIEW_FIELD_TYPE)(ui->comboBox_Type->currentIndex()+1);
    m_Field->description = ui->lineEdit_description->text();
    this->accept();
}

void NewFieldDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}

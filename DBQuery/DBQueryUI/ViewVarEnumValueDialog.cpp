#include "ViewVarEnumValueDialog.h"
#include "ui_ViewVarEnumValueDialog.h"
#include <QMessageBox>

ViewVarEnumValueDialog::ViewVarEnumValueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewVarEnumValueDialog)
{
    ui->setupUi(this);
    initTableWidget();

    QWidget::setTabOrder(ui->lineEdit_enumValue,ui->lineEdit_description);
    QWidget::setTabOrder(ui->lineEdit_description, ui->pbt_Add);
    QWidget::setTabOrder(ui->pbt_Add, ui->pbt_Delete);
    QWidget::setTabOrder(ui->pbt_Delete,ui->tableWidget);
    QWidget::setTabOrder(ui->tableWidget,ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_enumValue);
}

ViewVarEnumValueDialog::~ViewVarEnumValueDialog()
{
    delete ui;
}

QList<DBQ::ViewVariableEnumValue> ViewVarEnumValueDialog::getEnumValueList() const
{
    return m_enumValueList;
}

void ViewVarEnumValueDialog::setEnumValueList(const QList<DBQ::ViewVariableEnumValue> &enumValueList)
{
    m_enumValueList = enumValueList;
    bool ok = true;
    if(ok)
    {
        foreach (DBQ::ViewVariableEnumValue value, m_enumValueList)
        {
            DBQ::ViewVariableEnumValue pValueCopy ;
            pValueCopy = value;
            m_enumValueListCopy.append(pValueCopy);
            ok = false;
        }
    }
    initTableWidget();
}

void ViewVarEnumValueDialog::initTableWidget()
{
    QStringList header;
    header<<"枚举值"<<"描述";
    ui->tableWidget->setWindowTitle("枚举值列表");
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setRowCount(m_enumValueList.count());
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(30);
    ui->tableWidget->verticalHeader()->hide();
    int size = m_enumValueList.size();
    for(int i = 0 ; i < size; i++)
    {
        QTableWidgetItem *itemValue = new QTableWidgetItem(m_enumValueList.at(i).value);
        itemValue->setTextAlignment( Qt::AlignCenter);
        QTableWidgetItem *itemDesc = new QTableWidgetItem(m_enumValueList.at(i).desc);
        itemDesc->setTextAlignment( Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,itemValue);
        ui->tableWidget->setItem(i,1,itemDesc);
    }
}

bool ViewVarEnumValueDialog::getEnumValue(const QString &name)
{
    bool ok = false;
    DBQ::ViewVariableEnumValue enumValue;
    int size = m_enumValueList.size();
    for (int i = 0; i < size; ++i)
    {
        enumValue = m_enumValueList.at(i);
        if (enumValue.value == name)
        {
            ok = true;
        }
    }
    return ok;
}

void ViewVarEnumValueDialog::setEnumValueType(int type)
{
    ui->comboBox_Type->setCurrentIndex((int)type);
}

void ViewVarEnumValueDialog::saveEnumValueList()
{
    m_enumValueList.clear();
    DBQ::ViewVariableEnumValue enumValue;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if(ui->tableWidget->item(i,0)->text().isEmpty())
        {
            QMessageBox::information(NULL,"警告",tr("添加枚举值名称为空！"));
            return;
        }
        enumValue.value = ui->tableWidget->item(i,0)->text();
        enumValue.desc = ui->tableWidget->item(i,1)->text();
        m_enumValueList.push_back(enumValue);
    }
}

void ViewVarEnumValueDialog::on_pbt_Add_clicked()
{
    DBQ::ViewVariableEnumValue enumValue;
    if(ui->lineEdit_enumValue->text().isEmpty())
    {
        QMessageBox::information(NULL,"警告",tr("添加枚举值名称为空！"));
        return;
    }
    saveEnumValueList();

    QString value = ui->lineEdit_enumValue->text();
    if(!getEnumValue(value))
    {
        enumValue.value = ui->lineEdit_enumValue->text();
        enumValue.desc = ui->lineEdit_description->text();
        m_enumValueList.append(enumValue);
    }
    else
    {
        QMessageBox::information(NULL,"警告",tr("当前枚举值已存在！"));
        return;
    }
    ui->lineEdit_enumValue->clear();
    ui->lineEdit_description->clear();
    initTableWidget();
}

void ViewVarEnumValueDialog::on_pbt_Delete_clicked()
{
    QString strMsg = QString("确定删除'%1'枚举值吗？").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text());
    QMessageBox message(QMessageBox::Information,"提示", strMsg, QMessageBox::Yes | QMessageBox::No, NULL);
    if (QMessageBox::Yes == message.exec())
    {
        int rowIndex = ui->tableWidget->currentRow();
        if (rowIndex != -1)
        {
            ui->tableWidget->removeRow(rowIndex);
            m_enumValueList.takeAt(rowIndex);
        }
        ui->lineEdit_enumValue->clear();
        ui->lineEdit_description->clear();
        initTableWidget();
    }
}

void ViewVarEnumValueDialog::on_pbt_Confirm_clicked()
{
    saveEnumValueList();
    this->accept();
}

void ViewVarEnumValueDialog::on_pbt_Cancel_clicked()
{
    m_enumValueList.clear();
    foreach (DBQ::ViewVariableEnumValue pValueCopy, m_enumValueListCopy)
    {
        DBQ::ViewVariableEnumValue value;
        value = pValueCopy;
        m_enumValueList.append(value);
    }
    this->reject();
}



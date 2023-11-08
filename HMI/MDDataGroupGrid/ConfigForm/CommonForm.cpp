#include <QLineEdit>
#include "CommonForm.h"
#include "ui_CommonForm.h"
#include "IMDDataArchives.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"

CCommonForm::CCommonForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCommonForm)
{
    ui->setupUi(this);
    m_isDataArchives = false;

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

CCommonForm::~CCommonForm()
{
    delete ui;
}

void CCommonForm::initialize()
{
    loadTabHeaderProperty();
    loadTabContentProperty();
    creatConnection();
    setControlVisible();
}

void CCommonForm::setDBQueryPath(const QString &path)
{
    m_DBQueryPath = path;
}

void CCommonForm::initViewList(QString viewName)
{  
    QStringList viewList;
    viewList.clear();
    if(DBQ::View_GetAllViewNameFromDir(m_DBQueryPath,viewList))
    {
        if(!viewList.isEmpty())
        {
            ui->comboBox_views->addItems(viewList);
            ui->comboBox_views->setCurrentText(viewName);
            QString strErr = "";
            m_viewName = ui->comboBox_views->currentText();
            m_viewfields = DBQ::View_GetAllViewFieldsInViewFromDir(m_DBQueryPath,m_viewName,strErr);
        }
    }
}

QString CCommonForm::getViewName() const
{
    return m_viewName;
}

void CCommonForm::setViewName(QString viewName)
{
    m_viewName = viewName;
}

void CCommonForm::setFields(QList<CDataGroupField> *value)
{
    m_fields = *value;
}

QList<CDataGroupField> *CCommonForm::fields()
{
    return &m_fields;
}

void CCommonForm::loadTabHeaderProperty()
{
    ui->tableWidget->setRowCount(m_fields.count());
    ui->tableWidget->setColumnCount(5);
    QStringList  headStringList;
    headStringList << tr("") << tr("名称") << tr("宽度") << tr("格式化") << tr("描述");
    ui->tableWidget->setHorizontalHeaderLabels(headStringList);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->resizeSection(0,30);
    ui->tableWidget->horizontalHeader()->resizeSection(1,120);
    ui->tableWidget->horizontalHeader()->resizeSection(2,40);
    ui->tableWidget->horizontalHeader()->resizeSection(3,160);
    ui->tableWidget->horizontalHeader()->resizeSection(4,100);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CCommonForm::loadTabContentProperty()
{
    editList.clear();
    for(int i = 0; i < m_fields.count(); i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem);
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        if(!m_fields.at(i).checkState())
        {
            ui->tableWidget->item(i,0)->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui->tableWidget->item(i,0)->setCheckState(Qt::Checked);
        }
        edit = new QLineEdit();
        edit->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        edit->setMaxLength(4);
        edit->setFrame(false);
        QIntValidator* val = new QIntValidator(1,300,this);
        edit->setValidator(val);
        edit->setText(QString::number(m_fields.at(i).width()));
        editList.append(edit);

        QTableWidgetItem *item01 = new QTableWidgetItem(m_fields.at(i).name());
        item01->setFlags(item01->flags() & (~Qt::ItemIsEditable));
        ui->tableWidget->setItem(i,1,item01);
        ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidget->setCellWidget(i,2,edit);

        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(m_fields.at(i).dataFormat()));
        ui->tableWidget->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        QTableWidgetItem *item02 = new QTableWidgetItem(m_fields.at(i).comment());
        item02->setFlags(item02->flags() & (~Qt::ItemIsEditable));
        ui->tableWidget->setItem(i,4,item02);
        ui->tableWidget->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
}

void CCommonForm::setControlVisible()
{
    if(m_isDataArchives)
    {
        ui->label_views->setVisible(true);
        ui->comboBox_views->setVisible(true);
    }
    else
    {
        ui->label_views->setVisible(false);
        ui->comboBox_views->setVisible(false);
    }
}

MDDataType CCommonForm::convertViewFieldValueTypetoMDDataType(DBQ::VIEW_FIELD_TYPE fieldType)
{
    switch (fieldType)
    {
    case DBQ::VIEW_FIELD_TYPE_BOOL : return MDDataType::Bool;
    case DBQ::VIEW_FIELD_TYPE_SBYTE : return MDDataType::SByte;
    case DBQ::VIEW_FIELD_TYPE_BYTE : return MDDataType::Byte;
    case DBQ::VIEW_FIELD_TYPE_INT16 : return MDDataType::Int16;
    case DBQ::VIEW_FIELD_TYPE_UINT16 : return MDDataType::UInt16;
    case DBQ::VIEW_FIELD_TYPE_INT32 : return MDDataType::Int32;
    case DBQ::VIEW_FIELD_TYPE_UINT32 : return MDDataType::UInt32;
    case DBQ::VIEW_FIELD_TYPE_INT64 : return MDDataType::Int64;
    case DBQ::VIEW_FIELD_TYPE_UINT64 : return MDDataType::UInt64;
    case DBQ::VIEW_FIELD_TYPE_FLOAT : return MDDataType::Float;
    case DBQ::VIEW_FIELD_TYPE_DOUBLE : return MDDataType::Double;
    case DBQ::VIEW_FIELD_TYPE_DATETIME : return MDDataType::DateTime;
    case DBQ::VIEW_FIELD_TYPE_STRING : return MDDataType::String;
    default:return MDDataType::String;
    }
}

void CCommonForm::setDataGroupFields(QList<DBQ::ViewField *> viewFieldsList)
{
    m_fields.clear();

    for(int i = 0; i < viewFieldsList.count(); i++)
    {
        CDataGroupField field;
        field.setName(viewFieldsList.at(i)->fieldName);
        field.setComment(viewFieldsList.at(i)->description);
        field.setType(convertViewFieldValueTypetoMDDataType(viewFieldsList.at(i)->type));
        field.setWidth(100);
        field.setCheckState(true);
        if(field.type() == MDDataType::DateTime)
        {
            field.setDataFormat("yyyy-MM-dd hh:mm:ss");
            field.setWidth(200);
        }
        else if(field.type() == MDDataType::Double || field.type() == MDDataType::Float)
        {
            field.setDataFormat("%2f");
        }
        else if (field.type() == MDDataType::String || field.type() == MDDataType::Int32)
        {
            field.setDataFormat("");
        }
        m_fields.append(field);
    }
}


void CCommonForm::saveFields()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        CDataGroupField field;
        bool ok = false;
        field.setCheckState(ui->tableWidget->item(i,0)->checkState()==Qt::Checked);
        field.setName(ui->tableWidget->item(i,1)->text());
        field.setComment(ui->tableWidget->item(i,4)->text());
        field.setWidth(editList.at(i)->text().toInt(&ok,10));
        field.setDataFormat(ui->tableWidget->item(i,3)->text());
        field.setType(m_fields.at(i).type());
        m_fields[i] = field;
    }
}

void CCommonForm::recreatFields(QList<DBQ::ViewField *> viewFieldsList)
{
    setDataGroupFields(viewFieldsList);
    loadTabHeaderProperty();
    loadTabContentProperty();
    saveFields();
}

void CCommonForm::creatConnection()
{
    connect(ui->comboBox_views,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_viewNameIndexChanged(QString)));
    connect(ui->allButton, &QPushButton::clicked, this, &CCommonForm::slotSelectAllButton);
    connect(ui->noButton, &QPushButton::clicked, this, &CCommonForm::slotSelectNoneButton);
    connect(ui->upButton, &QPushButton::clicked, this, &CCommonForm::slotUpButton);
    connect(ui->downButton, &QPushButton::clicked, this, &CCommonForm::slotDownButton);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &CCommonForm::slotMoveProtectButton);
}

void CCommonForm::slotSelectAllButton()
{
    for(int i = 0;i<(ui->tableWidget->rowCount());i++)
    {
        ui->tableWidget->item(i,0)->setCheckState(Qt::Checked);
    }
}

void CCommonForm::slotSelectNoneButton()
{
    for(int i = 0;i<(ui->tableWidget->rowCount());i++)
    {
        ui->tableWidget->item(i,0)->setCheckState(Qt::Unchecked);
    }
}

void CCommonForm::slotUpButton()
{
    int selRow = ui->tableWidget->currentRow();
    Qt::CheckState strCurr0 = ui->tableWidget->item(selRow,0)->checkState();
    Qt::CheckState strNext0 = ui->tableWidget->item(selRow - 1,0)->checkState();
    ui->tableWidget->item(selRow,0)->setCheckState(strNext0);
    ui->tableWidget->item(selRow - 1,0)->setCheckState(strCurr0);
    for(int i = 1; i < 5; i++)
    {
        if(2 == i)
        {
            QString strCur = editList.at(selRow)->text();
            QString strNext = editList.at(selRow - 1)->text();
            editList.at(selRow)->setText(strNext);
            editList.at(selRow - 1)->setText(strCur);
        }
        else
        {
            QString strCur = ui->tableWidget->item(selRow,i)->text();
            QString strNext = ui->tableWidget->item(selRow - 1,i)->text();
            ui->tableWidget->item(selRow,i)->setText(strNext);
            ui->tableWidget->item(selRow - 1,i)->setText(strCur);
        }
    }
    ui->tableWidget->selectRow(selRow - 1);
}

void CCommonForm::slotDownButton()
{
    int selRow = ui->tableWidget->currentRow();
    Qt::CheckState strCurr0 = ui->tableWidget->item(selRow,0)->checkState();
    Qt::CheckState strNext0 = ui->tableWidget->item(selRow + 1,0)->checkState();
    ui->tableWidget->item(selRow,0)->setCheckState(strNext0);
    ui->tableWidget->item(selRow + 1,0)->setCheckState(strCurr0);
    for(int i = 1; i < 5; i++)
    {
        if(2 == i)
        {
            QString strCur = editList.at(selRow)->text();
            QString strNext = editList.at(selRow + 1)->text();
            editList.at(selRow)->setText(strNext);
            editList.at(selRow + 1)->setText(strCur);
        }
        else
        {
            QString strCur = ui->tableWidget->item(selRow,i)->text();
            QString strNext = ui->tableWidget->item(selRow + 1,i)->text();
            ui->tableWidget->item(selRow,i)->setText(strNext);
            ui->tableWidget->item(selRow + 1,i)->setText(strCur);
        }
    }
    ui->tableWidget->selectRow(selRow + 1);
}

void CCommonForm::slotMoveProtectButton()
{
    int currow = ui->tableWidget->currentRow();
    int maxrow = ui->tableWidget->rowCount()-1;
    if(currow == 0)
    {
        ui->upButton->setEnabled(false);
    }
    else
    {
        ui->upButton->setEnabled(true);
    }
    if(currow == maxrow)
    {
        ui->downButton->setEnabled(false);
    }
    else
    {
        ui->downButton->setEnabled(true);
    }
}

void CCommonForm::slot_viewNameIndexChanged(const QString &viewName)
{
    m_viewName = viewName;
    QString strErr = "";
    m_viewfields = DBQ::View_GetAllViewFieldsInViewFromDir(m_DBQueryPath,m_viewName,strErr);
    recreatFields(m_viewfields);
}

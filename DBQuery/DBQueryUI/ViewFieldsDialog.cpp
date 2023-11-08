#include "ViewFieldsDialog.h"
#include "ui_ViewFieldsDialog.h"
#include <MDDBQueryConfigure.h>
#include <QVariant>
#include <assert.h>
#include <QDebug>

Q_DECLARE_METATYPE(DBQ::ViewField *)

ViewFieldsDialog::ViewFieldsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFieldsDialog)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(0, 5);
    m_returnType = DBQUI::VIEW_FIELD_RETURN_TYPE_ALL;

    ui->fieldTableView->setModel(m_pModel);
    ui->fieldTableView->setSelectionBehavior(QAbstractItemView::SelectRows);    //! 整行选中的方式
    ui->fieldTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);     //! 禁止编辑
    m_pModel->setHeaderData(0, Qt::Horizontal, QObject::tr("字段名"));
    m_pModel->setHeaderData(1, Qt::Horizontal, QObject::tr("字段类型"));
    m_pModel->setHeaderData(2, Qt::Horizontal, QObject::tr("字段别名"));
    m_pModel->setHeaderData(3, Qt::Horizontal, QObject::tr("表名"));
    m_pModel->setHeaderData(4, Qt::Horizontal, QObject::tr("描述"));
    ui->fieldTableView->verticalHeader()->hide();                               //! 隐藏计数列
    ui->fieldTableView->horizontalHeader()->setStretchLastSection(true);

    QWidget::setTabOrder(ui->viewNameLineEdit,ui->fieldTableView);
    QWidget::setTabOrder(ui->fieldTableView, ui->okButton);
    QWidget::setTabOrder(ui->okButton, ui->cancelButton);
    QWidget::setTabOrder(ui->cancelButton, ui->viewNameLineEdit);
}

ViewFieldsDialog::~ViewFieldsDialog()
{
    delete ui;
    foreach(DBQ::ViewField * pField, m_FieldsSelected)
        delete pField;
}

bool ViewFieldsDialog::LoadView(const QString & configFileDir, const QString & viewName)
{
    ui->viewNameLineEdit->setText(viewName);
    QString fullPath = configFileDir;
    fullPath.append("/view_").append(viewName).append(".json");
    DBQ::View * pView = 0;
    QString errStr;
    if(!DBQ::View_Load(fullPath, DBQ::FILE_TYPE_JSON, &pView, errStr))
        return false; //TODO:log

    if(!pView)
        return false; //TODO:log

    //QVariant variant;


    QList<DBQ::ViewField *> showFields;
    showFields.clear();

    switch (m_returnType)
    {
    case DBQUI::VIEW_FIELD_RETURN_TYPE_ALL:
        foreach (DBQ::ViewField * pField, pView->fields)
        {
            DBQ::ViewField * pFieldCp = new DBQ::ViewField;
            *pFieldCp = * pField;
            showFields.append(pFieldCp);
        }
        break;
    case DBQUI::VIEW_FIELD_RETURN_TYPE_DATETIME:
    {
        showFields.clear();
        for(int i = 0; i < pView->fields.size(); i++)
        {
            if(pView->fields.at(i)->type == DBQ::VIEW_FIELD_TYPE_DATETIME)
            {
                showFields.append(pView->fields.at(i));
            }
        }
        break;
    }
    case DBQUI::VIEW_FIELD_RETURN_TYPE_VARIABLE:
    {
        showFields.clear();
        for(int i = 0; i < pView->fields.size(); i++)
        {
            if(pView->fields.at(i)->type != DBQ::VIEW_FIELD_TYPE_DATETIME)
            {
                showFields.append(pView->fields.at(i));
            }
        }
        break;
    }
    default:
        break;
    }

    int size = showFields.size();
    for(int i = 0; i < size; i++)
    {
        DBQ::ViewField * pField = new DBQ::ViewField;
        *pField = *showFields[i];

        //释放内存老的Field的指针
        //当第一次显示表格时 index无效
        QModelIndex index = m_pModel->index(i, 0);
        if(index.isValid())
        {
            QVariant var = m_pModel->data(index, Qt::UserRole);
            DBQ::ViewField * pFieldOld = var.value<DBQ::ViewField *>();
            if(pFieldOld)
                delete pFieldOld;
        }

        //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
        m_pModel->setItem(i, 0, new QStandardItem(pField->fieldName)); //! 字段名
        m_pModel->setItem(i, 1, new QStandardItem(DBQ::View_GetFieldTypeStringFromType(pField->type)));
        m_pModel->setItem(i, 2, new QStandardItem(pField->alias));
        m_pModel->setItem(i, 3, new QStandardItem(pField->tableName));
        m_pModel->setItem(i, 4, new QStandardItem(pField->description));

        QVariant variant;//! TODO:提到外面
        variant.setValue(pField);
        index = m_pModel->index(i, 0);
        m_pModel->setData(index, variant, Qt::UserRole);
    }

    delete pView;
    return true;
}

void ViewFieldsDialog::setReturnFieldType(DBQUI::VIEW_FIELD_RETURN_TYPE type)
{
    m_returnType = type;
}

void ViewFieldsDialog::on_okButton_clicked()
{
    foreach(DBQ::ViewField * pField, m_FieldsSelected)
        delete pField;
    m_FieldsSelected.clear();

    QItemSelectionModel *selections = ui->fieldTableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QVariant variant;
    QModelIndex indexSelected;
    foreach (QModelIndex index, selected)
    {
        //qDebug()<<index;
        //设置了按行选中的模式 所以会返回所有选中行的cell
        if(index.column() != 0)
            continue;

        indexSelected = m_pModel->index(index.row(), 0);
        variant = m_pModel->data(indexSelected, Qt::UserRole);
        DBQ::ViewField * pField = variant.value<DBQ::ViewField *>();
        assert(pField);
        DBQ::ViewField * pFieldCp = new DBQ::ViewField;
        *pFieldCp = *pField;
        m_FieldsSelected.push_back(pFieldCp);
    }
    this->accept();
}

void ViewFieldsDialog::on_cancelButton_clicked()
{
  this->reject();
}

void ViewFieldsDialog::GetFields(QList<DBQ::ViewField *> & fieldsOut)
{
    foreach (DBQ::ViewField * pField, m_FieldsSelected) {
        DBQ::ViewField * pFieldCp = new DBQ::ViewField;
        *pFieldCp = * pField;
        fieldsOut.append(pFieldCp);
    }
}

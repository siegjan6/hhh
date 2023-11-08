#include "SourceTreeDialog.h"
#include "ui_SourceTreeDialog.h"
#include <IMDDataArchives.h>
#include <IMDDataGroupManager.h>
#include <MDDataArchivesFactory.h>
#include <IMDDataSourceManager.h>
#include <assert.h>
#include <QDebug>
#include "IMDDatabaseParam.h"

SourceTreeDialog::SourceTreeDialog(QString title, QString path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SourceTreeDialog),
    m_dasourceTreePath(path),
    m_ReturnType((int)RETRUNMODE::TYPE_VIEWFIELD),
    m_pRoot(NULL),
    m_DataArchives(NULL),
    m_DBSourceSelected(new DBQ::DBSource),
    m_FieldSelected(new DBQ::ViewField),
    m_windowTitle(title)
{
    ui->setupUi(this);
    this->setWindowTitle(m_windowTitle);
    m_Fields.clear();
    m_FieldsSelected.clear();
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QWidget::setTabOrder(ui->treeWidget,ui->btnConfirm);
    QWidget::setTabOrder(ui->btnConfirm, ui->btnCancel);
    QWidget::setTabOrder(ui->btnCancel, ui->treeWidget);
}

SourceTreeDialog::~SourceTreeDialog()
{
    delete ui;

    if(m_pRoot)
    {
        delete m_pRoot;
        m_pRoot = NULL;
    }
    if(m_DataArchives)
    {
        delete m_DataArchives;
        m_DataArchives = NULL;
    }
    if(m_DBSourceSelected)
    {
        delete m_DBSourceSelected;
        m_DBSourceSelected = NULL;
    }
    if(m_FieldSelected)
    {
        delete m_FieldSelected;
        m_FieldSelected = NULL;
    }
}

DBQ::DBSource *SourceTreeDialog::getDBSouce()
{
    return m_DBSourceSelected;
}

DBQ::ViewField *SourceTreeDialog::getViewField()
{
    return m_FieldSelected;
}

void SourceTreeDialog::setReturnMode(int mode)
{
    m_ReturnType = mode;
}

int SourceTreeDialog::getReturnMode()
{
    return m_ReturnType;
}

QList<DBQ::DBSource *> SourceTreeDialog::getDBSourcesSelected() const
{
    return m_DBSourcesSelected;
}

QList<DBQ::ViewField *> SourceTreeDialog::getFieldsSelected() const
{
    return m_FieldsSelected;
}

void SourceTreeDialog::setStackWidgetShowMode(int mode)
{
    ui->stackedWidget->setCurrentIndex(mode);
    if(0 == mode)
    {
        IMDDataSource *pDatasrc = NULL;
        QListWidgetItem *pSrcItem = NULL;
        QVariant var;

        if(!m_DataArchives)
        {
            //! 加载数据源文件 生成数据源管理对象
            m_DataArchives = CMDDataArchivesFactory::createDataArchives();
            m_DataArchives->dataSourceManager()->load(m_dasourceTreePath);
        }

        for (int i = 0; i<m_DataArchives->dataSourceManager()->dataSourceCount(); ++i)
        {
            pDatasrc = m_DataArchives->dataSourceManager()->getDataSource(i);
            if (NULL == pDatasrc)
            {
                continue;
            }
            pSrcItem = new QListWidgetItem();
            pSrcItem->setText(pDatasrc->name());
            var.setValue(pDatasrc);
            pSrcItem->setData(Qt::UserRole, var);
            ui->listWidget->addItem(pSrcItem);
        }
    }
}

void SourceTreeDialog::setDbsourceNameList(const QStringList &dbsourceNameList)
{
    m_dbsourceNameList = dbsourceNameList;
}

void SourceTreeDialog::initDataSourceTree()
{
    IMDDataSource *pDatasrc = NULL;
    IMDDataTable *pTable = NULL;
    IMDDataField *pFiled = NULL;
    QTreeWidgetItem *pSrcItem = NULL;
    QTreeWidgetItem *pTabItem = NULL;
    QTreeWidgetItem *pFiledItem = NULL;
    QVariant var;

    if(!m_DataArchives)
    {
        //! 加载数据源文件 生成数据源管理对象
        m_DataArchives = CMDDataArchivesFactory::createDataArchives();
        m_DataArchives->dataSourceManager()->load(m_dasourceTreePath);
    }

    ui->treeWidget->setHeaderHidden(true);
    m_pRoot = new QTreeWidgetItem(ui->treeWidget,QStringList("数据源"));
    m_pRoot->setIcon(0,QIcon(":/res/NetSCADADS.png"));
    ui->treeWidget->addTopLevelItem(m_pRoot);

    for(int i = 0; i < m_dbsourceNameList.size(); i++)
    {
        QString dbsourceName = m_dbsourceNameList.at(i);
        pDatasrc = m_DataArchives->dataSourceManager()->getDataSource(dbsourceName);
        if (NULL == pDatasrc)
        {
            continue;
        }
        pSrcItem = new QTreeWidgetItem();
        pSrcItem->setText(0, pDatasrc->name());
        pSrcItem->setIcon(0,QIcon(":/res/datasource.png"));
        var.setValue(pDatasrc);
        pSrcItem->setData(0, Qt::UserRole, var);
        m_pRoot->addChild(pSrcItem);

        for (int j = 0; j < pDatasrc->dataTableCount(); ++j)
        {
            pTable = pDatasrc->getDataTable(j);
            if (NULL == pTable)
            {
                continue;
            }

            pTabItem = new QTreeWidgetItem(pSrcItem);
            pTabItem->setText(0, pTable->name());

            var.setValue(pTable);
            pTabItem->setData(0, Qt::UserRole, var);
            pTabItem->setIcon(0, QIcon(":/res/table.png"));
            pSrcItem->addChild(pTabItem);

            for (int k = 0; k < pTable->dataFieldCount(); ++k)
            {
                pFiled = pTable->getDataField(k);
                if (NULL == pFiled)
                {
                    continue;
                }

                pFiledItem = new QTreeWidgetItem(pTabItem);
                pFiledItem->setText(0, pFiled->name());

                var.setValue(pFiled);
                pFiledItem->setData(0, Qt::UserRole, var);
                pFiledItem->setIcon(0, QIcon(":/res/details.png"));
                pTabItem->addChild(pFiledItem);
            }
        }
        ui->treeWidget->expandItem(m_pRoot);
        ui->treeWidget->expandItem(pSrcItem);
    }
    ui->treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

DBQ::VIEW_FIELD_TYPE SourceTreeDialog::getViewFieldValueTypeString(MDDataType fieldType)
{
    switch (fieldType)
    {
    case MDDataType::Bool : return DBQ::VIEW_FIELD_TYPE_BOOL;
    case MDDataType::SByte : return DBQ::VIEW_FIELD_TYPE_SBYTE;
    case MDDataType::Byte : return DBQ::VIEW_FIELD_TYPE_BYTE;
    case MDDataType::Int16 : return DBQ::VIEW_FIELD_TYPE_INT16;
    case MDDataType::UInt16 : return DBQ::VIEW_FIELD_TYPE_UINT16;
    case MDDataType::Int32 : return DBQ::VIEW_FIELD_TYPE_INT32;
    case MDDataType::UInt32 : return DBQ::VIEW_FIELD_TYPE_UINT32;
    case MDDataType::Int64 : return DBQ::VIEW_FIELD_TYPE_INT64;
    case MDDataType::UInt64 : return DBQ::VIEW_FIELD_TYPE_UINT64;
    case MDDataType::Float : return DBQ::VIEW_FIELD_TYPE_FLOAT;
    case MDDataType::Double : return DBQ::VIEW_FIELD_TYPE_DOUBLE;
    case MDDataType::DateTime : return DBQ::VIEW_FIELD_TYPE_DATETIME;
    case MDDataType::String : return DBQ::VIEW_FIELD_TYPE_STRING;
    default:return DBQ::VIEW_FIELD_TYPE_UNKNOWN;
    }
}

void SourceTreeDialog::addDBSource()
{
    QListWidgetItem *item = NULL;
    IMDDataSource *pDatasrc = NULL;
    IMDDataBaseParam *pDbParam =NULL;
    item = ui->listWidget->currentItem();
    if(item)
    {
        pDatasrc = item->data(Qt::UserRole).value<IMDDataSource*>();
        if(pDatasrc)
        {
            pDatasrc = m_DataArchives->dataSourceManager()->getDataSource(item->text());
            if(pDatasrc)
            {
                pDbParam = pDatasrc->dbParam();
                if(pDbParam)
                {
                    m_DBSourceSelected->dbSrcName = item->text();
                    m_DBSourceSelected->host = pDbParam->dbServerIP();
                    m_DBSourceSelected->port = pDbParam->dbServerPort();
                    m_DBSourceSelected->userName = pDbParam->dbUser();
                    m_DBSourceSelected->bDefault = false;
                    if(!pDbParam->dbPassword().isEmpty())
                    {
                        m_DBSourceSelected->password = pDbParam->dbPassword();
                    }
                    switch(int(pDbParam->dbServeType()))
                    {
                    case 2:
                        m_DBSourceSelected->dbType = DBSOURCE_TYPE_MYSQL;
                        break;
                    case 1:
                        m_DBSourceSelected->dbType =  DBSOURCE_TYPE_SQL_SERVER;
                        m_DBSourceSelected->tablePrefix = QString(tr("dbo"));
                        break;
                    case 5:
                        m_DBSourceSelected->dbType =  DBSOURCE_TYPE_ORACLE;
                        break;
                    case 6:
                        m_DBSourceSelected->dbType =  DBSOURCE_TYPE_KINGBASE;
                        m_DBSourceSelected->tablePrefix = QString(tr("public"));
                        break;
                    default:
                        m_DBSourceSelected->dbType = DBSOURCE_TYPE_UNKNOWN;
                        break;
                    }
                    DBQ::DB db;
                    db.dbName = pDbParam->dbName();
                    db.bDefault = false;
                    m_DBSourceSelected->dbs.append(db);
                }
            }
        }
    }
}

void SourceTreeDialog::addDBSourcesToList(QList<QListWidgetItem *> selectedItemList, int size)
{
    IMDDataSource *pDatasrc = NULL;
    IMDDataBaseParam *pDbParam = NULL;
    m_DBSourcesSelected.clear();
    for(int i = 0; i < size; i++)
    {
        if(selectedItemList.at(i)->data(Qt::UserRole).value<IMDDataSource*>())
        {
            DBQ::DBSource *dbsource= new DBQ::DBSource;
            pDatasrc = m_DataArchives->dataSourceManager()->getDataSource(selectedItemList.at(i)->text());
            if(pDatasrc)
            {
                pDbParam = pDatasrc->dbParam();
                if(pDbParam)
                {
                    dbsource->dbSrcName = selectedItemList.at(i)->text();
                    dbsource->tablePrefix = pDatasrc->dataTablePrefix();
                    dbsource->host = pDbParam->dbServerIP();
                    dbsource->port = pDbParam->dbServerPort();
                    dbsource->userName = pDbParam->dbUser();
                    dbsource->bDefault = false;
                    if(!pDbParam->dbPassword().isEmpty())
                    {
                        dbsource->password = pDbParam->dbPassword();
                    }
                    switch(int(pDbParam->dbServeType()))
                    {
                    case 2:
                        dbsource->dbType = DBSOURCE_TYPE_MYSQL;
                        break;
                    case 1:
                        dbsource->dbType =  DBSOURCE_TYPE_SQL_SERVER;
                        break;
                    case 5:
                        dbsource->dbType =  DBSOURCE_TYPE_ORACLE;
                        break;
                    case 6:
                        dbsource->dbType =  DBSOURCE_TYPE_KINGBASE;
                        break;
                    default:
                        dbsource->dbType = DBSOURCE_TYPE_UNKNOWN;
                        break;
                    }
                    DBQ::DB db;
                    db.dbName = pDbParam->dbName();
                    dbsource->dbs.append(db);
                    m_DBSourcesSelected.push_back(dbsource);
                }
            }
        }
    }
}

void SourceTreeDialog::addDBViewField(QTreeWidgetItem *item)
{
    IMDDataSource *pDatasrc = NULL;
    IMDDataBaseParam *pDbParam =NULL;
    IMDDataTable *pDataTable = NULL;
    IMDDataField *pDataField = NULL;

    pDatasrc = m_DataArchives->dataSourceManager()->getDataSource(item->parent()->parent()->text(0));
    if(pDatasrc)
    {
        pDbParam = pDatasrc->dbParam();
        if(pDbParam)
        {
            pDataTable = pDatasrc->getDataTable(item->parent()->text(0));
            if(pDataTable)
            {
                pDataField = pDataTable->getDataField(item->text(0));

                m_FieldSelected->fieldName = pDataField->name();
                m_FieldSelected->tableName = item->parent()->text(0);
                m_FieldSelected->type = getViewFieldValueTypeString(pDataField->dataType());
                m_FieldSelected->alias = pDataField->name();
                m_FieldSelected->description = pDataField->description();
            }
        }
    }
}

void SourceTreeDialog::addTableFiledsToDBViewFieldsList(QTreeWidgetItem *item)
{
    IMDDataSource *pDatasrc = NULL;
    IMDDataBaseParam *pDbParam =NULL;
    IMDDataTable *pDataTable = NULL;
    pDatasrc = m_DataArchives->dataSourceManager()->getDataSource(item->parent()->text(0));
    if(pDatasrc)
    {
        pDbParam = pDatasrc->dbParam();
        if(pDbParam)
        {

            m_Fields.clear();
            pDataTable = item->data(0, Qt::UserRole).value<IMDDataTable*>();
            if(pDataTable)
            {
                for(int i = 0 ; i < pDataTable->dataFieldCount(); i++)
                {
                    m_Fields.append(pDataTable->getDataField(i));
                }
            }
            m_FieldsSelected.clear();
            for(int i = 0 ; i < m_Fields.count(); i++)
            {
                DBQ::ViewField * pField = new DBQ::ViewField();

                pField->fieldName = m_Fields.at(i)->name();
                pField->tableName = item->text(0);
                pField->type = getViewFieldValueTypeString(m_Fields.at(i)->dataType());
                pField->alias = m_Fields.at(i)->name();
                pField->description = m_Fields.at(i)->description();

                assert(pField);
                m_FieldsSelected.push_back(pField);
            }
        }
    }
}

void SourceTreeDialog::addSelectedFiledsToDBViewFieldsList(QList<QTreeWidgetItem*> selectedItemList, int size)
{
    IMDDataSource *pDatasrc = NULL;
    IMDDataBaseParam *pDbParam =NULL;
    IMDDataTable *pDataTable = NULL;
    IMDDataField *pDataField = NULL;
    m_FieldsSelected.clear();
    for(int i = 0; i < size; i++)
    {
        if(selectedItemList.at(i)->data(0, Qt::UserRole).value<IMDDataField*>())
        {
            DBQ::ViewField *field = new DBQ::ViewField;
            pDatasrc = m_DataArchives->dataSourceManager()->getDataSource(selectedItemList.at(i)->parent()->parent()->text(0));
            if(pDatasrc)
            {
                pDbParam = pDatasrc->dbParam();
                if(pDbParam)
                {
                    pDataTable = pDatasrc->getDataTable(selectedItemList.at(i)->parent()->text(0));
                    if(pDataTable)
                    {
                        pDataField = pDataTable->getDataField(selectedItemList.at(i)->text(0));

                        field->fieldName = pDataField->name();
                        field->tableName = selectedItemList.at(i)->parent()->text(0);
                        field->type = getViewFieldValueTypeString(pDataField->dataType());
                        field->alias = pDataField->name();
                        field->description = pDataField->description();
                        m_FieldsSelected.push_back(field);
                    }
                }
            }
        }
    }
}

void SourceTreeDialog::on_btnConfirm_clicked()
{
    QList<QListWidgetItem*> selectedDBSourceItemList = ui->listWidget->selectedItems();
    int DBSourceCount = selectedDBSourceItemList.size();

    QList<QTreeWidgetItem*> selectedFieldItemList = ui->treeWidget->selectedItems();
    int FieldsCount = selectedFieldItemList.size();

    switch (getReturnMode())
    {
    case (int)RETRUNMODE::TYPE_DBSOURCE:
        if(DBSourceCount == 1)
        {
            addDBSource();
        }
        else if(DBSourceCount > 1)
        {
            addDBSourcesToList(selectedDBSourceItemList,DBSourceCount);
        }
        break;
    case (int)RETRUNMODE::TYPE_VIEWFIELD:
        if(FieldsCount == 1)
        {
            QTreeWidgetItem *item = NULL;
            item = ui->treeWidget->currentItem();
            if(item)
            {
                if(item->data(0, Qt::UserRole).value<IMDDataTable*>())
                {
                    addTableFiledsToDBViewFieldsList(item);
                }
                else if(item->data(0, Qt::UserRole).value<IMDDataField*>())
                {
                    addDBViewField(item);
                }
                else
                {
                    this->accept();
                }
            }
        }
        else if(FieldsCount > 1)
        {
            addSelectedFiledsToDBViewFieldsList(selectedFieldItemList,FieldsCount);
        }
        break;
    default:
        break;
    }

    this->accept();
}

void SourceTreeDialog::on_btnCancel_clicked()
{
    this->reject();
}

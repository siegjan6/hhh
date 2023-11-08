#include "ViewFieldTableDialog.h"
#include "ui_ViewFieldTableDialog.h"
#include "NewFieldDialog.h"
#include "SourceTreeDialog.h"
#include <QMessageBox>

ViewFieldTableDialog::ViewFieldTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFieldTableDialog),
    m_pViewField_PopupMenu(NULL),
    m_pViewField_AddDataArchivesFieldAction(NULL),
    m_pViewField_NewAction(NULL),
    m_pViewField_DeleteAction(NULL),
    m_pViewField_ModifyAction(NULL),
    m_DataArchivesFilePath("")
{
    ui->setupUi(this);
    initTableWidget();
    QWidget::setTabOrder(ui->tableWidget, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->tableWidget);
}

ViewFieldTableDialog::~ViewFieldTableDialog()
{
    delete ui;

    if(m_pViewField_PopupMenu)
    {
        delete m_pViewField_PopupMenu;
        m_pViewField_PopupMenu = NULL;
    }
    if(m_pViewField_AddDataArchivesFieldAction)
    {
        delete m_pViewField_AddDataArchivesFieldAction;
        m_pViewField_AddDataArchivesFieldAction = NULL;
    }
    if(m_pViewField_NewAction)
    {
        delete m_pViewField_NewAction;
        m_pViewField_NewAction = NULL;
    }
    if(m_pViewField_DeleteAction)
    {
        delete m_pViewField_DeleteAction;
        m_pViewField_DeleteAction = NULL;
    }
    if(m_pViewField_ModifyAction)
    {
        delete m_pViewField_ModifyAction;
        m_pViewField_ModifyAction = NULL;
    }
}

void ViewFieldTableDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}

void ViewFieldTableDialog::on_pbt_Confirm_clicked()
{
    this->accept();
}

QString ViewFieldTableDialog::DataArchivesFilePath() const
{
    return m_DataArchivesFilePath;
}

void ViewFieldTableDialog::setDataArchivesFilePath(const QString &DataArchivesFilePath)
{
    m_DataArchivesFilePath = DataArchivesFilePath;
}

QList<DBQ::ViewField *> ViewFieldTableDialog::getViewFieldList() const
{
    return m_viewFieldList;
}

void ViewFieldTableDialog::setViewFieldList(const QList<DBQ::ViewField *> &viewFieldList)
{
    m_viewFieldList = viewFieldList;
}

void ViewFieldTableDialog::initTableWidget()
{
    disconnect(ui->tableWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_doubleClickedModifyViewField(QModelIndex)));
    QStringList header;
    header<<"字段名"<<"表名"<<"字段类型"<<"字段别名"<<"描述";
    ui->tableWidget->setWindowTitle("枚举值列表");
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(m_viewFieldList.count());
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(30);
    ui->tableWidget->verticalHeader()->hide();
    int size = m_viewFieldList.size();
    for(int i = 0 ; i < size; i++)
    {
        QTableWidgetItem *itemName = new QTableWidgetItem(m_viewFieldList.at(i)->fieldName);
        itemName->setTextAlignment( Qt::AlignCenter);
        QTableWidgetItem *itemTableName = new QTableWidgetItem(m_viewFieldList.at(i)->tableName);
        itemTableName->setTextAlignment( Qt::AlignCenter);
        QTableWidgetItem *itemType = new QTableWidgetItem(getViewFieldValueTypeString(m_viewFieldList.at(i)->type));
        itemType->setTextAlignment( Qt::AlignCenter);
        QTableWidgetItem *itemAlias = new QTableWidgetItem(m_viewFieldList.at(i)->alias);
        itemAlias->setTextAlignment( Qt::AlignCenter);
        QTableWidgetItem *itemDescription = new QTableWidgetItem(m_viewFieldList.at(i)->description);
        itemDescription->setTextAlignment( Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,itemName);
        ui->tableWidget->setItem(i,1,itemTableName);
        ui->tableWidget->setItem(i,2,itemType);
        ui->tableWidget->setItem(i,3,itemAlias);
        ui->tableWidget->setItem(i,4,itemDescription);
    }
    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sortByColumn(int)));
    connect(ui->tableWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_doubleClickedModifyViewField(QModelIndex)));
}

void ViewFieldTableDialog::contextMenuEvent(QContextMenuEvent *event)
{
    if(!m_pViewField_PopupMenu)
    {
        m_pViewField_PopupMenu = new QMenu(tr("新建"),ui->tableWidget);

        if(!m_pViewField_NewAction)
        {
            m_pViewField_AddDataArchivesFieldAction = new QAction(tr("从数据归档添加字段"),ui->tableWidget);
            connect(m_pViewField_AddDataArchivesFieldAction,SIGNAL(triggered()),this,SLOT(slot_AddViewFileld()));
            m_pViewField_PopupMenu->addAction(m_pViewField_AddDataArchivesFieldAction);
        }
        if(!m_pViewField_NewAction)
        {
            m_pViewField_NewAction = new QAction(tr("添加字段"),ui->tableWidget);
            connect(m_pViewField_NewAction,SIGNAL(triggered()),this,SLOT(slot_NewViewFileld()));
            m_pViewField_PopupMenu->addAction(m_pViewField_NewAction);
        }
        if(!m_pViewField_DeleteAction)
        {
            m_pViewField_DeleteAction = new QAction(tr("删除字段"),ui->tableWidget);
            connect(m_pViewField_DeleteAction,SIGNAL(triggered()),this,SLOT(slot_DeleteViewFileld()));
            m_pViewField_PopupMenu->addAction(m_pViewField_DeleteAction);
        }
        if(!m_pViewField_ModifyAction)
        {
            m_pViewField_ModifyAction = new QAction(tr("修改字段"),ui->tableWidget);
            connect(m_pViewField_ModifyAction,SIGNAL(triggered()),this,SLOT(slot_ModifyViewFileld()));
            m_pViewField_PopupMenu->addAction(m_pViewField_ModifyAction);
        }
    }
    if(m_pViewField_PopupMenu)
    {
        m_pViewField_PopupMenu->exec(QCursor::pos());
        event->accept();
    }
}

void ViewFieldTableDialog::slot_doubleClickedModifyViewField(QModelIndex index)
{
    Q_UNUSED(index);
    slot_ModifyViewFileld();
}

void ViewFieldTableDialog::slot_AddViewFileld()
{
    QString dataSrcFullPath = m_DataArchivesFilePath.left(m_DataArchivesFilePath.length()-7) + "DataArchives/" + QString("dataSource.dat");
    QStringList dbsourceNameList;
    if(DBQ::DBSource_GetNames(m_DataArchivesFilePath,dbsourceNameList))
    {
        SourceTreeDialog dlg(QString(tr("视图数据源字段")),dataSrcFullPath);
        dlg.setDbsourceNameList(dbsourceNameList);
        dlg.initDataSourceTree();
        dlg.setReturnMode(1);
        dlg.setStackWidgetShowMode(1);
        if(dlg.exec() == QDialog::Accepted)
        {
            DBQ::ViewField* viewFiled = new DBQ::ViewField;
            QList<DBQ::ViewField *> m_addFieldsList;
            if(!dlg.getFieldsSelected().isEmpty())
            {
                foreach (DBQ::ViewField * pField, dlg.getFieldsSelected())
                {
                    DBQ::ViewField * pFieldCp = new DBQ::ViewField;
                    *pFieldCp = *pField;
                    m_addFieldsList.append(pFieldCp);
                }
                int size = m_addFieldsList.size();
                for(int i = 0; i < size; i++)
                {
                    QString fieldName = m_addFieldsList.at(i)->fieldName;
                    if(fieldName.isEmpty())
                    {
                        QMessageBox::information(NULL,"警告",tr("字段名称为空！"));
                        return;
                    }
                    {
                        if(!checkViewFieldAliasFromFieldList(m_addFieldsList.at(i)->alias))
                        {
                            if(!checkViewFieldStringFromFieldList(m_addFieldsList.at(i)))
                            {
                                m_viewFieldList.push_back(m_addFieldsList.at(i));
                            }
                            else
                            {
                                QString strMsg = QString("字段%1已存在，是否替换已有字段？").arg(fieldName);
                                QMessageBox message(QMessageBox::Information,"提示", strMsg, QMessageBox::Yes | QMessageBox::No, NULL);
                                if (QMessageBox::Yes == message.exec())
                                {
                                    if(getViewField(m_addFieldsList.at(i)->fieldName,m_addFieldsList.at(i)->tableName))
                                    {
                                        m_viewFieldList.removeOne(getViewField(m_addFieldsList.at(i)->fieldName,m_addFieldsList.at(i)->tableName));
                                        m_viewFieldList.push_back(m_addFieldsList.at(i));
                                    }
                                }
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
                initTableWidget();
            }
            else
            {
                if(dlg.getViewField())
                {
                    *viewFiled = *dlg.getViewField();
                    QString fieldName = viewFiled->fieldName;
                    QString tableName = viewFiled->tableName;
                    if(fieldName.isEmpty())
                    {
                        QMessageBox::information(NULL,"警告",tr("字段名称为空！"));
                        return;
                    }
                    {
                        if(m_viewFieldList.size() == 0)
                        {
                            m_viewFieldList.push_back(viewFiled);
                            initTableWidget();
                        }
                        else if(m_viewFieldList.size() > 0)
                        {
                            if(checkViewFieldAliasFromFieldList(viewFiled->alias))
                            {
                                QMessageBox::information(NULL,"警告",tr("当前字段别名已存在！"));
                                return;
                            }

                            if(getViewField(fieldName,tableName) == NULL)
                            {
                                m_viewFieldList.push_back(viewFiled);
                                initTableWidget();
                            }
                            else
                            {
                                QMessageBox::information(NULL,"警告",tr("当前字段已存在！"));
                                return;
                            }


                        }
                        else
                        {
                            return;
                        }
                    }
                }
            }

        }
    }
}

void ViewFieldTableDialog::slot_NewViewFileld()
{
    NewFieldDialog dlg(QString(tr("新建字段")));
    if(dlg.exec() == QDialog::Accepted)
    {
        if(dlg.getField())
        {
            DBQ::ViewField* viewFiled = new DBQ::ViewField;
            *viewFiled = *dlg.getField();
            QString fieldName = viewFiled->fieldName;
            QString tableName = viewFiled->tableName;
            if(fieldName.isEmpty())
            {
                QMessageBox::information(NULL,"警告",tr("字段名称为空！"));
                return;
            }
            if(checkViewFieldAliasFromFieldList(viewFiled->alias))
            {
                QMessageBox::information(NULL,"警告",tr("当前字段别名已存在！"));
                return;
            }
            if(getViewField(fieldName,tableName) == NULL)
            {
                m_viewFieldList.push_back(viewFiled);
                initTableWidget();
            }
            else
            {
                QMessageBox::information(NULL,"警告",tr("当前字段已存在！"));
                return;
            }
        }
    }
}

void ViewFieldTableDialog::slot_DeleteViewFileld()
{
    QModelIndexList indexList1 = ui->tableWidget->selectionModel()->selectedIndexes();
    if(indexList1.isEmpty())
        return;
    QString strMsg = QString("确定删除当前选中的字段吗？");
    QMessageBox message(QMessageBox::Information,"提示", strMsg, QMessageBox::Yes | QMessageBox::No, NULL);
    if (QMessageBox::Yes == message.exec())
    {

        while (!indexList1.isEmpty())
        {
            QModelIndexList indexList = deleteRepeatList(indexList1);
            QModelIndex index = indexList.first();
            ui->tableWidget->removeRow(index.row());
            m_viewFieldList.takeAt(index.row());

            indexList1 = ui->tableWidget->selectionModel()->selectedIndexes();
        }
        initTableWidget();
    }
}

void ViewFieldTableDialog::slot_ModifyViewFileld()
{
    QString fieldName = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text();
    QString tableName = ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text();
    QString fieldAlias = ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text();
    NewFieldDialog dlg(QString(tr("修改字段")));
    if(getViewField(fieldName,tableName) == NULL)
    {
        return;
    }
    dlg.setOriginalViewFieldParameter(getViewField(fieldName,tableName));
    int ret = dlg.exec();
    if(ret == QDialog::Accepted)
    {
        if(getViewField(fieldName,tableName) != NULL)
        {
            if(dlg.getField())
            {
                DBQ::ViewField* viewFiled = new DBQ::ViewField;
                *viewFiled = *dlg.getField();

                QString fieldName = viewFiled->fieldName;
                QString newViewFieldAlias = viewFiled->alias;

                if(fieldName.isEmpty())
                {
                    QMessageBox::information(NULL,"警告",tr("字段名称为空！"));
                    return;
                }
                if((fieldAlias !=newViewFieldAlias) && checkViewFieldAliasFromFieldList(viewFiled->alias))
                {
                    QMessageBox::information(NULL,"警告",tr("当前字段别名已存在！"));
                    return;
                }

                int rowIndex = ui->tableWidget->currentRow();
                if (rowIndex != -1)
                {
                    ui->tableWidget->removeRow(rowIndex);
                    m_viewFieldList.takeAt(rowIndex);
                }

                if(getViewField(fieldName,tableName) == NULL)
                {
                    m_viewFieldList.append(viewFiled);
                    initTableWidget();
                }
                else
                {
                    QMessageBox::information(NULL,"警告",tr("当前字段已存在！"));
                    return;
                }
            }
        }
    }
}

QModelIndexList ViewFieldTableDialog::deleteRepeatList(QModelIndexList indexList)
{
    QModelIndex index,newIndex;
    QModelIndexList newIndexList;
    foreach(index,indexList)
    {
        if (newIndex.row() != index.row())
        {
            newIndex = index;
            newIndexList.append(newIndex);
        }
    }
    return newIndexList;
}

bool ViewFieldTableDialog::checkViewFieldStringFromFieldList(DBQ::ViewField *viewField)
{
    bool ok = false;
    QString viewFieldString;
    QString addViewFieldString = viewField->fieldName + "." +  viewField->tableName;
    for(int i = 0; i < m_viewFieldList.size(); i++)
    {
        viewFieldString = m_viewFieldList.at(i)->fieldName + "." +  m_viewFieldList.at(i)->tableName;
        if(viewFieldString == addViewFieldString)
        {
            ok = true;
            return ok;
        }
    }
    return ok;
}

bool ViewFieldTableDialog::checkViewFieldAliasFromFieldList(const QString &alias)
{
    bool ok = false;
    for(int i = 0; i < m_viewFieldList.size(); i++)
    {
        if(alias == m_viewFieldList.at(i)->alias)
        {
            ok = true;
            return ok;
        }
    }
    return ok;
}

void ViewFieldTableDialog::slot_sortByColumn(int column)
{
    if (column >= ui->tableWidget->columnCount() || column < 0)
            return;
    ui->tableWidget->sortByColumn(column,Qt::AscendingOrder);
}

DBQ::ViewField* ViewFieldTableDialog::getViewField(const QString &name,const QString &tableName)
{
    DBQ::ViewField* viewFiled = NULL;

    for (int i = 0; i < m_viewFieldList.size(); ++i)
    {
        viewFiled = m_viewFieldList.at(i);
        if (viewFiled && (viewFiled->fieldName == name) && (viewFiled->tableName == tableName))
        {
            return viewFiled;
        }
    }
    return NULL;
}

QString ViewFieldTableDialog::getViewFieldValueTypeString(DBQ::VIEW_FIELD_TYPE valueType)
{
    return DBQ::View_GetFieldTypeStringFromType(valueType);
}


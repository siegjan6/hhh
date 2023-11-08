#include "dbdatagrouptreecreator.h"
#include <IMDDataGroup.h>
#include <IMDDataGroupManager.h>
#include "mainwidget.h"

Q_DECLARE_METATYPE(IMDDataGroup *)

DBDataGroupTreeCreator::DBDataGroupTreeCreator(IMDDataArchives * pDataArchives, QTreeWidget * pTreeWidget)
    :m_pDataArchives(pDataArchives),
     m_pTreeWidget(pTreeWidget),
     m_pHistoryRootItem(0)
{
}

void DBDataGroupTreeCreator::build()
{
    QStringList dataSourceStrList;
    dataSourceStrList<<"数据组";
    QTreeWidgetItem * DBDataGroupRoot = new QTreeWidgetItem(m_pTreeWidget, dataSourceStrList,  MainTree_Node_Kind_Data_Group_Root);
    DBDataGroupRoot->setIcon(0, QIcon(":/img/images/folder.png"));
    LoadDBDataGroupTree(DBDataGroupRoot);
}

QTreeWidgetItem * DBDataGroupTreeCreator::getHistoryRootItem()
{
    return m_pHistoryRootItem;
}

void DBDataGroupTreeCreator::LoadDBDataGroupTree(QTreeWidgetItem *pRoot)
{
    buildAlmTree(pRoot);
    buildLogTree(pRoot);
    buildHisTree(pRoot);
}

void DBDataGroupTreeCreator::buildAlmTree(QTreeWidgetItem *pTreeRoot)
{
    QTreeWidgetItem * almFieldRoot = new QTreeWidgetItem(MainTree_Node_Kind_Data_Group_Alarm);
    almFieldRoot->setIcon(0, QIcon(":/img/images/file.png"));
    pTreeRoot->addChild(almFieldRoot);
    LoadAlmNotfy(almFieldRoot);
}

void DBDataGroupTreeCreator::LoadAlmOrLogNote(QTreeWidgetItem *pRoot, bool isAlm)
{
    if(NULL == pRoot)
    {
        //!TODO:log
        return;
    }

    IMDDataGroupManager *pManager = m_pDataArchives->dataGroupManager();
    if(NULL == pManager)
    {
        //!TODO:log
        return;
    }

    //!报警组或log组就是数据库表的模板 而非组的概念
    IMDDataGroup *pDataGroup = NULL;
    if (isAlm)
    {
        pDataGroup = pManager->getAlarmDataGroup();
    }
    else
    {
        pDataGroup = pManager->getLogDataGroup();
    }

    if (NULL == pDataGroup)
    {
        //!TODO:log
        return;
    }

    QVariant Var;
    Var.setValue (pDataGroup);
    pRoot->setData (0, Qt::UserRole, Var);
    pRoot->setText(0, pDataGroup->name());
}

void DBDataGroupTreeCreator::LoadAlmNotfy(QTreeWidgetItem *pRoot)
{
    LoadAlmOrLogNote(pRoot, true);
}

void DBDataGroupTreeCreator::LoadLogTable(QTreeWidgetItem *pRoot)
{
    LoadAlmOrLogNote(pRoot, false);
}

void DBDataGroupTreeCreator::buildLogTree(QTreeWidgetItem *pTreeRoot)
{
    QTreeWidgetItem * logFieldItem = new QTreeWidgetItem(MainTree_Node_Kind_Data_Group_Log);
    logFieldItem->setIcon(0, QIcon(":/img/images/file.png"));
    pTreeRoot->addChild(logFieldItem);
    LoadLogTable(logFieldItem);
}

void DBDataGroupTreeCreator::buildHisTree(QTreeWidgetItem *pTreeRoot)
{
    QTreeWidgetItem * hisGroupRoot = new QTreeWidgetItem(pTreeRoot, QStringList("历史组"), MainTree_Node_Kind_Data_Group_History_Root);
    hisGroupRoot->setIcon(0, QIcon(":/img/images/folder.png"));
    pTreeRoot->addChild(hisGroupRoot);
    LoadHisGroup(hisGroupRoot);
    m_pHistoryRootItem = hisGroupRoot;
}

void DBDataGroupTreeCreator::LoadHisGroup(QTreeWidgetItem *pRoot)
{
    if(NULL == pRoot)
    {
        //!TODO:log
        return;
    }

    IMDDataGroupManager *pManager = m_pDataArchives->dataGroupManager();
    if(NULL == pManager)
    {
        //!TODO:log
        return;
    }

    //!TODO:应该加锁
    QTreeWidgetItem *pItem = NULL;
    int iCount = pManager->hisDataGroupCount();
    for(int i = 0; i < iCount; i++)
    {
        IMDDataGroup *pHisGroup = pManager->getHisDataGroup(i);
        if(NULL == pHisGroup)
        {
            //!TODO:log
            continue;
        }

        pItem = new QTreeWidgetItem(QStringList(pHisGroup->name()), MainTree_Node_Kind_Data_Group_History_Table);
        pItem->setIcon(0,QIcon(":/img/images/file.png"));
        pRoot->addChild(pItem);

        QVariant Var;
        Var.setValue (pHisGroup);
        pItem->setData (0, Qt::UserRole, Var);
    }
}


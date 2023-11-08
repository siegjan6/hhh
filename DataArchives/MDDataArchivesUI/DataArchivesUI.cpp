#include "DataArchivesUI.h"
#include "DataGroupFramework.h"
#include "MDDataGroupTableWidget.h"
#include "IMDDataSourceManager.h"
#include "IMDDataGroupManager.h"
#include "IMDDataArchives.h"
#include "IMDDataItem.h"
#include "MDDataDefine.h"
#include "MDDataSourceFactory.h"
#include "MDDataGroupFactory.h"
#include "MDNewDataGroup.h"
#include "MDProjectConfiger.h"
#include "MDMultiLanguage.h"

#define DBSTUDIO_CHECK_NULL     if(NULL == m_studioInterface->treeWidget())\
                                {\
                                    return;\
                                }\
                                \
                                QTreeWidgetItem *pItem = m_studioInterface->treeWidget()->currentItem();\
                                if(NULL == pItem)\
                                {\
                                    return;\
                                }\
                                \
                                if(NULL == m_pDataArchives)\
                                {\
                                    return;\
                                }

CDataArchivesUI::CDataArchivesUI()
{
    initData();
    initLanguage();
}

CDataArchivesUI::~CDataArchivesUI()
{
    unLoad();

    if(m_pDBSource_NewAction)
    {
        delete m_pDBSource_NewAction;
        m_pDBSource_NewAction = NULL;
    }
    if(m_pDBSource_DeleteAction)
    {
        delete m_pDBSource_DeleteAction;
        m_pDBSource_DeleteAction = NULL;
    }
    if(m_pDBSource_PropertiesAction)
    {
        delete m_pDBSource_PropertiesAction;
        m_pDBSource_PropertiesAction = NULL;
    }

    if(m_pDBSourceTable_NewAction)
    {
        delete m_pDBSourceTable_NewAction;
        m_pDBSourceTable_NewAction = NULL;
    }

    if(m_pDBSourceTable_DeleteAction)
    {
        delete m_pDBSourceTable_DeleteAction;
        m_pDBSourceTable_DeleteAction = NULL;
    }
    if(m_pDBSourceTable_PropertiesAction)
    {
        delete m_pDBSourceTable_PropertiesAction;
        m_pDBSourceTable_PropertiesAction = NULL;
    }

    if(m_pDBSourcePopupMenu)
    {
        delete m_pDBSourcePopupMenu;
        m_pDBSourcePopupMenu = NULL;
    }

    if(m_pHisGroup_NewAction)
    {
        delete m_pHisGroup_NewAction;
        m_pHisGroup_NewAction = NULL;
    }

    if(m_pHisGroup_DeleteAction)
    {
        delete m_pHisGroup_DeleteAction;
        m_pHisGroup_DeleteAction = NULL;
    }

    if(m_pHisGroup_PropertiesAction)
    {
        delete m_pHisGroup_PropertiesAction;
        m_pHisGroup_PropertiesAction = NULL;
    }

    if(m_pHisGroupPopupMenu)
    {
        delete m_pHisGroupPopupMenu;
        m_pHisGroupPopupMenu = NULL;
    }

    if (m_pMulLnaguageTool)
    {
        delete m_pMulLnaguageTool;
        m_pMulLnaguageTool = NULL;
    }
}

void CDataArchivesUI::changeLanguage()
{
    initLanguage();

    if (m_isOpenDataSrcFramwork)
    {
//        m_pDBstudioFramework->changeLanguage();
    }

    if (m_isOpenDataGrpFramwork)
    {
//        m_pDataGrpFramework->changeLanguage();
    }

    refreshText();
}

void CDataArchivesUI::load()
{
    m_pDataArchives = m_studioInterface->projectConfiger()->dataArchives();

    connect(m_studioInterface->treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CDataArchivesUI::onMouseDbClick);
    connect(m_studioInterface->treeWidget(), &QTreeWidget::customContextMenuRequested, this, &CDataArchivesUI::onMouseRClick);

    MDLOG_INFO_TO_LOGGER(QString("UI---load dataArchives tree note and datagroup tree note!"));

    buildDBTree();
    buildDBDataGroupTree();  
}

void CDataArchivesUI::unLoad()
{
    MDLOG_INFO_TO_LOGGER(QString("UI---unload dataArchives tree note and datagroup tree note!"));
/* treeItem's parent will free child item.
    if(m_HisGroupRoot)
    {
        delete m_HisGroupRoot;
        m_HisGroupRoot = NULL;
    }

    if(m_AlmFieldRoot)
    {
        delete m_AlmFieldRoot;
        m_AlmFieldRoot = NULL;
    }

    if(m_LogFieldItem)
    {
        delete m_LogFieldItem;
        m_LogFieldItem = NULL;
    }

    if(m_DBDataGroupRoot)
    {
        delete m_DBDataGroupRoot;
        m_DBDataGroupRoot = NULL;
    }

    if(m_DBStudioRoot)
    {
        delete m_DBStudioRoot;
        m_DBStudioRoot = NULL;
    }
*/

    if(m_pDBstudioFramework)
    {
        m_pDBstudioFramework->close();
        delete m_pDBstudioFramework;
        m_pDBstudioFramework = NULL;
    }

    if(m_pDataGrpFramework)
    {
        m_pDataGrpFramework->close();
        delete m_pDataGrpFramework;
        m_pDataGrpFramework = NULL;
    }

    m_isOpenDataSrcFramwork         = false; //! 数据源MDI主窗体打开标识
    m_isOpenDataGrpFramwork         = false; //! 数据组MDI主窗体打开标识
}

void CDataArchivesUI::buildAlmTree(QTreeWidgetItem *pTreeRoot)
{
    m_AlmFieldRoot = new QTreeWidgetItem;
    m_AlmFieldRoot->setIcon(0, QIcon(":/images/file.png"));
    pTreeRoot->addChild(m_AlmFieldRoot);
    LoadAlmNotfy(m_AlmFieldRoot);
}

void CDataArchivesUI::buildHisTree(QTreeWidgetItem *pTreeRoot)
{
    m_HisGroupRoot = new QTreeWidgetItem(QStringList(CMDMultiLanguage::instance()->value(STRING_HISGRP)));;
    m_HisGroupRoot->setIcon(0, QIcon(":/images/folder.png"));
    pTreeRoot->addChild(m_HisGroupRoot);
    LoadHisGroup(m_HisGroupRoot);
}

void CDataArchivesUI::buildLogTree(QTreeWidgetItem *pTreeRoot)
{
    m_LogFieldItem = new QTreeWidgetItem;
    m_LogFieldItem->setIcon(0, QIcon(":/images/file.png"));
    pTreeRoot->addChild(m_LogFieldItem);
    LoadLogTable(m_LogFieldItem);
}

void CDataArchivesUI::buildDBTree()
{
    m_DBStudioRoot = new QTreeWidgetItem(QStringList(CMDMultiLanguage::instance()->value(STRING_DATASOURCE)));
    m_DBStudioRoot->setIcon(0, QIcon(":/images/folder.png"));
    m_rootInStudio->addChild(m_DBStudioRoot);
    LoadDBTree(m_DBStudioRoot);
}

void CDataArchivesUI::buildDBDataGroupTree()
{
    m_DBDataGroupRoot= new QTreeWidgetItem(QStringList(CMDMultiLanguage::instance()->value(STRING_DATAGROUP)));
    m_DBDataGroupRoot->setIcon(0, QIcon(":/images/folder.png"));
    m_rootInStudio->addChild(m_DBDataGroupRoot);
    LoadDBDataGroupTree(m_DBDataGroupRoot);
}

void CDataArchivesUI::LoadDBDataGroupTree(QTreeWidgetItem *pRoot)
{
    buildAlmTree(pRoot);
    buildLogTree(pRoot);
    buildHisTree(pRoot);
}

void CDataArchivesUI::LoadDBTree(QTreeWidgetItem *pRoot)
{
    if(NULL == pRoot)
    {
        return;
    }

    if(NULL == m_pDataArchives)
    {
        return;
    }

    IMDDataSourceManager *pDataSrcManager = m_pDataArchives->dataSourceManager();
    if(NULL == pDataSrcManager)
    {
        return;
    }

    QString strName = "";
    int count = pDataSrcManager->dataSourceCount();
    for(int i = 0; i < count; i++)
    {
        IMDDataSource *pDataSource = pDataSrcManager->getDataSource(i);
        if(NULL == pDataSource)
        {
            continue;
        }

        strName = pDataSource->name();
        QTreeWidgetItem *pItem = new QTreeWidgetItem(QStringList(strName));
        pItem->setIcon(0,QIcon(":/images/file.png"));
        pRoot->addChild(pItem);

        QVariant Var;
        Var.setValue (pDataSource);
        pItem->setData (0, Qt::UserRole, Var);

        if (!pDataSource->isShowTable())
        {
            continue;
        }

        int tabCount = pDataSource->dataTableCount();
        for(int j = 0; j < tabCount; j++)
        {
            IMDDataTable *pTable = pDataSource->getDataTable(j);
            if(NULL == pTable)
            {
                continue;
            }

            QTreeWidgetItem *pItemTable = new QTreeWidgetItem(QStringList(pTable->name()));
            pItemTable->setIcon(0,QIcon(":/images/file.png"));
            pItem->addChild(pItemTable);

            QVariant Var;
            Var.setValue (pTable);
            pItemTable->setData (0, Qt::UserRole, Var);

        }
    }
}

void CDataArchivesUI::LoadHisGroup(QTreeWidgetItem *pRoot)
{
    if(NULL == pRoot)
    {
        return;
    }

    IMDDataGroupManager *pManager = m_pDataArchives->dataGroupManager();
    if(NULL == pManager)
    {
        return;
    }

    QTreeWidgetItem *pItem = NULL;
    int iCount = pManager->hisDataGroupCount();
    for(int i = 0; i < iCount; i++)
    {
        IMDDataGroup *pHisGroup = pManager->getHisDataGroup(i);
        if(NULL == pHisGroup)
        {
            continue;
        }

        pItem = new QTreeWidgetItem(QStringList(pHisGroup->name()));
        pItem->setIcon(0,QIcon(":/images/file.png"));
        pRoot->addChild(pItem);

        QVariant Var;
        Var.setValue (pHisGroup);
        pItem->setData (0, Qt::UserRole, Var);
    }
}

void CDataArchivesUI::LoadAlmNotfy(QTreeWidgetItem *pRoot)
{
    LoadAlmOrLogNote(pRoot, true);
}

void CDataArchivesUI::LoadLogTable(QTreeWidgetItem *pRoot)
{
    LoadAlmOrLogNote(pRoot, false);
}

void CDataArchivesUI::LoadAlmOrLogNote(QTreeWidgetItem *pRoot, bool isAlm)
{
    if(NULL == pRoot)
    {
        return;
    }

    IMDDataGroupManager *pManager = m_pDataArchives->dataGroupManager();
    if(NULL == pManager)
    {
        return;
    }

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
        return;
    }

    QVariant Var;
    Var.setValue (pDataGroup);
    pRoot->setData (0, Qt::UserRole, Var);
    pRoot->setText(0, pDataGroup->name());
}

// hItem 節點是否为htrDestParent子节点?(包括自己本身)
bool CDataArchivesUI::isItemInChildren(QTreeWidgetItem *hItem, QTreeWidgetItem *htrDestParent)
{
    if(hItem && htrDestParent)
    {
        do
        {
            if (hItem == htrDestParent)
                break;
        }
        while ((hItem = hItem->parent()) != NULL);
        return (hItem != NULL);
    }
    return false;
}

void CDataArchivesUI::slotitemActiveNewHisGroup()
{
    if((NULL == m_pDataArchives) || (NULL == m_studioInterface))
    {
        return;
    }

    IMDDataGroupManager *pDataGrpMana = m_pDataArchives->dataGroupManager();
    if (NULL == pDataGrpMana)
    {
        return;
    }

    IMDDataGroup *pHisDataGrp = CMDDataGroupFactory::createDataGroup();
    CMDNewDataGroup dataGrpDlg;
    dataGrpDlg.initData(pHisDataGrp, pDataGrpMana);
    if (QDialog::Accepted != dataGrpDlg.exec())
    {
        delete pHisDataGrp;
        pHisDataGrp = NULL;
        return;
    }        

    //把默认记录时间字段增加到第一行的位置
    IMDDataItem *item = CMDDataGroupFactory::createDataItem();
    item->setDataType(MDDataType::DateTime);
    item->setName(LOG_RECORDTIME);
    item->setDescription("记录时间");
    pHisDataGrp->addItem(item);

    item = CMDDataGroupFactory::createDataItem();
    item->setDataType(MDDataType::Int32);
    item->setName(LOG_RECORDTIME_MS);
    item->setDescription("记录时间毫秒");
    pHisDataGrp->addItem(item);

    if(m_HisGroupRoot)
    {
        QTreeWidgetItem *pItem = new QTreeWidgetItem(QStringList(pHisDataGrp->name()));
        pItem->setIcon(0,QIcon(":/images/file.png"));
        m_HisGroupRoot->addChild(pItem);

        QVariant Var;
        Var.setValue (pHisDataGrp);
        pItem->setData (0, Qt::UserRole, Var);

        if (!pDataGrpMana->addHisDataGroup(pHisDataGrp))
        {
            delete pHisDataGrp;
            pHisDataGrp = NULL;
            return;
        }

        pDataGrpMana->save();

        m_studioInterface->treeWidget()->setCurrentItem(pItem);
        if (NULL == m_pDataGrpFramework)
        {
            m_pDataGrpFramework = new CDataGroupFramework();
            m_pDataGrpFramework->setDataArchivesUI(this);
        }

        m_pDataGrpFramework->setDataGrpProperty(m_studioInterface->rtdbManagerUI(), m_pDataArchives, pHisDataGrp);
        if (!m_isOpenDataGrpFramwork)
        {
            m_isOpenDataGrpFramwork = true;
            m_studioInterface->openMDIWindow(m_pDataGrpFramework);
        }

        m_studioInterface->setActiveForm(m_pDataGrpFramework);
    }    
}

void CDataArchivesUI::slotitemActiveDeleteHisGroup()
{
    DBSTUDIO_CHECK_NULL

    IMDDataGroupManager *pDataGrpMana = m_pDataArchives->dataGroupManager();
    if (NULL == pDataGrpMana)
    {
        return;
    }

    IMDDataGroup *pHistoryGroup = pItem->data(0,Qt::UserRole).value< IMDDataGroup* >();
    if(NULL == pHistoryGroup)
    {
        return;
    }

    QMessageBox message(m_studioInterface->treeWidget());
    message.setWindowTitle(CMDMultiLanguage::instance()->value(tr("提示")));
    message.addButton(CMDMultiLanguage::instance()->value(tr("确定")), QMessageBox::YesRole);
    message.addButton(CMDMultiLanguage::instance()->value(tr("取消")), QMessageBox::NoRole);
    QString strFront = CMDMultiLanguage::instance()->value(tr("确定删除 "));
    QString strName = "\'" + pHistoryGroup->name() + "\'";
    QString strBack = CMDMultiLanguage::instance()->value(tr("吗？"));
    message.setText(strFront + strName + strBack);
    CMDMultiLanguage::instance()->translateWidget(&message);

    if (QMessageBox::AcceptRole != message.exec())
    {
        return;
    }

    if (m_isOpenDataGrpFramwork)
    {
        if (pHistoryGroup->name() == m_pDataGrpFramework->windowTitle())
        {
            m_pDataGrpFramework->close();
            m_isOpenDataGrpFramwork = false;
        }
    }

    pDataGrpMana->removeHisDataGroup(pHistoryGroup);
    pDataGrpMana->save();
    if(m_HisGroupRoot)
    {
        m_HisGroupRoot->removeChild(pItem);
    }
}

void CDataArchivesUI::slotitemActiveDataGroup()
{
    DBSTUDIO_CHECK_NULL

    if (NULL == m_studioInterface)
    {
        return;
    }

    if (NULL == m_pDataGrpFramework)
    {
        m_pDataGrpFramework = new CDataGroupFramework();
        m_pDataGrpFramework->setDataArchivesUI(this);
    }

    IMDDataGroupManager *pDataGrpMana = m_pDataArchives->dataGroupManager();
    if (NULL == pDataGrpMana)
    {
        return;
    }

    IMDDataGroup *pHistoryGroup = pItem->data(0,Qt::UserRole).value< IMDDataGroup* >();
    if(NULL == pHistoryGroup)
    {
        return;
    }

    m_pDataGrpFramework->setDataGrpProperty(m_studioInterface->rtdbManagerUI(), m_pDataArchives, pHistoryGroup);

    pItem->setText(0, pHistoryGroup->name());
    pDataGrpMana->save();

    if (!m_isOpenDataGrpFramwork)
    {
        m_isOpenDataGrpFramwork = true;
        m_studioInterface->openMDIWindow(m_pDataGrpFramework);
    }
    m_studioInterface->setActiveForm(m_pDataGrpFramework);
}

void CDataArchivesUI::onMouseRClick(const QPoint &pos)
{
    QTreeWidgetItem *pItem = m_studioInterface->treeWidget()->itemAt(pos);

    if(isItemInChildren(pItem, m_AlmFieldRoot))
    {// 报警字段
    }
    else if(isItemInChildren(pItem, m_HisGroupRoot))
    {// 历史组
        ShowHisGroupMenu(pos);
    }
    else if(isItemInChildren(pItem, m_LogFieldItem))
    {// 日志
    }
    else if(isItemInChildren(pItem, m_DBStudioRoot))
    {// 数据库配置
        ShowDBMenu(pos);
    }        
}

void CDataArchivesUI::onMouseLClick(QTreeWidgetItem */*pItem*/, int /*column*/)
{
}

void CDataArchivesUI::onMouseDbClick(QTreeWidgetItem *pItem, int /*column*/)
{
    if(isItemInChildren(pItem, m_AlmFieldRoot)
       || isItemInChildren(pItem, m_LogFieldItem)
       || isItemInChildren(pItem, m_HisGroupRoot))
    {// 数据组
        slotitemActiveDataGroup();
    }
    else if(isItemInChildren(pItem, m_DBStudioRoot))
    {// 数据库配置
        slotItemActiveDataSource();
    }
}

void CDataArchivesUI::slotItemActiveDataSource()
{
    DBSTUDIO_CHECK_NULL

    if(NULL == m_studioInterface)
    {
        return;
    }

    if (NULL == m_pDBstudioFramework)
    {
        m_pDBstudioFramework = new CDBstudioFramework();
        m_pDBstudioFramework->setDataArchivesUI(this);
        m_pDBstudioFramework->setDataSourceManager(m_pDataArchives->dataSourceManager());
    }

    pItem = m_studioInterface->treeWidget()->currentItem();
    if(pItem && pItem != m_DBStudioRoot)
    {
        IMDDataSource *pDBSource = pItem->data(0,Qt::UserRole).value< IMDDataSource* >();
        if(pDBSource)
        {
            m_pDBstudioFramework->initUI(m_pDataArchives, pDBSource);
        }

        IMDDataTable *pDBTable = pItem->data(0,Qt::UserRole).value< IMDDataTable* >();
        if(pDBTable)
        {
            QTreeWidgetItem *pParent = pItem->parent();
            if(pParent)
            {
                pDBSource = pParent->data(0,Qt::UserRole).value< IMDDataSource* >();
            }

            m_pDBstudioFramework->initUI(m_pDataArchives, pDBTable, pDBSource);
        }

        if (!m_isOpenDataSrcFramwork)
        {
            m_isOpenDataSrcFramwork = true;
            m_studioInterface->openMDIWindow(m_pDBstudioFramework);
        }

        m_studioInterface->setActiveForm(m_pDBstudioFramework);
    }
}

void CDataArchivesUI::ShowHisGroupMenu(const QPoint &pos)
{
    if(!m_pHisGroupPopupMenu)
    {
        m_pHisGroupPopupMenu = new QMenu(STRING_NEW, m_studioInterface->treeWidget());

        if(!m_pHisGroup_NewAction)
        {
            m_pHisGroup_NewAction = new QAction(STRING_NEW, m_pHisGroupPopupMenu);
            connect(m_pHisGroup_NewAction,SIGNAL(triggered()),this,SLOT(slotitemActiveNewHisGroup()));
        }
        if(!m_pHisGroup_DeleteAction)
        {
            m_pHisGroup_DeleteAction = new QAction(STRING_DELETE,  m_pHisGroupPopupMenu);
            connect(m_pHisGroup_DeleteAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteHisGroup()));
        }
        if(!m_pHisGroup_PropertiesAction)
        {
            m_pHisGroup_PropertiesAction = new QAction(STRING_PROPERTY, m_pHisGroupPopupMenu);
            connect(m_pHisGroup_PropertiesAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDataGroup()));
        }

        m_pHisGroupPopupMenu->addAction(m_pHisGroup_NewAction);
        m_pHisGroupPopupMenu->addAction(m_pHisGroup_DeleteAction);
        m_pHisGroupPopupMenu->addAction(m_pHisGroup_PropertiesAction);

        refreshText();
    }
    if(m_pHisGroupPopupMenu)
    {
        m_pHisGroupPopupMenu->exec(QCursor::pos());
    }
}

void CDataArchivesUI::ShowDBMenu(const QPoint &pos)
{
    if(!m_pDBSourcePopupMenu)
    {
        m_pDBSourcePopupMenu = new QMenu(STRING_NEW, m_studioInterface->treeWidget());

        if(!m_pDBSource_NewAction)
        {
            m_pDBSource_NewAction = new QAction(STRING_NEW, m_pDBSourcePopupMenu);
            connect(m_pDBSource_NewAction,SIGNAL(triggered()),this,SLOT(slotitemActiveNewDBSource()));
        }
        if(!m_pDBSource_DeleteAction)
        {
            m_pDBSource_DeleteAction = new QAction(STRING_DEL_DATASRC, m_pDBSourcePopupMenu);
            connect(m_pDBSource_DeleteAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteDBSource()));
        }
        if(!m_pDBSource_PropertiesAction)
        {
            m_pDBSource_PropertiesAction = new QAction(STRING_DATASRC_PROPERTY, m_pDBSourcePopupMenu);
            connect(m_pDBSource_PropertiesAction,SIGNAL(triggered()),this,SLOT(slotItemActiveDataSource()));
        }
        if(!m_pDBSourceTable_NewAction)
        {
            m_pDBSourceTable_NewAction = new QAction(STRING_NEW_TABLE, m_pDBSourcePopupMenu);
            connect(m_pDBSourceTable_NewAction,SIGNAL(triggered()),this,SLOT(slotitemActiveNewDBSourceTable()));
        }
        if(!m_pDBSourceTable_DeleteAction)
        {
            m_pDBSourceTable_DeleteAction = new QAction(STRING_DEL_TABLE, m_pDBSourcePopupMenu);
            connect(m_pDBSourceTable_DeleteAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteDBSourceTable()));
        }
        if(!m_pDBSourceTable_PropertiesAction)
        {
            m_pDBSourceTable_PropertiesAction = new QAction(STRING_TAB_PROPERTY, m_pDBSourcePopupMenu);
            connect(m_pDBSourceTable_PropertiesAction,SIGNAL(triggered()),this,SLOT(slotItemActiveDataSource()));
        }
        // 加数据源菜单
        m_pDBSourcePopupMenu->addAction(m_pDBSource_NewAction);
        m_pDBSourcePopupMenu->addAction(m_pDBSource_DeleteAction);
        m_pDBSourcePopupMenu->addAction(m_pDBSource_PropertiesAction);
        // 加分隔符
        m_pDBSourcePopupMenu->addSeparator();
        // 加数据表菜单
        m_pDBSourcePopupMenu->addAction(m_pDBSourceTable_NewAction);
        m_pDBSourcePopupMenu->addAction(m_pDBSourceTable_DeleteAction);
        m_pDBSourcePopupMenu->addAction(m_pDBSourceTable_PropertiesAction);

        refreshText();
    }

    if(m_pDBSourcePopupMenu)
    {        
        if(m_studioInterface->treeWidget())
        {
            QTreeWidgetItem *pItem = m_studioInterface->treeWidget()->currentItem();
            if(pItem)
            {
                if(pItem == m_DBStudioRoot)
                {// 如果点中的是根节点
                    m_pDBSource_DeleteAction->setDisabled(true);
                    m_pDBSource_PropertiesAction->setDisabled(true);

                    // 禁止数据表菜单
                    m_pDBSourceTable_NewAction->setDisabled(true);
                    m_pDBSourceTable_DeleteAction->setDisabled(true);
                    m_pDBSourceTable_PropertiesAction->setDisabled(true);
                }
                else
                {
                    IMDDataSource *pDBSource = pItem->data(0,Qt::UserRole).value< IMDDataSource* >();
                    if(pDBSource)
                    {
                        m_pDBSource_NewAction->setDisabled(false);
                        m_pDBSource_DeleteAction->setDisabled(false);
                        m_pDBSource_PropertiesAction->setDisabled(false);

                        m_pDBSourceTable_NewAction->setDisabled(false);
                        m_pDBSourceTable_DeleteAction->setDisabled(true);
                        m_pDBSourceTable_PropertiesAction->setDisabled(true);
                    }
                    IMDDataTable *pDBTable = pItem->data(0,Qt::UserRole).value< IMDDataTable* >();
                    if(pDBTable)
                    {
                        m_pDBSource_NewAction->setDisabled(true);
                        m_pDBSource_DeleteAction->setDisabled(true);
                        m_pDBSource_PropertiesAction->setDisabled(true);

                        m_pDBSourceTable_NewAction->setDisabled(false);
                        m_pDBSourceTable_DeleteAction->setDisabled(false);
                        m_pDBSourceTable_PropertiesAction->setDisabled(false);
                    }
                }
            }
        }
        m_pDBSourcePopupMenu->exec(QCursor::pos());
    }
}

void CDataArchivesUI::refreshText()
{
    if (m_DBStudioRoot)
    {
        m_DBStudioRoot->setText(0, CMDMultiLanguage::instance()->value(STRING_DATASOURCE));
    }

    if (m_DBDataGroupRoot)
    {
        m_DBDataGroupRoot->setText(0, CMDMultiLanguage::instance()->value(STRING_DATAGROUP));
    }

    if (m_HisGroupRoot)
    {
        m_HisGroupRoot->setText(0, CMDMultiLanguage::instance()->value(STRING_HISGRP));
    }

    if (m_pHisGroup_NewAction)
    {
        m_pHisGroup_NewAction->setText(CMDMultiLanguage::instance()->value(STRING_NEW));
    }

    if (m_pHisGroup_DeleteAction)
    {
        m_pHisGroup_DeleteAction->setText(CMDMultiLanguage::instance()->value(STRING_DELETE));
    }

    if (m_pHisGroup_PropertiesAction)
    {
        m_pHisGroup_PropertiesAction->setText(CMDMultiLanguage::instance()->value(STRING_PROPERTY));
    }

    if (m_pDBSource_NewAction)
    {
        m_pDBSource_NewAction->setText(CMDMultiLanguage::instance()->value(STRING_NEW_DATASRC));
    }

    if (m_pDBSource_DeleteAction)
    {
        m_pDBSource_DeleteAction->setText(CMDMultiLanguage::instance()->value(STRING_DEL_DATASRC));
    }

    if (m_pDBSourceTable_NewAction)
    {
        m_pDBSourceTable_NewAction->setText(CMDMultiLanguage::instance()->value(STRING_NEW_TABLE));
    }

    if (m_pDBSourceTable_DeleteAction)
    {
        m_pDBSourceTable_DeleteAction->setText(CMDMultiLanguage::instance()->value(STRING_DEL_TABLE));
    }

    if (m_pDBSourceTable_PropertiesAction)
    {
        m_pDBSourceTable_PropertiesAction->setText(CMDMultiLanguage::instance()->value(STRING_TAB_PROPERTY));
    }
}

void CDataArchivesUI::slotitemActiveNewDBSourceTable()
{
    if(NULL == m_studioInterface->treeWidget())
    {
        return;
    }

    QTreeWidgetItem *pItem = m_studioInterface->treeWidget()->currentItem();
    if(NULL == pItem)
    {
        return;
    }

    if(NULL == m_pDataArchives)
    {
        return;
    }

    pItem = m_studioInterface->treeWidget()->currentItem();
    IMDDataTable *pDBTable = pItem->data(0,Qt::UserRole).value< IMDDataTable* >();
    if(pDBTable)
    {// 如果选择的是表，取父节点
        pItem = pItem->parent();
    }

    IMDDataSource *pDBSource = pItem->data(0,Qt::UserRole).value< IMDDataSource* >();
    if( NULL == pDBSource)
    {
        return;
    }

    pDBTable = CMDDataSourceFactory::createDataTable();
    CDBTableDialog dlg;
    dlg.initData(pDBSource, pDBTable, m_pDataArchives->dataGroupManager());
    if(dlg.exec() == QDialog::Accepted)
    {
        if(pDBSource->addDataTable(pDBTable))
        {
            QTreeWidgetItem *pItemNew = new QTreeWidgetItem(QStringList(pDBTable->name()));
            pItemNew->setIcon(0,QIcon(":/images/file.png"));
            pItem->addChild(pItemNew);

            QVariant Var;
            Var.setValue (pDBTable);
            pItemNew->setData (0, Qt::UserRole, Var);

            m_pDataArchives->dataSourceManager()->save();

            m_studioInterface->treeWidget()->setCurrentItem(pItemNew);
            if (NULL == m_pDBstudioFramework)
            {
                m_pDBstudioFramework = new CDBstudioFramework();
                m_pDBstudioFramework->setDataArchivesUI(this);
                m_pDBstudioFramework->setDataSourceManager(m_pDataArchives->dataSourceManager());
            }

            m_pDBstudioFramework->initUI(m_pDataArchives, pDBTable, pDBSource);
            if (!m_isOpenDataSrcFramwork)
            {
                m_isOpenDataSrcFramwork = true;
                m_studioInterface->openMDIWindow(m_pDBstudioFramework);
            }

            m_studioInterface->setActiveForm(m_pDBstudioFramework);
        }
        else
        {
            delete pDBTable;
            pDBTable = NULL;
        }
    }
    else
    {
        delete pDBTable;
        pDBTable = NULL;
    }
}

void CDataArchivesUI::slotitemActiveDeleteDBSourceTable()
{
    DBSTUDIO_CHECK_NULL

    IMDDataSourceManager *pDataSrcManager = m_pDataArchives->dataSourceManager();
    if(NULL == pDataSrcManager)
    {
        return;
    }

    IMDDataTable *pDBTable = pItem->data(0,Qt::UserRole).value< IMDDataTable* >();
    if(pDBTable)
    {// 如果选择的是表，取父节点
        QTreeWidgetItem *pParnetItem = pItem->parent();
        if(NULL == pParnetItem)
        {
            return;
        }
        IMDDataSource *pDBSource = pParnetItem->data(0,Qt::UserRole).value< IMDDataSource* >();
        if(NULL == pDBSource)
        {
            return;
        }

        QMessageBox message(m_studioInterface->treeWidget());
        message.setWindowTitle(CMDMultiLanguage::instance()->value(tr("提示")));
        message.addButton(CMDMultiLanguage::instance()->value(tr("确定")), QMessageBox::YesRole);
        message.addButton(CMDMultiLanguage::instance()->value(tr("取消")), QMessageBox::NoRole);
        QString strFront = CMDMultiLanguage::instance()->value(tr("确定删除 "));
        QString strName = "\'" + pDBTable->name() + "\'";
        QString strBack = CMDMultiLanguage::instance()->value(tr("吗？"));
        message.setText(strFront + strName + strBack);
        CMDMultiLanguage::instance()->translateWidget(&message);

        if (QMessageBox::AcceptRole == message.exec())
        {
            if (m_isOpenDataSrcFramwork)
            {
                if (pDBTable->name() == m_pDBstudioFramework->windowTitle())
                {
                    m_isOpenDataGrpFramwork = false;
                    m_pDBstudioFramework->close();
                }
            }

            pDBSource->removeDataTable(pDBTable);
            pDataSrcManager->save();
            pParnetItem->removeChild(pItem);
        }
    }
}

void CDataArchivesUI::initData()
{
    m_AlmFieldRoot                     = NULL;  //! 报警组根结点
    m_HisGroupRoot                     = NULL;  //! 历史组根结点
    m_LogFieldItem                     = NULL;  //! 日志根结点
    m_DBStudioRoot                     = NULL;  //! 数据源根结点
    m_DBDataGroupRoot                  = NULL;  //! 数据组根结点

    /*!
    \section 历史组菜单
    */
    m_pHisGroupPopupMenu                       = NULL;  //! 历史组菜单
    m_pHisGroup_NewAction                      = NULL;  //! 历史组新建选项
    m_pHisGroup_DeleteAction                   = NULL;  //! 历史组删除选项
    m_pHisGroup_PropertiesAction               = NULL;  //! 历史组属性选项

    /*!
    \section 数据源菜单
    */
    m_pDBSourcePopupMenu                       = NULL;  //! 数据源菜单
    m_pDBSource_NewAction                      = NULL;  //! 数据源新建选项
    m_pDBSource_DeleteAction                   = NULL;  //! 数据源删除选项
    m_pDBSource_PropertiesAction               = NULL;  //! 数据源属性选项

    /*!
    \section 数据表菜单
    */
    m_pDBSourceTable_NewAction                 = NULL;  //! 数据表新建选项
    m_pDBSourceTable_DeleteAction              = NULL;  //! 数据表删除选项
    m_pDBSourceTable_PropertiesAction          = NULL;  //! 数据表属性选项

    m_pDBstudioFramework            = NULL;  //! 数据源MDI主窗体
    m_studioInterface              = NULL;
    m_pDataGrpFramework             = NULL;  //! 数据组MDI主窗体
    m_pDataArchives                 = NULL;  //! 数据归档
    m_pMulLnaguageTool              = NULL;  //! 多语言翻译工具

    m_isOpenDataSrcFramwork         = false; //! 数据源MDI主窗体打开标识
    m_isOpenDataGrpFramwork         = false; //! 数据组MDI主窗体打开标识
}

void CDataArchivesUI::initLanguage()
{
    if (NULL == m_pMulLnaguageTool)
    {
        m_pMulLnaguageTool = CMDMultiLanguage::createWidgetTool();
        m_pMulLnaguageTool->init(this);
    }

    m_pMulLnaguageTool->changeLanguage();
}

void CDataArchivesUI::slotitemActiveNewDBSource()
{
    DBSTUDIO_CHECK_NULL

    IMDDataSourceManager *pDataSrcManager = m_pDataArchives->dataSourceManager();
    if(NULL == pDataSrcManager)
    {
        return;
    }

    IMDDataSource *pDataSrc = CMDDataSourceFactory::createDataSource();
    if(NULL == pDataSrc)
    {
        return;
    }

    CNewDBsourceDialog dlg;
    dlg.initUI(pDataSrc, m_pDataArchives);
    if(dlg.exec() == QDialog::Accepted)
    {
        if(pDataSrcManager->addDataSource(pDataSrc))
        {
            pItem = new QTreeWidgetItem(QStringList(pDataSrc->name()));
            pItem->setIcon(0,QIcon(":/images/file.png"));
            m_DBStudioRoot->addChild(pItem);

            QVariant Var;
            Var.setValue (pDataSrc);
            pItem->setData (0, Qt::UserRole, Var);

            pDataSrcManager->save();

            if (pDataSrc->isShowTable())
            {
                IMDDataTable *pTable = NULL;
                int count = pDataSrc->dataTableCount();
                for(int j = 0; j < count; j++)
                {
                    pTable = pDataSrc->getDataTable(j);
                    if(NULL == pDataSrc)
                    {
                        continue;
                    }

                    QTreeWidgetItem *pItemTable = new QTreeWidgetItem(QStringList(pTable->name()));
                    pItemTable->setIcon(0,QIcon(":/images/file.png"));
                    pItem->addChild(pItemTable);

                    QVariant Var;
                    Var.setValue (pTable);
                    pItemTable->setData (0, Qt::UserRole, Var);
                }
            }

            m_studioInterface->treeWidget()->setCurrentItem(pItem);
            if (NULL == m_pDBstudioFramework)
            {
                m_pDBstudioFramework = new CDBstudioFramework();
                m_pDBstudioFramework->setDataArchivesUI(this);
                m_pDBstudioFramework->setDataSourceManager(m_pDataArchives->dataSourceManager());
            }

            m_pDBstudioFramework->initUI(m_pDataArchives, pDataSrc);
            if (!m_isOpenDataSrcFramwork)
            {
                m_isOpenDataSrcFramwork = true;
                m_studioInterface->openMDIWindow(m_pDBstudioFramework);
            }

            m_studioInterface->setActiveForm(m_pDBstudioFramework);
        }
        else
        {
            delete pDataSrc;
            pDataSrc = NULL;
        }
    }
    else
    {
        delete pDataSrc;
        pDataSrc = NULL;
    }
}

void CDataArchivesUI::slotitemActiveDeleteDBSource()
{
    DBSTUDIO_CHECK_NULL

    IMDDataSourceManager *pDataSrcManager = m_pDataArchives->dataSourceManager();
    if(NULL == pDataSrcManager)
    {
        return;
    }

    QString strName = pItem->text(0);
    IMDDataSource *PDataSrc = pDataSrcManager->getDataSource(strName);
    if(NULL == PDataSrc)
    {
        return;
    }

    QMessageBox message(m_studioInterface->treeWidget());
    message.setWindowTitle(CMDMultiLanguage::instance()->value(tr("提示")));
    message.addButton(CMDMultiLanguage::instance()->value(tr("确定")), QMessageBox::YesRole);
    message.addButton(CMDMultiLanguage::instance()->value(tr("取消")), QMessageBox::NoRole);
    QString strFront = CMDMultiLanguage::instance()->value(tr("确定删除 "));
    QString strBack = CMDMultiLanguage::instance()->value(tr(" 吗？"));
    message.setText(strFront + "\'" + strName + "\'" + strBack);
    CMDMultiLanguage::instance()->translateWidget(&message);

    if (QMessageBox::AcceptRole == message.exec())
    {
        if (m_isOpenDataSrcFramwork)
        {
            if (PDataSrc->name() == m_pDBstudioFramework->windowTitle())
            {
                m_isOpenDataGrpFramwork = false;
                m_pDBstudioFramework->close();
            }
        }

        pDataSrcManager->removeDataSource(PDataSrc);
        pDataSrcManager->save();
        if(m_DBStudioRoot)
        {
            m_DBStudioRoot->removeChild(pItem);
            delete pItem;
            pItem = NULL;
        }
    }
}

void CDataArchivesUI::closeEvent(CBaseForm *pForm)
{
    if(m_pDBstudioFramework != NULL && m_pDBstudioFramework == pForm)
    {
        m_isOpenDataSrcFramwork = false;
        m_pDBstudioFramework->close();
        delete m_pDBstudioFramework;
        m_pDBstudioFramework = NULL;
    }

    if(m_pDataGrpFramework != NULL && m_pDataGrpFramework == pForm)
    {
        m_isOpenDataGrpFramwork = false;
        m_pDataGrpFramework->close();
        delete m_pDataGrpFramework;
        m_pDataGrpFramework = NULL;
    }
}

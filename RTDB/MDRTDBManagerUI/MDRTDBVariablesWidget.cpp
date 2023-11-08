#include "MDRTDBVariablesWidget.h"

#include <QFileInfo>
#include <QDir>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>

#include "MDRTDBManagerFactory.h"
#include "MDRTDBManagerUI.h"
#include "MDRTDBVariablesWindow.h"
#include "AddVarGroupDialog.h"
#include "VariablePropertyDialog.h"
#include "IMDTreeItem.h"
#include "IMDTreeModel.h"
#include "IMDRTDBManager.h"
#include "MDMultiLanguage.h"
#include "PropertyBrowser.h"
#include "MDRTDBBatchChangeVariable.h"
#include "VariablesBatchChangeDialog.h"
#include "MDObjBatchOperationExpert.h"
#include "MDRTDBVariableProperty.h"
#include "MDVariableGroupProperty.h"
#include "TreeViewEx.h"

#include "OPCUaClient.h"
#include <string>
#include <QDebug>

#ifdef _WIN32
#include "MMKV/MMKV.h"
#elif defined(__linux__)
#include "MMKV.h"
#endif

CMDRTDBVariablesWidget::CMDRTDBVariablesWidget(QWidget *parent) : QWidget(parent)
{
    m_variableTreeModel = NULL;
    m_varProxyModel = NULL;
    m_mdRTDBManager = NULL;
    m_projectConfiger= NULL;
    m_treeView = new CTreeViewEx(this);
    m_variableWriteValueDialog =NULL;

    m_treeView->setAlternatingRowColors(true);
    m_treeView->setAnimated(false);
    m_treeView->setAllColumnsShowFocus(true);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    //设置多选
    m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_treeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_treeView->header()->setSectionResizeMode(QHeaderView::Interactive);

    //!允许点击标题栏排序
    m_treeView->setSortingEnabled(true);       
    //按钮文本
    m_addVarGroupText = tr("添加变量组");
    m_addVarText = tr("添加变量");
    m_syncVarText = tr("变量同步至tres");
    m_syncVarFromOpcUaServerText = tr("从OPCUAServer同步变量");

    m_copyText = tr("复制");
    m_pasteText = tr("粘帖");
    m_deleteText = tr("删除");

    m_runText = tr("配置->运行");
    m_writeVarText = tr("写变量");

    m_configText = tr("属性");
    m_batchChangeVarPropText = tr("批量修改");
    m_batchAddVarText = tr("批量增加");
    //多语言对象
    m_translateTool = CMDMultiLanguage::createWidgetTool();
    m_translateTool->init(this);

    m_batchOp = NULL;
    m_batchVar = NULL;
    m_toolBar = NULL;   
    initialMenu();

    //bind event
    connect(m_treeView, &QTreeView::clicked, this, &CMDRTDBVariablesWidget::treeViewClicked);
    connect(m_treeView, &QTreeView::doubleClicked, this, &CMDRTDBVariablesWidget::treeViewDoubleClicked);
    connect(m_treeView, &QTreeView::pressed, this, &CMDRTDBVariablesWidget::treeViewNodeMouseClicked);
    connect(m_treeView, &CTreeViewEx::mousePressed, this, &CMDRTDBVariablesWidget::mousePressed);
    m_selectedModelIndex = QModelIndex();
    m_copyObjItem = NULL;

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addWidget(m_treeView);
    m_mainLayout->setMargin(1);
    setLayout(m_mainLayout);

    //响应键盘事件
    this->setFocusPolicy(Qt::StrongFocus);

    m_rtdbUI = NULL;
    m_curItem = NULL;
    m_MDStudioInterface = NULL;

    m_timer.setInterval(1000);
    m_timer.start();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    m_variableProperty = new CMDRTDBVariableProperty();
    m_variableGroupProperty = new CMDVariableGroupProperty();
}

//! 刷新变量列表控件
void CMDRTDBVariablesWidget::onTimerOut()
{
    if(m_mdRTDBManager->isRunning() && m_varProxyModel != NULL)
    {
        emit m_varProxyModel->layoutChanged();
        emit m_varProxyModel->dataChanged(QModelIndex(), QModelIndex());
    }
}


CMDRTDBVariablesWidget::~CMDRTDBVariablesWidget()
{
    if(m_variableTreeModel)
    {
        delete m_variableTreeModel;
        m_variableTreeModel = NULL;

        delete m_varProxyModel;
        m_varProxyModel = NULL;
    }
    if(m_mainLayout)
    {
        delete m_mainLayout;
        m_mainLayout = NULL;
    }
    if(m_variableWriteValueDialog)
    {
        m_variableWriteValueDialog->close();
        delete m_variableWriteValueDialog;
        m_variableWriteValueDialog = NULL;
    }

    delete m_treeView;//Y2hw

    delete m_variableProperty;
    delete m_variableGroupProperty;

}

void CMDRTDBVariablesWidget::setRootTreeItem(CMDRTDBManagerUI* RTDBUI, const QString& groupName)
{
    m_groupName = groupName;
    m_rtdbUI = RTDBUI;
}

bool CMDRTDBVariablesWidget::save()
{
    if(m_mdRTDBManager!= NULL)
    {
        m_mdRTDBManager->saveVariableGroupConfiguration();
        return true;
    }
    return false;
}

void CMDRTDBVariablesWidget::setProjectConfiger(CMDProjectConfiger *projectConfiger)
{
    m_projectConfiger = projectConfiger;
    if(m_projectConfiger== NULL)
    {
        return;
    }
    m_mdRTDBManager = m_projectConfiger->nsRTDBManager();
    m_variableTreeModel = CMDRTDBManagerFactory::createVariableTreeModel(m_mdRTDBManager);
    m_variableTreeModel->setTreeItemIcon(QIcon(":/images/VariableGroup.png"),MDTreeItemType::VariableGroup);
    m_variableTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);
    m_curItem = m_mdRTDBManager->variableGroup();
    if(!m_groupName.isEmpty())
        m_curItem = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(m_groupName);

    if(m_rtdbUI != NULL)
        m_MDStudioInterface = m_rtdbUI->MDStudioInterface();
    //刷新变量名列表
    m_deviceNameList.clear();
    IDevice* dev = NULL;
    m_deviceNameList.append("InvalidDeviceName");
    int nDevCnt = m_mdRTDBManager->driverManager()->getDeviceCount();
    for(int i = 0; i<nDevCnt; i++)
    {
        dev = m_mdRTDBManager->driverManager()->getDeviceAt(i);
        if(dev != NULL)
            m_deviceNameList.append(dev->name());
    }
    m_selectedModelIndex = QModelIndex();
    bindVariableTreeModel();
}


CMDProjectConfiger* CMDRTDBVariablesWidget::getProjectConfiger()
{
    return m_projectConfiger;
}

bool CMDRTDBVariablesWidget::getRunningStatus()
{
    return m_mdRTDBManager->isRuntimeMode() || m_mdRTDBManager->isRunning();
}

void CMDRTDBVariablesWidget::unBindVariableTreeModel()
{
    //m_treeView->setModel(NULL);
    if(m_variableTreeModel!=NULL)
    {
        delete m_variableTreeModel;
        m_variableTreeModel= NULL;
    }
    if(m_varProxyModel != NULL)
    {
        delete m_varProxyModel;
        m_varProxyModel = NULL;
    }
    m_treeSelModel = NULL;
}

//!动态绑定运行或配置变量列表框
void CMDRTDBVariablesWidget::bindVariableTreeModel()
{
    if(m_mdRTDBManager!=NULL)
    {
        if(m_mdRTDBManager->isRunning())
        {
            m_treeView->setModel(NULL);
            if(m_variableTreeModel!=NULL)
            {
                delete m_variableTreeModel;
                m_variableTreeModel= NULL;
            }
            if(m_varProxyModel != NULL)
            {
                delete m_varProxyModel;
                m_varProxyModel = NULL;
            }
            m_variableTreeModel = CMDRTDBManagerFactory::createVariableRunTreeModel(m_mdRTDBManager);
            m_varProxyModel = new QSortFilterProxyModel();
            m_varProxyModel->setSourceModel(m_variableTreeModel);

            m_variableTreeModel->setTreeItemIcon(QIcon(":/images/VariableGroup.png"),MDTreeItemType::VariableGroup);
            m_variableTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);
            m_variableTreeModel->setCurRootTreeItem(m_groupName);
            //m_treeView->setModel(m_variableTreeModel);
            m_treeView->setModel(m_varProxyModel);
        }
        else
        {
            m_treeView->setModel(NULL);
            if(m_variableTreeModel!=NULL)
            {
                delete m_variableTreeModel;
                m_variableTreeModel= NULL;

                delete m_varProxyModel;
                m_varProxyModel = NULL;
            }

            //!设置当前Group

            m_variableTreeModel = CMDRTDBManagerFactory::createVariableTreeModel(m_mdRTDBManager);

            m_varProxyModel = new QSortFilterProxyModel();
            m_varProxyModel->setSourceModel(m_variableTreeModel);

            m_variableTreeModel->setTreeItemIcon(QIcon(":/images/VariableGroup.png"),MDTreeItemType::VariableGroup);
            m_variableTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);
            m_variableTreeModel->setCurRootTreeItem(m_groupName);
            //m_treeView->setModel(m_variableTreeModel);
            m_treeView->setModel(m_varProxyModel);
            m_treeSelModel = m_treeView->selectionModel();
            connect(m_treeSelModel,&QItemSelectionModel::selectionChanged,this,&CMDRTDBVariablesWidget::treeSelectionChanged);
        }
        m_treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    }
}

//!多语言
void CMDRTDBVariablesWidget::changeLanguage()
{
    m_variableTreeModel->changeLanguage();
}

void CMDRTDBVariablesWidget::initialMenu()
{
    m_actAddVarGroup = new QAction(QIcon(":/images/Group.png"), m_addVarGroupText, this);
    m_actAddVar = new QAction(QIcon(":/images/tag_orange_add.png"), m_addVarText, this);
    m_actSyncVar = new QAction(QIcon(":/images/sync_vars2tres.png"), m_syncVarText, this);
    m_actSyncVarFromOpcUaServer = new QAction(QIcon(":/images/sync_vars_from_opcuaserver.png"), m_syncVarFromOpcUaServerText, this);
    m_actConfig = new QAction(QIcon(":/images/Config.png"), m_configText, this);
    m_actBatchChangeVarProp = new QAction(QIcon(":/images/BatchChange.png"), m_batchChangeVarPropText, this);
    m_actBatchAddVar = new QAction(QIcon(":/images/BatchAdd.png"), m_batchAddVarText, this);
    m_actCopy = new QAction(QIcon(":/rs/image/copy"), m_copyText, this);
    m_actPaste = new QAction(QIcon(":/rs/image/Paste"), m_pasteText, this);
    m_actDelete = new QAction(QIcon(":/rs/image/delete"), m_deleteText, this);
    m_actRun = new QAction(QIcon(":/images/start.png"), m_runText, this);
    m_actWriteVar= new QAction(QIcon(":/images/tags_edit.png"), m_writeVarText, this);
    m_actCopy->setShortcut(QKeySequence::Copy);
    m_actPaste->setShortcut(QKeySequence::Paste);
    m_actDelete->setShortcut(QKeySequence::Delete);

    connect(m_actAddVarGroup, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemAddVarGroupClick);
    connect(m_actAddVar, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemAddVarClick);
    connect(m_actSyncVar, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemSyncVarClick);
    connect(m_actSyncVarFromOpcUaServer, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemSyncVarFromOpcUaServerClick);
    connect(m_actConfig, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemConfigClick);
    connect(m_actCopy, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemCopyClick);
    connect(m_actPaste, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemPasteClick);
    connect(m_actDelete, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemDeleteClick);
    connect(m_actRun, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemRunClick);
    connect(m_actWriteVar, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemWriteVarClick);
    connect(m_actBatchChangeVarProp, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemBatchChangeVarPropClick);
    connect(m_actBatchAddVar, &QAction::triggered, this, &CMDRTDBVariablesWidget::menuItemBatchAddVarClick);

    m_menu = new QMenu(m_treeView);
    m_menu->addAction(m_actAddVarGroup);
    m_menu->addAction(m_actAddVar);
    m_menu->addAction(m_actSyncVarFromOpcUaServer);
//    m_menu->addAction(m_actSyncVar);
    m_menu->addSeparator();

    m_menu->addAction(m_actCopy);
    m_menu->addAction(m_actPaste);
    m_menu->addAction(m_actDelete);
    m_menu->addSeparator();

    m_menu->addAction(m_actBatchAddVar);
    m_menu->addAction(m_actBatchChangeVarProp);
    m_menu->addSeparator();

    m_menu->addAction(m_actRun);
    m_menu->addAction(m_actWriteVar);
    m_menu->addAction(m_actConfig);


    //!多语言翻译
    m_translateTool->changeLanguage();

    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeView, &QTreeView::customContextMenuRequested,
            this, &CMDRTDBVariablesWidget::menuStripNodeOpening);

    //!toolbar
    //!创建ToolBar
    if(m_toolBar == NULL)
    {
        m_toolBar = new QToolBar(tr("变量"),this);
        m_toolBar->addAction(m_actAddVarGroup);
        m_toolBar->addAction(m_actAddVar);
        m_toolBar->addAction(m_actSyncVarFromOpcUaServer);
        m_toolBar->addAction(m_actSyncVar);
        m_toolBar->addSeparator();

        m_toolBar->addAction(m_actCopy);
        m_toolBar->addAction(m_actPaste);
        m_toolBar->addAction(m_actDelete);
        m_toolBar->addSeparator();

        m_toolBar->addAction(m_actBatchChangeVarProp);
        m_toolBar->addAction(m_actBatchAddVar);
        m_toolBar->addSeparator();

        m_toolBar->addAction(m_actRun);
        m_toolBar->addAction(m_actWriteVar);
        m_toolBar->addAction(m_actConfig);
    }
}

QToolBar* CMDRTDBVariablesWidget::toolBar()
{
   return m_toolBar;
}
/*!
\section 树控件操作
*/
void CMDRTDBVariablesWidget::treeViewClicked(const QModelIndex &index)
{
    QModelIndex index0 = m_varProxyModel->mapToSource(index);
    m_selectedModelIndex = index0;
    if(m_variableWriteValueDialog)
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item->itemType() == MDTreeItemType::Variable)
        {
            IMDRTDBVariable * var = (IMDRTDBVariable*)item;            
            if(var !=NULL)
            {
                IMDVariableGroup* group = (IMDVariableGroup*)var->parentItem(MDTreeItemType::VariableGroup);
                if(group->localName()  !="SystemVariableGroup")
                {
                    m_variableWriteValueDialog->setVariable(var);
                }
            }
        }
    }
    treeViewNodeMouseClicked(index);
}

void CMDRTDBVariablesWidget::treeViewDoubleClicked(const QModelIndex &index)
{
    //m_selectedModelIndex = index;
    treeViewNodeMouseClicked(index);
    menuItemConfigClick();//Y2hw
    /*
    //!检查是否是变量
    if(index.isValid())
    {
        IMDTreeItem * item = static_cast<IMDTreeItem*>(index.internalPointer());
        //if(item->itemType() == MDTreeItemType::Variable)
            //setEditTriggers(QAbstractItemView::NoEditTriggers)
        //m_treeView->setEditTriggers(QAbstractItemView::DoubleClicked);
    }
    */
}
void CMDRTDBVariablesWidget::treeViewNodeMouseClicked(const QModelIndex &index)
{
    QModelIndex index0 = m_varProxyModel->mapToSource(index);
    m_selectedModelIndex = index0;
    if(!m_selectedModelIndex.isValid() || m_MDStudioInterface == NULL)
        return;

    refershActionStatus(m_mdRTDBManager->isRunning(),false,0,false);
    IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
    if(item->itemType() == MDTreeItemType::Variable)
    {
        IMDRTDBVariable * var = (IMDRTDBVariable*)item;

        //!设置属性框
        if(var != NULL)
        {
            m_variableProperty->setRTDBManager(m_mdRTDBManager);
            m_variableProperty->setPropertyBrowser(m_MDStudioInterface->propertyBrowser());

            m_variableProperty->copy(var);
            m_variableProperty->setParentWidget(this);

            //设备名属性设置在后面便于刷新属性框的设备名属性
            m_variableProperty->setDeviceNameList(m_deviceNameList);

            CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
            property->setObject(m_variableProperty);
            property->resetEnums("DeviceName",m_deviceNameList);
            property->updateProperty();
            //属性不可更改，变量可写
            if(m_mdRTDBManager->isRunning())
            {
                property->setPropertyReadOnly();
            }
            else
            {
                property->setPropertyReadOnly(false);
            }

            m_MDStudioInterface->propertyBrowser()->setExpandAll();

        }
    }
    else if(item->itemType() == MDTreeItemType::VariableGroup)
    {
        IMDVariableGroup * group = (IMDVariableGroup*)item;
        //!设置属性框
        if(group != NULL)
        {
            m_variableGroupProperty->copy(group);
            m_variableGroupProperty->setParentWidget(this);
            CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
            property->setObject(m_variableGroupProperty);
            property->updateProperty();
            if(m_mdRTDBManager->isRunning())
            {
                property->setPropertyReadOnly();
            }
            else
            {
                property->setPropertyReadOnly(false);
            }
            m_MDStudioInterface->propertyBrowser()->setExpandAll();
        }
    }
}


/*!
\section 键盘事件
*/
void CMDRTDBVariablesWidget::keyPressEvent( QKeyEvent *k )
{
    //key事件中一定要判断节点是否存在和有效
    if(m_treeView != NULL)
    {
        QModelIndex index0 = m_treeView->currentIndex();
        QModelIndex index = m_varProxyModel->mapToSource(index0);
        if (index.isValid())
        {
            if(m_mdRTDBManager->isRuntimeMode())
            {
                return QWidget::keyPressEvent(k);
            }
            m_selectedModelIndex = index;
            switch (k->key())
            {
                case Qt::Key_Delete:
                        menuItemDeleteClick();
                    break;
                case Qt::Key_C:		//copy
                    if ((k->modifiers() & Qt::ControlModifier) != 0)
                    {
                        menuItemCopyClick();
                    }
                    break;
                case Qt::Key_V:		//paste
                    if ((k->modifiers() & Qt::ControlModifier) != 0)
                    {
                        menuItemPasteClick();
                    }
                    break;
                default:
                    break;
            }
        }
    }
    QWidget::keyPressEvent(k);
}

void CMDRTDBVariablesWidget::mousePressed()
{
    QModelIndex curIndex = m_treeView->currentIndex();
    if(curIndex.isValid())
    {
        //m_treeView->setCurrentIndex(curIndex);
        //treeViewNodeMouseClicked(curIndex);
    }
    else
    {
        m_selectedModelIndex = QModelIndex();
        refershActionStatus(m_mdRTDBManager->isRunning(),false,0,false);
    }
}

//!弹出右键菜单
void CMDRTDBVariablesWidget::menuStripNodeOpening(const QPoint &pos)
{
    QModelIndex index0 = m_treeView->indexAt(pos);
    refershActionStatus(m_mdRTDBManager->isRunning(),false,0,false);
    m_selectedModelIndex = m_varProxyModel->mapToSource(index0);
    m_menu->exec(QCursor::pos());
    return;
    /*
    bool bIndex = false;
    if (index.isValid() || m_curItem != NULL)
    {
        IMDTreeItem *item = m_curItem;
        if(index.isValid())
        {
            item = static_cast<IMDTreeItem*>(index.internalPointer());
            bIndex = true;
        }
        if(m_mdRTDBManager->isRuntimeMode())
        {
            m_actAddVarGroup->setVisible(false);
            m_actAddVar->setVisible(false);
            m_actCopy->setVisible(false);
            m_actPaste->setVisible(false);
            m_actDelete->setVisible(false);
            m_actRun->setVisible(false);
            //如果是变量，支持写变量
            if(item->itemType() == MDTreeItemType::Variable)
                m_actWriteVar->setVisible(true);
            else
                m_actWriteVar->setVisible(false);

            m_actBatchChangeVarProp->setVisible(false);
            m_actBatchAddVar->setVisible(false);
            m_actConfig->setVisible(true);
            m_menu->exec(QCursor::pos());
            m_selectedModelIndex = index;
            return;
        }

        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            m_actBatchChangeVarProp->setVisible(false);
            IMDVariableGroup* group = (IMDVariableGroup*)item;
            if(group->localName()  =="SystemVariableGroup")
            {
                m_actAddVarGroup->setVisible(false);
                m_actAddVar->setVisible(false);
                m_actConfig->setVisible(false);
                m_actCopy->setVisible(false);
                m_actPaste->setVisible(false);
                m_actDelete->setVisible(false);
                m_actRun->setVisible(true);
                m_actWriteVar->setVisible(false);
                m_actBatchChangeVarProp->setVisible(false);
                m_actBatchAddVar->setVisible(false);
                m_selectedModelIndex = index;
            }
            else
            {
                if(m_mdRTDBManager->isRunning())//自定义Group
                {
                    m_actAddVarGroup->setVisible(false);
                    m_actAddVar->setVisible(false);

                    m_actBatchChangeVarProp->setVisible(false);
                    m_actBatchAddVar->setVisible(false);
                    m_actCopy->setVisible(false);
                    m_actPaste->setVisible(false);
                    m_actDelete->setVisible(false);

                    m_actRun->setVisible(true);
                    m_actWriteVar->setVisible(false);

                    m_actConfig->setVisible(true);
                    m_actRun->setIcon(QIcon(":/images/stop.png"));
                    m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
                }
                else//Group 编辑状态
                {
                    //!检查复制的对象类型。为Variable时，总Group节点时，不粘贴变量，
                    int nType = 9999;//not exit                    
                    if(m_copyObjItem != NULL)
                        nType = (int)m_copyObjItem->itemType();

                    if(group->localName().isEmpty())//顶层group
                    {
                        m_actAddVar->setVisible(false);
                        m_actCopy->setVisible(false);

                        //判断是否需要粘贴
                        if(nType == (int)MDTreeItemType::VariableGroup)
                            m_actPaste->setVisible(true);
                        else
                            m_actPaste->setVisible(false);

                        m_actDelete->setVisible(false);
                        m_actConfig->setVisible(false);
                    }
                    else
                    {
                         m_actAddVar->setVisible(true);
                         m_actCopy->setVisible(true);
                         if(nType != 9999)
                             m_actPaste->setVisible(true);
                         m_actDelete->setVisible(true);
                         m_actConfig->setVisible(true);
                    }

                    m_actAddVarGroup->setVisible(true);

                    m_actBatchChangeVarProp->setVisible(false);
                    m_actBatchAddVar->setVisible(false);
                    m_actCopy->setVisible(true);

                    m_actRun->setVisible(true);
                    m_actWriteVar->setVisible(false);

                    m_actRun->setIcon(QIcon(":/images/start.png"));
                    m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
                    m_actWriteVar->setVisible(false);

                }
                m_selectedModelIndex = index;
                 m_menu->exec(QCursor::pos());
            }
        }
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;

            IMDVariableGroup* group = (IMDVariableGroup*)rtdbVariable->parentItem(MDTreeItemType::VariableGroup);
            if(group->localName()  =="SystemVariableGroup")
            {
                m_actAddVarGroup->setVisible(false);
                m_actAddVar->setVisible(false);
                m_actConfig->setVisible(false);
                m_actCopy->setVisible(false);
                m_actPaste->setVisible(false);
                m_actDelete->setVisible(false);
                m_actRun->setVisible(true);
                m_actWriteVar->setVisible(false);
                m_actBatchChangeVarProp->setVisible(false);
                m_actBatchAddVar->setVisible(false);
                if(m_mdRTDBManager->isRunning())
                {
                   m_actRun->setIcon(QIcon(":/images/stop.png"));
                   m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
                }
                else
                {
                    m_actRun->setIcon(QIcon(":/images/start.png"));
                    m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
                }
                m_selectedModelIndex = index;
                m_menu->exec(QCursor::pos());
            }
            else//自定义变量
            {
                if(m_mdRTDBManager->isRunning())
                {
                    m_actAddVarGroup->setVisible(false);
                    m_actAddVar->setVisible(false);

                    m_actCopy->setVisible(false);
                    m_actPaste->setVisible(false);
                    m_actDelete->setVisible(false);

                    m_actRun->setVisible(true);
                    m_actWriteVar->setVisible(true);

                    m_actRun->setIcon(QIcon(":/images/stop.png"));
                    m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
                    m_actConfig->setVisible(true);
                    m_actBatchChangeVarProp->setVisible(false);
                    m_actBatchAddVar->setVisible(false);
                }
                else
                {
                    m_actAddVarGroup->setVisible(false);
                    m_actCopy->setVisible(true);

                    //检查copy对象是否为DataBlock
                    if(m_copyObjItem != NULL)
                    {
                        if(m_copyObjItem->itemType() == MDTreeItemType::Variable)
                            m_actPaste->setVisible(true);
                        else
                            m_actPaste->setVisible(false);
                    }
                    else
                    {
                        m_actPaste->setVisible(false);
                    }
                    m_actDelete->setVisible(true);

                    m_actRun->setVisible(true);
                    m_actWriteVar->setVisible(false);                    
                    m_actRun->setIcon(QIcon(":/images/start.png"));
                    m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));

                }
                m_selectedModelIndex = index;
                m_menu->exec(QCursor::pos());
            }
        }
    }
    else
    {
        if(m_mdRTDBManager->isRuntimeMode())
        {
            return;
        }
        if(m_mdRTDBManager->isRunning())
        {
            m_actAddVarGroup->setVisible(false);
            m_actAddVar->setVisible(false);
            m_actConfig->setVisible(false);
            m_actCopy->setVisible(false);
            m_actWriteVar->setVisible(false);
            m_actPaste->setVisible(false);
            m_actDelete->setVisible(false);
            m_actBatchChangeVarProp->setVisible(false);
            m_actBatchAddVar->setVisible(false);
            m_actRun->setVisible(true);
            m_actRun->setIcon(QIcon(":/images/stop.png"));
            m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
        }
        else
        {
            m_actAddVarGroup->setVisible(true);
            m_actAddVar->setVisible(false);
            m_actConfig->setVisible(false);
            m_actCopy->setVisible(false);
            m_actWriteVar->setVisible(false);
            m_actBatchChangeVarProp->setVisible(false);
            m_actBatchAddVar->setVisible(false);
            //判断是否是Group
            //检查copy对象是否为DataBlock
            if(m_copyObjItem != NULL)
            {
                if(m_copyObjItem->itemType() == MDTreeItemType::VariableGroup)
                    m_actPaste->setVisible(true);
                else
                    m_actPaste->setVisible(false);
            }
            else
            {
                m_actPaste->setVisible(false);
            }

            m_actDelete->setVisible(false);
            m_actRun->setVisible(true);
            m_actRun->setIcon(QIcon(":/images/start.png"));
            m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
        }
        m_selectedModelIndex = index;
        m_menu->exec(QCursor::pos());
    }*/
}
/*!
\section 右键菜单响应处理
*/
void CMDRTDBVariablesWidget::menuItemAddVarGroupClick()
{
    //if (m_selectedModelIndex.isValid())
    IMDTreeItem *item = NULL;
    if(!m_selectedModelIndex.isValid())
        item = m_curItem;
    else
        item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());

    if(item != NULL)
    {
        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            m_treeView->setUpdatesEnabled(false);
            //emit m_variableTreeModel->layoutAboutToBeChanged();
            CAddVarGroupDialog dlg(this);
            dlg.setCurrentVariableGroup((IMDVariableGroup*)item);
            dlg.exec();

            if(QDialog::Accepted == dlg.result())
            {
                //!刷新左边树节点
                if(m_rtdbUI != NULL)
                {
                    m_rtdbUI->refreshTreeNode(true,(qulonglong)m_curItem,(qulonglong)item);
                }
            }
            //emit m_variableTreeModel->layoutChanged();
            m_treeView->setUpdatesEnabled(true);
            emit this->signal_Modified();
        }
        //没有可能走到这里
        /*
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
            IMDVariableGroup* group = (IMDVariableGroup*)rtdbVariable->parentItem(MDTreeItemType::VariableGroup);
            CAddVarGroupDialog dlg(this);
            dlg.setCurrentVariableGroup(group);
            dlg.exec();
            if(QDialog::Accepted == dlg.result())
            {
                emit m_variableTreeModel->layoutChanged();
                //!刷新左边树节点
                if(m_rtdbUI != NULL)
                {
                    m_rtdbUI->refreshTreeNode(true,(qulonglong)m_curItem,(qulonglong)group);
                }
            }
        }*/
    }
    else
    {
        if(m_mdRTDBManager!=NULL)
        {
            m_treeView->setUpdatesEnabled(false);
            //emit m_variableTreeModel->layoutAboutToBeChanged();
            CAddVarGroupDialog dlg(this);
            dlg.setCurrentVariableGroup(m_mdRTDBManager->variableGroup());
            dlg.exec();
            if(QDialog::Accepted == dlg.result())
            {
                //!刷新左边树节点
                if(m_rtdbUI != NULL)
                {
                    IMDVariableGroup* parentGroup = (IMDVariableGroup*)m_mdRTDBManager->variableGroup();
                    m_rtdbUI->refreshTreeNode(true,(qulonglong)m_curItem,(qulonglong)parentGroup);
                }
            }
            //emit m_variableTreeModel->layoutChanged();
            m_treeView->setUpdatesEnabled(true);
            emit this->signal_Modified();
        }
    }    
}
void CMDRTDBVariablesWidget::menuItemAddVarClick()
{
    IMDTreeItem *item = NULL;
    if(!m_selectedModelIndex.isValid())
    {
        if(!m_groupName.isEmpty())
            item = m_curItem;
    }
    else
    {
        item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
    }
    if (item != NULL)
    {
        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            m_treeView->setUpdatesEnabled(false);
            emit m_variableTreeModel->layoutAboutToBeChanged();
            CVariablePropertyDialog dlg(this);
            dlg.setProjectConfiger(m_projectConfiger);
            dlg.setCurrentVariableGroup((IMDVariableGroup*)item);
            dlg.initial();
            dlg.exec();
        }
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            m_treeView->setUpdatesEnabled(false);
            emit m_variableTreeModel->layoutAboutToBeChanged();
            IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
            IMDVariableGroup* group = (IMDVariableGroup*)rtdbVariable->parentItem(MDTreeItemType::VariableGroup);
            CVariablePropertyDialog dlg(this);
            dlg.setProjectConfiger(m_projectConfiger);
            dlg.setCurrentVariableGroup(group);
            dlg.initial();
            dlg.exec();
        }
        emit m_variableTreeModel->layoutChanged();
        m_treeView->setUpdatesEnabled(true);
        emit this->signal_Modified();
    }
}

// todo: sync var to tres
void CMDRTDBVariablesWidget::menuItemSyncVarClick()
{
    IMDTreeItem *item = NULL;
    if(!m_selectedModelIndex.isValid())
    {
        if(!m_groupName.isEmpty())
            item = m_curItem;
    }
    else
    {
        item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
    }
    if (item != NULL)
    {
        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            emit m_variableTreeModel->layoutAboutToBeChanged();
            CVariablePropertyDialog dlg(this);
            dlg.setProjectConfiger(m_projectConfiger);
            dlg.setCurrentVariableGroup((IMDVariableGroup*)item);
            dlg.initial();
            dlg.exec();
        }
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            emit m_variableTreeModel->layoutAboutToBeChanged();
            IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
            IMDVariableGroup* group = (IMDVariableGroup*)rtdbVariable->parentItem(MDTreeItemType::VariableGroup);
            CVariablePropertyDialog dlg(this);
            dlg.setProjectConfiger(m_projectConfiger);
            dlg.setCurrentVariableGroup(group);
            dlg.initial();
            dlg.exec();
        }
        emit m_variableTreeModel->layoutChanged();
        emit this->signal_Modified();
    }
}

struct OPCUANodeUserData:QObjectUserData {
    ushort ns;
    uint nodeid;
    QString browsename;
    int nodeclass;
    uint typedefinition;
    uint pnodeid;
    uint datatype;
    QString propvalue;
    QString varfullpath;
};

MDDataType getDataTypeFromOPCUa(uint tp)
{
//#define UA_NS0ID_BOOLEAN 1 /* DataType */
//#define UA_NS0ID_SBYTE 2 /* DataType */
//#define UA_NS0ID_BYTE 3 /* DataType */
//#define UA_NS0ID_INT16 4 /* DataType */
//#define UA_NS0ID_UINT16 5 /* DataType */
//#define UA_NS0ID_INT32 6 /* DataType */
//#define UA_NS0ID_UINT32 7 /* DataType */
//#define UA_NS0ID_INT64 8 /* DataType */
//#define UA_NS0ID_UINT64 9 /* DataType */
//#define UA_NS0ID_FLOAT 10 /* DataType */
//#define UA_NS0ID_DOUBLE 11 /* DataType */
//#define UA_NS0ID_STRING 12 /* DataType */
//#define UA_NS0ID_DATETIME 13 /* DataType */
    if(tp>=1 && tp <=11)
        return (MDDataType)(tp-1);
    else if(tp == 12)
        return MDDataType::String;
    else if(tp == 13)
        return MDDataType::DateTime;

    return MDDataType::Null;
}

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/cfg/env.h"

static void load_spdlog_levels()
{
    // Set the log level to "info" and mylogger to "trace":
    // SPDLOG_LEVEL=info,mylogger=trace && ./example
    spdlog::cfg::load_env_levels();
    // or from command line:
    // ./example SPDLOG_LEVEL=info,mylogger=trace
    // #include "spdlog/cfg/argv.h" // for loading levels from argv
    // spdlog::cfg::load_argv_levels(args, argv);
}

static bool spdloginit = false;
// todo: sync var from opc ua server
void CMDRTDBVariablesWidget::menuItemSyncVarFromOpcUaServerClick()
{
    QMessageBox box;
    box.setText(tr("同步变量"));
    box.setWindowTitle(CMDMultiLanguage::instance()->value(tr("确认")));
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    int reply = box.exec();
    if(reply == QMessageBox::No)
        return;

    if(!spdloginit)
    {
        // Log levels can be loaded from argv/env using "SPDLOG_LEVEL"
        load_spdlog_levels();

        // Customize msg format for all loggers
        spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");

        // periodically flush all *registered* loggers every 3 seconds:
        // warning: only use if all your loggers are thread safe ("_mt" loggers)
        spdlog::flush_every(std::chrono::seconds(3));

        spdlog::info("Welcome to ua cpp wrapper!");

        spdlog::init_thread_pool(8192, 1);

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        //console_sink->set_level(spdlog::level::warn);
        console_sink->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");

        // Create a file rotating logger with 5mb size max and 3 rotated files
        auto max_size = 1048576 * 5;
        auto max_files = 3;
        //auto file_sink = spdlog::rotating_logger_mt<spdlog::async_factory>("uacpp_logger", "logs/uacpp.log", max_size, max_files);
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/uacpp_mindstudiosvc.log", max_size, max_files);
        //file_sink->set_level(spdlog::level::warn);
        file_sink->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");

        std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
        auto logger = std::make_shared<spdlog::async_logger>("uacpp_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        spdlog::register_logger(logger);

        logger->info("start refresh...");

        spdlog::get("uacpp_logger")->info("=================refresh ua variable from ua server=================");
        spdloginit = true;
    }

    QString prjKVPath = m_mdRTDBManager->projectPath() + QDir::separator() + "KV";
#ifdef _WIN32
    std::wstring std_rootDir = prjKVPath.toStdWString();
    MMKV::initializeMMKV(std_rootDir);
#elif defined(__linux__)
    std::string std_rootDir = prjKVPath.toStdString();
    MMKV::initializeMMKV(std_rootDir, MMKVLogInfo);
#endif

    std::string std_id = "__varid__";
    MMKV* m_mmkv_id = MMKV::mmkvWithID(std_id, MMKVMode::MMKV_MULTI_PROCESS);
    if(m_mmkv_id) {
        m_mmkv_id->clearAll();
        m_mmkv_id->set(false, "_enabled_");
    }

    std::string std_id_type = "__varidtype__";
    MMKV* m_mmkv_id_type = MMKV::mmkvWithID(std_id_type, MMKVMode::MMKV_MULTI_PROCESS);
    if(m_mmkv_id_type) {
        m_mmkv_id_type->clearAll();
        m_mmkv_id_type->set(false, "_enabled_");
    }

    m_treeView->collapseAll();
    m_treeView->update();
    m_treeView->setUpdatesEnabled(false);
    emit m_variableTreeModel->layoutAboutToBeChanged();

    QString error;
    const QString OPCUA_ROOT = "OPCUA__";
    const QString OPCUA_OBJECT_ROOT = "Objects";
    const QString VIRTUAL = "virtual";
    IMDVariableGroup* varRoot = m_mdRTDBManager->variableGroup();
    IMDVariableGroup* OPCUA_ROOTFolder = varRoot->getChildVariableGroup(OPCUA_ROOT);
    if(OPCUA_ROOTFolder){
        // 如果原先存在 OPCUA_ROOT 节点,则先清除
        bool ret = varRoot->removeChildVariableGroup(OPCUA_ROOT);
        if(ret){
            OPCUA_ROOTFolder = NULL;
        }
    }
    IMDVariableGroup* OPCUA_VIRTUALFolder = varRoot->getChildVariableGroup(VIRTUAL);
    if(OPCUA_VIRTUALFolder){
        // 如果原先存在 OPCUA_ROOT 节点,则先清除
        OPCUA_VIRTUALFolder->removeAllChildVariableGroup();
        OPCUA_VIRTUALFolder->removeAllVariable();
    }
    if(NULL == OPCUA_VIRTUALFolder){
        bool ret = varRoot->addChildVariableGroup(VIRTUAL,"",error);
        if(ret)
            OPCUA_VIRTUALFolder = varRoot->getChildVariableGroup(VIRTUAL);
    }
    if(NULL == OPCUA_ROOTFolder){
        bool ret = varRoot->addChildVariableGroup(OPCUA_ROOT,"",error);
        if(ret)
            OPCUA_ROOTFolder = varRoot->getChildVariableGroup(OPCUA_ROOT);
    }

    if(NULL == OPCUA_ROOTFolder)
        return;

    if(NULL == OPCUA_VIRTUALFolder)
        return;

    IMDVariableGroup* parentVarGroup = OPCUA_ROOTFolder;
    IMDVariableGroup* vparentVarGroup = OPCUA_VIRTUALFolder;
    IMDRTDBVariable* cur_rtdbVariable = NULL;
    IMDRTDBVariable* vcur_rtdbVariable = NULL;

    std::string address = "opc.tcp://192.167.100.188:4840";
    address = "opc.tcp://172.16.14.10:4840";
    OPCUaClient client;
    OPCUA::StatusCode ret = client.connect(address);
    if(ret != OPCUA::StatusCode::ALL_OK)
        return;
    std::string sProtocol;
    client.getObjectNodeChild(sProtocol, 1);
    //std::cout << sProtocol;
    QString spt(sProtocol.c_str());
    qDebug() << spt;
    QStringList sptlist = spt.split("\n");
    foreach(QString sline, sptlist)
    {
        QStringList slinelist = sline.split("|");
        // [0]-NAMESPACE
        // [1]-NODEID
        // [2]-BROWSE NAME
        // [3]-NODE CLASS
        // [4]-TYPE DEFINITION
        // [5]-Parent NODEID
        // [6]-DataTypeOrPropValue(if existed)|property node value(if existed)
        // [7]-nodeDataType(if existed) for ua writeVariable interface
        int len = slinelist.count();
        if(len >= 6 && len <= 8){
            OPCUANodeUserData* oud = new OPCUANodeUserData();
            oud->ns = slinelist[0].toUShort();
            oud->nodeid = slinelist[1].toUInt();
            oud->browsename = slinelist[2];
            oud->nodeclass = slinelist[3].toInt();
            oud->typedefinition = slinelist[4].toUInt();
            oud->pnodeid = slinelist[5].toUInt();

            // 文件夹
            if(oud->ns == 1){

                QObjectUserData* ud = parentVarGroup->userData(Qt::UserRole);
                if(ud){
                    OPCUANodeUserData* pd = (OPCUANodeUserData*)ud;
                    if(pd){
                        if(oud->pnodeid != pd->nodeid){
                            // 父节点切换
                            if(oud->pnodeid == 85){
                                parentVarGroup = OPCUA_ROOTFolder;
                                vparentVarGroup = OPCUA_VIRTUALFolder;
                            }
                            else{
                                parentVarGroup = OPCUA_ROOTFolder->getGroupById(oud->pnodeid);
                                vparentVarGroup = OPCUA_VIRTUALFolder->getGroupById(oud->pnodeid);
                            }
                        }
                    }
                }
                // add new group
                bool ret =  parentVarGroup->addChildVariableGroup(oud->browsename, "", error);
                if(ret){
                    parentVarGroup = parentVarGroup->getChildVariableGroup(oud->browsename);
                    parentVarGroup->setId(oud->nodeid);
                    parentVarGroup->setUserData(Qt::UserRole, oud);
                }

                bool ret1 =  vparentVarGroup->addChildVariableGroup(oud->browsename, "", error);
                if(ret1){
                    vparentVarGroup = vparentVarGroup->getChildVariableGroup(oud->browsename);
                    vparentVarGroup->setId(oud->nodeid);
                    vparentVarGroup->setUserData(Qt::UserRole, oud);
                }
            }

            else if(oud->ns == 2 && len == 8){
                // 变量值节点（其子节点为变量属性节点）
                oud->datatype = slinelist[6].toUInt();

                cur_rtdbVariable = m_mdRTDBManager->variableManager()->createVariable(parentVarGroup);
                cur_rtdbVariable->setLocalName(oud->browsename);
                cur_rtdbVariable->setDataType(getDataTypeFromOPCUa(oud->datatype));
                //cur_rtdbVariable->setUnit(m_generalDialog->m_unit);
                //cur_rtdbVariable->setDescription(m_generalDialog->m_description);
                //cur_rtdbVariable->setPrecision(m_generalDialog->m_precision);
                cur_rtdbVariable->setDeviceName("cproua");
                //cur_rtdbVariable->setAddress(parentVarGroup->name()+"."+oud->browsename);
                cur_rtdbVariable->setVariableType(MDVariableType::IOVariable);
                cur_rtdbVariable->setOriginalDataType(getDataTypeFromOPCUa(oud->datatype));
                //cur_rtdbVariable->setDataSaveRulerParameter(m_variableArchivesDialog->m_dataSaveRulerParameter);
                //cur_rtdbVariable->setWriteProtectParameter(m_writeProtectDialog->m_writeProtectParameter);
                //cur_rtdbVariable->setVariableAlarmParameter(m_variableAlarmDialog->m_variableAlarmParameter);
                //cur_rtdbVariable->setDataConversionParameter(m_dataConversionDialog->m_dataConversionParameter);
                CMDActionSPList* splist = cur_rtdbVariable->actionSPList();
                splist->append(MDActionType::variable_WriteValue );
                cur_rtdbVariable->setActionSPList(splist);
                //cur_rtdbVariable->setInitializeParameter(m_intializeDialog->m_initialParameter);
                parentVarGroup = OPCUA_ROOTFolder->getGroupById(oud->pnodeid);
                if(!parentVarGroup->addVariable(cur_rtdbVariable, error))
                {
                    delete cur_rtdbVariable;
                    cur_rtdbVariable = NULL;
                }

                QString varFullPath = cur_rtdbVariable->name();
                varFullPath = varFullPath.mid(OPCUA_ROOT.length()+1);
                varFullPath = varFullPath.replace('.', '/');
                //QMessageBox::information(0,"spt",varFullPath);
                std::string sk = varFullPath.toStdString();
                QString nsid = QString("%1|%2").arg(oud->ns).arg(oud->nodeid);
                std::string sv = nsid.toStdString();
                if (m_mmkv_id) m_mmkv_id->set(sv, sk);
                uint32_t tp = slinelist[7].toUInt();
                if(m_mmkv_id_type) m_mmkv_id_type->set(tp, sv);
                //std::cout << sv << "--" << tp << std::endl;
                //QMessageBox::information(0,"varidtype",QString("k=%1,v=%2").arg(nsid).arg(tp));

                vcur_rtdbVariable = m_mdRTDBManager->variableManager()->createVariable(vparentVarGroup);
                vcur_rtdbVariable->setLocalName(oud->browsename);
                vcur_rtdbVariable->setDataType(getDataTypeFromOPCUa(oud->datatype));
                vcur_rtdbVariable->setVariableType(MDVariableType::MiddleVariable);
                vcur_rtdbVariable->setOriginalDataType(getDataTypeFromOPCUa(oud->datatype));
                CMDActionSPList* vsplist = vcur_rtdbVariable->actionSPList();
                vsplist->append(MDActionType::variable_WriteValue);
                cur_rtdbVariable->setActionSPList(vsplist);
                vparentVarGroup = OPCUA_VIRTUALFolder->getGroupById(oud->pnodeid);
                if(!vparentVarGroup->addVariable(vcur_rtdbVariable, error))
                {
                    delete vcur_rtdbVariable;
                    vcur_rtdbVariable = NULL;
                }
            }

            // 变量属性节点
            else if(oud->ns == 3 && NULL != cur_rtdbVariable){
                if(len == 7)
                    oud->propvalue = slinelist[6];

                if(oud->browsename == "AsText"){

                }
                else if(oud->browsename == "Label"){

                }
                else if(oud->browsename == "Description"){
                    //
                    cur_rtdbVariable->setDescription(oud->propvalue);
                    vcur_rtdbVariable->setDescription(oud->propvalue);
                }
                else if(oud->browsename == "Prefix"){

                }
                else if(oud->browsename == "Units"){
                    //
                    cur_rtdbVariable->setUnit(oud->propvalue);
                    vcur_rtdbVariable->setUnit(oud->propvalue);
                }
                else if(oud->browsename == "SetPoint"){

                }
                else if(oud->browsename == "Minimum"){
                    //
                    MDVariableWriteProtectParameter pp = cur_rtdbVariable->writeProtectParameter();
                    if(cur_rtdbVariable->dataType() == MDDataType::Double ||
                           cur_rtdbVariable->dataType() == MDDataType::Float )
                    {
                        int it = (int)oud->propvalue.toDouble();
                        float f = *(float*)&it;
                        pp.m_minValue = QString("%1").arg(f);
                        qDebug() << cur_rtdbVariable->name() << " type: " << (int)cur_rtdbVariable->dataType() << "min:" << pp.m_minValue;
                    }
                    else
                        pp.m_minValue = oud->propvalue;
                    pp.m_enableWriteProtect = true;
                    cur_rtdbVariable->setWriteProtectParameter(pp);
                    vcur_rtdbVariable->setWriteProtectParameter(pp);
                }
                else if(oud->browsename == "Maximum"){
                    //
                    MDVariableWriteProtectParameter pp = cur_rtdbVariable->writeProtectParameter();
                    if(cur_rtdbVariable->dataType() == MDDataType::Double ||
                           cur_rtdbVariable->dataType() == MDDataType::Float )
                    {
                        int it = (int)oud->propvalue.toDouble();
                        float f = *(float*)&it;
                        pp.m_maxValue = QString("%1").arg(f);
                        qDebug() << cur_rtdbVariable->name() << " type: " << (int)cur_rtdbVariable->dataType() << "max:" << pp.m_maxValue;
                    }
                    else
                        pp.m_maxValue = oud->propvalue;

                    pp.m_enableWriteProtect = true;
                    cur_rtdbVariable->setWriteProtectParameter(pp);
                    vcur_rtdbVariable->setWriteProtectParameter(pp);
                }
                else if(oud->browsename == "ForeColor"){

                }
                else if(oud->browsename == "BackColor"){

                }
                else if(oud->browsename == "Name"){
                    // 变量全路径
                    oud->varfullpath = oud->propvalue;
                    cur_rtdbVariable->setAddress(OPCUA_OBJECT_ROOT+"."+oud->varfullpath);
                    vcur_rtdbVariable->setAddress(OPCUA_ROOT+"."+oud->varfullpath);
                }
                else if(oud->browsename == "Index"){

                }
                else if(oud->browsename == "Alarms"){

                }
                else if(oud->browsename == "TextOff"){

                }
                else if(oud->browsename == "TextOn"){

                }
                else if(oud->browsename == "StateCount"){

                }
                else if(oud->browsename == "Deadband"){

                }

            }
        }

    }

    emit m_variableTreeModel->layoutChanged();
    m_treeView->setUpdatesEnabled(true);
    emit this->signal_Modified();
    if(m_mmkv_id) m_mmkv_id->set(true, "_enabled_");
    m_mmkv_id->sync();
    m_mmkv_id->close();

    if(m_mmkv_id_type) m_mmkv_id_type->set(true, "_enabled_");
    m_mmkv_id_type->sync();
    m_mmkv_id_type->close();
    client.disconnect();

    spdlog::get("uacpp_logger")->info("end refresh!");
}

void CMDRTDBVariablesWidget::menuItemConfigClick()
{
    IMDTreeItem *item = NULL;
    if(!m_selectedModelIndex.isValid())
    {
        if(!m_groupName.isEmpty())
            item = m_curItem;
    }
    else
    {
        item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
    }
    if (item != NULL)
    {
        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            m_treeView->setUpdatesEnabled(false);
            //emit m_variableTreeModel->layoutAboutToBeChanged();
            CAddVarGroupDialog dlg(this);
            dlg.setConfigModel(true);
            dlg.setRunModel(m_mdRTDBManager->isRunning());
            dlg.setCurrentVariableGroup((IMDVariableGroup*)item);
            dlg.initial();
            dlg.exec();
            if(QDialog::Accepted == dlg.result())
            {
                if(m_rtdbUI)
                     m_rtdbUI->refreshTreeNode(true,(qulonglong)m_curItem,(qulonglong)item);
            }
            //emit m_variableTreeModel->layoutChanged();
            m_treeView->setUpdatesEnabled(true);
            emit this->signal_Modified();
        }
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
            if(rtdbVariable->variableType()!= MDVariableType::SystemVariable)
            {
                m_treeView->setUpdatesEnabled(false);
                //emit m_variableTreeModel->layoutAboutToBeChanged();
                CVariablePropertyDialog dlg(this);
                dlg.setProjectConfiger(m_projectConfiger);
                dlg.setCurrentVariable(rtdbVariable);
                dlg.setRunModel(m_mdRTDBManager->isRunning());
                IMDVariableGroup* group = (IMDVariableGroup*)item->parentItem(MDTreeItemType::VariableGroup);
                dlg.setCurrentVariableGroup(group);
                dlg.initial();
                dlg.exec();
//                emit m_variableTreeModel->layoutChanged();
                m_treeView->setUpdatesEnabled(true);
                emit this->signal_Modified();
            }
        }        
    }
}

void CMDRTDBVariablesWidget::menuItemRunClick()
{
    m_copyObjItem = NULL;
    m_rtdbUI->clearCopyItem();
    /*
    if(m_mdRTDBManager->isRunning())//即将停止
    {
        m_actRun->setIcon(QIcon(":/images/start.png"));
        m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
        m_actAddVarGroup->setVisible(true);
        //当前树为group或list中有选中项
        IMDTreeItem *item = NULL;
        if(!m_selectedModelIndex.isValid())
            item = m_curItem;
        else
            item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item != NULL)
            m_actAddVar->setVisible(true);
        else
            m_actAddVar->setVisible(false);
        //如果有单选的Variable，批量增加
        item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item != NULL && item->itemType() == MDTreeItemType::Variable)
        {
            m_actBatchAddVar->setVisible(true);
        }
        else
        {
            m_actBatchAddVar->setVisible(false);
        }
        //有选中项，看copy+delete
        if(item != NULL)
        {
            m_actCopy->setVisible(true);
            m_actDelete->setVisible(true);
        }
        else
        {
            m_actCopy->setVisible(false);
            m_actDelete->setVisible(false);
        }
        //有多于1个的Item，批量修改
        setBatchChangeItemVisible();
        m_actPaste->setVisible(false);
    }
    else//即将运行
    {
        m_actRun->setIcon(QIcon(":/images/stop.png"));
        m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
        m_actAddVarGroup->setVisible(false);
        m_actAddVar->setVisible(false);
        m_actBatchAddVar->setVisible(false);
        m_actBatchChangeVarProp->setVisible(false);
        m_actCopy->setVisible(false);
        m_actPaste->setVisible(false);
        m_actDelete->setVisible(false);
        //单个VariableItem时，写变量显示
        IMDTreeItem* item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item != NULL && item->itemType() == MDTreeItemType::Variable)
            m_actWriteVar->setVisible(true);
        else
            m_actWriteVar->setVisible(false);
        //单个Item时，属性显示
        QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
        if(selectedList.count() == 1)
            m_actConfig->setVisible(true);
        else
            m_actConfig->setVisible(false);

        m_copyModelIndex = QModelIndex();
        m_selectedModelIndex = QModelIndex();
    }*/
    refershActionStatus(!m_mdRTDBManager->isRunning(),false,0,false);
    emit this->signal_Run();
}
void CMDRTDBVariablesWidget::menuItemWriteVarClick()
{
    if (m_selectedModelIndex.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item->itemType() == MDTreeItemType::Variable)
        {
            IMDRTDBVariable * var = (IMDRTDBVariable*)item;
            if(var !=NULL)
            {
                //CVariableWriteValueDialog dlg(this);
               // dlg.setVariable(var);
               // dlg.setModal(false);
               // dlg.show();
               // dlg.setWindowFlags(Qt::WindowStaysOnTopHint);
                if(m_variableWriteValueDialog == NULL)
                {
                    m_variableWriteValueDialog = new CVariableWriteValueDialog();
                    m_variableWriteValueDialog->setModal(false);
                    m_variableWriteValueDialog->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint );
                }
                if(m_variableWriteValueDialog)
                {
                    m_variableWriteValueDialog->setVariable(var);
                    m_variableWriteValueDialog->show();
                    m_variableWriteValueDialog->move ((QApplication::desktop()->width() - m_variableWriteValueDialog->width())/2,(QApplication::desktop()->height() - m_variableWriteValueDialog->height())/2);
                }
            }
        }
    }
}

void CMDRTDBVariablesWidget::menuItemCopyClick()
{
    if(m_selectedModelIndex.isValid())
    {
        m_rtdbUI->clearCopyItem();
        m_copyObjItem = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());

        refershActionStatus(m_mdRTDBManager->isRunning(),false,0,true);
    }
    else
    {
        m_rtdbUI->menuItemCopyClick();
    }
}
void CMDRTDBVariablesWidget::menuItemPasteClick()
{
    if(m_copyObjItem == NULL)
    {
        m_rtdbUI->menuItemPasteClick();
        return;
    }

    bool isRootGroup = false;
    IMDVariableGroup* parentVarGroup = NULL;
    //!检查当前是否有选中项  Y2hw
    //!避免m_treeView失去焦点时，有时copy组到根目录时，copy到当前组
    QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
    if(selectedList.size() ==0)
    {
        m_selectedModelIndex = QModelIndex();
    }
    if (m_selectedModelIndex.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            parentVarGroup = (IMDVariableGroup*)item;
        }
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            IMDRTDBVariable * var = (IMDRTDBVariable*)item;
            parentVarGroup = (IMDVariableGroup*)var->parentItem(MDTreeItemType::VariableGroup);
        }
    }
    else
    {
        if(m_mdRTDBManager!=NULL)
        {
            if(m_curItem == NULL)
            {
                parentVarGroup = m_mdRTDBManager->variableGroup();
                isRootGroup = true;
            }
            else
            {
                parentVarGroup = (IMDVariableGroup*)m_curItem;
            }
        }
    }

    if (parentVarGroup!=NULL && m_copyObjItem != NULL)
    {
        if(m_copyObjItem->itemType() == MDTreeItemType::VariableGroup)
        {
            IMDVariableGroup* varGroup = (IMDVariableGroup*)m_copyObjItem;
            m_treeView->setUpdatesEnabled(false);
            emit m_variableTreeModel->layoutAboutToBeChanged();
            parentVarGroup->copyPasteVariableGroup(varGroup);
            emit m_variableTreeModel->layoutChanged();
            m_treeView->setUpdatesEnabled(true);
            emit this->signal_Modified();
            if(m_rtdbUI)
                m_rtdbUI->refreshTreeNode(true,(qulonglong)m_curItem,(qulonglong)parentVarGroup);
        }
        else if(m_copyObjItem->itemType() == MDTreeItemType::Variable && !isRootGroup)
        {
            IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)m_copyObjItem;
            m_treeView->setUpdatesEnabled(false);
            emit m_variableTreeModel->layoutAboutToBeChanged();
            parentVarGroup->copyPasteVariable(rtdbVariable);
            emit m_variableTreeModel->layoutChanged();
            m_treeView->setUpdatesEnabled(true);
            emit this->signal_Modified();
        }
    }
}
void CMDRTDBVariablesWidget::menuItemDeleteClick()
{
    //如果是左边树+toolbar
    if(!m_selectedModelIndex.isValid())
    {
        m_rtdbUI->menuItemDeleteClick();
        return;
    }

    //如果删除了Group，需刷新左边的树
    bool bDelGroup = false;
    QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
    QModelIndexList selectedMapList;
    //批量转换选中的index
    for(int i = 0; i<selectedList.count(); i++)
    {
        selectedMapList.append(m_varProxyModel->mapToSource(selectedList[i]));
    }

    if(selectedMapList.count() > 0)
    {
        QString str;
        QModelIndex itemIndex;
        if(selectedMapList.count()  == 1)
        {
            itemIndex = selectedMapList[0];
            IMDTreeItem* item = static_cast<IMDTreeItem*>(itemIndex.internalPointer());
            QString str0,strTranslate;
            if(item->itemType() == MDTreeItemType::VariableGroup)
            {
                IMDVariableGroup* varGroup = (IMDVariableGroup*)item;
                str0 = tr("确定要删除变量组");
                strTranslate = CMDMultiLanguage::instance()->value(str0);
                str = QString("%1：%2 ?").arg(strTranslate).arg(varGroup->localName());
            }
            else
            {
                IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
                str0 = tr("确定要删除变量");
                strTranslate = CMDMultiLanguage::instance()->value(str0);
                str = QString("%1：%2 ?").arg(strTranslate).arg(rtdbVariable->localName());
            }
        }
        else
        {
            str = tr("确定要批量删除?");
        }
        QMessageBox box;
        box.setText(str);
        box.setWindowTitle(CMDMultiLanguage::instance()->value(tr("确认")));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        int reply = box.exec();
        if(reply == QMessageBox::Yes)
        {
            bool bdelCopyItem = false;
            QModelIndex itemIndex0;
            IMDTreeItem* item = NULL;
            m_treeView->setUpdatesEnabled(false);
            emit m_variableTreeModel->layoutAboutToBeChanged();
            while(selectedMapList.count() > 0)
            {
                itemIndex0 = selectedMapList[selectedMapList.count() - 1];
                item = static_cast<IMDTreeItem*>(itemIndex.internalPointer());
                if(m_copyObjItem == item)
                {
                    m_copyObjItem = NULL;
                    bdelCopyItem = true;
                }

                //检查是否删除了Group
                if(!bDelGroup)
                {
                    if(item != NULL && item->itemType() == MDTreeItemType::VariableGroup)
                        bDelGroup = true;
                }

                m_variableTreeModel->deleteTreeItem(itemIndex0);

                selectedMapList.removeLast();
//                selectedList = m_treeView->selectionModel()->selectedRows();
//                //批量转换选中的index
//                selectedMapList.clear();
//                for(int i = 0; i<selectedList.count(); i++)
//                {
//                    selectedMapList.append(m_varProxyModel->mapToSource(selectedList[i]));
//                }
            }
            emit m_variableTreeModel->layoutChanged();
            m_treeView->setUpdatesEnabled(true);
            emit this->signal_Modified();
            //刷新左边树
            if(m_rtdbUI != NULL)
            {
                if(bDelGroup)
                {
                    m_rtdbUI->refreshTreeNode(true,(qulonglong)m_curItem,(qulonglong)m_curItem);
                }
                else
                {/*
                    if(m_groupName.isEmpty())
                        m_rtdbUI->setWindowTitle(true,tr("变量"));
                    else
                        m_rtdbUI->setWindowTitle(true,m_groupName);*/
                }
            }
            m_selectedModelIndex = QModelIndex();
            if(bdelCopyItem)
            {
                m_actPaste->setVisible(false);
                toolBar()->update();
            }
        }
    }//end if(selectedList.count() > 0)
}
void CMDRTDBVariablesWidget::menuItemRenameClick()
{
    /*/
    QTreeWidgetItem* node = selectNode();
    m_operateType = NodeOperate::Rename;
    m_oldFullName = getFilePath(node);
    setNodeEdit(node);
   /*/
}

//!批量修改变量属性
//! 如果是IO变量，设备名无效时不改变原来的属性
void CMDRTDBVariablesWidget::menuItemBatchChangeVarPropClick()
{
    QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
    QModelIndexList selectedMapList;
    //批量转换选中的index
    IMDTreeItem *item = NULL;
    IMDRTDBVariable* rtdbVariable = NULL;
    IMDVariableGroup* group = NULL;
    int ntemp0,ntemp1;
    CMDRTDBBatchChangeVariable *batchChangeVariable = new CMDRTDBBatchChangeVariable();
    //属性比较标志，默认为1都要比较，等于0即：已经有不相等的，无需再比较
    QByteArray propIndex(23,1);

    for(int i = 0; i<selectedList.count(); i++)
    {
        selectedMapList.append(m_varProxyModel->mapToSource(selectedList[i]));
        item = static_cast<IMDTreeItem*>(selectedMapList[i].internalPointer());
        //不是Variable go
        if(item->itemType() != MDTreeItemType::Variable)
            continue;

        rtdbVariable =(IMDRTDBVariable *)item;
        group = (IMDVariableGroup*)rtdbVariable->parentItem(MDTreeItemType::VariableGroup);
        if(rtdbVariable == NULL || group == NULL)
            continue;

        if(i == 0)//所选变量如属性相同，在批量对话框显示
        {
            //通用
            batchChangeVariable->m_groupName = group->name();
            batchChangeVariable->m_variableType = (CMDRTDBBatchChangeVariable::MDVariableTypeProperty)((int)rtdbVariable->variableType()+1);
            batchChangeVariable->m_dataType = (CMDRTDBBatchChangeVariable::MDDataTypeProperty)((int)rtdbVariable->dataType()+1);
            batchChangeVariable->m_precision = rtdbVariable->precision();

            //driver
            batchChangeVariable->m_deviceName = rtdbVariable->deviceName();
            batchChangeVariable->m_dataType = (CMDRTDBBatchChangeVariable::MDDataTypeProperty)((int)rtdbVariable->originalDataType()+1);

            //Init
            batchChangeVariable->m_initializeParameter.m_initType = rtdbVariable->initializeParameter().m_initType;
            batchChangeVariable->m_initializeParameter.m_value = rtdbVariable->initializeParameter().m_value;

            //convert
            batchChangeVariable->m_dataConversionParameter = rtdbVariable->dataConversionParameter();

            //write
            batchChangeVariable->m_writeProtectParameter = rtdbVariable->writeProtectParameter();

            //archive
            batchChangeVariable->m_dataSaveRulerParameter = rtdbVariable->dataSaveRulerParameter();
        }//end i == 0
        //比较，不=时，批量变量属性归0
        //通用
        if(propIndex[0] == char(1))
        {
            if(batchChangeVariable->groupName().compare(group->name()) != 0)
            {
                batchChangeVariable->setGroupName("None");
                propIndex[0] = 0;
            }
        }
        if(propIndex[1] == char(1))
        {
            ntemp0 = (int)batchChangeVariable->m_variableType;
            ntemp1 = (int)rtdbVariable->variableType()+1;
            if(ntemp0 != ntemp1)
            {
                batchChangeVariable->setVariableTypeProp(CMDRTDBBatchChangeVariable::MDVariableTypeProperty::None);
                propIndex[1] = 0;
            }
        }
        if(propIndex[2] == char(1))
        {
            ntemp0 = (int)batchChangeVariable->m_dataType;
            ntemp1 = (int)rtdbVariable->dataType()+1;
            if(ntemp0 != ntemp1)
            {
                batchChangeVariable->setDataTypeProp(CMDRTDBBatchChangeVariable::MDDataTypeProperty::None);
                propIndex[2] = 0;
            }
        }
        if(propIndex[3] == char(1))
        {
            if(batchChangeVariable->precision() != rtdbVariable->precision())
            {
                batchChangeVariable->setPrecision(-1);
                propIndex[3] = 0;
            }
        }

        //driver
        if(propIndex[4] == char(1))
        {
            if(batchChangeVariable->m_deviceName.compare(rtdbVariable->deviceName()) != 0)
            {
                batchChangeVariable->setGroupName("None");
                propIndex[4] = 0;
            }
        }
        if(propIndex[5] == char(1))
        {
            ntemp0 = (int)batchChangeVariable->m_originalDataType;
            ntemp1 = (int)rtdbVariable->originalDataType()+1;
            if(ntemp0 != ntemp1)
            {
                batchChangeVariable->setOriginalDataTypeProp(CMDRTDBBatchChangeVariable::MDDataTypeProperty::None);
                propIndex[5] = 0;
            }
        }


        //Init
        if(propIndex[6] == char(1))
        {
            ntemp0 = (int)batchChangeVariable->m_initializeParameter.m_initType;
            ntemp1 = (int)rtdbVariable->initializeParameter().m_initType;
            if(ntemp0 != ntemp1)
            {
                batchChangeVariable->setInitType(CMDRTDBBatchChangeVariable::MDRTInitModeProperty::None);
                propIndex[6] = 0;
            }
        }
        if(propIndex[7] == char(1))
        {
            if(batchChangeVariable->initValue().compare(rtdbVariable->initializeParameter().m_value) != 0)
            {
                batchChangeVariable->setInitValue("");
                propIndex[7] = 0;
            }
        }

        //convert
        if(propIndex[8] == char(1))
        {
            if(batchChangeVariable->m_dataConversionParameter.m_enableLinearConversion != rtdbVariable->dataConversionParameter().m_enableLinearConversion)
            {
                batchChangeVariable->setLinearConvertion(false);
                propIndex[8] = 0;
            }
        }
        if(propIndex[9] == char(1))
        {
            ntemp0 = (int)batchChangeVariable->m_dataConversionParameter.m_conversionType;
            ntemp1 = (int)rtdbVariable->dataConversionParameter().m_conversionType;
            if(ntemp0 != ntemp1)
            {
                batchChangeVariable->setConvertionType(CMDRTDBBatchChangeVariable::MDVariableDataConversionTypeProperty::None);
                propIndex[9] = 0;
            }
        }
        if(propIndex[10] == char(1))
        {
            if(batchChangeVariable->maxValue() != rtdbVariable->dataConversionParameter().m_maxValue)
            {
                batchChangeVariable->setMaxValue(0);
                propIndex[10] = 0;
            }
        }
        if(propIndex[11] == char(1))
        {
            if(batchChangeVariable->minValue() != rtdbVariable->dataConversionParameter().m_minValue)
            {
                batchChangeVariable->setMinValue(0);
                propIndex[11] = 0;
            }
        }
        if(propIndex[12] == char(1))
        {
            if(batchChangeVariable->maxDriverValue() != rtdbVariable->dataConversionParameter().m_originalMaxValue)
            {
                batchChangeVariable->setMaxDriverValue(0);
                propIndex[12] = 0;
            }
        }
        if(propIndex[13] == char(1))
        {
            if(batchChangeVariable->minDriverValue() != rtdbVariable->dataConversionParameter().m_originalMinValue)
            {
                batchChangeVariable->setMinDriverValue(0);
                propIndex[13] = 0;
            }
        }
        if(propIndex[14] == char(1))
        {
            if(batchChangeVariable->byteOrder().compare(rtdbVariable->dataConversionParameter().m_byteOrder) != 0)
            {
                batchChangeVariable->setByteOrder("");
                propIndex[14] = 0;
            }
        }

        //write
        if(propIndex[15] == char(1))
        {
            if(batchChangeVariable->m_writeProtectParameter.m_enableWrite != rtdbVariable->writeProtectParameter().m_enableWrite)
            {
                batchChangeVariable->setEnableWrite(false);
                propIndex[15] = 0;
            }
        }
        if(propIndex[16] == char(1))
        {
            if(batchChangeVariable->m_writeProtectParameter.m_writeProtectType != rtdbVariable->writeProtectParameter().m_writeProtectType)
            {
                batchChangeVariable->setWriteProtectType(0);
                propIndex[16] = 0;
            }
        }
        if(propIndex[17] == char(1))
        {
            if(batchChangeVariable->maxWriteValue() != rtdbVariable->writeProtectParameter().m_maxValue)
            {
                batchChangeVariable->setWriteMaxValue(0);
                propIndex[17] = 0;
            }
        }
        if(propIndex[18] == char(1))
        {
            if(batchChangeVariable->minWriteValue() != rtdbVariable->writeProtectParameter().m_minValue)
            {
                batchChangeVariable->setWriteMinValue(0);
                propIndex[18] = 0;
            }
        }

        //archive
        if(propIndex[19] == char(1))
        {
            ntemp0 = (int)batchChangeVariable->dataSaveRulerProp();
            ntemp1 = (int)rtdbVariable->dataSaveRulerParameter().m_saveRuler;
            if(ntemp0 != ntemp1)
            {
                batchChangeVariable->setDataSaveRulerProp(CMDRTDBBatchChangeVariable::MDRTDataSaveRulerProperty::None);
                propIndex[19] = 0;
            }
        }
        if(propIndex[20] == char(1))
        {
            ntemp0 = (int)batchChangeVariable->dataSaveModeProp();
            ntemp1 = (int)rtdbVariable->dataSaveRulerParameter().m_saveMode+1;
            if(ntemp0 != ntemp1)
            {
                batchChangeVariable->setDataSaveModeProp(CMDRTDBBatchChangeVariable::MDRTDataSaveModeProperty::None);
                propIndex[20] = 0;
            }
        }
        if(propIndex[21] == char(1))
        {
            if(batchChangeVariable->saveInterval() != rtdbVariable->dataSaveRulerParameter().m_saveCycle)
            {
                batchChangeVariable->setSaveInterval(1);
                propIndex[21] = 0;
            }
        }
        if(propIndex[22] == char(1))
        {
            if(batchChangeVariable->memorySavePeriod() != rtdbVariable->dataSaveRulerParameter().m_timeRange)
            {
                batchChangeVariable->setMemorySavePeriod(360);
                propIndex[22] = 0;
            }
        }
    }
    if(selectedMapList.count() > 0)
    {
        CMDVariablesBatchChangeDialog dlg;
        batchChangeVariable->setRTDBManager(m_mdRTDBManager);
        batchChangeVariable->setPropertyBrowser(dlg.getPropertyBrowse());

        batchChangeVariable->setParentWidget(this);
        dlg.getPropertyBrowse()->setExpandAll();
        dlg.exec();

        if(QDialog::Accepted == dlg.result())
        {
            IDevice*          batchDevice = NULL;
            IMDVariableGroup* batchGroup = NULL;
            qulonglong temp0,temp1;
            QString strErr;            
            int nIndex = 0;

            //batchGroup
            if(batchChangeVariable->groupName() != "None")
                batchGroup = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(batchChangeVariable->groupName());

            //batchDevice
            if(batchChangeVariable->deviceName() != "None")
                batchDevice = m_mdRTDBManager->driverManager()->getDevice(batchChangeVariable->deviceName());
            temp0 = (qulonglong)batchGroup;

            for(int i = 0; i<selectedMapList.count(); i++)
            {
                item = static_cast<IMDTreeItem*>(selectedMapList[i].internalPointer());
                if(item->itemType() != MDTreeItemType::Variable)
                    continue;

                rtdbVariable =(IMDRTDBVariable *)item;
                group = (IMDVariableGroup*)rtdbVariable->parentItem(MDTreeItemType::VariableGroup);
                if(rtdbVariable != NULL && group != NULL)
                {
                    //!通用:groupname,variabletype,projecttype,precision
                    //变量类型
                    nIndex = (int)batchChangeVariable->variableTypeProp();
                    //变量名
                    if(nIndex > 0)
                    {
                       rtdbVariable->setVariableType((MDVariableType)(nIndex-1));
                       if(rtdbVariable->variableType() == MDVariableType::IOVariable && batchDevice != NULL)
                       {
                            rtdbVariable->setDeviceName(batchChangeVariable->deviceName());
                       }
                    }
                    //变量组
                    temp1 = (qulonglong)group;
                    if(batchGroup != NULL && temp1 != temp0)
                    {
                        group->removeVariable(rtdbVariable);
                        batchGroup->addVariable(rtdbVariable,strErr);
                    }
                    //数据类型
                    nIndex = (int)batchChangeVariable->dataTypeProp();
                    if(nIndex > 0)
                       rtdbVariable->setDataType((MDDataType)(nIndex-1));
                    //精度
                    if(batchChangeVariable->precision() != -1)
                        rtdbVariable->setPrecision(batchChangeVariable->precision());
                    //!驱动信息
                    //原始类型
                    nIndex = (int)batchChangeVariable->originalDataTypeProp();
                    if(nIndex > 0)
                        rtdbVariable->setOriginalDataType((MDDataType)(nIndex-1));

                    //!初始化
                    if(batchChangeVariable->enableChangeInit())
                        rtdbVariable->setInitializeParameter(batchChangeVariable->getInitPara());

                    //!工程转换
                    if(batchChangeVariable->enableChangeConvert())
                        rtdbVariable->setDataConversionParameter(batchChangeVariable->getDataConvertPara());

                    //!工程转换
                    if(batchChangeVariable->enableChangeConvert())
                        rtdbVariable->setDataConversionParameter(batchChangeVariable->getDataConvertPara());

                    //!写保护
                    if(batchChangeVariable->enableChangeWrite())
                        rtdbVariable->setWriteProtectParameter(batchChangeVariable->getWriteProtectPara());

                    //!归档
                    if(batchChangeVariable->enableChangeArchive())
                        rtdbVariable->setDataSaveRulerParameter(batchChangeVariable->getArchivePara());

                    //!安全策略
                    if(batchChangeVariable->enableChangeAction())
                    {
                        CMDActionSPList asp = batchChangeVariable->getActionPara();
                        rtdbVariable->setActionSPList(&asp);
                    }
                }
            }//end for
            delete batchChangeVariable;
            emit m_variableTreeModel->layoutChanged();
            emit this->signal_Modified();
        }
    }
}
//!批量添加变量
void CMDRTDBVariablesWidget::menuItemBatchAddVarClick()
{
    QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
    QModelIndexList selectedMapList;
    //批量转换选中的index
    for(int i = 0; i<selectedList.count(); i++)
    {
        selectedMapList.append(m_varProxyModel->mapToSource(selectedList[i]));
    }
    if(selectedMapList.count() == 1)
    {
        IMDTreeItem* item = static_cast<IMDTreeItem*>(selectedMapList[0].internalPointer());
        IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
        if(rtdbVariable == NULL)
            return;
        m_batchVar = rtdbVariable;
       if(m_batchVar == NULL)
          return;

        QJsonArray props;
        //名字
        QJsonObject jsname = buildPropJSON(tr("Name"),tr("名字"),rtdbVariable->name());
        props.append(jsname);

        //描述
        QJsonObject jsdesc =buildPropJSON(tr("Description"),tr("描述"),rtdbVariable->getDescription());
        props.append(jsdesc);
        //device
        QJsonObject jsdev;
        if(rtdbVariable->variableType() == MDVariableType::IOVariable)
            jsdev =buildPropJSON(tr("DeviceName"),tr("设备名"),rtdbVariable->deviceName());
        else
            jsdev =buildPropJSON(tr("DeviceName"),tr("设备名"),"");
        props.append(jsdev);

        //address
        QJsonObject jsaddr =buildPropJSON(tr("Address"),tr("地址"),rtdbVariable->address());
        props.append(jsaddr);

        QJsonObject js;
        js.insert("BatchAdd",props);

        if(m_batchOp == NULL)
        {
            m_batchOp = new CMDObjBatchOperationExpert();
            connect(m_batchOp, &CMDObjBatchOperationExpert::newJsonCreate, this, &CMDRTDBVariablesWidget::onBatchAdd);
        }
        m_batchOp->setRefJson(js);        
    }
}

QJsonObject CMDRTDBVariablesWidget::buildPropJSON(const QString& propID, const QString& propComment,const QString& refValue)
{
    QJsonObject jsProp;

    jsProp.insert("PropID", propID);
    jsProp.insert("PropComment", propComment);
    jsProp.insert("RefValue", refValue);

    /*
    QString strtemp,strFormat;
    //formatstring
    int nIndex = refValue.indexOf(QRegExp("[0123456789]"));
    int nNextChIndex;
    if(nIndex == -1)
    {
        strFormat = refValue+"(*)";
    }
    else
    {
        nNextChIndex = refValue.indexOf(QRegExp("[a-z,A-Z]"),nIndex+1);
        strFormat = refValue.left(nIndex);
        strFormat += "(*)";
        if(nNextChIndex != -1)
            strFormat += refValue.mid(nNextChIndex);
    }
    jsProp.insert("FormatValue", strFormat);

    if(nIndex == -1)
    {
        jsProp.insert("Start", "0");
    }
    else
    {
        strtemp = QString::number(nIndex);
        jsProp.insert("Start", strtemp);
    }
    */
    jsProp.insert("FormatValue", refValue);
    jsProp.insert("Start", 0);
    jsProp.insert("Step", 1);
    jsProp.insert("Base", 10);
    return jsProp;
}

void CMDRTDBVariablesWidget::onBatchAdd(const QJsonObject &json)
{
    if(m_batchOp == NULL || m_batchVar == NULL)
        return;

    QJsonArray ja = json.value("BatchAdd").toArray();
    QString strName,strDesc,strDevName,strAddr;
    foreach (const QJsonValue& v , ja)
    {
        if("Name" == v.toObject().value("PropID").toString())
        {
            strName = v.toObject().value("Value").toString();
        }
        else if("Description" == v.toObject().value("PropID").toString())
        {
            strDesc = v.toObject().value("Value").toString();
        }
        if("DeviceName" == v.toObject().value("PropID").toString())
        {
            strDevName = v.toObject().value("Value").toString();
        }
        else if("Address" == v.toObject().value("PropID").toString())
        {
            strAddr = v.toObject().value("Value").toString();
        }
    }

    //校验是否设置正确
    //变量名重复
    IMDVariableGroup* pGroup = NULL;
    QString strError,strlocalName;
    int nIndex = strName.lastIndexOf('.');
    if(nIndex != -1)
    {
        pGroup = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(strName.left(nIndex));
        if(pGroup == NULL)
        {
            strError = tr("添加失败：") + strName;
            strError += tr(":变量组不存在");
            m_batchOp->setErrorInfor(strError);
            return;
        }
        strlocalName = strName.mid(nIndex + 1);
        IMDRTDBVariable* var = pGroup->getVariable(strlocalName);
        if(var != NULL)
        {
            strError = tr("添加失败：") + strName;
            strError += tr(":变量名重复");
            m_batchOp->setErrorInfor(strError);
            return;
        }
    }

    //设备存在
    IDevice* pdev = NULL;
    if(m_batchVar->variableType() == MDVariableType::IOVariable)
    {
        pdev = m_mdRTDBManager->driverManager()->getDevice(strDevName);
        if(pdev == NULL)
        {
            strError = tr("添加失败：") + strName;
            strError += tr(":设备不存在");
            m_batchOp->setErrorInfor(strError);
            return;
        }
    }
    //地址正确
    IMDRTDBVariable* newvar = m_mdRTDBManager->variableManager()->createVariable();
    newvar->copy(m_batchVar);
    newvar->setLocalName(strlocalName);
    newvar->setDeviceName(strDevName);
    newvar->setAddress(strAddr);
    newvar->setDescription(strDesc);
    QString strInernalErr;
    bool bCheckOK = false;    

    if(m_batchVar->variableType() == MDVariableType::IOVariable)
    {
        bCheckOK = pdev->checkVarAddress(newvar,strInernalErr);
    }
    else
    {
        bCheckOK = true;
    }
    if(!bCheckOK)
    {
        delete newvar;
        strError = tr("添加失败：") + strName;
        strError += strInernalErr;
        m_batchOp->setErrorInfor(strError);
        return;
    }
    else
    {
        m_treeView->setUpdatesEnabled(false);
        //emit m_variableTreeModel->layoutAboutToBeChanged();
        if(!pGroup->addVariable(newvar,strInernalErr))
        {
            delete newvar;
            strError = tr("添加失败：") + strName;
            m_batchOp->setErrorInfor(strError);
            //emit m_variableTreeModel->layoutChanged();
            m_treeView->setUpdatesEnabled(true);
            return;
        }
        //emit m_variableTreeModel->layoutChanged();
        m_treeView->setUpdatesEnabled(true);
        emit this->signal_Modified();
    }
}

void CMDRTDBVariablesWidget::refreshItemProp(qulonglong leftTreeNode)
{
    //emit m_varProxyModel->m_variableTreeModel();

    emit m_varProxyModel->dataChanged(QModelIndex(), QModelIndex());
    //!刷新左边树节点

    if(m_rtdbUI != NULL && leftTreeNode != 0)
        m_rtdbUI->refreshTreeNode(true,(qulonglong)m_curItem,leftTreeNode);
}

void CMDRTDBVariablesWidget::refershActionStatus(bool bRunning, bool bLeftTree,int nodeType,bool hasPasteItem)
{
    //替换run文本及图标
    if(bRunning)
    {
       m_actRun->setIcon(QIcon(":/images/stop.png"));
       m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
    }
    else
    {
        m_actRun->setIcon(QIcon(":/images/start.png"));
        m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
    }

    if(bLeftTree && nodeType == 999)
    {
        m_actAddVarGroup->setEnabled(false);
        m_actAddVar->setEnabled(false);
        m_actCopy->setEnabled(false);
        m_actPaste->setEnabled(false);
        m_actDelete->setEnabled(false);
        m_actRun->setEnabled(!m_mdRTDBManager->isRuntimeMode());
        m_actWriteVar->setEnabled(false);
        m_actBatchChangeVarProp->setEnabled(false);
        m_actBatchAddVar->setEnabled(false);
        m_actConfig->setEnabled(false);
        m_toolBar->update();
        return;
    }
    bool bSelGroup = false;//单个变量组
    bool bSelVar = false;//单选变量
    bool bMultiSelVar = false;//多变量选中
    bool bMixSel = false;//混合选中
    bool bMultiSelGroup = false;//多变量组选中
    QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
    //批量转换选中的index
    IMDTreeItem *item = NULL;
    int nVariableItemCount = 0;
    int nGroupCount = 0;
    if(selectedList.count() == 1)
    {
        item = static_cast<IMDTreeItem*>((m_varProxyModel->mapToSource(selectedList[0])).internalPointer());
        if(item->itemType() == MDTreeItemType::Variable)
            bSelVar = true;
        else
            bSelGroup = true;
    }
    else
    {
        for(int i = 0; i<selectedList.count(); i++)
        {
            item = static_cast<IMDTreeItem*>((m_varProxyModel->mapToSource(selectedList[i])).internalPointer());
            //检查是否是混合选择
            if(item->itemType() == MDTreeItemType::Variable)
            {
                if(nGroupCount > 0)
                {
                    bMixSel = true;
                    break;
                }
                nVariableItemCount++;
            }
            else if(item->itemType() == MDTreeItemType::VariableGroup)
            {
                if(nVariableItemCount > 0)
                {
                    bMixSel = true;
                    break;
                }
                nGroupCount++;
            }
        }
        //检查多选Var或Group
        if(nGroupCount > 1 && nVariableItemCount == 0)
            bMultiSelGroup = true;
        else if(nVariableItemCount > 1 && nGroupCount == 0)
            bMultiSelVar = true;
    }

    if(m_mdRTDBManager->isRuntimeMode())
    {
        m_actAddVarGroup->setEnabled(false);
        m_actAddVar->setEnabled(false);
        m_actCopy->setEnabled(false);
        m_actPaste->setEnabled(false);
        m_actDelete->setEnabled(false);
        m_actRun->setEnabled(false);
		if (item == NULL)
			return;
        //如果是变量，支持写变量
        if(item->itemType() == MDTreeItemType::Variable)
        {
            IMDRTDBVariable * var = (IMDRTDBVariable*)item;
            m_actWriteVar->setEnabled(bSelVar && var->writeProtectParameter().m_enableWrite);
        }
        else
        {
            m_actWriteVar->setEnabled(false);
        }

        m_actBatchChangeVarProp->setEnabled(false);
        m_actBatchAddVar->setEnabled(false);
        m_actConfig->setEnabled(true);
        m_toolBar->update();
        return;
    }
    if(bRunning)
    {
        m_actAddVarGroup->setEnabled(false);
        m_actAddVar->setEnabled(false);
        m_actBatchAddVar->setEnabled(false);
        m_actBatchChangeVarProp->setEnabled(false);
        m_actCopy->setEnabled(false);
        m_actPaste->setEnabled(false);
        m_actDelete->setEnabled(false);
        if(bLeftTree)
        {
            m_actWriteVar->setEnabled(false);
            m_actConfig->setEnabled(!m_groupName.isEmpty());
        }
        else
        {
            IMDRTDBVariable * var = (IMDRTDBVariable*)item;
            m_actWriteVar->setEnabled(bSelVar &&  var->writeProtectParameter().m_enableWrite);
            m_actConfig->setEnabled(bSelGroup || bSelVar);
        }
    }
    else
    {
        if(bLeftTree)
        {
            if(m_groupName == "SystemVariableGroup")
            {
                m_actAddVarGroup->setEnabled(false);
                m_actAddVar->setEnabled(false);
                m_actBatchAddVar->setEnabled(false);
                m_actBatchChangeVarProp->setEnabled(false);
                m_actCopy->setEnabled(false);
                m_actPaste->setEnabled(false);
                m_actDelete->setEnabled(false);
                m_actWriteVar->setEnabled(false);
                m_actConfig->setEnabled(false);
            }
            else
            {
                m_actAddVarGroup->setEnabled(true);
                m_actAddVar->setEnabled(!m_groupName.isEmpty());
                m_actBatchAddVar->setEnabled(false);
                m_actBatchChangeVarProp->setEnabled(false);
                m_actCopy->setEnabled(!m_groupName.isEmpty());
                if(hasPasteItem)
                {
                    m_actPaste->setEnabled(true);
                }
                else if(m_copyObjItem != NULL && !m_groupName.isEmpty())
                {
                    //复制变量或变量组
                    m_actPaste->setEnabled(true);
                }
                else
                {
                    m_actPaste->setEnabled(false);
                }
                m_actDelete->setEnabled(!m_groupName.isEmpty());
                m_actWriteVar->setEnabled(false);
                m_actConfig->setEnabled(!m_groupName.isEmpty());
            }
        }
        else
        {
            m_actWriteVar->setEnabled(false);
            if(bMixSel || bMultiSelGroup || bMultiSelVar)
            {
                m_actAddVarGroup->setEnabled(false);
                m_actAddVar->setEnabled(false);
                m_actBatchAddVar->setEnabled(false);
                m_actCopy->setEnabled(false);
                m_actPaste->setEnabled(false);
                m_actDelete->setEnabled(true);
                m_actConfig->setEnabled(false);
                m_actBatchChangeVarProp->setEnabled(bMultiSelVar);
            }
            else
            {
                if(bSelGroup)
                {
                    IMDVariableGroup* group = (IMDVariableGroup*)item;
                    if(group->localName()  =="SystemVariableGroup")
                    {
                        m_actAddVarGroup->setEnabled(false);
                        m_actAddVar->setEnabled(false);
                        m_actConfig->setEnabled(false);
                        m_actCopy->setEnabled(false);
                        m_actPaste->setEnabled(false);
                        m_actDelete->setEnabled(false);
                        m_actWriteVar->setEnabled(false);
                        m_actBatchChangeVarProp->setEnabled(false);
                        m_actBatchAddVar->setEnabled(false);
                        m_toolBar->update();
                        return;
                    }
                }
                m_actAddVarGroup->setEnabled(true);
                m_actAddVar->setEnabled(!m_groupName.isEmpty() || bSelGroup || bSelVar);
                m_actBatchAddVar->setEnabled(bSelVar);
                m_actCopy->setEnabled(bSelGroup || bSelVar);
                if(m_copyObjItem != NULL)
                {
                    //判断是变量or变量组
                    if(m_copyObjItem != NULL)
                    {
                        if(m_copyObjItem->itemType() == MDTreeItemType::Variable)
                        {
                            if(!m_groupName.isEmpty() || bSelGroup ||  bSelVar)//避免复制项为Var，当前节点为变量根节点
                                m_actPaste->setEnabled(true);
                            else
                                m_actPaste->setEnabled(false);
                        }
                        else
                            m_actPaste->setEnabled(true);
                    }
                }
                else if(m_rtdbUI->getCopyObjItem() != NULL)
                {
                    m_actPaste->setEnabled(true);
                }
                else
                {
                    m_actPaste->setEnabled(false);
                }
                m_actDelete->setEnabled(bSelGroup || bSelVar || bMultiSelVar|| bMultiSelGroup || bMixSel);
                m_actConfig->setEnabled(bSelGroup || bSelVar || !m_groupName.isEmpty());
                m_actBatchChangeVarProp->setEnabled(bMultiSelVar);
            }
        }
    }

    m_toolBar->update();
}

void CMDRTDBVariablesWidget::clearCopyItem()
{
    m_copyObjItem = NULL;
}

void CMDRTDBVariablesWidget::treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    refershActionStatus(m_mdRTDBManager->isRunning(),false,0,false);
}

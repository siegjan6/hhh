
#include <QFileInfo>
#include <QDir>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>

#include "MDRTDBManagerUI.h"
#include "MDRTDBVariablesWindow.h"
#include "MDRTDBDevicesWindow.h"
#include "MDRTDBDevicesWidget.h"
#include "MDRTDBVariablesWidget.h"
#include "MDRTDBManagerFactory.h"
#include "IMDRTDBManager.h"
#include "SelectVariableDialog.h"
#include "MDMultiLanguage.h"

#include "AddDeviceDialog.h"
#include "AddVarGroupDialog.h"
#include "PropertyBrowser.h"
#include "DatablockMonitorDialog.h"
#include "MDVariableGroupProperty.h"
#include "CustomEvent.h"

//树结构节点类型
#define RTDBRootItem    999
#define GroupRootItem   1000
#define GroupItem       1001
#define DevRootItem     1100
#define DevItem         1101
#define DevDBItem       1102

//const QString m_rtdbRootTreeItemName = "数据采集";
//const QString m_varGroupRootTreeItemName = "变量";
//const QString m_deviceRootTreeItemName ="设备";

CMDRTDBManagerUI::CMDRTDBManagerUI()
{
    m_MDStudioInterface = NULL;
    m_treeWidget = NULL;
    m_rtdbRootTreeItem = NULL;
    m_varGroupRootTreeItem = NULL;
    m_deviceRootTreeItem = NULL;
    m_variablesWindow = NULL;
    m_devicesWindow = NULL;
    m_mdRTDBManager = NULL;

    //左边树节点文本
    m_rtdbRootTreeItemName = tr("数据采集");
    m_varGroupRootTreeItemName = tr("变量");
    m_deviceRootTreeItemName = tr("设备");

    //按钮文本
    m_addDeviceText = tr("添加设备");
    m_addDataBlockText = tr("添加数据块");
    m_addGroupText = tr("添加变量组");
    m_deleteText = tr("删除");
    m_copyText = tr("复制");
    m_pasteText = tr("粘帖");
    m_DBPacketStartText = tr("开始监控");
    m_DBPacketStopText = tr("停止监控");

    m_copyTreeItem = NULL;
    m_copyObjItem = NULL;
    m_datablockMonitorDlg = NULL;
    m_variableGroupProperty = new CMDVariableGroupProperty();
}
CMDRTDBManagerUI::~CMDRTDBManagerUI()
{
    if(m_variablesWindow!= NULL)
    {
        m_variablesWindow->unBindVariableTreeModel();
        m_variablesWindow->close();
        delete m_variablesWindow;
        m_variablesWindow = NULL;
    }
    if(m_devicesWindow!= NULL)
    {
        m_devicesWindow->unBindDeviceTreeModel();
        m_devicesWindow->close();
        delete m_devicesWindow;
        m_devicesWindow = NULL;
    }
    if(m_datablockMonitorDlg != NULL)
        delete m_datablockMonitorDlg;

    if(m_variableGroupProperty != NULL)
       delete m_variableGroupProperty;
}

/*!
section Studio 接口
*/
//根节点名称
QString CMDRTDBManagerUI::rootName()
{
    return m_rtdbRootTreeItemName;
}

//根节点图标
QIcon* CMDRTDBManagerUI::rootIcon()
{
    return NULL;
}
QTreeWidgetItem* CMDRTDBManagerUI::rootItem()
{
    return m_rtdbRootTreeItem;
}

//设置树根节点
void CMDRTDBManagerUI::setRootItem(QTreeWidgetItem* value)
{
    m_rtdbRootTreeItem = value;
    m_rtdbRootTreeItem->setData(0,Qt::UserRole,QVariant(RTDBRootItem));
}

//设置Studio接口
void CMDRTDBManagerUI::setStudioInterface(IStudioInterface* value)
{
    m_MDStudioInterface = value;
}

//加载
void CMDRTDBManagerUI::load()
{
    //多窗口容器
    if(m_MDStudioInterface != NULL)
    {
        m_treeWidget = m_MDStudioInterface->treeWidget();
        connect(m_treeWidget,SIGNAL(keyPressEvent()),this,SLOT(CMDRTDBManagerUI::keyPressEvent()));
        m_projectConfiger = m_MDStudioInterface->projectConfiger();
        //项目配置
        if(NULL != m_projectConfiger)
        {
            m_mdRTDBManager = m_projectConfiger->nsRTDBManager();
            if(m_mdRTDBManager != NULL)
            {
                m_mdRTDBManager->driverManager()->setPropertyBrowse(m_MDStudioInterface->propertyBrowser());
                m_mdRTDBManager->driverManager()->setRTDBManagerUI(this);
            }
        }
    }
    if(m_rtdbRootTreeItem != NULL)
        initialRTDBRootTreeItem();

    //!初始化右键菜单
    initialTreeItemMenu();
}

//卸载
void CMDRTDBManagerUI::unload()
{
}

void CMDRTDBManagerUI::collectWords()
{

}

//!获取选中变量名
bool CMDRTDBManagerUI::selectVariables(QStringList &varNameList)
{
    bool ret = false;
    QList<IMDVariable*>varList;
    if(selectVariables(varList))
    {
        for(int i=0;i<varList.count();i++)
        {
            IMDVariable *variable = (IMDVariable *)varList[i];
            varNameList.append(variable->name());
        }
        ret = true;
    }
    return ret;
}
//!获取选中变量
bool CMDRTDBManagerUI::selectVariables(QList<IMDVariable*>&varList)
{
    bool ret = false;
    CSelectVariableDialog dlg;
    dlg.setRTDBManager(m_mdRTDBManager);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        dlg.getSelectedVariables(varList);
        ret = true;
    }
    return ret;
}
//!由变量或设备窗口触发，重新绑定Model
void  CMDRTDBManagerUI::on_run()
{
    if(m_mdRTDBManager!=NULL)
    {
        if(m_mdRTDBManager->isRunning())
        {
            m_mdRTDBManager->stop();
        }
        else
        {
            m_mdRTDBManager->start();
        }
        if(m_variablesWindow!= NULL)
        {
            m_variablesWindow->bindVariableTreeModel();
        }
        if(m_devicesWindow!= NULL)
        {
            m_devicesWindow->bindDeviceTreeModel();
        }
    }
}
IMDRTDBManager *CMDRTDBManagerUI::nsRTDBManager()
{
    return m_mdRTDBManager;
}

//!设置配置对象，同时获取m_mdRTDBManager
//void CMDRTDBManagerUI::setProjectConfiger(CMDProjectConfiger *projectConfiger)
//{
//    m_projectConfiger = projectConfiger;
//    if(NULL != m_projectConfiger)
//    {
//        m_mdRTDBManager = m_projectConfiger->nsRTDBManager();
//    }
//}
//!获取框架Frame
//void CMDRTDBManagerUI::setMDIMoudleParam(IMDIMoudleParam *mdiMoudleParam)
//{
//    m_mdiMoudleParam = mdiMoudleParam;
//    if(NULL != m_mdiMoudleParam)
//    {
//        m_rtdbRootTreeItem = m_mdiMoudleParam->treeRootItem();
//        initialRTDBRootTreeItem();

//        //!初始化右键菜单
//        initialTreeItemMenu();
//    }
//}

IStudioInterface * CMDRTDBManagerUI::MDStudioInterface()
{
    return m_MDStudioInterface;
}

void CMDRTDBManagerUI::initialRTDBRootTreeItem()
{
    if(m_rtdbRootTreeItem != NULL)
    {
        clearTreeItem(m_rtdbRootTreeItem);

        //修改左边树节点文本
        m_rtdbRootTreeItem->setText(0,CMDMultiLanguage::instance()->value(m_rtdbRootTreeItemName));
        m_rtdbRootTreeItem->setIcon(0, QIcon(":/images/RTDB.png"));


        m_varGroupRootTreeItem = new QTreeWidgetItem(QStringList(CMDMultiLanguage::instance()->value(m_varGroupRootTreeItemName)));
        m_varGroupRootTreeItem->setIcon(0,QIcon(":/images/VariableGroup.png"));
        m_varGroupRootTreeItem->setData(0,Qt::UserRole,QVariant(GroupRootItem));
        m_rtdbRootTreeItem->addChild(m_varGroupRootTreeItem);

        m_deviceRootTreeItem = new QTreeWidgetItem(QStringList(CMDMultiLanguage::instance()->value(m_deviceRootTreeItemName)));
        m_deviceRootTreeItem->setIcon(0,QIcon(":/images/device.png"));
        m_rtdbRootTreeItem->addChild(m_deviceRootTreeItem);
        m_deviceRootTreeItem->setData(0,Qt::UserRole,QVariant(DevRootItem));
        refreshTree();

        //bind event
        connect(m_treeWidget, &QTreeWidget::doubleClicked, this, &CMDRTDBManagerUI::treeWidgetDoubleClick);
        connect(m_treeWidget, &QTreeWidget::itemPressed, this, &CMDRTDBManagerUI::treeWidgetNodeMouseClick);
    }
}
/*
void CMDRTDBManagerUI::setWindowTitle(bool bVar,const QString& title)
{
    if(bVar)
        m_variablesWindow->setWindowTitle(title);
    else
        m_devicesWindow->setWindowTitle(title);
}
*/
void CMDRTDBManagerUI::refreshTreeNode(bool bVar, qulonglong nodeData, qulonglong expandNodeData)
{
    qulonglong utemp = 0;
    QTreeWidgetItem* findItem = NULL;
    QTreeWidgetItem* expandItem = NULL;
    bool searchExpandItem = false;
    if(nodeData == 0)
        return;

    if(nodeData != expandNodeData)
        searchExpandItem = true;

    if(bVar)
    {
        IMDVariableGroup* group = NULL;
        utemp = (qulonglong)m_mdRTDBManager->variableGroup();
        if(nodeData == utemp)
        {
            findItem = m_varGroupRootTreeItem;
            group = m_mdRTDBManager->variableGroup();
            m_variablesWindow->setWindowTitle(tr("变量"));
        }
        else
        {
            findItem = getTreeNodeWithData(m_varGroupRootTreeItem,nodeData);
            if(findItem != NULL)
            {
                group = (IMDVariableGroup*)nodeData;
                findItem->setText(0,group->localName());
                m_variablesWindow->setWindowTitle(group->name());
            }
        }
        if(findItem != NULL && group != NULL)
        {
            clearTreeItem(findItem);
            if(searchExpandItem)
            {
                setChildGroupTreeItem(group,findItem,expandNodeData,&expandItem);
            }
            else
            {
                setChildGroupTreeItem(group,findItem,0,&expandItem);
                expandItem = findItem;
            }
            m_treeWidget->expandItem(m_varGroupRootTreeItem);

            if(expandItem != NULL && searchExpandItem)
            {
                QTreeWidgetItem* expandItemParent = expandItem->parent();

                while(expandItemParent != m_varGroupRootTreeItem)
                {
                    m_treeWidget->expandItem(expandItemParent);
                    expandItemParent = expandItemParent->parent();
                }
            }
        }
    }
    else
    {
        IDevice* dev = NULL;
        utemp = (qulonglong)m_mdRTDBManager->driverManager();
        if(nodeData == utemp)
        {
            findItem = m_deviceRootTreeItem;
            m_devicesWindow->setWindowTitle(tr("设备"));            
        }
        else
        {
            findItem = getTreeNodeWithData(m_deviceRootTreeItem,nodeData);
            if(findItem != NULL)
            {
                dev = (IDevice*)nodeData;
                findItem->setText(0,dev->name());
                m_devicesWindow->setWindowTitle(dev->name());
            }
        }
        if(findItem != NULL)
        {
            clearTreeItem(findItem);
            if(dev == NULL)//设备根节点
            {
                IMDDriverManager* rootdevice = m_mdRTDBManager->driverManager();
                if(rootdevice != NULL)
                {
                    IDevice* childdev = NULL;
                    QTreeWidgetItem * childtreeitem = NULL;
                    for(int i = 0; i<rootdevice->getDeviceCount(); i++)
                    {
                        childdev = rootdevice->getDeviceAt(i);
                        childtreeitem = new QTreeWidgetItem(QStringList(childdev->name()));

                        childtreeitem->setIcon(0,QIcon(":/images/device.png"));
                        childtreeitem->setData(1,Qt::UserRole,QVariant(DevItem));
                        utemp = (qulonglong)childdev;
                        if(searchExpandItem && utemp == expandNodeData)
                        {
                            searchExpandItem = false;
                            expandItem = childtreeitem;
                        }
                        childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));
                        m_deviceRootTreeItem->addChild(childtreeitem);
                        //添加数据块
                        if(searchExpandItem)
                        {
                            expandItem = setChildDBTreeItem(childdev,childtreeitem,expandNodeData);
                            if(expandItem != NULL)
                                searchExpandItem = false;
                        }
                        else
                        {
                            setChildDBTreeItem(childdev,childtreeitem,0);
                        }
                    }
                }
            }
            else//某具体设备
            {
                if(searchExpandItem)
                {
                    expandItem = setChildDBTreeItem(dev,findItem,expandNodeData);
                }
                else
                {
                    expandItem = findItem;
                    setChildDBTreeItem(dev,findItem,0);
                }
            }
            m_treeWidget->expandItem(m_deviceRootTreeItem);
        }
    }
    //expand some item
    if(expandItem != NULL)
    {
        m_treeWidget->setItemHidden(expandItem,false);
        m_treeWidget->expandItem(expandItem);
    }
}

void CMDRTDBManagerUI::clearCopyItem()
{
    m_copyTreeItem = NULL;    
    m_copyObjItem = NULL;
}

QTreeWidgetItem* CMDRTDBManagerUI::getTreeNodeWithData(QTreeWidgetItem* fromTreeItem, qulonglong nodeData)
{
    QTreeWidgetItem* treeItem = NULL;
    QTreeWidgetItem* retTreeItem = NULL;
    qulonglong utemp = 0;
    int nCnt = fromTreeItem->childCount();
    for(int i = 0; i<nCnt; i++)
    {
        treeItem = fromTreeItem->child(i);
        utemp = treeItem->data(0,Qt::WhatsThisRole).toInt();
        if(utemp == nodeData)
        {
            retTreeItem = treeItem;
            break;
        }
        if(treeItem->childCount() > 0)
        {
            retTreeItem = getTreeNodeWithData(treeItem,nodeData);
            if(retTreeItem != NULL)
                break;
        }
    }
    return retTreeItem;
}

void CMDRTDBManagerUI::refreshTree()
{
    //增加变量组及设备子节点
    if(m_mdRTDBManager != NULL)
    {
        QTreeWidgetItem* selTreeItem = NULL;
        QTreeWidgetItem* curItem = getSelectTreeItem();

        qulonglong utobesel = 0;

        if(curItem == m_rtdbRootTreeItem ||
           curItem == m_varGroupRootTreeItem ||
           curItem == m_deviceRootTreeItem)
        {
            selTreeItem = curItem;
            utobesel = NULL;
        }
        else
        {
            if(curItem != NULL)
            {
                utobesel = curItem->data(0,Qt::WhatsThisRole).toInt();
                curItem = NULL;
                if(utobesel < RTDBRootItem)
                    return;
            }
        }
        clearTreeItem(m_varGroupRootTreeItem);
        clearTreeItem(m_deviceRootTreeItem);

        IMDVariableGroup* rootgroup = m_mdRTDBManager->variableGroup();
        if(rootgroup != NULL)
        {
            setChildGroupTreeItem(rootgroup,m_varGroupRootTreeItem,utobesel,&selTreeItem);
        }
        qulonglong utemp;
        IMDDriverManager* rootdevice = m_mdRTDBManager->driverManager();
        if(rootdevice != NULL)
        {
            IDevice* childdev = NULL;
            QTreeWidgetItem * childtreeitem = NULL;
            QTreeWidgetItem * childseltreeitem = NULL;
            for(int i = 0; i<rootdevice->getDeviceCount(); i++)
            {
                childdev = rootdevice->getDeviceAt(i);
                childtreeitem = new QTreeWidgetItem(QStringList(childdev->name()));

                childtreeitem->setIcon(0,QIcon(":/images/device.png"));//1234 换图标
                childtreeitem->setData(1,Qt::UserRole,QVariant(DevItem));
                utemp = (qulonglong)childdev;
                childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));
                if(utemp == utobesel)
                     selTreeItem =  childtreeitem;
                m_deviceRootTreeItem->addChild(childtreeitem);
                //添加数据块
                childseltreeitem = setChildDBTreeItem(childdev,childtreeitem,utobesel);
                if(childseltreeitem != NULL)
                    selTreeItem = childseltreeitem;
            }
        }
        //!设置选中节点
        if(selTreeItem == NULL)
            selTreeItem = curItem;
        if(selTreeItem != NULL)
        {
            //m_treeWidget->setCurrentItem(selTreeItem);
            m_treeWidget->expandItem(selTreeItem);
            treeWidgetNodeMouseClick(selTreeItem);
        }
    }
}

void CMDRTDBManagerUI::initialTreeItemMenu()
{
    m_actDelete = new QAction(QIcon(":/rs/image/delete"), m_deleteText, this);
    connect(m_actDelete, &QAction::triggered, this, &CMDRTDBManagerUI::menuItemDeleteClick);

    m_actCopy = new QAction(QIcon(":/rs/image/copy"), m_copyText, this);
    connect(m_actCopy, &QAction::triggered, this, &CMDRTDBManagerUI::menuItemCopyClick);

    m_actPaste = new QAction(QIcon(":/rs/image/paste"), m_pasteText, this);
    connect(m_actPaste, &QAction::triggered, this, &CMDRTDBManagerUI::menuItemPasteClick);

    m_actAddDevice = new QAction(QIcon(":/rs/image/device"), m_addDeviceText, this);
    connect(m_actAddDevice, &QAction::triggered, this, &CMDRTDBManagerUI::menuItemAddDeviceClick);

    m_actAddDataBlock = new QAction(QIcon(":/rs/image/datablock"), m_addDataBlockText, this);
    connect(m_actAddDataBlock, &QAction::triggered, this, &CMDRTDBManagerUI::menuItemAddDataBlockClick);

    m_actAddGroup = new QAction(QIcon(":/rs/image/variablegroup"), m_addGroupText, this);
    connect(m_actAddGroup, &QAction::triggered, this, &CMDRTDBManagerUI::menuItemAddVarGroupClick);

    m_actDBPacket = new QAction(QIcon(":/images/start.png"), m_DBPacketStartText, this);
    connect(m_actDBPacket, &QAction::triggered, this, &CMDRTDBManagerUI::menuItemPacketClick);

    m_actCopy->setShortcut(QKeySequence::Copy);
    m_actPaste->setShortcut(QKeySequence::Paste);
    m_actDelete->setShortcut(QKeySequence::Delete);

    m_menu = new QMenu(m_treeWidget);
    m_menu->addAction(m_actDelete);
    m_menu->addAction(m_actCopy);
    m_menu->addAction(m_actPaste);
    m_menu->addAction(m_actAddDevice);
    m_menu->addAction(m_actAddDataBlock);
    m_menu->addAction(m_actAddGroup);
    m_menu->addAction(m_actDBPacket);

    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeWidget, &QTreeView::customContextMenuRequested,this, &CMDRTDBManagerUI::menuStripNodeOpening);
}

void CMDRTDBManagerUI::menuItemPacketClick()
{
    QTreeWidgetItem* treeItem = getSelectTreeItem();
    if(treeItem == NULL)
        return;

    QTreeWidgetItem *parentitem = treeItem->parent();
    if(parentitem != NULL)
    {
        IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(parentitem->text(0));
        if(pdev != NULL)
        {
            IDataBlock* dataBlock = pdev->getDataBlock(treeItem->text(0));
            if(dataBlock == NULL)
                return;
            if(m_datablockMonitorDlg == NULL)
                m_datablockMonitorDlg = new CDataBlockMonitorDialog(NULL);

            if(m_datablockMonitorDlg != NULL && dataBlock != NULL)
            {
                //是否已处于监控？
                if(dataBlock->msgMonitor())
                {
                    dataBlock->setMsgMonitor(false);
                    dataBlock->setMonitorMode(IDataBlock::MDMonitorType::Binary);
                }
                else
                {
                    m_datablockMonitorDlg->setMonitoredDataBlock(dataBlock);
                    //m_datablockMonitorDlg-
                    m_datablockMonitorDlg->show();
                }
            }//end  m_datablockMonitorDlg != NULL && dataBlock != NULL
        }//end datablock
    }
}

/*!
\section 键盘事件
*/
void CMDRTDBManagerUI::keyPressEvent( QKeyEvent *k )
{
    QTreeWidgetItem* curItem = m_treeWidget->currentItem();

    if(!isRTDBTreeItem(curItem))
        return;

    if(m_mdRTDBManager->isRuntimeMode())
    {
        return;
    }

    if(m_treeWidget != NULL)
    {
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
void CMDRTDBManagerUI::menuStripNodeOpening(const QPoint &pos)
{
    QTreeWidgetItem *item = m_treeWidget->itemAt(pos);
    if(item == NULL)
        return;

    if(!isRTDBTreeItem(item))
        return;


    //!运行时，不操作
    if(m_mdRTDBManager->isRunning())
    {
        if(item->data(item->columnCount()-1,Qt::UserRole).toInt() == DevDBItem)
        {
            m_actDelete->setVisible(false);
            m_actCopy->setVisible(false);
            m_actPaste->setVisible(false);
            m_actAddDevice->setVisible(false);
            m_actAddDataBlock->setVisible(false);
            m_actAddGroup->setVisible(false);

            //获取DataBlock
            IDataBlock * db = NULL;
            QTreeWidgetItem* parentItem = item->parent();
            if(parentItem != NULL)
            {
                QString strDevName = parentItem->text(0);
                IDevice * dev = m_mdRTDBManager->driverManager()->getDevice(strDevName);
                if(dev != NULL)
                {
                    db = dev->getDataBlock(item->text(0));
                }
            }

            if(db != NULL && db->msgMonitor())
                 m_actDBPacket->setText(m_DBPacketStopText);
            else
                 m_actDBPacket->setText(m_DBPacketStartText);

            m_actDBPacket->setVisible(true);
            m_menu->exec(QCursor::pos());
        }
        return;
    }

    //!检查复制项
    bool bPasteDev = false;
    bool bPasteDB = false;
    bool bPasteGroup = false;
    if(m_copyObjItem != NULL)
    {
        if(m_copyObjItem->itemType() == MDTreeItemType::DataBlock)
            bPasteDB = true;
        else if(m_copyObjItem->itemType() == MDTreeItemType::Device)
            bPasteDev = true;
        else if(m_copyObjItem->itemType() == MDTreeItemType::VariableGroup)
            bPasteGroup = true;
    }
    //!配置模式下，数据包监控隐藏
    m_actDBPacket->setVisible(false);
    //!设备顶层： 增加设备
    if(item->data(item->columnCount()-1,Qt::UserRole).toInt() == DevRootItem)
    {
        m_actDelete->setVisible(false);
        m_actCopy->setVisible(false);
        if(bPasteDev)
        {
            m_actPaste->setVisible(true);
        }
        else
        {
            m_actPaste->setVisible(false);
        }
        m_actAddDevice->setVisible(true);
        m_actAddDataBlock->setVisible(false);
        m_actAddGroup->setVisible(false);
        m_menu->exec(QCursor::pos());
    }
    else if(item->data(item->columnCount()-1,Qt::UserRole).toInt() == DevItem)
    {
        m_actDelete->setVisible(true);
        m_actCopy->setVisible(true);
        if(bPasteDev || bPasteDB)
            m_actPaste->setVisible(true);
        else
            m_actPaste->setVisible(false);
        m_actAddDevice->setVisible(true);
        m_actAddDataBlock->setVisible(true);
        m_actAddGroup->setVisible(false);
        m_menu->exec(QCursor::pos());
    }
    else if(item->data(item->columnCount()-1,Qt::UserRole).toInt() == DevDBItem)
    {
        m_actDelete->setVisible(true);
        m_actCopy->setVisible(true);
        if(bPasteDB || bPasteDev)
            m_actPaste->setVisible(true);
        else
            m_actPaste->setVisible(false);
        m_actAddDevice->setVisible(true);
        m_actAddDataBlock->setVisible(true);
        m_actAddGroup->setVisible(false);
        m_actDBPacket->setVisible(false);
        m_menu->exec(QCursor::pos());
    }
    else if(item->data(item->columnCount()-1,Qt::UserRole).toInt() == GroupRootItem)
    {
        m_actDelete->setVisible(false);
        m_actCopy->setVisible(false);
        if(bPasteGroup)
            m_actPaste->setVisible(true);
        else
            m_actPaste->setVisible(false);
        m_actAddDevice->setVisible(false);
        m_actAddDataBlock->setVisible(false);
        m_actAddGroup->setVisible(true);
        m_menu->exec(QCursor::pos());
    }
    else if(item->data(item->columnCount()-1,Qt::UserRole).toInt() == GroupItem)
    {
        m_actDelete->setVisible(true);
        m_actCopy->setVisible(true);
        if(bPasteGroup)
            m_actPaste->setVisible(true);
        else
            m_actPaste->setVisible(false);
        //!系统变量组不支持删除、复制、粘贴
        qulonglong utemp = item->data(0,Qt::WhatsThisRole).toULongLong();
        IMDVariableGroup* pGroup = (IMDVariableGroup*)utemp;
        if(pGroup->name() == "SystemVariableGroup")
        {
            m_actDelete->setVisible(false);
            m_actCopy->setVisible(false);
            m_actPaste->setVisible(false);
        }
        m_actAddDevice->setVisible(false);
        m_actAddDataBlock->setVisible(false);
        m_actAddGroup->setVisible(false);
        m_menu->exec(QCursor::pos());
    }
}

void CMDRTDBManagerUI::menuItemDeleteClick()
{
    QTreeWidgetItem *item = m_treeWidget->selectedItems().at(0);
    if(item == NULL)
        return;
    int ndata = item->data(item->columnCount() - 1,Qt::UserRole).toInt();
    qulonglong nObjData = item->data(0,Qt::WhatsThisRole).toULongLong();
    //!删除后，默认选中上面的兄弟节点，为空时，选下面的节点，再为空时选父节点
    QTreeWidgetItem *nextselitem = m_treeWidget->itemAbove(item);

    if(nextselitem != NULL)
    {
        if(nextselitem->data(nextselitem->columnCount() - 1,Qt::UserRole) != ndata)
            nextselitem = NULL;
    }

    if(nextselitem == NULL)
    {
        nextselitem = m_treeWidget->itemBelow(item);
        if(nextselitem != NULL)
        {
            if(nextselitem->data(nextselitem->columnCount() - 1,Qt::UserRole) != ndata)
                nextselitem = NULL;
        }
    }

    if(nextselitem == NULL)
        nextselitem = item->parent();

    QString strItemText = getTreeNodeText(item);
    QString str0 = tr("确定要删除");
    QString strTranslate = CMDMultiLanguage::instance()->value(str0);
    QString str = QString("%1：%2 ?").arg(strTranslate).arg(strItemText);

    QMessageBox box;
    box.setText(str);
    box.setWindowTitle(CMDMultiLanguage::instance()->value(tr("确认")));
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    int reply = box.exec();
    if(reply == QMessageBox::No)
        return;
    //清空copy项
    if((qulonglong)m_copyObjItem == nObjData)
        m_copyObjItem = NULL;

    if(ndata == DevItem || ndata ==DevDBItem)
    {
        m_devicesWindow->unBindDeviceTreeModel();
        if(ndata == DevItem)//删除设备
        {
            m_mdRTDBManager->driverManager()->removeDevice(item->text(0));
            clearTreeItem(item);
            delete item;
            m_devicesWindow->on_Modified();
        }
        else//删除DataBlock
        {
            QTreeWidgetItem *parentitem = item->parent();
            if(parentitem != NULL)
            {
                IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(parentitem->text(0));
                if(pdev != NULL)
                {
                    pdev->removeDataBlock(item->text(0));
                    clearTreeItem(item);
                    delete item;
                    m_devicesWindow->on_Modified();
                }
            }
        }
    }
    else if(ndata == GroupItem)//Group,从父节点删除子Group
    {
        m_variablesWindow->unBindVariableTreeModel();
        QTreeWidgetItem *parentitem = item->parent();
        IMDVariableGroup* pParentGroup = NULL;
        if(parentitem != NULL)
        {
            ndata = parentitem->data(parentitem->columnCount() - 1,Qt::UserRole).toInt();
            if(ndata == GroupRootItem)
            {
                m_mdRTDBManager->variableGroup()->removeChildVariableGroup(strItemText);
                clearTreeItem(item);
                delete item;
                m_variablesWindow->on_Modified();
            }
            else
            {
                QString strGroupName = getTreeNodeText(parentitem);
                pParentGroup = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(strGroupName);
                if(pParentGroup != NULL)
                {
                    pParentGroup->removeChildVariableGroup(item->text(0));
                    clearTreeItem(item);
                    delete item;
                    m_variablesWindow->on_Modified();
                }
            }
        }
    }

    //获取当前Item,重新绑定Model
    m_treeWidget->setCurrentItem(nextselitem);
    refreshTree();
    if(m_variablesWindow != NULL && m_variablesWindow->isActived())
        m_variablesWindow->getVariableWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),
                                                                   true,
                                                                   ndata,
                                                                   m_copyObjItem->itemType() == MDTreeItemType::VariableGroup);
    else if(m_devicesWindow != NULL && m_devicesWindow->isActived())
        m_devicesWindow->getDeviceWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),
                                                                   true,
                                                                   ndata,
                                                                   m_copyObjItem);
    //m_treeWidget->setItemSelected(nextselitem,true);
    //m_treeWidget->update();
}

void CMDRTDBManagerUI::menuItemAddDeviceClick()
{
    if(m_mdRTDBManager == NULL)
        return;

    CAddDeviceDialog dlg;
    dlg.setDriverManager(m_mdRTDBManager->driverManager());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        m_devicesWindow->on_Modified();
        QTreeWidgetItem *childtreeitem = new QTreeWidgetItem(QStringList(dlg.getDeviceName()));
        childtreeitem->setIcon(0,QIcon(":/images/device.png"));

        childtreeitem->setData(1,Qt::UserRole,QVariant(DevItem));
        qulonglong utemp = (qulonglong)(m_mdRTDBManager->driverManager()->getDevice(dlg.getDeviceName()));
        childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));
        m_deviceRootTreeItem->addChild(childtreeitem);
        m_treeWidget->setCurrentItem(childtreeitem);
        treeWidgetNodeMouseClick(childtreeitem);
    }
}
void CMDRTDBManagerUI::menuItemAddDataBlockClick()
{
    QTreeWidgetItem *item = m_treeWidget->selectedItems().at(0);
    if(item == NULL)
        return;

    //检查是Device还是DataBlock
    QTreeWidgetItem *pParentItem = NULL;
    int ndata = item->data(item->columnCount() - 1,Qt::UserRole).toInt();
    if(ndata == DevItem)
    {
        pParentItem = item;
    }
    else if(ndata == DevDBItem)
    {
        pParentItem = item->parent();
    }
    if(pParentItem == NULL)
        return;

    IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(pParentItem->text(0));

    if(pdev != NULL)
    {
        IDataBlock* pDB = pdev->createDataBlock();
        if(pDB != NULL)
        {
            QTreeWidgetItem *childtreeitem = new QTreeWidgetItem(QStringList(pDB->name()));
            childtreeitem->setIcon(0,QIcon(":/images/datablock.png"));//1234 换图标

            childtreeitem->setData(1,Qt::UserRole,QVariant(DevDBItem));
            qulonglong utemp = (qulonglong)(pDB);
            childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));

            pParentItem->addChild(childtreeitem);
            m_treeWidget->setCurrentItem(childtreeitem);
            treeWidgetNodeMouseClick(childtreeitem);

            m_devicesWindow->on_Modified();
        }
    }
}

void CMDRTDBManagerUI::menuItemAddVarGroupClick()
{
    QTreeWidgetItem *item = m_treeWidget->selectedItems().at(0);
    if(item == NULL)
        return;

    //检查是Group根目录还是任一子Group
    QTreeWidgetItem *pParentItem = item;
    bool bRoot = false;
    int ndata = item->data(item->columnCount() - 1,Qt::UserRole).toInt();
    if(ndata == GroupRootItem)
        bRoot = true;

    if(pParentItem == NULL)
        return;

    IMDVariableGroup* pParentGroup = NULL;
    if(bRoot)
        pParentGroup = m_mdRTDBManager->variableGroup();
    else
        pParentGroup = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(getTreeNodeText(pParentItem));

    if(pParentGroup == NULL)
        return;

    CAddVarGroupDialog dlg;
    dlg.setCurrentVariableGroup(pParentGroup);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        QTreeWidgetItem *childtreeitem = new QTreeWidgetItem(QStringList(dlg.getGroupName()));
        pParentItem->addChild(childtreeitem);
        childtreeitem->setIcon(0,QIcon(":/images/VariableGroup.png"));
        childtreeitem->setData(1,Qt::UserRole,QVariant(GroupItem));
        IMDVariableGroup* pChildGroup = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(getTreeNodeText(childtreeitem));
        qulonglong utemp = (qulonglong)pChildGroup;
        childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));
        m_treeWidget->setCurrentItem(childtreeitem);
        treeWidgetNodeMouseClick(childtreeitem);
        m_treeWidget->update();
        m_variablesWindow->on_Modified();
    }
}

/*
void CMDRTDBManagerUI::menuItemConfigClick()
{
    m_copyModelIndex = QModelIndex();
    if (m_selectedModelIndex.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item->itemType() == MDTreeItemType::Device)
        {
            ((IDevice*)item)->configDecice(this);
            emit m_deviceTreeModel->layoutChanged();
            //!刷新左边树节点
            m_rtdbUI->refreshTree();
        }
        else if(item->itemType() == MDTreeItemType::DataBlock)
        {
            IDataBlock* dataBlock = (IDataBlock*)item;
            IDevice* device = (IDevice* )dataBlock->parent(MDTreeItemType::Device);
            if(device)
            {
               device->configDataBlock(dataBlock,this);
               emit m_deviceTreeModel->layoutChanged();
               //!刷新左边树节点
               m_rtdbUI->refreshTree();
            }
        }
    }
}*/
void CMDRTDBManagerUI::menuItemCopyClick()
{
    m_copyTreeItem = NULL;
    m_copyObjItem = NULL;
    //清除右边设备及变量的复制项
    if(m_variablesWindow != NULL)
        m_variablesWindow->getVariableWidget()->clearCopyItem();
    if(m_devicesWindow != NULL)
        m_devicesWindow->getDeviceWidget()->clearCopyItem();
    //group || dev || datablock
    //如果是RTDB根节点或变量、设备跟节点，忽略
    int ndata = m_curTreeItem->data(m_curTreeItem->columnCount() - 1,Qt::UserRole).toInt();
    if(m_curTreeItem != NULL)
    {
        if(ndata != RTDBRootItem && ndata != GroupRootItem && ndata != DevRootItem)
        {
            m_copyTreeItem = m_curTreeItem;
            m_copyObjItem = (IMDTreeItem*)m_curTreeItem->data(0,Qt::WhatsThisRole).toULongLong();
        }
    }
    if(m_copyObjItem != NULL)
    {
        if(m_variablesWindow != NULL && m_variablesWindow->isActived())
            m_variablesWindow->getVariableWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),
                                                                       true,
                                                                       ndata,
                                                                       m_copyObjItem->itemType() == MDTreeItemType::VariableGroup);
        else if(m_devicesWindow != NULL && m_devicesWindow->isActived())
            m_devicesWindow->getDeviceWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),
                                                                       true,
                                                                       ndata,
                                                                       m_copyObjItem);

    }
    else
    {
        if(m_variablesWindow != NULL && m_variablesWindow->isActived())
            m_variablesWindow->getVariableWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),
                                                                       true,
                                                                       ndata,
                                                                       false);
        else if(m_devicesWindow != NULL && m_devicesWindow->isActived())
            m_devicesWindow->getDeviceWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),
                                                                       true,
                                                                       ndata,
                                                                       0);
    }
}

void CMDRTDBManagerUI::menuItemPasteClick()
{
    if(m_mdRTDBManager==NULL|| m_copyObjItem == NULL)
    {
        return;
    }
    QTreeWidgetItem *item = m_treeWidget->selectedItems().at(0);
    if(item == NULL)
    {
        return;
    }
    //三种可能:dev+db+group
    int ndata = item->data(item->columnCount() - 1,Qt::UserRole).toInt();    
    if(m_copyObjItem->itemType() == MDTreeItemType::Device)//dev
    {
        if(m_copyObjItem != NULL)
        {
            IDevice* pPasteDev = m_mdRTDBManager->driverManager()->copyPasteDevice((IDevice*)m_copyObjItem);            
            QTreeWidgetItem *childtreeitem = new QTreeWidgetItem(QStringList(pPasteDev->name()));
            childtreeitem->setIcon(0,QIcon(":/images/device.png"));
            childtreeitem->setData(1,Qt::UserRole,QVariant(DevItem));
            qulonglong utemp = (qulonglong)pPasteDev;
            childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));
            m_deviceRootTreeItem->addChild(childtreeitem);
            m_treeWidget->setCurrentItem(childtreeitem);
            setChildDBTreeItem(pPasteDev,childtreeitem,0);
            treeWidgetNodeMouseClick(childtreeitem);
            m_devicesWindow->on_Modified();
        }
    }
    else if(m_copyObjItem->itemType() == MDTreeItemType::DataBlock)//db
    {
        if(ndata != DevDBItem && ndata != DevItem)
            return;

        QTreeWidgetItem *devitem = item;
        //获取copy数据块所在device
        IDevice* pCopyFromDev = NULL;
        QTreeWidgetItem *pFromdevitem = m_copyTreeItem->parent();
        if(pFromdevitem == NULL)
            return;
        pCopyFromDev = (IDevice*)m_copyObjItem->parentItem(MDTreeItemType::Device);
        //IDataBlock* pCopyFromDB = pCopyFromDev->getDataBlock(m_copyTreeItem->text(0));
        //if(pCopyFromDev == NULL || pCopyFromDB == NULL)
         //   return;

        //获取目标Device
        if(ndata == DevDBItem)
            devitem = item->parent();

        if(devitem == NULL)
            return;

        //增加数据块
        IDevice* pParentDev = m_mdRTDBManager->driverManager()->getDevice(devitem->text(0));
        IDataBlock* pdb = pParentDev->copyPasteDataBlock((IDataBlock*)m_copyObjItem);
        if(pdb != NULL)
        {
            QTreeWidgetItem *childtreeitem = new QTreeWidgetItem(QStringList(pdb->name()));
            childtreeitem->setIcon(0,QIcon(":/images/datablock.png"));
            childtreeitem->setData(1,Qt::UserRole,QVariant(DevDBItem));
            qulonglong utemp = (qulonglong)pdb;
            childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));

            devitem->addChild(childtreeitem);
            m_treeWidget->setCurrentItem(childtreeitem);
            treeWidgetNodeMouseClick(childtreeitem);
            m_devicesWindow->on_Modified();
        }
    }
    else if(m_copyObjItem->itemType() == MDTreeItemType::VariableGroup)
    {
        //获取copy变量组
        IMDVariableGroup* pCopyGroup = (IMDVariableGroup*)m_copyObjItem;

        if(pCopyGroup == NULL)
            return;

        //获取目标Group
        IMDVariableGroup* pPasteGroup = NULL;
        if(ndata == GroupRootItem)
        {
            pPasteGroup = m_mdRTDBManager->variableGroup();
        }
        else
        {
            pPasteGroup = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(getTreeNodeText(item));
        }

        if(pPasteGroup == NULL)
            return;
        IMDVariableGroup* pGroup = pPasteGroup->copyPasteVariableGroup(pCopyGroup);
        if(pGroup != NULL)
        {
            QTreeWidgetItem *childtreeitem = new QTreeWidgetItem(QStringList(pGroup->localName()));
            childtreeitem->setIcon(0,QIcon(":/images/VariableGroup.png"));//1234 换图标
            childtreeitem->setData(1,Qt::UserRole,QVariant(GroupItem));
            qulonglong utemp = (qulonglong)pGroup;
            childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));

            item->addChild(childtreeitem);
            m_treeWidget->setCurrentItem(childtreeitem);
            setChildGroupTreeItem(pGroup,childtreeitem,0,NULL);
            treeWidgetNodeMouseClick(childtreeitem);
            m_variablesWindow->on_Modified();
        }
    }    
}

void CMDRTDBManagerUI::setChildGroupTreeItem(IMDVariableGroup* parentGroup,
                                             QTreeWidgetItem * parentTreeItem,
                                             qulonglong uselTreeNode,
                                             QTreeWidgetItem ** selTreeItem)
{
    QTreeWidgetItem * childtreeitem = NULL;
    IMDVariableGroup* childgroup = NULL;
    qulonglong  utemp = 0;
    if(parentGroup != NULL && parentTreeItem != NULL)
    {
        for(int i = 0; i<parentGroup->getChildVariableGroupCount(); i++)
        {
            childgroup = parentGroup->getChildVariableGroupAt(i);
            childtreeitem = new QTreeWidgetItem(QStringList(childgroup->localName()));
            childtreeitem->setIcon(0,QIcon(":/images/VariableGroup.png"));
            childtreeitem->setData(childtreeitem->columnCount() - 1,Qt::UserRole,QVariant(GroupItem));
            utemp = (qulonglong)childgroup;

            childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));
            parentTreeItem->addChild(childtreeitem);

            //!判断是否是选中的节点
            if(selTreeItem != NULL && *selTreeItem == NULL && uselTreeNode == utemp)
            {
                *selTreeItem = childtreeitem;
            }
            //if has child group
            if(childgroup->getChildVariableGroupCount() > 0)
            {
                setChildGroupTreeItem(childgroup,childtreeitem,uselTreeNode,selTreeItem);
            }
        }
    }
}

QTreeWidgetItem* CMDRTDBManagerUI::setChildDBTreeItem(IDevice* parentDev,
                                                      QTreeWidgetItem * parentTreeItem,
                                                      qulonglong uselTreeNode)
{
    QTreeWidgetItem * rettreeitem = NULL;
    QTreeWidgetItem * childtreeitem = NULL;
    IDataBlock* db = NULL;
    qulonglong utemp;
    for(int i =0; i<parentDev->getDataBlockCount(); i++)
    {
        db = parentDev->getDataBlockAt(i);
        if(db != NULL)
        {
            childtreeitem = new QTreeWidgetItem(QStringList(db->name()));
            childtreeitem->setIcon(0,QIcon(":/images/datablock.png"));//1234 换图标
            childtreeitem->setData(childtreeitem->columnCount() - 1,Qt::UserRole,QVariant(DevDBItem));
            utemp = (qulonglong)db;
            childtreeitem->setData(0,Qt::WhatsThisRole,QVariant(utemp));

            parentTreeItem->addChild(childtreeitem);

            //!判断是否是选中的节点
            if(rettreeitem == NULL && uselTreeNode == utemp)
            {
                rettreeitem = childtreeitem;
            }
        }
    }
    return rettreeitem;
}

/*!
\section 树节点操作，判断
*/
void CMDRTDBManagerUI::clearTreeItem(QTreeWidgetItem *treeItem)
{
    if(treeItem != NULL)
    {
        for (int i = 0; i < treeItem->childCount(); i++)
        {
            QTreeWidgetItem* childItem = treeItem->child(i);
            clearTreeItem(childItem);
            delete childItem;
        }
        treeItem->takeChildren();
    }
}
QTreeWidgetItem *CMDRTDBManagerUI::getSelectTreeItem()
{
    if (m_treeWidget != NULL)
    {
        QTreeWidgetItem* node = m_treeWidget->currentItem();
        if (node != NULL &&isRTDBTreeItem(node))
            return  node;
    }
    return NULL;
}

IMDTreeItem* CMDRTDBManagerUI::getCopyObjItem()
{
    return m_copyObjItem;
}

bool CMDRTDBManagerUI::isRTDBRootTreeItem(QTreeWidgetItem *treeItem)
{
    if(treeItem == m_rtdbRootTreeItem)
        return true;
    return false;
}
//!修改树控件节点文本
void CMDRTDBManagerUI::changeLanguage()
{
    //修改左边树节点文本
//    m_rtdbRootTreeItem->setText(0,CMDMultiLanguage::instance()->value(m_rtdbRootTreeItemName));
//    m_varGroupRootTreeItem->setText(0,CMDMultiLanguage::instance()->value(m_varGroupRootTreeItemName));
//    m_deviceRootTreeItem->setText(0,CMDMultiLanguage::instance()->value(m_deviceRootTreeItemName));

    //修改右边列表框
    if(m_variablesWindow != NULL)
        m_variablesWindow->changeLanguage();
    if(m_devicesWindow != NULL)
        m_devicesWindow->changeLanguage();
    if(m_datablockMonitorDlg != NULL)
        m_datablockMonitorDlg->changeLanguage();
}

bool CMDRTDBManagerUI::isRTDBTreeItem(QTreeWidgetItem *treeItem)
{
    if(treeItem == NULL )
        return false;
    /*
    return  isRTDBRootTreeItem(treeItem)
            || isVarGroupRootTreeItem(treeItem)
            || isDeviceRootTreeItem(treeItem);*/

    int itemtype = treeItem->data(treeItem->columnCount() - 1,Qt::UserRole).toInt();
    if(itemtype >= RTDBRootItem)
    {
        if(treeItem == m_rtdbRootTreeItem)
            return true;

        QTreeWidgetItem* parentItem = treeItem->parent();
        while(parentItem != NULL && parentItem != m_rtdbRootTreeItem)
        {
            parentItem = parentItem->parent();
        }
        if(parentItem == m_rtdbRootTreeItem)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
//    //增加了子节点
//    bool bRTDB = isRTDBRootTreeItem(treeItem)
//                    || isVarGroupRootTreeItem(treeItem)
//                    || isDeviceRootTreeItem(treeItem);

//    //检查子节点的data
//    if(!bRTDB)
//    {
//        int itemtype = treeItem->data(treeItem->columnCount() - 1,Qt::UserRole).toInt();
//        if(itemtype >= GroupRootItem)
//            bRTDB = true;
//    }
//    return bRTDB;
}
void CMDRTDBManagerUI::on_devicesWindow_destroyed(QObject* obj)
{
    if(m_devicesWindow!= NULL)
    {
        m_devicesWindow->deleteLater();
        m_devicesWindow = NULL;
    }
}

void CMDRTDBManagerUI::on_variablesWindow_destroyed(QObject* obj)
{
    if(m_variablesWindow!= NULL)
    {
        m_variablesWindow->deleteLater();
        m_variablesWindow = NULL;
    }
}
//!双击树控件，显示相应窗口
void CMDRTDBManagerUI::treeWidgetDoubleClick()
{
    QTreeWidgetItem* treeItem = getSelectTreeItem();

    if(!isRTDBTreeItem(treeItem))
        return;
     treeWidgetNodeMouseClick(treeItem);
     return;    
}
void CMDRTDBManagerUI::treeWidgetNodeMouseClick(QTreeWidgetItem *treeItem)
{
    if (!isRTDBTreeItem(treeItem))
        return;

    m_treeWidget->setCurrentItem(treeItem);
    m_curTreeItem = treeItem;
        //QTreeWidgetItem* treeItem = getSelectTreeItem();
    int itemtype = treeItem->data(treeItem->columnCount() - 1,Qt::UserRole).toInt();
    QString strItemText = getTreeNodeText(treeItem);
    if(itemtype >= DevRootItem)//device
    {
        if(m_devicesWindow == NULL)
        {
            m_devicesWindow = new CMDRTDBDevicesWindow();

            m_devicesWindow->setRootTreeItem(this,strItemText);
            m_devicesWindow->setProjectConfiger(m_projectConfiger);

            m_MDStudioInterface->openMDIWindow(m_devicesWindow);
            connect(m_devicesWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_devicesWindow_destroyed(QObject*)));
            connect(m_devicesWindow, SIGNAL(signal_Run()),this, SLOT(on_run()));
        }
        else
        {
            m_devicesWindow->unBindDeviceTreeModel();
            m_devicesWindow->setRootTreeItem(this,strItemText);
            m_devicesWindow->setProjectConfiger(m_projectConfiger);
        }
        if(strItemText.isEmpty())
            strItemText = tr("设备");
        m_devicesWindow->setWindowTitle(strItemText);
        m_MDStudioInterface->setActiveForm(m_devicesWindow);

        //是否有可粘贴项，刷新ToolBar
        m_devicesWindow->getDeviceWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),true,itemtype,m_copyObjItem);
    }
    else if(itemtype >= GroupRootItem)//Group
    {
        if(m_variablesWindow == NULL)
        {
            m_variablesWindow = new CMDRTDBVariablesWindow();
            m_variablesWindow->setRootTreeItem(this,strItemText);
            m_variablesWindow->setProjectConfiger(m_projectConfiger);

            m_MDStudioInterface->openMDIWindow(m_variablesWindow);
            connect(m_variablesWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_variablesWindow_destroyed(QObject*)));
            connect(m_variablesWindow, SIGNAL(signal_Run()),this, SLOT(on_run()));
        }
        else
        {
            m_variablesWindow->unBindVariableTreeModel();
            m_variablesWindow->setRootTreeItem(this,strItemText);
            m_variablesWindow->setProjectConfiger(m_projectConfiger);
        }
        if(strItemText.isEmpty())
            strItemText = tr("变量");
        m_variablesWindow->setWindowTitle(strItemText);
        m_MDStudioInterface->setActiveForm(m_variablesWindow);
        if(m_copyObjItem != NULL)
            m_variablesWindow->getVariableWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),true,itemtype,m_copyObjItem->itemType() == MDTreeItemType::VariableGroup? true:false);
        else
            m_variablesWindow->getVariableWidget()->refershActionStatus(m_mdRTDBManager->isRunning(),true,itemtype,false);
    }
    //!运行模式不显示属性
    if(m_mdRTDBManager->isRuntimeMode())
        return;

    //!显示节点属性
    if(itemtype == DevItem)
    {
        IDevice * dev = m_mdRTDBManager->driverManager()->getDevice(strItemText);
        //!设置属性框
        if(dev != NULL)
        {
            CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
            property->setObject(dev);
            property->setExpandAll();
        }
    }
    else if(itemtype == DevDBItem)
    {
        QTreeWidgetItem* parentItem = treeItem->parent();
        if(parentItem != NULL && m_MDStudioInterface != NULL)
        {
            QString strDevName = parentItem->text(0);
            IDevice * dev = m_mdRTDBManager->driverManager()->getDevice(strDevName);
            if(dev != NULL)
            {
                IDataBlock * db = dev->getDataBlock(treeItem->text(0));
                //!设置属性框
                if(db != NULL)
                {
                    CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
                    property->setObject(db);
                    property->setExpandAll();
                }
            }
        }
    }
    else if(itemtype == GroupItem)
    {
        IMDVariableGroup * group = m_mdRTDBManager->variableGroup()->getChildVariableGroupWithName(strItemText);
        if(group != NULL)
        {
            m_variableGroupProperty->setParentWidget(m_variablesWindow->getVariableWidget());
            m_variableGroupProperty->copy(group);
            CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
            property->setObject(m_variableGroupProperty);
            property->setExpandAll();
        }
    }
}
QString CMDRTDBManagerUI::getTreeNodeText(QTreeWidgetItem* curTreeItem)
{
    QString strItemText = "";
    int nType = curTreeItem->data(curTreeItem->columnCount() - 1,Qt::UserRole).toInt();
    if(nType >= DevRootItem)//device
    {
        if(nType == DevRootItem)
        {
            strItemText = "";
        }
        else
        {
            if(nType == DevItem)
            {
                strItemText = curTreeItem->text(0);
            }
            else
            {
                QTreeWidgetItem* parentItem = curTreeItem->parent();
                if(parentItem != NULL)
                    strItemText = parentItem->text(0) + "." + curTreeItem->text(0);
            }
        }
    }
    else
    {
        if(nType == GroupRootItem)
        {
            strItemText = "";
        }
        else
        {
            //text可能是嵌套多层的子Group
            QTreeWidgetItem* parenttreeItem = curTreeItem->parent();
            strItemText = curTreeItem->text(0);
            while(parenttreeItem != NULL && parenttreeItem->data(parenttreeItem->columnCount() - 1,Qt::UserRole).toInt() > GroupRootItem)
            {
                strItemText = parenttreeItem->text(0) + "." + strItemText;
                parenttreeItem = parenttreeItem->parent();
            }
        }
    }
    return strItemText;
}
void CMDRTDBManagerUI::customEvent(QEvent *e)
{
    if(e->type() == NameChangedEvent)
    {
        CCustomEvent* event = (CCustomEvent*)e;
        qulonglong handle = event->param().toULongLong();
        int fromModule = event->fromModule();

        //find the tree node
        QTreeWidgetItem* treeItem = NULL;
        if(fromModule == 1)//driver=device
            treeItem = getTreeNodeWithData(m_deviceRootTreeItem,handle);
        else
            treeItem = getTreeNodeWithData(m_varGroupRootTreeItem,handle);

        //change the text
        if(treeItem != NULL)
            treeItem->setText(0,event->value().toString());

        //如果当前树节点不是被改名节点，刷新右边视图
        if(m_curTreeItem != treeItem)
        {
            if(fromModule == 1)
            {
                CMDRTDBDevicesWidget* varWidget =  m_devicesWindow->getDeviceWidget();
                varWidget->refreshTreeView();
            }
        }
    }
}

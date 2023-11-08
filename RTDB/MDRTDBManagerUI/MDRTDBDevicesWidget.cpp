#include "MDRTDBDevicesWidget.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QKeyEvent>
#include <QLineEdit>
#include <QHeaderView>
#include "MDRTDBManagerFactory.h"
#include "MDRTDBManagerUI.h"
#include "MDRTDBDevicesWindow.h"
#include "AddDeviceDialog.h"
#include "IMDRTDBManager.h"
#include "MDRTDBManagerUI.h"
#include "MDMultiLanguage.h"
#include "DatablockMonitorDialog.h"
#include "PropertyBrowser.h"
#include "VariablePropertyDialog.h"
#include "MDRTDBVariableProperty.h"
CMDRTDBDevicesWidget::CMDRTDBDevicesWidget(QWidget *parent) : QWidget(parent)
{
    m_deviceTreeModel = NULL;
    m_devProxyModel = NULL;
    m_mdRTDBManager = NULL;
    m_projectConfiger = NULL;
    m_datablockMonitorDlg = NULL;
    m_toolBar = NULL;

    m_treeView = new QTreeView();
    //!不刷新计算所有行的行高
    m_treeView->setUniformRowHeights(true);
    m_treeView->setAlternatingRowColors(true);
    m_treeView->setAnimated(false);
    m_treeView->setAllColumnsShowFocus(true);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //!允许点击标题栏排序
    m_treeView->setSortingEnabled(true);

    //设置多选
    m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //按钮文本
    m_addDeviceText = tr("添加设备");
    m_addDataBlockText = tr("添加数据块");
    m_configText = tr("属性");
    m_deleteText = tr("删除");
    m_copyText = tr("复制");
    m_pasteText = tr("粘帖");
    m_runText = tr("配置->运行");
    m_packetText = tr("数据包监控");

    //多语言对象
    m_translateTool = CMDMultiLanguage::createWidgetTool();
    m_translateTool->init(this);

    initialMenu();

    //bind event
    connect(m_treeView, &QTreeView::clicked, this, &CMDRTDBDevicesWidget::treeViewClicked);
    connect(m_treeView, &QTreeView::doubleClicked, this, &CMDRTDBDevicesWidget::treeViewDoubleClicked);
    connect(m_treeView, &QTreeView::pressed, this, &CMDRTDBDevicesWidget::treeViewNodeMouseClicked);

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addWidget(m_treeView);
    m_mainLayout->setMargin(1);
    setLayout(m_mainLayout);

    //响应键盘事件
    this->setFocusPolicy(Qt::StrongFocus);

    m_datablockMonitorDlg = NULL;
    m_devName = "";
    m_curItem = NULL;
    m_rtdbUI = NULL;
    m_MDStudioInterface = NULL;
    m_copyObjItem = NULL;

    m_timer.setInterval(1000);
    m_timer.start();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));    
    m_variableProperty = new CMDRTDBVariableProperty();
}

//! 刷新变量列表控件
void CMDRTDBDevicesWidget::onTimerOut()
{
    if(m_mdRTDBManager->isRunning())
    {
        if(m_mdRTDBManager->isRunning() && m_devProxyModel != NULL)
        {
            emit m_devProxyModel->layoutChanged();
            emit m_devProxyModel->dataChanged(QModelIndex(), QModelIndex());
        }
    }
}

CMDRTDBDevicesWidget::~CMDRTDBDevicesWidget()
{
    if(m_deviceTreeModel!=NULL)
        delete m_deviceTreeModel;

    if(m_devProxyModel != NULL)
        delete m_devProxyModel;

    delete m_mainLayout;
    delete m_treeView;//Y2hw
    if(m_datablockMonitorDlg != NULL)
        delete m_datablockMonitorDlg;
    delete m_variableProperty;
}
bool CMDRTDBDevicesWidget::save()
{
    if(m_mdRTDBManager!= NULL)        
    {
        m_mdRTDBManager->saveDeviceConfiguration();
        return true;
    }
    return false;
}
void CMDRTDBDevicesWidget::setProjectConfiger(CMDProjectConfiger *projectConfiger)
{
    m_projectConfiger = projectConfiger;
    if(m_projectConfiger== NULL)
    {
       return;
    }
    m_mdRTDBManager = m_projectConfiger->nsRTDBManager();
    //m_deviceTreeModel = CMDRTDBManagerFactory::createDeviceTreeModel(m_mdRTDBManager);
    //m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/device.png"),MDTreeItemType::Device);
    //m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/datablock.png"),MDTreeItemType::DataBlock);
    //m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);
    //m_treeView->setModel(m_deviceTreeModel);
    m_curItem = m_mdRTDBManager->driverManager();
    if(!m_devName.isEmpty())
    {
        int nIndex = m_devName.indexOf('.');
        if(nIndex == -1)//dev
        {
            m_curItem = m_mdRTDBManager->driverManager()->getDevice(m_devName);
        }
        else//db
        {
            IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(m_devName.left(nIndex));
            m_curItem = pdev->getDataBlock(m_devName.mid(nIndex + 1));
        }
    }
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

    if(m_rtdbUI != NULL)
        m_MDStudioInterface = m_rtdbUI->MDStudioInterface();

    bindDeviceTreeModel();
}

void CMDRTDBDevicesWidget::refreshTreeView()
{
    emit m_devProxyModel->dataChanged(QModelIndex(), QModelIndex());
}

void CMDRTDBDevicesWidget::unBindDeviceTreeModel()
{
    if(m_deviceTreeModel!=NULL)
    {
        delete m_deviceTreeModel;
        m_deviceTreeModel= NULL;
    }

    if(m_devProxyModel != NULL)
    {
        delete m_devProxyModel;
        m_devProxyModel = NULL;
    }
}
void CMDRTDBDevicesWidget::setRootTreeItem(CMDRTDBManagerUI* RTDBUI, const QString& devName)
{
    m_devName = devName;
    m_rtdbUI = RTDBUI;
}
void CMDRTDBDevicesWidget::refershActionStatus(bool bRunning, bool bLeftTree, int nodeType, IMDTreeItem* copyItem)
{
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

    if(bLeftTree && nodeType== 999)//RTDB
    {
        m_actAddDevice->setEnabled(false);
        m_actAddDataBlock->setEnabled(false);
        m_actCopy->setEnabled(false);
        m_actPaste->setEnabled(false);
        m_actDelete->setEnabled(false);

        m_actRun->setEnabled(!m_mdRTDBManager->isRuntimeMode());
        m_actPacket->setEnabled(false);
        m_actConfig->setEnabled(false);
        m_toolBar->update();
        return;
    }
    bool bSelDev = false;//单个设备
    bool bSelDB = false;//单个数据块
    bool bMultiSelDB = false;//多数据块选中
    bool bMixSel = false;//混合选中
    bool bMultiSelDev = false;//多设备选中
    QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
    //批量转换选中的index
    IMDTreeItem *item = NULL;
    int nDBCount = 0;
    int nDevCount = 0;
    if(selectedList.count() == 1)
    {
        item = static_cast<IMDTreeItem*>((m_devProxyModel->mapToSource(selectedList[0])).internalPointer());
        if(item->itemType() == MDTreeItemType::DataBlock)
            bSelDB = true;
        else
            bSelDev = true;
    }
    else
    {
        for(int i = 0; i<selectedList.count(); i++)
        {
            item = static_cast<IMDTreeItem*>((m_devProxyModel->mapToSource(selectedList[i])).internalPointer());
            //检查是否是混合选择
            if(item->itemType() == MDTreeItemType::DataBlock)
            {
                if(nDevCount > 0)
                {
                    bMixSel = true;
                    break;
                }
                nDBCount++;
            }
            else if(item->itemType() == MDTreeItemType::VariableGroup)
            {
                if(nDBCount > 0)
                {
                    bMixSel = true;
                    break;
                }
                nDevCount++;
            }
        }
        //检查多选Var或Group
        if(nDevCount > 1 && nDBCount == 0)
            bMultiSelDev = true;
        else if(nDBCount > 1 && nDevCount == 0)
            bMultiSelDB = true;
    }

    if(m_mdRTDBManager->isRuntimeMode())
    {
        m_actAddDevice->setEnabled(false);
        m_actAddDataBlock->setEnabled(false);
        m_actCopy->setEnabled(false);
        m_actPaste->setEnabled(false);
        m_actDelete->setEnabled(false);
        m_actRun->setEnabled(false);
        //如果是数据块，可以监控数据包
        if(item->itemType() == MDTreeItemType::DataBlock)
        {
            m_actPacket->setEnabled(true);
        }
        else
        {
            m_actPacket->setEnabled(false);
        }
        m_actConfig->setEnabled(true);
        m_toolBar->update();
        return;
    }
    if(bRunning)
    {
        m_actAddDevice->setEnabled(false);
        m_actAddDataBlock->setEnabled(false);
        m_actCopy->setEnabled(false);
        m_actPaste->setEnabled(false);
        m_actDelete->setEnabled(false);
        m_actRun->setEnabled(true);
        if(m_curItem->itemType() == MDTreeItemType::DataBlock)
            m_actPacket->setEnabled(true);
        else
            m_actPacket->setEnabled(false);

        if(bLeftTree)
        {
            m_actConfig->setEnabled(m_curItem->itemType() != MDTreeItemType::DriverManager?true:false);
        }
        else
        {
            m_actConfig->setEnabled(bSelDev || bSelDB);
        }
    }
    else//配置
    {
        bool bDriverManager = m_curItem->itemType() == MDTreeItemType::DriverManager?true:false;

        m_actPacket->setEnabled(false);
        m_actRun->setEnabled(true);
        if(bLeftTree)
        {
            m_actAddDevice->setEnabled(true);
            m_actAddDataBlock->setEnabled(!bDriverManager);
            m_actCopy->setEnabled(!bDriverManager);

            IMDTreeItem * treeItem = NULL;
            if(copyItem != NULL)
                treeItem = copyItem;
            else if(m_copyObjItem != NULL)
                treeItem = m_copyObjItem;

            if(treeItem == NULL)
            {
                m_actPaste->setEnabled(false);
            }
            else
            {
                //任意节点都可粘贴设备
                if(treeItem->itemType() == MDTreeItemType::Device)
                {
                    m_actPaste->setEnabled(true);
                }
                else if(treeItem->itemType() == MDTreeItemType::DataBlock)
                {
                    //不是同一驱动不能粘贴数据块
                    IDevice* fromDev = (IDevice*)treeItem->parentItem(MDTreeItemType::Device);
                    IDevice* curDevice = NULL;
                    if(m_curItem->itemType() == MDTreeItemType::Device)
                        curDevice = (IDevice*)m_curItem;
                    else if(m_curItem->itemType() == MDTreeItemType::DataBlock)
                        curDevice = (IDevice*)m_curItem->parentItem(MDTreeItemType::Device);

                    if(curDevice != NULL && fromDev->driver() == curDevice->driver())
                        m_actPaste->setEnabled(true);
                    else
                        m_actPaste->setEnabled(false);
                }
                else
                {
                    m_actPaste->setEnabled(false);
                }
            }
            m_actDelete->setEnabled(!bDriverManager);
            m_actConfig->setEnabled(!bDriverManager);
        }
        else
        {
            if(bMixSel || bMultiSelDev || bMultiSelDB)
            {
                m_actAddDevice->setEnabled(false);
                m_actAddDataBlock->setEnabled(false);

                m_actCopy->setEnabled(false);
                m_actPaste->setEnabled(false);
                m_actDelete->setEnabled(true);
                m_actConfig->setEnabled(false);
            }
            else
            {
                m_actAddDevice->setEnabled(true);
                m_actAddDataBlock->setEnabled(!bDriverManager);
                m_actCopy->setEnabled(bSelDev || bSelDB);
                IMDTreeItem * treeItem = NULL;
                if(copyItem != NULL)
                    treeItem = copyItem;
                else if(m_copyObjItem != NULL)
                    treeItem = m_copyObjItem;

                if(treeItem == NULL)
                {
                    m_actPaste->setEnabled(false);
                }
                else
                {
                    //任意节点都可粘贴设备
                    if(treeItem->itemType() == MDTreeItemType::Device)
                    {
                        m_actPaste->setEnabled(true);
                    }
                    else if(treeItem->itemType() == MDTreeItemType::DataBlock)
                    {
                        //不是同一驱动不能粘贴数据块
                        IDevice* fromDev = (IDevice*)treeItem->parentItem(MDTreeItemType::Device);
                        IDevice* curDevice = NULL;
                        if(m_curItem->itemType() == MDTreeItemType::Device)
                            curDevice = (IDevice*)m_curItem;
                        else if(m_curItem->itemType() == MDTreeItemType::DataBlock)
                            curDevice = (IDevice*)m_curItem->parentItem(MDTreeItemType::Device);

                        if(curDevice != NULL && fromDev->driver() == curDevice->driver())
                            m_actPaste->setEnabled(true);
                        else
                            m_actPaste->setEnabled(false);
                    }
                    else
                    {
                        m_actPaste->setEnabled(false);
                    }
                }

                m_actDelete->setEnabled(bSelDev || bSelDB || bMultiSelDB|| bMultiSelDev || bMixSel);
                m_actConfig->setEnabled(bSelDev || bSelDB || m_curItem->itemType() != MDTreeItemType::DriverManager);
            }
        }
    }

    m_toolBar->update();
}

//!动态绑定树列表
void CMDRTDBDevicesWidget::bindDeviceTreeModel()
{
    if(m_mdRTDBManager!=NULL)
    {
        if(m_mdRTDBManager->isRunning())
        {
            m_treeView->setModel(NULL);
            if(m_deviceTreeModel!=NULL)
            {
                delete m_deviceTreeModel;
                m_deviceTreeModel= NULL;
            }

            if(m_devProxyModel != NULL)
            {
                delete m_devProxyModel;
                m_devProxyModel = NULL;
            }

            m_deviceTreeModel = CMDRTDBManagerFactory::createDeviceRunTreeModel(m_mdRTDBManager);
            m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/device.png"),MDTreeItemType::Device);
            m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/datablock.png"),MDTreeItemType::DataBlock);
            m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);
            m_deviceTreeModel->setCurRootTreeItem(m_devName);

            m_devProxyModel = new QSortFilterProxyModel();
            m_devProxyModel->setSourceModel(m_deviceTreeModel);
            m_treeView->setModel(m_devProxyModel);
        }
        else
        {
            m_treeView->setModel(NULL);
            if(m_deviceTreeModel!=NULL)
            {
                delete m_deviceTreeModel;
                m_deviceTreeModel= NULL;
            }

            if(m_devProxyModel != NULL)
            {
                delete m_devProxyModel;
                m_devProxyModel = NULL;
            }

            m_deviceTreeModel = CMDRTDBManagerFactory::createDeviceTreeModel(m_mdRTDBManager);

            m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/device.png"),MDTreeItemType::Device);
            m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/datablock.png"),MDTreeItemType::DataBlock);
            m_deviceTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);
            m_deviceTreeModel->setCurRootTreeItem(m_devName);

            m_devProxyModel = new QSortFilterProxyModel();
            m_devProxyModel->setSourceModel(m_deviceTreeModel);
            m_treeView->setModel(m_devProxyModel);
        }
    }
}

//!多语言
void CMDRTDBDevicesWidget::changeLanguage()
{
     if(m_deviceTreeModel!=NULL)
         m_deviceTreeModel->changeLanguage();

     if(m_datablockMonitorDlg != NULL)
         m_datablockMonitorDlg->changeLanguage();
}

//!初始化右键菜单
void CMDRTDBDevicesWidget::initialMenu()
{
    m_actAddDevice = new QAction(QIcon(":/images/device.png"), m_addDeviceText, this);
    m_actAddDataBlock = new QAction(QIcon(":/images/datablock.png"), m_addDataBlockText, this);
    m_actConfig = new QAction(QIcon(":/images/Config.png"), m_configText, this);
    m_actCopy = new QAction(QIcon(":/rs/image/copy"), m_copyText, this);
    m_actPaste = new QAction(QIcon(":/rs/image/Paste"), m_pasteText, this);
    m_actDelete = new QAction(QIcon(":/rs/image/delete"), m_deleteText, this);
    m_actRun = new QAction(QIcon(":/images/start.png"), m_runText, this);
    m_actPacket = new QAction(QIcon(":/images/packet.png"), m_packetText, this);

    m_actCopy->setShortcut(QKeySequence::Copy);
    m_actPaste->setShortcut(QKeySequence::Paste);
    m_actDelete->setShortcut(QKeySequence::Delete);

    connect(m_actAddDevice, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemAddDeviceClick);
    connect(m_actAddDataBlock, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemAddDataBlockClick);
    connect(m_actConfig, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemConfigClick);
    connect(m_actCopy, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemCopyClick);
    connect(m_actPaste, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemPasteClick);
    connect(m_actDelete, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemDeleteClick);
    connect(m_actRun, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemRunClick);
    connect(m_actPacket, &QAction::triggered, this, &CMDRTDBDevicesWidget::menuItemPacketClick);

    m_menu = new QMenu(m_treeView);
    /*
    m_menu->addAction(m_actAddDevice);
    m_menu->addAction(m_actAddDataBlock);
    m_menu->addAction(m_actDelete);
    m_menu->addSeparator();
    m_menu->addAction(m_actConfig);
    m_menu->addSeparator();
    m_menu->addAction(m_actCopy);
    m_menu->addAction(m_actPaste);
    m_menu->addAction(m_actRun);
    m_menu->addSeparator();
    m_menu->addAction(m_actPacket);
    */
    m_menu->addAction(m_actAddDevice);
    m_menu->addAction(m_actAddDataBlock);
    m_menu->addSeparator();

    m_menu->addAction(m_actCopy);
    m_menu->addAction(m_actPaste);
    m_menu->addAction(m_actDelete);
    m_menu->addSeparator();

    m_menu->addAction(m_actRun);
    m_menu->addAction(m_actPacket);
    m_menu->addAction(m_actConfig);

    //!多语言
    m_translateTool->changeLanguage();

    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeView, &QTreeView::customContextMenuRequested,
            this, &CMDRTDBDevicesWidget::menuStripNodeOpening);

    //!创建ToolBar
    if(m_toolBar == NULL)
    {
        m_toolBar = new QToolBar(tr("设备"),this);
        m_toolBar->addAction(m_actAddDevice);
        m_toolBar->addAction(m_actAddDataBlock);
        m_toolBar->addSeparator();

        m_toolBar->addAction(m_actCopy);
        m_toolBar->addAction(m_actPaste);
        m_toolBar->addAction(m_actDelete);
        m_toolBar->addSeparator();

        m_toolBar->addAction(m_actRun);
        m_toolBar->addAction(m_actPacket);
        m_toolBar->addAction(m_actConfig);
    }
}
QToolBar* CMDRTDBDevicesWidget::toolBar()
{
   return m_toolBar;
}

/*!
\section 点击树控件
*/
void CMDRTDBDevicesWidget::treeViewClicked(const QModelIndex &index)
{
    QModelIndex index0 = m_devProxyModel->mapToSource(index);
    m_selectedModelIndex = index0;
    treeViewNodeMouseClicked(index);
}

void CMDRTDBDevicesWidget::treeViewDoubleClicked(const QModelIndex &index)
{
    //m_selectedModelIndex = index;
    m_selectedModelIndex = m_devProxyModel->mapToSource(index);
    menuItemConfigClick();
    treeViewNodeMouseClicked(index);
}
void CMDRTDBDevicesWidget::treeViewNodeMouseClicked(const QModelIndex &index)
{
    if(m_devProxyModel == NULL || m_MDStudioInterface == NULL)
        return;

    QModelIndex index0 = m_devProxyModel->mapToSource(index);
    m_selectedModelIndex = index0;

    refershActionStatus(m_mdRTDBManager->isRunning(),false,0,0);
    if(!m_selectedModelIndex.isValid())
        return;

    IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
    if(item == NULL)
        return;

    m_curItem = item;

    CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
    if(item->itemType() == MDTreeItemType::Device)
    {
        IDevice * dev = (IDevice*)item;
        //!设置属性框
        if(dev != NULL)
        {
            property->setObject(dev);
        }
    }
    else if(item->itemType() == MDTreeItemType::DataBlock)
    {
        IDataBlock * db = (IDataBlock*)item;
        //!设置属性框
        if(db != NULL)
        {
            property->setObject(db);
        }
    }
    else if(item->itemType() == MDTreeItemType::Variable)
    {
        /*
        IMDRTDBVariable * var = (IMDRTDBVariable*)item;
        //!设置属性框
        if(var != NULL)
        {
            CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
            property->setObject(var);
        }*/

        IMDRTDBVariable * var = (IMDRTDBVariable*)item;

        //!设置属性框
        if(var != NULL)
        {
            m_variableProperty->setRTDBManager(m_mdRTDBManager);
            m_variableProperty->setPropertyBrowser(m_MDStudioInterface->propertyBrowser());

            m_variableProperty->copy(var);
            //m_variableProperty->setParentWidget(this);

            //设备名属性设置在后面便于刷新属性框的设备名属性
            m_variableProperty->setDeviceNameList(m_deviceNameList);

            CPropertyBrowser* property = m_MDStudioInterface->propertyBrowser();
            property->setObject(m_variableProperty);
            property->resetEnums("DeviceName",m_deviceNameList);
            property->updateProperty();
        }
    }
    if(m_mdRTDBManager->isRunning() || item->itemType() == MDTreeItemType::Variable)
        property->setPropertyReadOnly();
    else
        property->setPropertyReadOnly(false);
    property->setExpandAll();
}

/*!
\section 键盘事件
*/
void CMDRTDBDevicesWidget::keyPressEvent( QKeyEvent *k )
{
    //key事件中一定要判断节点是否存在和有效
    if(m_treeView != NULL)
    {
        QModelIndex index = m_treeView->currentIndex();
        QModelIndex index0 = m_devProxyModel->mapToSource(index);

        if (index.isValid())
        {
            if(m_mdRTDBManager->isRuntimeMode())
            {
                return QWidget::keyPressEvent(k);
            }
            m_selectedModelIndex = index0;
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
//!右键菜单
void CMDRTDBDevicesWidget::menuStripNodeOpening(const QPoint &pos)
{
    //m_translateTool->changeLanguage();

    m_actAddDevice->setText(CMDMultiLanguage::instance()->value(m_addDeviceText));
    m_actAddDataBlock->setText(CMDMultiLanguage::instance()->value(m_addDataBlockText));
    m_actConfig->setText(CMDMultiLanguage::instance()->value(m_configText));
    m_actCopy->setText(CMDMultiLanguage::instance()->value(m_copyText));
    m_actPaste->setText(CMDMultiLanguage::instance()->value(m_pasteText));
    m_actDelete->setText(CMDMultiLanguage::instance()->value(m_deleteText));

    m_actPacket->setText(CMDMultiLanguage::instance()->value(m_packetText));


    QModelIndex index0 = m_treeView->indexAt(pos);
    m_selectedModelIndex = m_devProxyModel->mapToSource(index0);

    refershActionStatus(m_mdRTDBManager->isRunning(),false,0,0);
    m_menu->exec(QCursor::pos());
    return;
    /*
    if (index.isValid() || m_curItem != NULL)
    {
        IMDTreeItem *item = NULL;
        if(!index.isValid())
        {
            item = m_curItem;
        }
        else
        {
            item = static_cast<IMDTreeItem*>(index.internalPointer());
        }
        if(m_mdRTDBManager->isRuntimeMode())
        {
            m_actAddDevice->setVisible(false);
            m_actAddDataBlock->setVisible(false);
            m_actConfig->setVisible(true);
            m_actCopy->setVisible(false);
            m_actPaste->setVisible(false);
            m_actDelete->setVisible(false);
            m_actRun->setVisible(false);
            if(item->itemType() == MDTreeItemType::DataBlock)
            {
                IDataBlock* dataBlock = (IDataBlock*)item;
                if(dataBlock->msgMonitor())
                     m_actPacket->setText(CMDMultiLanguage::instance()->value(tr("停止监控")));
                else
                     m_actPacket->setText(CMDMultiLanguage::instance()->value(tr("数据包监控")));
            }
            else
            {
                m_actPacket->setVisible(false);
            }
            m_menu->exec(QCursor::pos());
            m_selectedModelIndex = index;
            return;
        }
        if(item->itemType() == MDTreeItemType::DriverManager)
        {
            if(m_mdRTDBManager->isRunning())
            {
                m_actAddDevice->setVisible(false);
                m_actAddDataBlock->setVisible(false);
                m_actConfig->setVisible(true);
                m_actCopy->setVisible(false);
                m_actPaste->setVisible(false);
                m_actDelete->setVisible(false);
                m_actRun->setVisible(true);
                m_actPacket->setVisible(false);
               m_actRun->setIcon(QIcon(":/images/stop.png"));
               m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
            }
            else
            {
                m_actAddDevice->setVisible(true);
                m_actAddDataBlock->setVisible(false);

                m_actCopy->setVisible(true);
                //判断是否有粘贴,且需是同一驱动
                if(m_copyObjItem != NULL)
                    m_actPaste->setVisible(true);
                else
                    m_actPaste->setVisible(false);
                m_actDelete->setVisible(false);

                m_actRun->setVisible(true);
                m_actPacket->setVisible(false);

                m_actConfig->setVisible(true);
                m_actRun->setIcon(QIcon(":/images/start.png"));
                m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
            }
            m_menu->exec(QCursor::pos());
            m_selectedModelIndex = index;
        }
        else if(item->itemType() == MDTreeItemType::Device)
        {
            if(m_mdRTDBManager->isRunning())
            {
                m_actAddDevice->setVisible(false);
                m_actAddDataBlock->setVisible(false);
                m_actConfig->setVisible(true);
                m_actCopy->setVisible(false);
                m_actPaste->setVisible(false);
                m_actDelete->setVisible(false);
                m_actRun->setVisible(true);
                m_actPacket->setVisible(false);
            }
            else
            {
                m_actAddDevice->setVisible(true);
                m_actAddDataBlock->setVisible(true);

                m_actCopy->setVisible(true);
                //判断是否有粘贴
                if(m_copyModelIndex.isValid())
                    m_actPaste->setVisible(true);
                else
                    m_actPaste->setVisible(false);
                m_actDelete->setVisible(true);

                m_actRun->setVisible(true);
                m_actPacket->setVisible(false);

                m_actConfig->setVisible(true);
            }
            m_menu->exec(QCursor::pos());
            m_selectedModelIndex = index;
        }
        else if(item->itemType() == MDTreeItemType::DataBlock)
        {
            if(m_mdRTDBManager->isRunning())
            {
                m_actAddDevice->setVisible(false);
                m_actAddDataBlock->setVisible(false);

                m_actCopy->setVisible(false);
                m_actPaste->setVisible(false);
                m_actDelete->setVisible(false);

                m_actRun->setVisible(true);
                m_actPacket->setVisible(true);

                m_actConfig->setVisible(true);
               m_actRun->setIcon(QIcon(":/images/stop.png"));
               m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));

               //获取数据包监控标志设置显示文本               
               IDataBlock* dataBlock = (IDataBlock*)item;
               if(dataBlock->msgMonitor())
                    m_actPacket->setText(CMDMultiLanguage::instance()->value(tr("停止监控")));
               else
                    m_actPacket->setText(CMDMultiLanguage::instance()->value(tr("开始监控")));

            }
            else
            {
                m_actAddDevice->setVisible(true);
                m_actAddDataBlock->setVisible(true);

                m_actCopy->setVisible(true);
                //检查copy对象是否为DataBlock
                if(m_copyModelIndex.isValid())
                {
                    IMDTreeItem *itemcopy = static_cast<IMDTreeItem*>(m_copyModelIndex.internalPointer());
                    if(itemcopy->itemType() == MDTreeItemType::DataBlock)
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
                m_actPacket->setVisible(false);

                m_actConfig->setVisible(true);
                m_actRun->setIcon(QIcon(":/images/start.png"));
                m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
            }
            m_menu->exec(QCursor::pos());
            m_selectedModelIndex = index;
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
            m_actAddDevice->setVisible(false);
            m_actAddDataBlock->setVisible(false);
            m_actConfig->setVisible(false);
            m_actCopy->setVisible(false);
            m_actPaste->setVisible(false);
            m_actDelete->setVisible(false);
            m_actRun->setVisible(true);
            m_actPacket->setVisible(false);
            m_actRun->setIcon(QIcon(":/images/stop.png"));
            m_actRun->setText(CMDMultiLanguage::instance()->value(tr("调试->配置")));
        }
        else
        {
            m_actAddDevice->setVisible(true);
            m_actAddDataBlock->setVisible(true);

            m_actCopy->setVisible(false);
            //判断是否待粘贴的是否是Device
            if(m_copyModelIndex.isValid())
            {
                IMDTreeItem *itemcopy = static_cast<IMDTreeItem*>(m_copyModelIndex.internalPointer());
                if(itemcopy->itemType() == MDTreeItemType::Device)
                {
                    m_actPaste->setVisible(true);
                }
                else
                {
                    m_actPaste->setVisible(false);
                }
            }
            else
            {
                m_actPaste->setVisible(false);
            }

            m_actDelete->setVisible(false);
            m_actRun->setVisible(true);
            m_actRun->setIcon(QIcon(":/images/start.png"));
            m_actRun->setText(CMDMultiLanguage::instance()->value(tr("配置->调试")));
            m_actConfig->setVisible(false);
            m_actPacket->setVisible(false);
        }
        m_menu->exec(QCursor::pos());
        m_selectedModelIndex = index;
    }
    m_toolBar->update();
    */
}

void CMDRTDBDevicesWidget::menuItemRunClick()
{
    m_copyObjItem = NULL;
    refershActionStatus(!m_mdRTDBManager->isRunning(),false,0,0);
    emit this->signal_Run();
}
void CMDRTDBDevicesWidget::menuItemPacketClick()
{
    IMDTreeItem* db = NULL;
    if(!m_selectedModelIndex.isValid())
    {
        if(m_curItem != NULL && m_curItem->itemType() == MDTreeItemType::DataBlock)
            db = m_curItem;
    }
    else
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item != NULL &&  item->itemType() == MDTreeItemType::DataBlock)
            db = item;
    }
    if(db != NULL)
    {
        IDataBlock* dataBlock = (IDataBlock*)db;
        if(m_datablockMonitorDlg == NULL)
            m_datablockMonitorDlg = new CDataBlockMonitorDialog(this);

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
    }    
}

void CMDRTDBDevicesWidget::menuItemAddDeviceClick()
{
    if(m_mdRTDBManager == NULL)
    {
        return;
    }
    CAddDeviceDialog dlg(this);
    dlg.setDriverManager(m_mdRTDBManager->driverManager());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        emit m_deviceTreeModel->layoutChanged();
        emit this->signal_Modified();
        if(m_rtdbUI != NULL)
        {
            IDevice* dev = m_mdRTDBManager->driverManager()->getDevice(dlg.getDeviceName());
            m_rtdbUI->refreshTreeNode(false,(qulonglong)m_mdRTDBManager->driverManager(),(qulonglong)dev);
        }
    }
}

void CMDRTDBDevicesWidget::menuItemAddDataBlockClick()
{
    m_copyObjItem = NULL;
    IMDTreeItem *newDBItem = NULL;
    qulonglong uFocusNode = 0;//左边树展开节点
    IDevice* pdev= NULL;

    if (m_selectedModelIndex.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item->itemType() == MDTreeItemType::Device)
        {
            emit m_deviceTreeModel->layoutAboutToBeChanged();
            newDBItem = (IMDTreeItem*)((IDevice*)item)->createDataBlock(this);
            uFocusNode = (qulonglong)item;
            pdev = (IDevice*)item;
            emit m_deviceTreeModel->layoutChanged();
            emit this->signal_Modified();
        }
        else if(item->itemType() == MDTreeItemType::DataBlock)
        {
            //获取Device
            emit m_deviceTreeModel->layoutAboutToBeChanged();
            IDevice* dev = (IDevice*)item->parentItem(MDTreeItemType::DataBlock);
            newDBItem = (IMDTreeItem*)dev->createDataBlock(this);
            uFocusNode = (qulonglong)dev;
            pdev = dev;
            emit m_deviceTreeModel->layoutChanged();
            emit this->signal_Modified();
        }
    }
    else//点击空白处添加数据块
    {
        //!检查左边树节点属性
        if(m_curItem != NULL)
        {
            if(m_curItem->itemType() == MDTreeItemType::DataBlock)
                pdev = (IDevice*)m_curItem->parentItem(MDTreeItemType::Device);
            else
                pdev = (IDevice*)m_curItem;
            if(pdev != NULL)
            {
                emit m_deviceTreeModel->layoutAboutToBeChanged();
                newDBItem = (IMDTreeItem*)pdev->createDataBlock(this);
                uFocusNode = (qulonglong)pdev;
                emit m_deviceTreeModel->layoutChanged();
                emit this->signal_Modified();
            }
        }
    }

    if(NULL != newDBItem && pdev != NULL)
    {
        m_rtdbUI->refreshTreeNode(false,(qulonglong)pdev,uFocusNode);
    }
}
void CMDRTDBDevicesWidget::menuItemConfigClick()
{
    m_copyObjItem = NULL;
    IMDTreeItem *item = NULL;
    if(!m_selectedModelIndex.isValid())
    {
        if(m_curItem != NULL)
            item = m_curItem;
    }
    else
    {
        item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
    }
    if (item != NULL)
    {
        if(item->itemType() == MDTreeItemType::Device)
        {
            emit m_deviceTreeModel->layoutAboutToBeChanged();
            ((IDevice*)item)->configDecice(this);
            emit m_deviceTreeModel->layoutChanged();
            emit this->signal_Modified();
            //!刷新左边树节点
            if(m_rtdbUI != NULL)
                m_rtdbUI->refreshTreeNode(false,(qulonglong)m_curItem,(qulonglong)item);
        }
        else if(item->itemType() == MDTreeItemType::DataBlock)
        {
            IDataBlock* dataBlock = (IDataBlock*)item;
            IDevice* device = (IDevice* )dataBlock->parentItem(MDTreeItemType::Device);
            if(device)
            {
               emit m_deviceTreeModel->layoutAboutToBeChanged();
               device->configDataBlock(dataBlock,this);
               emit m_deviceTreeModel->layoutChanged();
               emit this->signal_Modified();
               //!刷新左边树节点
               if(m_rtdbUI != NULL)
               {
                    m_rtdbUI->refreshTreeNode(false,(qulonglong)device,(qulonglong)device);
               }
            }
        }
        else if(item->itemType() == MDTreeItemType::Variable)//可以查看，不可更改
        {
            IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
            if(rtdbVariable->variableType()!= MDVariableType::SystemVariable)
            {
                CVariablePropertyDialog dlg(this);
                dlg.setProjectConfiger(m_projectConfiger);
                dlg.setCurrentVariable(rtdbVariable);
                dlg.setRunModel(m_mdRTDBManager->isRunning());
                IMDVariableGroup* group = (IMDVariableGroup*)item->parentItem(MDTreeItemType::VariableGroup);
                dlg.setCurrentVariableGroup(group);
                dlg.setRunModel(true);
                dlg.initial();
                dlg.exec();
            }
        }
    }
}

void CMDRTDBDevicesWidget::menuItemCopyClick()
{
    if(m_selectedModelIndex.isValid())
    {
        m_rtdbUI->clearCopyItem();
        m_copyObjItem = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        refershActionStatus(m_mdRTDBManager->isRunning(),false,0,0);
    }
    else
    {
        m_rtdbUI->menuItemCopyClick();
    }
}

void CMDRTDBDevicesWidget::clearCopyItem()
{
    m_copyObjItem = NULL;
}

void CMDRTDBDevicesWidget::menuItemPasteClick()
{
    if(m_copyObjItem == NULL)
    {
        m_rtdbUI->menuItemPasteClick();
        return;
    }

    IDevice* parentDevice  = NULL;
    IMDTreeItem *theParentItem = NULL;
    IMDTreeItem *theFocusItem = NULL;
    bool bRefresh = false;
    qulonglong uFocusNode = 0;//左边树展开节点
    if( m_selectedModelIndex.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item->itemType() == MDTreeItemType::Device)
        {
             parentDevice = (IDevice*)item;
        }
        else if(item->itemType() == MDTreeItemType::DataBlock)
        {
            IDataBlock * db = (IDataBlock*)item;
            parentDevice = (IDevice*)db->parentItem(MDTreeItemType::Device);
        }

        //粘贴Device
        if(m_copyObjItem->itemType() == MDTreeItemType::Device)
        {
            IDevice* device = (IDevice*)m_copyObjItem;
            emit m_deviceTreeModel->layoutAboutToBeChanged();
            theFocusItem = (IMDTreeItem*)m_mdRTDBManager->driverManager()->copyPasteDevice(device);
            uFocusNode = (qulonglong)theFocusItem;
            theParentItem = (IMDTreeItem*)m_mdRTDBManager->driverManager();
            emit this->signal_Modified();
        }
        else if(m_copyObjItem->itemType() == MDTreeItemType::DataBlock && NULL!=parentDevice)//粘贴数据块
        {
            IDataBlock * db = (IDataBlock*)m_copyObjItem;
            emit m_deviceTreeModel->layoutAboutToBeChanged();
            theFocusItem = (IMDTreeItem*)parentDevice->copyPasteDataBlock(db);
            uFocusNode = (qulonglong)theFocusItem;
            theParentItem = (IMDTreeItem*)parentDevice;
            emit this->signal_Modified();
        }
    }
    else//没有被粘贴的对象，默认到当前节点的父节点
    {
        if(m_copyObjItem->itemType() == MDTreeItemType::Device)
        {
            IDevice* device = (IDevice*)m_copyObjItem;
            emit m_deviceTreeModel->layoutAboutToBeChanged();
            theFocusItem = (IMDTreeItem*)m_mdRTDBManager->driverManager()->copyPasteDevice(device);
            theParentItem = (IMDTreeItem*)m_mdRTDBManager->driverManager();
            uFocusNode = (qulonglong)theFocusItem;
            emit this->signal_Modified();
        }
        else if(m_copyObjItem->itemType() == MDTreeItemType::DataBlock)
        {
            if(m_curItem->itemType() == MDTreeItemType::Device)
                parentDevice = (IDevice*)m_curItem;
            else if(m_curItem->itemType() == MDTreeItemType::DataBlock)
                parentDevice = (IDevice*)m_curItem->parentItem(MDTreeItemType::Device);

            if(parentDevice != NULL)
            {
                IDataBlock * db = (IDataBlock*)m_copyObjItem;
                IDevice* device = (IDevice*)parentDevice;
                emit m_deviceTreeModel->layoutAboutToBeChanged();
                theParentItem = parentDevice;
                theFocusItem = (IMDTreeItem*)device->copyPasteDataBlock(db);
                uFocusNode = (qulonglong)theFocusItem;
                emit this->signal_Modified();
            }
        }
    }
    //结束操作，刷新界面
    if(theFocusItem != NULL)
        bRefresh = true;
    emit m_deviceTreeModel->layoutChanged();
    if(bRefresh)
    {
        if(m_rtdbUI != NULL)
            m_rtdbUI->refreshTreeNode(false,(qulonglong) theParentItem,uFocusNode);
        m_treeView->selectionModel()->clear();
    }
}
void CMDRTDBDevicesWidget::menuItemDeleteClick()
{
    //如果是左边树+toolbar
    if(!m_selectedModelIndex.isValid())
    {
        m_rtdbUI->menuItemDeleteClick();
        return;
    }

    //以前只支持单选，现可以多选
    //如果删除了Group，需刷新左边的树
    QModelIndexList selectedList = m_treeView->selectionModel()->selectedRows();
    QModelIndexList selectedMapList;
    //批量转换选中的index
    for(int i = 0; i<selectedList.count(); i++)
    {
        selectedMapList.append(m_devProxyModel->mapToSource(selectedList[i]));
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
            if(item->itemType() == MDTreeItemType::Device)
            {
                IDevice* pdev = (IDevice*)item;
                str0 = tr("确定要删除设备");
                strTranslate = CMDMultiLanguage::instance()->value(str0);
                str = QString("%1：%2 ?").arg(strTranslate).arg(pdev->name());
            }
            else
            {
                IDataBlock* pDB =(IDataBlock*)item;
                str0 = tr("确定要删除数据块");
                strTranslate = CMDMultiLanguage::instance()->value(str0);
                str = QString("%1：%2 ?").arg(strTranslate).arg(pDB->name());
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
            emit m_deviceTreeModel->layoutAboutToBeChanged();
            while(selectedMapList.count() > 0)
            {
                itemIndex0 = selectedMapList[selectedMapList.count() - 1];
                item = static_cast<IMDTreeItem*>(itemIndex.internalPointer());
                if(m_copyObjItem == item)
                {
                    m_copyObjItem = NULL;
                    bdelCopyItem = true;
                }


                m_deviceTreeModel->deleteTreeItem(itemIndex0);

                selectedList = m_treeView->selectionModel()->selectedRows();
                //批量转换选中的index
                selectedMapList.clear();
                for(int i = 0; i<selectedList.count(); i++)
                {
                    selectedMapList.append(m_devProxyModel->mapToSource(selectedList[i]));
                }
            }
            emit m_deviceTreeModel->layoutChanged();
            emit this->signal_Modified();
            if(bdelCopyItem)
            {
                m_actPaste->setVisible(false);
                m_toolBar->update();
            }
            //刷新左边树
            if(m_rtdbUI != NULL)
                 m_rtdbUI->refreshTreeNode(false,(qulonglong)m_curItem,(qulonglong)m_curItem);

            m_selectedModelIndex = QModelIndex();
        }
    }//end if(selectedList.count() > 0)
    /*
    if (m_selectedModelIndex.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(m_selectedModelIndex.internalPointer());
        if(item->itemType() == MDTreeItemType::Device)
        {
            IDevice* device = (IDevice*)item;
            //****************************** Y2hw *****************************
            //QString str = QString("确定要删除设备：%1 ?").arg(device->name());
            //QMessageBox::StandardButton reply = QMessageBox::question(this, "确认", str, QMessageBox::Yes | QMessageBox::No);
            QString str0 = tr("确定要删除设备");
            QString strTranslate = CMDMultiLanguage::instance()->value(str0);
            QString str = QString("%1：%2 ?").arg(strTranslate).arg(device->name());

            QMessageBox box;
            box.setText(str);
            box.setWindowTitle(CMDMultiLanguage::instance()->value(tr("确认")));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            CMDMultiLanguage::instance()->translateWidget(&box);

            int reply = box.exec();
            //----------------------------------------------------------------
            if(reply == QMessageBox::Yes)
            {
                if(m_copyModelIndex == m_selectedModelIndex)
                    m_copyModelIndex = QModelIndex();

                emit m_deviceTreeModel->layoutAboutToBeChanged();
                m_deviceTreeModel->deleteTreeItem(m_selectedModelIndex);
                emit m_deviceTreeModel->layoutChanged();

                m_selectedModelIndex = QModelIndex();
                if(m_rtdbUI != NULL)
                    m_rtdbUI->refreshTreeNode(false,(qulonglong)m_curItem,(qulonglong)m_curItem);
            }
        }
        else if(item->itemType() == MDTreeItemType::DataBlock)
        {
            //!可能删除多个DataBlock
            QModelIndex devIndex = m_selectedModelIndex.parent();
            IDataBlock* db =(IDataBlock *)item;
            IMDTreeItem* dev = (IMDTreeItem*)db->parentItem(MDTreeItemType::Device);
            //****************************** Y2hw *****************************

            //QString str = QString("确定要删除数据块：%1 ?").arg(db->name());
            //QMessageBox::StandardButton reply = QMessageBox::question(this, "确认", str, QMessageBox::Yes | QMessageBox::No);

            QString str0 = tr("确定要删除数据块");
            QString strTranslate = CMDMultiLanguage::instance()->value(str0);
            QString str = QString("%1：%2 ?").arg(strTranslate).arg(db->name());

            QMessageBox box;
            box.setText(str);
            box.setWindowTitle(CMDMultiLanguage::instance()->value(tr("确认")));
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            CMDMultiLanguage::instance()->translateWidget(&box);

            int reply = box.exec();
            //--------------------------------------------------------------
            if(reply == QMessageBox::Yes)
            {
                if(m_copyModelIndex == m_selectedModelIndex)
                    m_copyModelIndex = QModelIndex();
                QList<QPersistentModelIndex> parentList;
                QPersistentModelIndex dd = QPersistentModelIndex(devIndex);
                parentList.append(dd);

                emit m_deviceTreeModel->layoutAboutToBeChanged(parentList);
                m_deviceTreeModel->deleteTreeItem(m_selectedModelIndex);

                emit m_deviceTreeModel->layoutChanged(parentList);

                m_selectedModelIndex = QModelIndex();
                if(m_rtdbUI != NULL)
                    m_rtdbUI->refreshTreeNode(false,(qulonglong)m_curItem,(qulonglong)dev);

            }            
        }
    }*/
}



/*!
\author enli 2015.01.08
\section 配置模式管理器，由tang调用CMDRTDBManagerUIFactory::createMDRTDBManagerUI()创建
*/
#ifndef MDRTDBMANAGERUI_H
#define MDRTDBMANAGERUI_H

#include "IMDRTDBManagerUI.h"
#include <qtreewidget.h>
#include "MDProjectConfiger.h"
#include "IMDVariableGroup.h"
#include "IDevice.h"

#include "interface/Studio/IStudioInterface.h"

class CMDRTDBVariablesWindow;
class CMDRTDBDevicesWindow;
class IMDRTDBManager;
class CDataBlockMonitorDialog;
class CMDVariableGroupProperty;

class  CMDRTDBManagerUI:public IMDRTDBManagerUI
{
    Q_OBJECT
public:
    CMDRTDBManagerUI();
    ~CMDRTDBManagerUI();

/*!
section Studio 接口
*/
    //!根节点名称
    QString rootName();
    //!根节点图标
    QIcon* rootIcon();
    //!获取、设置树根节点
    QTreeWidgetItem* rootItem();
    void setRootItem(QTreeWidgetItem* value);
    //!设置Studio接口
    void setStudioInterface(IStudioInterface* value);

    //!加载
    void load();
    //!卸载
    void unload();

    //!多语言
    void changeLanguage();
    void collectWords();

/*!
\section 接口
*/
public:
    //!设置项目关联的项目信息对象
    //void setProjectConfiger(CMDProjectConfiger *projectConfiger) ;
    //void setMDIMoudleParam(IMDIMoudleParam *mdiMoudleParam) ;
    IStudioInterface * MDStudioInterface();
    //!显示变量选择对话框，并显示变量
    bool selectVariables(QList<IMDVariable*>&varList);
    bool selectVariables(QStringList &varNameList);
private:
    //!设备和变量tab停靠Frame
    IStudioInterface *m_MDStudioInterface;
    CMDProjectConfiger *m_projectConfiger;
//////////////////////////////////////////////////////////////////////////
//!文本信息
private:
//////////////////////////////////////////////////////////////////////////
//!实时数据库TreeWidget,左边树控件
public:
    void initialRTDBRootTreeItem();
    void clearTreeItem(QTreeWidgetItem *treeItem);
    bool isRTDBTreeItem(QTreeWidgetItem *treeItem);
    bool isRTDBRootTreeItem(QTreeWidgetItem* treeItem);

    //!获取当前Item
    QTreeWidgetItem *getSelectTreeItem();
    IMDTreeItem* getCopyObjItem();
    //!从m_projectConfiger获取
    IMDRTDBManager *nsRTDBManager();
     void  debug();
    void customEvent(QEvent *e);
    //!刷新Tree
    void refreshTree();
    void refreshTreeNode(bool bVar, qulonglong nodeData, qulonglong expandNodeData);
    //!当在List中复制某项后，清除左边树维护的复制项
    void clearCopyItem();
    //!xxxWidget的ToolBar会调到这里
    void menuItemDeleteClick();
    void menuItemCopyClick();
    void menuItemPasteClick();    
    void menuItemPacketClick();
private:
     //!树控件，从IStudioInterface获取
    QTreeWidget *m_treeWidget;
    //!实时数据库节点
    QTreeWidgetItem * m_rtdbRootTreeItem;
    //!变量节点
    QTreeWidgetItem *m_varGroupRootTreeItem;
    //!设备节点
    QTreeWidgetItem *m_deviceRootTreeItem;
    //!双击变量节点，显示窗口
    CMDRTDBVariablesWindow *m_variablesWindow;
    //!双击设备节点，显示窗口
    CMDRTDBDevicesWindow *m_devicesWindow;
    //!数据块监控窗口
    CDataBlockMonitorDialog* m_datablockMonitorDlg;
private:
    //!添加group到左边树结构,uselTreeNode:需要展开的节点，selTreeItem：需要展开节点句柄
    void setChildGroupTreeItem(IMDVariableGroup* parentGroup,
                                           QTreeWidgetItem * parentTreeItem,
                                           qulonglong uselTreeNode,
                                           QTreeWidgetItem ** selTreeItem);
    //!添加Dev及DB到左边树结构,curItemText:待选中的节点名,返回被选中的节点
    QTreeWidgetItem* setChildDBTreeItem(IDevice* parentDev,
                                        QTreeWidgetItem * parentTreeItem,
                                        qulonglong uselTreeNode);

    //!添加右键菜单
    void initialTreeItemMenu();
    //!获取坐标树节点文本
    QString getTreeNodeText(QTreeWidgetItem* curTreeItem);
    //!获取树节点对象,nodeData:whatisthis 句柄,展开节点句柄expandNodeData
    QTreeWidgetItem* getTreeNodeWithData(QTreeWidgetItem* fromTreeItem, qulonglong nodeData);
//////////////////////////////////////////////////////////////////////////
//treeWidget event
private:
    //!当前复制项
    QTreeWidgetItem* m_copyTreeItem;
    //对象句柄，有时重新刷新Tree后m_copyTreeItem会变化；可以在List面粘贴
    IMDTreeItem*     m_copyObjItem;
    QTreeWidgetItem* m_curTreeItem;
    QAction* m_actDelete;
    QAction* m_actCopy;
    QAction* m_actPaste;
    QAction* m_actAddDevice;
    QAction* m_actAddDataBlock;
    QAction* m_actAddGroup;
    QAction* m_actDBPacket;

    QString m_addDeviceText;
    QString m_addDataBlockText;
    QString m_addGroupText;
    QString m_deleteText;
    QString m_copyText;
    QString m_pasteText;
    QString m_DBPacketStartText;
    QString m_DBPacketStopText;

    QMenu* m_menu;


    void menuItemAddDeviceClick();
    void menuItemAddDataBlockClick();    
    void menuItemAddVarGroupClick();
    void menuStripNodeOpening(const QPoint &pos);
    //!键盘事件
    void keyPressEvent( QKeyEvent *k );
private:
    void treeWidgetDoubleClick();
    void treeWidgetNodeMouseClick(QTreeWidgetItem* treeItem);
private slots:
    void on_devicesWindow_destroyed(QObject* obj);
    void on_variablesWindow_destroyed(QObject* obj);
    //!由m_variablesWindow或m_devicesWindow触发
    void on_run();

//实时数据库
private:
    IMDRTDBManager *m_mdRTDBManager;
    QString m_rtdbRootTreeItemName = "实时数据库";
    QString m_varGroupRootTreeItemName = "变量";
    QString m_deviceRootTreeItemName ="设备";
    CMDVariableGroupProperty* m_variableGroupProperty;
};

#endif // MDRTDBMANAGERUI_H

/*!
\author enli 2015.01.08
\section 设备管理HMI，，实现右键功能由CMDRTDBDevicesWindow（配置模式）和CMDRTDBRuntimeDialog创建（运行模式）
*/

#ifndef CMDRTDBDEVICESWIDGET_H
#define CMDRTDBDEVICESWIDGET_H
#include <QWidget>
#include <QTreeView>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QToolBar>

#include "IMDTreeModel.h"
#include "IMDRTDBManager.h"
#include "MDProjectConfiger.h"
#include "MDRTDBManagerUI.h"



class ITranslateWidgetTool;
class CDataBlockMonitorDialog;
class CMDRTDBVariableProperty;

class CMDRTDBDevicesWidget  : public QWidget
{
    Q_OBJECT
public:
    explicit CMDRTDBDevicesWidget(QWidget *parent = 0);
    ~CMDRTDBDevicesWidget();

public:
    //!触发m_mdRTDBManager保存配置
    bool save();
    QToolBar* toolBar();
public:
    void setProjectConfiger(CMDProjectConfiger *projectConfiger);
    //!刷新数据，model不变
    void refreshTreeView();
    //!QTreeView绑定Model
    void bindDeviceTreeModel();
    void unBindDeviceTreeModel();
    //!左边树设置当前节点
    void setRootTreeItem(CMDRTDBManagerUI* RTDBUI,  const QString& rootItem);    
    //!左边树copy某项后，清除本地维护的复制项
    void clearCopyItem();
    //!刷新ToolBar的enable和disable.nodeType = Device/DataBlock/RTDB
    void refershActionStatus(bool bRunning, bool bLeftTree, int nodeType, IMDTreeItem* copyItem);

public:
    //!多语言
    void changeLanguage();
private:
    //!由MDRTDBDeviceWindow释放
    QToolBar *m_toolBar;
    QTreeView *m_treeView;
    IMDTreeModel *m_deviceTreeModel;
    QSortFilterProxyModel* m_devProxyModel;
    IMDRTDBManager *m_mdRTDBManager;
    CMDProjectConfiger *m_projectConfiger;
    QHBoxLayout *m_mainLayout ;
    QString m_devName;
    //!当前的设备或数据块节点
    IMDTreeItem* m_curItem;
    //!左边树
    CMDRTDBManagerUI* m_rtdbUI;
    IStudioInterface* m_MDStudioInterface;
private:
    void onTreeViewDoubleClick(const QModelIndex & index);

//////////////////////////////////////////////////////////////////////////
//实时数据库TreeWidget
    /*/
public:
    void initialRTDBRootTreeItem();
    void clearTreeItem(QTreeWidgetItem *treeItem);

    QModelIndex  getSelectTreeItem();
    QTreeWidgetItem *getRTDBRootTreeItem();
    QTreeWidgetItem *getVarGroupRootTreeItem();
    QTreeWidgetItem *getDeviceRootTreeItem();

    bool isRTDBRootTreeItem(QTreeWidgetItem* treeItem);
    bool isVarGroupRootTreeItem(QTreeWidgetItem* treeItem);
    bool isVarGroupTreeItem(QTreeWidgetItem* treeItem);
    bool isDeviceRootTreeItem(QTreeWidgetItem* treeItem);
    bool isDeviceTreeItem(QTreeWidgetItem* treeItem);

    bool isRTDBTreeItem(QTreeWidgetItem* treeItem);
private:
    QTreeWidget *m_treeWidget;
    QTreeWidgetItem * m_rtdbRootTreeItem;
    QTreeWidgetItem *m_varGroupRootTreeItem;
    QTreeWidgetItem *m_deviceRootTreeItem;
    QTreeWidgetItem *m_copyTreeItem;/*/
//////////////////////////////////////////////////////////////////////////
/*!
\section treeWidget 事件
*/
private:
    void treeViewClicked(const QModelIndex &index);
    void treeViewDoubleClicked(const QModelIndex &index);
    void treeViewNodeMouseClicked(const QModelIndex &index);

    void treeWidgetAfterLabelEdit(QTreeWidgetItem *treeItem, QLineEdit* edit);    
    //!键盘事件
    void keyPressEvent( QKeyEvent *k );    
 /*
//////////////////////////////////////////////////////////////////////////
//property
public:
    QTreeWidgetItem* selectNode();
//////////////////////////////////////////////////////////////////////////
//node operate
private:
    QTreeWidgetItem* getRoot(QTreeWidgetItem* node);
    bool isRoot(QTreeWidgetItem* node);
    bool isHmiNode(QTreeWidgetItem* node);
    QTreeWidgetItem* createNode(const QString& fullPath, FileType type);
    QTreeWidgetItem* cloneNode(QTreeWidgetItem* source);
    void insertSortedNode(QTreeWidgetItem* parent, QTreeWidgetItem* child);
    void getNodes(const QString& path, const QString& pattern, QTreeWidgetItem* parent);
    void copyChildNodes(QTreeWidgetItem* source, QTreeWidgetItem* target);
    CHMIFramework* getFramework(QTreeWidgetItem* node);
    void setNodeIcon(QTreeWidgetItem* node, FileType type);
//////////////////////////////////////////////////////////////////////////
//file operate
private:
    FileType getFileType(QTreeWidgetItem* node);
    QString getFilePath(QTreeWidgetItem* node);
    void copyFolder(const QString& source, const QString& target);
    void removeDirectory(const QString& fullName);*/
//////////////////////////////////////////////////////////////////////////
/*!
\section 右键菜单管理
*/
private:
    void initialMenu();
private:
    QMenu* m_menu;
private:
    /*
    void setMenuState(NodeType type);
    bool isValidCopy(QTreeWidgetItem* copyNode);
    QString createNewName(QTreeWidgetItem* parentNode, FileType type);
    void createCopyName(QTreeWidgetItem* parentNode, QTreeWidgetItem* newNode);
    void nodeAfterLabelEditEvent(NodeOperate operate, QTreeWidgetItem* node, const QString& oldFullName);
    bool isValidName(QTreeWidgetItem* node, QString& label, QString& message);
    void addEvent(FileType type);
    //如果copyNode包含在deleteNode中，则copyNode置为NULL
    void resetCopyNode(QTreeWidgetItem* deleteNode);
    */
private:
    QAction* m_actAddDevice;
    QAction* m_actAddDataBlock;

    QAction* m_actCopy;
    QAction* m_actPaste;
    QAction* m_actDelete;

    QAction* m_actRun;
    //数据块抓包
    QAction* m_actPacket;

    QAction* m_actConfig;
    /*
    QAction* m_actAddVarGroup = NULL;
    QAction* m_actAddDevice = NULL;
    QAction* m_actConfigDevice = NULL;
    QAction* m_actDelete = NULL;
    QAction* m_actCopy = NULL;
    QAction* m_actPaste = NULL;
    */
//////////////////////////////////////////////////////////////////////////
//menu event
private:
    void menuStripNodeOpening(const QPoint &pos);
    void menuItemAddDeviceClick();
    void menuItemAddDataBlockClick();
    void menuItemConfigClick();
    void menuItemCopyClick();
    void menuItemPasteClick();
    void menuItemDeleteClick();
    void menuItemRunClick();
    void menuItemPacketClick();
    /*
    void menuStripNodeOpening(const QPoint &pos);
    void menuItemAddVarGroupClick();
    void menuItemAddDeviceClick();
    void menuItemConfigDeviceClick();
    void menuItemCopyClick();
    void menuItemPasteClick();
    void menuItemDeleteClick();
    void menuItemRenameClick();
    */

    //bool showMessage(const QString& text);
    //void setNodeEdit(QTreeWidgetItem* node);
    //void editingFinished();

private:
    QModelIndex m_selectedModelIndex;
    IMDTreeItem* m_copyObjItem;

    QTimer m_timer;
    ITranslateWidgetTool* m_translateTool;
    QString m_addDeviceText;
    QString m_addDataBlockText;
    QString m_configText;
    QString m_deleteText;
    QString m_copyText;
    QString m_pasteText;
    QString m_runText;
    QString m_packetText;
    CDataBlockMonitorDialog* m_datablockMonitorDlg;
    //!数据块下的变量
    QStringList       m_deviceNameList;
    CMDRTDBVariableProperty* m_variableProperty;
signals:
    void signal_Modified();
    void signal_Run();
public slots:
    //!运行时刷新界面
    void onTimerOut();
};

#endif // CMDRTDBDEVICEWIDGET_H

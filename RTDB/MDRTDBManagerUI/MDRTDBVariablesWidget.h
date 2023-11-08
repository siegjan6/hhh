/*!
\author enli 2015.01.08
\section 驱动变量管理，实现右键功能
*/
#ifndef CMDRTDBVARIABLESWIDGET_H
#define CMDRTDBVARIABLESWIDGET_H
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
#include "VariableWriteValueDialog.h"
#include "MDRTDBManagerUI.h"

class ITranslateWidgetTool;
class CMDObjBatchOperationExpert;
class IMDVariableGroup;
class CMDRTDBVariableProperty;
class CMDVariableGroupProperty;
class CTreeViewEx;
class CMDRTDBVariablesWidget  : public QWidget
{
    Q_OBJECT
public:
    explicit CMDRTDBVariablesWidget(QWidget *parent = 0);
    ~CMDRTDBVariablesWidget();
public:
    bool save();
    QToolBar* toolBar();
public:
    void setProjectConfiger(CMDProjectConfiger *projectConfiger);
    //!变量属性设置安全策略使用
    CMDProjectConfiger* getProjectConfiger();
    bool getRunningStatus();

    void bindVariableTreeModel();
    void unBindVariableTreeModel();
    void setRootTreeItem(CMDRTDBManagerUI* RTDBUI, const QString& groupName);
    //!修改变量或变量组属性后，及时刷新属性显示，
    void refreshItemProp(qulonglong leftTreeNode);

    //!刷新Toolbar的action的状态
    //bRunning:点击Run按钮时，状态刚好相反
    //bLeftTree：左边树单击事件,hasPasteItem:左边是否有需粘贴的项
    void refershActionStatus(bool bRunning, bool bLeftTree, int nodeType,bool hasPasteItem);
    //!左边树copy某项后，清除本地维护的复制项
    void clearCopyItem();
/*!
   section 多语言
*/
     void changeLanguage();

private:
    QToolBar *m_toolBar;
    CTreeViewEx *m_treeView;
    IMDTreeModel *m_variableTreeModel;
    QSortFilterProxyModel* m_varProxyModel;
    QItemSelectionModel* m_treeSelModel;
    //!选中项
    QModelIndex m_selectedModelIndex;
    //!copy项
    IMDTreeItem* m_copyObjItem;
    //!系统项
    IMDRTDBManager *m_mdRTDBManager;
    CMDProjectConfiger *m_projectConfiger;
    QHBoxLayout *m_mainLayout;

    //!变量写对话框，一直存在至析构
    CVariableWriteValueDialog *m_variableWriteValueDialog;
    QString m_groupName;
    //!左边树节点中某个Group节点
    IMDTreeItem* m_curItem;

    //!左边树
    CMDRTDBManagerUI* m_rtdbUI;
    IStudioInterface* m_MDStudioInterface;
    QStringList       m_deviceNameList;

    //!运行时刷新View
    QTimer m_timer;

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
\section 树控件事件
*/

protected:
    void treeViewEntered(const QModelIndex &index);
    void treeViewClicked(const QModelIndex &index);
    void treeViewDoubleClicked(const QModelIndex &index);
    void treeViewNodeMouseClicked(const QModelIndex &index);

    void treeWidgetAfterLabelEdit(QTreeWidgetItem *treeItem, QLineEdit* edit);
    //!键盘事件
    void keyPressEvent( QKeyEvent *k );
    //!Tree鼠标事件,可以处理点击空白处
    void mousePressed();
    //QModelIndex getSelect
 /*
//////////////////////////////////////////////////////////////////////////
//!property

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
//!右键菜单管理
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
    //!增加变量组，增加变量
    QAction* m_actAddVarGroup;
    QAction* m_actAddVar;
    QAction* m_actSyncVar;
    QAction* m_actSyncVarFromOpcUaServer;

    //!删除，复制，粘贴
    QAction* m_actDelete;
    QAction* m_actCopy;
    QAction* m_actPaste;

    //!配置<->调试
    QAction* m_actRun;
    QAction * m_actWriteVar;

    //!属性，批量增加变量，批量修改属性
    QAction* m_actConfig;
    QAction * m_actBatchChangeVarProp;
    QAction * m_actBatchAddVar;

    QString m_addVarGroupText;
    QString m_addVarText;
    QString m_syncVarText;
    QString m_syncVarFromOpcUaServerText;
    QString m_deleteText;
    QString m_configText;
    QString m_copyText;
    QString m_pasteText;
    QString m_runText ;
    QString m_writeVarText;
    QString m_batchChangeVarPropText;
    QString m_batchAddVarText;
    //!多语言操作对象
    ITranslateWidgetTool* m_translateTool;

    //!批量增加
    CMDObjBatchOperationExpert* m_batchOp;
    //!参考变量
    IMDRTDBVariable*  m_batchVar;
    //!变量属性
    CMDRTDBVariableProperty* m_variableProperty;
    CMDVariableGroupProperty* m_variableGroupProperty;
    /*
    QAction* m_actAddVarGroup = NULL;
    QAction* m_actAddDevice = NULL;
    QAction* m_actConfigDevice = NULL;
    QAction* m_actDelete = NULL;
    QAction* m_actCopy = NULL;
    QAction* m_actPaste = NULL;
    */
//////////////////////////////////////////////////////////////////////////
/*!
\section 右键动作响应
*/
private:
    void menuStripNodeOpening(const QPoint &pos);
    void menuItemAddVarGroupClick();
    void menuItemAddVarClick();
    void menuItemSyncVarFromOpcUaServerClick();
    void menuItemSyncVarClick();
    void menuItemConfigClick();
    void menuItemCopyClick();
    void menuItemPasteClick();
    void menuItemDeleteClick();
    void menuItemRenameClick();
    void menuItemRunClick();
    void menuItemWriteVarClick();
    void menuItemBatchChangeVarPropClick();
    void menuItemBatchAddVarClick();

/*!
\section 私有函数
*/
    //选项变化
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    //批量增加构建Item
    QJsonObject buildPropJSON(const QString& propID, const QString& propComment,const QString& refValue);

signals:
    void signal_Run();
    void signal_Modified();
public slots:
    //!运行时刷新界面
    void onTimerOut();
    void onBatchAdd(const QJsonObject &json);
};

#endif // CMDRTDBVARIABLESWIDGET_H

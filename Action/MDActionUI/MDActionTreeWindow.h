/*!

\author dGFuZ3p3

\section 功能

动作 ：对象的管理树类。

*/

#ifndef MDACTIONTREEWINDOW_H
#define MDACTIONTREEWINDOW_H

#include "BaseForm.h"
#include "APC.h"
#include "MDObjectTreeWindow.h"
#include "MDActionObjectsForBrowser.h"

class CMDActionConfiger;
class CMDActionUIImp;
class CMDActionTreeWindow : public CMDObjectTreeWindow
{
    Q_OBJECT
public:
    explicit CMDActionTreeWindow(QWidget *parent = 0);
    ~CMDActionTreeWindow();

/*!
\section 初始化
*/
public:
    void init(CMDActionUIImp* actionUI, CMDActionConfiger *actionConfiger);
    bool selectSingleObject(MDActionType type, const QString& source, QString& dest);
protected:
    CMDActionUIImp* m_actionUI;
    CMDActionConfiger *m_actionConfiger;
    QVariant m_actionIcon;

/*!
\section 复制与粘贴
*/
protected:
    QString createName(const QString& sourceName);

/*!
\section 批量增加
*/
public:
    CMDActionProperty_Config* m_batchNewProperty;

/*!
\section 属性框支持；批量修改属性
*/
protected:
    QObject* getMDObjectForBactch(QList<CMDActionProperty_Config*>& MDObjects);
    QObject* getMDObject(CMDActionProperty_Config* baseProp);
    CMDActionProperty_ConfigForBrowser m_ActionProperty_ConfigForBrowser;
    CAPC_VariableWriteValueForBrowser m_VariableWriteValueForBrowser;
    CAPC_WindowOperationForBrowser m_WindowOperationForBrowser;

/*!
\section 重载的接口
*/
public:
    virtual bool save(); //! 保存到文件
public:
    virtual void buildObjectTree(CMDObjectTreeItem* rootItem);
    virtual bool addObject(void* objectUI, void *objectConfiger, qint16 subType);
    virtual bool deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems);
    virtual void objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems);
    virtual bool configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems);
    virtual bool configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem);
    virtual bool pasteObject(void* objectPtr);
public:
    virtual QString objectName(void* objectPtr);
    virtual bool resultOnSetData(qint16 objectType);
    virtual QVariant objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const;
    virtual bool isTopObject(qint16 objectType);
public:
    virtual void initTreeColumnWidth(bool enableChecked, QTreeView *treeView);
    virtual void initHeader(bool enableChecked, QStringList& headerList);
public:
    virtual void onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem);
    virtual bool initAddAction();
    virtual void initSubMenuOfAdd(QMenu* subMenu);
public:
    virtual bool batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem);
    virtual bool onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg);
protected:
    QVariant actionData(bool enableChecked, CMDActionProperty_Config* actionProperty, int propIndex) const;

/*!
\section 子菜单：新建具体的动作（写变量、保存历史组 ...）
*/
private:
    void doAdd_historyGroup_Save();
    void doAdd_variable_WriteValue();
    void doAdd_window_Open();
    void doAdd_window_Close();

/*!
\section 多语言
*/
public:
    void changeLanguage();
};

#endif // MDACTIONTREEWINDOW_H

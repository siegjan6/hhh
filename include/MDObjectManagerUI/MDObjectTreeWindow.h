/*!

\author dGFuZ3p3

\section 功能

管理对象树的窗口类。
从 CBaseForm 派生是为了 MindSCADA Studio 可对其进行管理。

*/

#ifndef MDOBJECTTREEWINDOW_H
#define MDOBJECTTREEWINDOW_H

#include <QModelIndex>
#include "../MDBaseForm/BaseForm.h"
#include "MDObjectTreeWindowPara.h"
#include "MDObjectManagerUI_Global.h"

class QTreeView;
class CMDObjectTreeModel;
class CMDObjectTreeItem;
class MDOBJECTMANAGERUISHARED_EXPORT CMDObjectTreeWindow : public CBaseForm
{
    Q_OBJECT
public:
    explicit CMDObjectTreeWindow(QWidget *parent = 0);
    ~CMDObjectTreeWindow();

/*!
\section 初始化与关闭
*/
public:
    void init(void* objectUI, void *objectConfiger);
    bool doModalSelectObjectUI(const QString& title, qint16 objectType, const QStringList& sourceList, QStringList& destList);
    bool doModalSelectObjectUI(const QString& title, qint16 objectType, const QStringList& sourceList, QList<void*>& destList);
public:
    void closeEvent(QCloseEvent *e);
    virtual bool save();

/*!
\section 初始化树控件；处理树控件事件
*/
protected:
    void initTreeView();
private:
    void treeViewClicked(const QModelIndex &index);
    void treeViewDoubleClicked(const QModelIndex &index);
    void treeViewNodeMouseClicked(const QModelIndex &index);
public:
    void resetModelData();

/*!
\section 批量增加与配置
*/
public:
    void showBatchAddDialog(const QJsonArray& ja);
    QJsonObject propToJson(const QString& propID, const QString& propComment,
                                              const QString& refValue, const QString& formatValue, int start, int step, int base);
public:
    int showBatchConfigDialog(QObject* objForPropertyBrowser, const QString& title);

/*!
\section 管理菜单；新建-》具体菜单
*/
public:
    void showSubMenuForAdd();
protected:
    void initMenu();
    void setMenuItemVisible(bool visible);
private:
    void onCustomContextMenuRequested(const QPoint &pos);
protected:
    void doAdd(qint16 type);
    void doConfig();
    void doConfigInBrowser();
    void doCopy();
    void doPaste();
    void doDelete();
protected:
    void doBatchAdd();
protected:
    QAction* addAction(QMenu* subMenu, const QString& objectTypeText);
//protected:
//    template <typename Func2>
//    QAction* addAction(QMenu* subMenu, const QString& objectTypeText,
//                                           const typename QtPrivate::FunctionPointer<Func2>::Object *receiver, Func2 slot);
protected:
    void keyPressEvent(QKeyEvent * event); // 支持复制和粘贴的快捷键

/*!
\section 派生类需要重载的接口
*/
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
/*!
\section 多语言
*/
public:
    void changeLanguage();

/*!
\section 参数
*/
public:
    CMDObjectTreeWindowPara m_para;
};

#endif // MDOBJECTTREEWINDOW_H

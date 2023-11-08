/*!

\author dGFuZ3p3

\section 功能

事件 ：对象的管理树类。

*/

#ifndef MDEVENTTREEWINDOW_H
#define MDEVENTTREEWINDOW_H

#include <QModelIndex>
#include "BaseForm.h"
#include "MDEventConfiger.h"
#include "MDObjectTreeWindow.h"
#include "MDEventObjectsForBrowser.h"

class QTreeView;
class CMDEventTreeModel;
class CMDEventConfiger;
class CMDEventUIImp;
class CMDEventTreeWindow : public CMDObjectTreeWindow
{
    Q_OBJECT
public:
    explicit CMDEventTreeWindow(QWidget *parent = 0);
    ~CMDEventTreeWindow();

/*!
\section 初始化与关闭
*/
public:
    void closeEvent(QCloseEvent *e);
public:
    void init(CMDEventUIImp* eventUI, CMDEventConfiger *eventConfiger);
    bool selectActions(const QStringList& sourceList, QStringList& destList);
    bool selectSingleObject(MDEventType type, const QString& source, QString& dest);
private:
    CMDEventConfiger *m_eventConfiger ;// NULL;
    CMDEventUIImp* m_eventUI ;// NULL;
protected:
    QVariant m_actionIcon;
    QVariant m_eventIcon;

/*!
\section 复制与粘贴
*/
protected:
    QString createName(const QString& sourceName);

/*!
\section 批量增加
*/
public:
    CMDEventProperty_Config* m_batchNewProperty;

/*!
\section 属性框支持；批量修改属性
*/
protected:
    QObject* getMDObjectForBactch(QList<CMDEventProperty_Config*>& MDObjects);
    QObject* getMDObject(CMDEventProperty_Config* baseProp);
    CMDEventProperty_ConfigForBrowser m_EventProperty_ConfigForBrowser;
    CEPC_VariableValueWriteForBrowser m_VariableValueWriteForBrowser;
    CEPC_TimeCycleForBrowser m_TimeCycleForBrowser;
    CEPC_VariableQualityChangedForBrowser m_VariableQualityChangedForBrowser;
    CEPC_VariableValueChangedInRangeForBrowser m_VariableValueChangedInRangeForBrowser;

/*!
\section 重载的接口
*/
public:
    bool save(); //! 保存到文件
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
    QVariant eventData(bool enableChecked, CMDEventProperty_Config* eventProperty, int propIndex) const;
    QVariant actionData(const QString& actionName, int propIndex) const;

/*!
\section 子菜单：新建具体的事件（变量数值变化、周期 ...）
*/
private:
    void doAdd_Variable_ValueChanged();
    void doAdd_Variable_ValueChangedInRang();
    void doAdd_Variable_QualityChanged();
    void doAdd_Timer_Cycle();
    void doAdd_Combined();

/*!
\section 多语言
*/
public:
    void changeLanguage();
};

#endif // MDEVENTTREEWINDOW_H

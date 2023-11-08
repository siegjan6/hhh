/*!

\author dGFuZ3p3

\section 功能

报警条件：对象的管理树类。

*/

#ifndef MDALARMCONDITIONTREEWINDOW_H
#define MDALARMCONDITIONTREEWINDOW_H

#include "BaseForm.h"
#include "MDObjectTreeWindow.h"
#include "MDAlarmConditionForBrowser.h"

class CMDAlarmConditionConfiger;
class CMDAlarmConditionUIImp;
class CMDAlarmCondition;
class CMDAlarmConditionTreeWindow : public CMDObjectTreeWindow
{
    Q_OBJECT
public:
    explicit CMDAlarmConditionTreeWindow(QWidget *parent = 0);
    ~CMDAlarmConditionTreeWindow();

/*!
\section 初始化
*/
public:
    void init(CMDAlarmConditionUIImp* UI, CMDAlarmConditionConfiger *configer);
protected:
    CMDAlarmConditionConfiger *m_configer ;// NULL;
    QVariant m_icon;
    CMDAlarmConditionUIImp* m_conditionUI ;// NULL;

public:
    bool selectAlarmLevel(const QString& source, QString& dest);
    bool selectVariable(const QString& source, QString& dest);

/*!
\section 复制与粘贴
*/
protected:
    QString createName(const QString& sourceName);

/*!
\section 批量增加
*/
public:
    CMDAlarmCondition* m_batchNewProperty;

/*!
\section 属性框支持；批量修改属性
*/
protected:
    CMDAlarmConditionForBrowser m_AlarmConditionForBrowser;

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
    QVariant conditionData(bool enableChecked, CMDAlarmCondition* prop, int propIndex) const;
};

#endif // MDALARMCONDITIONTREEWINDOW_H

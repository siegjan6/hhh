/*!

\author dGFuZ3p3

\section 功能

权限组 ：对象的管理树类。

*/

#ifndef MDPOWERGROUPTREEWINDOW_H
#define MDPOWERGROUPTREEWINDOW_H

#include "BaseForm.h"
#include "MDObjectTreeWindow.h"
#include "MDPowerGroupForBrowser.h"

class CMDPowerGroupConfiger;
class CMDPowerGroupUIImp;
class CMDPowerGroup;
class CMDPowerGroupTreeWindow : public CMDObjectTreeWindow
{
    Q_OBJECT
public:
    explicit CMDPowerGroupTreeWindow(QWidget *parent = 0);
    ~CMDPowerGroupTreeWindow();

/*!
\section 初始化
*/
public:
    void init(CMDPowerGroupUIImp* UI, CMDPowerGroupConfiger *configer);
    bool selectAccounts(const QStringList& sourceList, QStringList& destList);
protected:
    CMDPowerGroupConfiger *m_configer ;// NULL;
    CMDPowerGroupUIImp* m_UIImp ;// NULL;
    QVariant m_powerGroupIcon;
    QVariant m_accountIcon;

/*!
\section 复制与粘贴
*/
protected:
    QString createName(const QString& sourceName);

/*!
\section 批量增加
*/
public:
    CMDPowerGroup* m_batchNewProperty;

/*!
\section 属性框支持；批量修改属性
*/
protected:
    CMDPowerGroupForBrowser m_PowerGroupForBrowser;

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
    QVariant powerGroupData(bool enableChecked, CMDPowerGroup* prop, int propIndex) const;
    QVariant accountData(const QString& accountName, int propIndex) const;

/*!
\section 多语言
*/
public:
    void changeLanguage();
};

#endif // MDPOWERGROUPTREEWINDOW_H

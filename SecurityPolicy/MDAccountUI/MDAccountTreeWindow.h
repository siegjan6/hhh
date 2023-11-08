/*!

\author dGFuZ3p3

\section 功能

帐户 ：对象的管理树类。

*/

#ifndef MDACCOUNTTREEWINDOW_H
#define MDACCOUNTTREEWINDOW_H

#include "BaseForm.h"
#include "MDObjectTreeWindow.h"
#include "MDAccountForBrowser.h"

class CMDAccountConfiger;
class CMDAccountUIImp;
class CMDAccountProperty_Config;
class CMDAccountTreeWindow : public CMDObjectTreeWindow
{
    Q_OBJECT
public:
    explicit CMDAccountTreeWindow(QWidget *parent = 0);
    ~CMDAccountTreeWindow();

/*!
\section 初始化
*/
public:
    void init(CMDAccountUIImp* UI, CMDAccountConfiger *configer);
protected:
    CMDAccountUIImp* m_UIImp;
    CMDAccountConfiger *m_configer;
    QVariant m_icon;

/*!
\section 复制与粘贴
*/
protected:
    QString createName(const QString& sourceName);

/*!
\section 批量增加
*/
public:
    CMDAccountProperty_Config* m_batchNewProperty;

/*!
\section 属性框支持；批量修改属性
*/
protected:
    CMDAccountForBrowser m_AccountForBrowser;

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
    QVariant accountData(bool enableChecked, CMDAccountProperty_Config* prop, int propIndex) const;

/*!
\section 多语言
*/
public:
    void changeLanguage();
};

#endif // MDACCOUNTTREEWINDOW_H

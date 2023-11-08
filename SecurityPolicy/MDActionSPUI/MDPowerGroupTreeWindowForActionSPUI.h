/*!

\author dGFuZ3p3

\section 功能

选择权限组界面类。

*/

#ifndef MDPOWERGROUPTREEWINDOWFORACTIONSPUI_H
#define MDPOWERGROUPTREEWINDOWFORACTIONSPUI_H

#include "BaseForm.h"
#include "MDObjectTreeWindow.h"

class CMDPowerGroupConfiger;
class CMDPowerGroup;
class CMDPowerGroupTreeWindowForActionSPUI : public CMDObjectTreeWindow
{
    Q_OBJECT
public:
    explicit CMDPowerGroupTreeWindowForActionSPUI(QWidget *parent = 0);
    ~CMDPowerGroupTreeWindowForActionSPUI();

/*!
\section 初始化
*/
public:
    void init(CMDPowerGroupConfiger *configer);
protected:
    CMDPowerGroupConfiger *m_configer ;// NULL;
    QVariant m_powerGroupIcon;
    QVariant m_accountIcon;

/*!
\section 重载的接口
*/
public:
    virtual void buildObjectTree(CMDObjectTreeItem* rootItem);
    virtual QString objectName(void* objectPtr);
    virtual bool resultOnSetData(qint16 objectType);
    virtual QVariant objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const;
    virtual bool isTopObject(qint16 objectType);
public:
    virtual void initTreeColumnWidth(bool enableChecked, QTreeView *treeView);
    virtual void initHeader(bool enableChecked, QStringList& headerList);
protected:
    QVariant powerGroupData(bool enableChecked, CMDPowerGroup* prop, int propIndex) const;
    QVariant accountData(const QString& accountName, int propIndex) const;
};

#endif // MDPOWERGROUPTREEWINDOWFORACTIONSPUI_H

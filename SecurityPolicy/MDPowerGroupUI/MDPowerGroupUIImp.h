/*!

\author dGFuZ3p3

\section 功能

权限组界面类的实现类，管理权限组界面，提供选择权限组的界面。

*/

#ifndef MDPOWERGROUPUIIMP_H
#define MDPOWERGROUPUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDPowerGroupTreeWindow;
class CMDPowerGroupConfiger;
class CMDAccountUI;
class CMDPowerGroupUIImp : public QObject
{
    Q_OBJECT
public:
    CMDPowerGroupUIImp(QObject *parent = 0);
    ~CMDPowerGroupUIImp();

/*!
\section 初始化
*/
public:
    void setAccountUI(CMDAccountUI* accountUI);

    QTreeWidgetItem *m_rootInStudio ;// NULL;
    IStudioInterface* m_studioInterface;
    CMDAccountUI* m_accountUI ;// NULL;
public:
    void load();
    void unLoad();

/*!
\section 与开发器交互；通过树控件管理对象
*/
public:
    void onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos);
    void onMouseLClick(QTreeWidgetItem *pItem,int column);
    void onMouseDbClick(QTreeWidgetItem *pItem,int column);
    QString fileItemPath(QTreeWidgetItem *pItem,int column);
protected:
    QTreeWidget *treeWidget();
    QTreeWidgetItem *treeRoot();
    void buildPowerGroupTree(QTreeWidgetItem *pTreeRoot);
private:
    CMDPowerGroupTreeWindow* m_powerGroupTreeWindow ;// NULL;
    QTreeWidgetItem *selectedTreeItem();
private slots:
    void on_powerGroupTreeWindow_destroyed(QObject* obj);

/*!
\section 显示对象选择界面
*/
public:
    bool selectPowerGroups(const QStringList& sourceList, QStringList& destList);

/*!
\section 创建对象树管理界面，目前用于运行时管理权限组
*/
public:
    QWidget* createMgrWidget();

/*!
\section 多语言
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // MDPOWERGROUPUIIMP_H

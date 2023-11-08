/*!

\author dGFuZ3p3

\section 功能

帐户界面类的实现类，管理帐户界面，提供选择帐户的界面。

*/

#ifndef MDACCOUNTUIIMP_H
#define MDACCOUNTUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDAccountTreeWindow;
class CMDSecurityPolicyConfiger;
class CMDAccountUIImp : public QObject
{
    Q_OBJECT
public:
    CMDAccountUIImp(QObject *parent = 0);
    ~CMDAccountUIImp();

/*!
\section 初始化
*/
public:
    QTreeWidgetItem *m_rootInStudio ;// NULL;
    IStudioInterface* m_studioInterface;
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
    void buildAccountTree(QTreeWidgetItem *pTreeRoot);
private:
    CMDAccountTreeWindow* m_accountTreeWindow ;// NULL;
    QTreeWidgetItem *selectedTreeItem();
private slots:
    void on_accountTreeWindow_destroyed(QObject* obj);

/*!
\section 显示对象选择界面
*/
public:
    bool selectAccounts(const QStringList& sourceList, QStringList& destList);

/*!
\section 创建对象树管理界面，目前用于运行时管理帐户
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

#endif // MDACCOUNTUIIMP_H

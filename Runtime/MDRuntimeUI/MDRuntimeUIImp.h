/*!

\author dGFuZ3p3

\section 功能

运行时参数界面类的实现类，设置与运行有关的参数。

*/

#ifndef MDRUNTIMEUIIMP_H
#define MDRUNTIMEUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDProjectConfiger;
class IHMIFramework;
class CMDRuntimeUIImp : public QObject
{
    Q_OBJECT
public:
    CMDRuntimeUIImp(QObject *parent = 0);
    ~CMDRuntimeUIImp();

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
    void buildTree(QTreeWidgetItem *pTreeRoot);
private:
    QTreeWidgetItem *selectedTreeItem();

/*!
\section 多语言
*/
public:
   void changeLanguage();
};

#endif // MDRUNTIMEUIIMP_H

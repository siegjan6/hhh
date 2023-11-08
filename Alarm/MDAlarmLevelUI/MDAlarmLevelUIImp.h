/*!

\author dGFuZ3p3

\section 功能

报警等级界面类。

*/

#ifndef MDALARMLEVELUIIMP_H
#define MDALARMLEVELUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDAlarmLevelTreeWindow;
class CMDAlarmConfiger;
class CMDAlarmLevelUIImp : public QObject
{
    Q_OBJECT
public:
    CMDAlarmLevelUIImp(QObject *parent = 0);
    ~CMDAlarmLevelUIImp();
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
    CMDAlarmLevelTreeWindow* m_treeWindow ;// NULL;
    QTreeWidgetItem *selectedTreeItem();
private slots:
    void on_treeWindow_destroyed(QObject* obj);

/*!
\section 显示对象选择界面
*/
public:
    bool select(const QString& source, QString& dest);

/*!
\section 多语言
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // MDALARMLEVELUIIMP_H

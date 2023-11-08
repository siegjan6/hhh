/*!

\author dGFuZ3p3

\section 功能

报警条件界面类，管理报警条件界面，提供选择报警条件的界面。

*/

#ifndef MDALARMCONDITIONUIIMP_H
#define MDALARMCONDITIONUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDAlarmConditionTreeWindow;
class CMDAlarmConditionUIContacter;
class CMDAlarmConditionUIImp : public QObject
{
    Q_OBJECT
public:
    CMDAlarmConditionUIImp(QObject *parent = 0);
    ~CMDAlarmConditionUIImp();
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
\section 与外界联系的接口
*/
public:
    void setContacter(CMDAlarmConditionUIContacter* contacter);
    CMDAlarmConditionUIContacter* contacter();
protected:
    CMDAlarmConditionUIContacter* m_contacter ;// NULL;

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
    CMDAlarmConditionTreeWindow* m_treeWindow ;// NULL;
    QTreeWidgetItem *selectedTreeItem();
private slots:
    void on_treeWindow_destroyed(QObject* obj);

/*!
\section 显示对象选择界面
*/
public:
    bool select(const QStringList& sourceList, QStringList& destList);

/*!
\section 多语言
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // MDALARMCONDITIONUIIMP_H

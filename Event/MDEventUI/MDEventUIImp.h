/*!

\author dGFuZ3p3

\section 功能

事件界面类的实现类，管理事件界面，提供选择事件的界面。

*/

#ifndef MDEVENTUIIMP_H
#define MDEVENTUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDEventTreeWindow;
class CMDEventUIContacter;
class CMDProjectConfiger;
class CMDEventUIImp : public QObject
{
    Q_OBJECT
public:
    CMDEventUIImp(QObject *parent = 0);
    ~CMDEventUIImp();
/*!
\section 初始化
*/
public:
    IStudioInterface* m_studioInterface;
    QTreeWidgetItem* m_rootInStudio;
public:
    void load();
    void unLoad();

/*!
\section 与外界联系的接口
*/
public:
    void setContacter(CMDEventUIContacter* contacter);
    CMDEventUIContacter* contacter();
protected:
    CMDEventUIContacter* m_contacter ;// NULL;

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
    void buildEventTree(QTreeWidgetItem *pTreeRoot);
private:
    CMDEventTreeWindow* m_evenTreeWindow ;// NULL;
    QTreeWidgetItem *selectedTreeItem();
private slots:
    void on_eventTreeWindow_destroyed(QObject* obj);

/*!
\section 多语言
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // MDEVENTUIIMP_H

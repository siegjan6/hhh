/*!

\author dGFuZ3p3

\section 功能

动作界面类的实现类，管理动作界面，提供选择动作的界面。

*/

#ifndef MDACTIONUIIMP_H
#define MDACTIONUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDActionTreeWindow;
class CMDProjectConfiger;
class CMDActionUIContacter;
class CMDActionUIImp : public QObject
{
    Q_OBJECT
public:
    CMDActionUIImp(QObject *parent = 0);
    ~CMDActionUIImp();

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
    void setContacter(CMDActionUIContacter* contacter);
    CMDActionUIContacter* contacter();
protected:
    CMDActionUIContacter* m_contacter;

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
    void buildActionTree(QTreeWidgetItem *pTreeRoot);
private:
    CMDActionTreeWindow* m_actionTreeWindow;
    QTreeWidgetItem *selectedTreeItem();
private slots:
    void on_actionTreeWindow_destroyed(QObject* obj);

/*!
\section 显示对象选择界面
*/
public:
    bool selectActions(const QStringList& sourceList, QStringList& destList);

/*!
\section 多语言
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // MDACTIONUIIMP_H

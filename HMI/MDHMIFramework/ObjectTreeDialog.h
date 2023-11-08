/*!
\author aGRw 2015.03.17
\section 控件管理窗体
*/
#ifndef DRAWOBJTREEDIALOG_H
#define DRAWOBJTREEDIALOG_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "IHMIForm.h"
#include "MDDrawObj.h"
#include "IObjectChangedEvent.h"
#include "MDMultiLanguage.h"

namespace Ui {
class DrawObjTreeDialog;
}

enum class OperationType
{
    Mouse, Event, Other
};

class CObjectTreeDialog : public QWidget, public IObjectChangedEvent
{
    Q_OBJECT
public:
    explicit CObjectTreeDialog(QWidget *parent = 0);
    ~CObjectTreeDialog();
private:
    Ui::DrawObjTreeDialog *ui;
//////////////////////////////////////////////////////////////////////////
public:
    //!设置数据
    void setHMIForm(IHMIForm* value);
private:
    IHMIForm* m_form;
//////////////////////////////////////////////////////////////////////////
private:
    QTreeWidgetItem* createItem(QTreeWidgetItem* parent, CMDDrawObj* obj);
    //!visible | checked状态变化
    void itemChanged(QTreeWidgetItem *item, int column);
    QTreeWidget* tree();
    void deleteItems(QList<CMDDrawObj*>* objs);
    QTreeWidgetItem* findItem(CMDDrawObj* obj);
    QTreeWidgetItem* findChildItem(QTreeWidgetItem* parent, CMDDrawObj* obj);
    int objectIndex(CMDDrawObj* obj);
    //!通过节点选中控件
    void itemSelectObject();
    void itemSelectionChanged();
    void selectDefault();
    void clear();
//////////////////////////////////////////////////////////////////////////
protected:
    void resizeEvent(QResizeEvent * event);
    QSize sizeHint() const;
/*!
\section 控件对象变化通知事件
*/
public:
    void add(QList<CMDDrawObj *> *list) ;
    void dele(QList<CMDDrawObj *> *list) ;
    void group(CMDDrawObj* obj) ;
    void unGroup(CMDDrawObj* obj) ;
    void combine(CMDDrawObj* obj);
    void unCombine(CMDDrawObj* obj);

    void top(CMDDrawObj* obj) ;
    void last(CMDDrawObj* obj) ;
    void front(CMDDrawObj* obj) ;
    void back(CMDDrawObj* obj) ;

    void select(QList<CMDDrawObj*>* list) ;

    void nameChanged(CMDDrawObj* obj) ;

    void reset() ;
//////////////////////////////////////////////////////////////////////////
private:
    //!节点选择类型,目的是itemSelectionChanged和select不会互相触发
    OperationType m_operationType;
    //控件图标
    QMap<int, QIcon*>* m_icons;
/*!
\section 控件对象变化通知事件
*/
public:
    void changeLanguage();
private:
    ITranslateWidgetTool* m_transTool;
};

#endif // DRAWOBJTREEDIALOG_H

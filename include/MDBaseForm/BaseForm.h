/*!
\author luyj 2016.09.07
\section 窗体基类
*/
#ifndef BASEFORM_H
#define BASEFORM_H

#include <QtCore/qglobal.h>

#if defined(BASEFORM_LIBRARY)
#  define BASEFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASEFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QMdiSubWindow>
#include <QList>

class CBaseFormData;

class BASEFORMSHARED_EXPORT CBaseForm : public QMainWindow
{
    Q_OBJECT
public:
    CBaseForm(QWidget *parent = 0);
    virtual ~CBaseForm();
private:
    CBaseFormData* m_data;
public:
    virtual QList<QMenu *>* menus() const{return NULL;}
    virtual QList<QToolBar*>* toolBars() const{return NULL;}
    //保存，用于调用save函数和设置modified状态
    void saveEvent();
    virtual bool save(){ return true;}
    //是否修改
    virtual void setModify(bool value);
    virtual bool isModify();
    //外部设置的父窗口
    void setPwid(int value);
    int pwid();
    //设置文本
    virtual void setWindowText(const QString &text);
    //是否激活
    virtual void setActive(bool value);
    virtual bool isActived();
    //是否需要滚动条
    void setNeedScroll(bool value);
    bool needScroll();
    //是否显示修改标记
    void setModifiedVisible(bool value);
    bool modifiedVisible();
    //不包含*的窗体标题
    QString factWindowTitle();
public:
    //关闭窗体
    virtual void closeForm();
    //隐藏窗体
    virtual void hideForm();
    //显示窗体
    virtual void showForm();
    //获取父窗体
    QMdiSubWindow* mdiSubWindow();
private:
    void resetText();
};

#endif // BASEFORM_H

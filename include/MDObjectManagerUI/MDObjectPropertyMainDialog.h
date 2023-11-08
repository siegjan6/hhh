/*!

\author dGFuZ3p3

\section 功能

对象属性主对话类，该类将管理对象所具有的属性页。

*/

#ifndef MDOBJECTPROPERTYMAINDIALOG_H
#define MDOBJECTPROPERTYMAINDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QListWidget>

#include "MDObjectManagerUI_Global.h"
#include "MDObjectPropertyMainDialogPara.h"

class MDOBJECTMANAGERUISHARED_EXPORT CMDObjectPropertyMainDialog : public QDialog
{
    Q_OBJECT
public:
    CMDObjectPropertyMainDialog(QWidget *parent = 0);
    ~CMDObjectPropertyMainDialog();
/*!
\section 初始化
*/
public:
    void initial(void* objectUI, void* configer, qint16 subType);
    void initial(void* objectUI, void* configer, void *sourceProperty);
private:
    void helper_init(void* objectUI, void* configer, void *sourceProperty);
private:
    void initChangeLanguage();

/*!
\section 处理对 OK 和 Cancel 按钮的点击
*/
private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

/*!
\section 可重载的接口
*/
protected:
    virtual void* newProperty(qint16 subType, void* sourceProperty);
    virtual void initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty);
    virtual bool modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage);
    virtual bool addProperty(void* configer, void* destProperty, QString& resultMessage);

/*!
\section 参数
*/
public:
    CMDObjectPropertyMainDialogPara m_para;
};

#endif // MDOBJECTPROPERTYMAINDIALOG_H

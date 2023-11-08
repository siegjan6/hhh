/*!

\author dGFuZ3p3

\section 功能

对象属性页面对话类，该类将管理对象所具有的一种独立属性。

*/

#ifndef MDOBJECTPROPERTYDIALOG_H
#define MDOBJECTPROPERTYDIALOG_H

#include <QDialog>
#include "MDObjectManagerUI_Global.h"
#include "MDObjectPropertyDialogPara.h"

class MDOBJECTMANAGERUISHARED_EXPORT CMDObjectPropertyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMDObjectPropertyDialog(QWidget *parent = 0);
    ~CMDObjectPropertyDialog();
public:
    virtual void initial(void* objectUI, void *sourceProperty);
    virtual bool UI2Property(void *destProperty);
public:
    void setLanguageExceptions(const QStringList* controlTypesNoT, const QStringList* controlNamesT_InNoT = NULL);
    void initChangeLanguage();
public:
    void keyPressEvent(QKeyEvent * ke);

/*!
\section 参数
*/
public:
    void* objectUI();
    CMDObjectPropertyDialogPara m_para;
};

#endif // MDOBJECTPROPERTYDIALOG_H

/*!

\author dGFuZ3p3

\section 功能

对象属性主对话类的参数类的实现类。

*/

#ifndef MDOBJECTPROPERTYMAINDIALOGPARAIMP_H
#define MDOBJECTPROPERTYMAINDIALOGPARAIMP_H

class QListWidget;
class QStackedWidget;
class QPushButton;
class QWidget;
class ITranslateWidgetTool;
class CMDObjectPropertyMainDialogParaImp
{
public:
    CMDObjectPropertyMainDialogParaImp();
    void initialUI(QWidget* dialog);
public:
    void *m_configer ;
    void *m_sourceProperty ;
    void *m_destProperty ;
public:
    QListWidget *m_listWidget ;
    QStackedWidget *m_stackWidget ;
public:
    QPushButton *okButton ;
    QPushButton *cancleButton ;
public:
    ITranslateWidgetTool* m_twTool;
};

#endif // MDOBJECTPROPERTYMAINDIALOGPARAIMP_H

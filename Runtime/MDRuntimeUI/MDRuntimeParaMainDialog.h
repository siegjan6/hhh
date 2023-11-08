/*!

\author dGFuZ3p3

\section 功能

运行参数：配置对话主类，管理项目、报警、窗体等对话类。

*/

#ifndef MDRUNTIMEPARAMAINDIALOG_H
#define MDRUNTIMEPARAMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"
#include "MDRuntimePara.h"

class CMDRuntimeParaConfiger;
class CMDRuntimeUIImp;
class CMDRuntimeParaMainDialog: public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    CMDRuntimeParaMainDialog(QWidget *parent = 0);
    ~CMDRuntimeParaMainDialog();
public:
    void setUIImp(CMDRuntimeUIImp* uiImp);
protected:
    CMDRuntimeUIImp* m_uiImp ;// NULL;
protected:
    virtual void* newProperty(qint16 subType, void* sourceProperty);
    virtual void initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty);
    virtual bool modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage);
    virtual bool addProperty(void* configer, void* destProperty, QString& resultMessage);
};

#endif // MDRUNTIMEPARAMAINDIALOG_H

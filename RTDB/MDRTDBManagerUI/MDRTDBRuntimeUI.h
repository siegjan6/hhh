/*!
\author enli 2015.01.08
\section 实时数据库运行对象管理
*/
#ifndef MDRTDBRUNTIMEUI_H
#define MDRTDBRUNTIMEUI_H
#include <QMdiArea>
#include "IMDRTDBRuntimeUI.h"


class CMDRTDBRuntimeDialog;
class CMDRTDBRuntimeUI:public QObject,IMDRTDBRuntimeUI
{
    Q_OBJECT
public:
    CMDRTDBRuntimeUI();
    ~CMDRTDBRuntimeUI();
/*!
\section 接口
*/
public:
     void setProjectConfiger(CMDProjectConfiger *projectConfiger);
    void openRTDBWindow(QWidget *parent);
    void closeRTDBWindow();
/*!
section 多语言
*/
public:
    void changeLanguage();

private slots:
    void on_Closed();
private:
     CMDProjectConfiger*m_mdProjectConfiger;
     CMDRTDBRuntimeDialog *m_rtdbDialog ;
};

#endif // MDRTDBRUNTIMEUI_H

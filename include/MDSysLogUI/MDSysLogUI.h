/*!

\author dGFuZ3p3

\section 功能 - 系统日志界面管理类，创建并管理日志界面，具体功能如下：

、createModaless 创建基于QDialog 的界面，该界面具有以下特点：
 （1）非模态；
 （2）点击【x】按钮时，对话不会被销毁，而是被隐藏，可以被再次显示；
 （3）ESC 键被屏蔽；
 （4）外部可通过 destroyModaless 销毁该界面。
、显示一个模态对话，外部不能控制；
、显示一个非模态对话，外部不能控制；
、关闭通过 createModaless 和 showModaless 创建的全部界面，参考 closeAll。

*/

#ifndef MDSYSLOGUI_H
#define MDSYSLOGUI_H

#include <QString>
#include "MDSysLogUI_Global.h"

class QWidget;
class QDialog;
class CMDSysLogger;
class CMDSysLogUIImp;
class MDSYSLOGUISHARED_EXPORT CMDSysLogUI
{
public:
    CMDSysLogUI();
    ~CMDSysLogUI();
public:
    QDialog* createModaless(CMDSysLogger* logger, const QString& title = "", QWidget *parent = NULL);
    bool destroyModaless(QDialog* loggerDialog);
public:
    void showModal(CMDSysLogger* logger, const QString& title = "", QWidget *parent = NULL);
    void showModaless(CMDSysLogger* logger, const QString& title = "", QWidget *parent = NULL);
public:
    void closeAll();
protected:
    CMDSysLogUIImp* m_imp;
};

#endif // MDSYSLOGUI_H

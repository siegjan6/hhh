/*!

\author dGFuZ3p3

\section 功能

具体子模块专家代理类，某些类实现了基类的可重载行为，例如，完成动作执行。

*/

#ifndef MDPROJECTMODULEEXPERTPROXY_H
#define MDPROJECTMODULEEXPERTPROXY_H

#include "MDProjectModuleExpert.h"

class CMDVariableExpert;
/*!
 * \brief 变量专家代理类
 */
class CMDVariableExpertProxy : public CMDProjectModuleExpert
{
public:
    CMDVariableExpertProxy();
    CMDVariableExpert* m_variableExpert ;// NULL;
public:
    virtual qint8 executeAction(CMDAction* action);
};

class CMDEventExpert;
/*!
 * \brief 事件专家代理类
 */
class CMDEventExpertProxy : public CMDProjectModuleExpert
{
public:
    CMDEventExpertProxy();
    CMDEventExpert* m_eventExpert ;// NULL;
};

class CMDActionExpert;
/*!
 * \brief 动作专家代理类
 */
class CMDActionExpertProxy : public CMDProjectModuleExpert
{
public:
    CMDActionExpertProxy();
    CMDActionExpert* m_actionExpert ;// NULL;
};

class CMDHistoryExpert;
/*!
 * \brief 历史专家代理类
 */
class CMDHistoryExpertProxy : public CMDProjectModuleExpert
{
public:
    CMDHistoryExpertProxy();
    CMDHistoryExpert* m_historyExpert ;// NULL;
public:
    virtual qint8 executeAction(CMDAction* action);
};

class CMDProjectExpertImp;
/*!
 * \brief HMI 专家代理类
 */
class CMDHMIExpertProxy : public CMDProjectModuleExpert
{
public:
    CMDHMIExpertProxy();
    CMDProjectExpertImp* m_projectExpert ;// NULL;
public:
    bool open(const QString& name, int pwid = 0, int x = 0, int y = 0, int w = 0, int h = 0);
    void close(const QString& name);
    void move(const QString& name, int left, int top);
    void hide(const QString& name);
    void callJsFunc(const QString& name, QString funcName, QString params);

    void showByPid(int pid, int phwnd = 0);
    void hideByPid(int pid, int phwnd = 0);
    void closeByPid(int pid, int phwnd = 0);
public:
    void enterFullScreen();
    void exitFullScreen();
    void showRecipeRuntime();
    void hideRecipeRuntime();
    void showRuntime();
    void hideRuntime();

public:
    virtual qint8 executeAction(CMDAction* action);
};

class CMDProjectExpertImp;
/*!
 * \brief 代理类的管理类，用于简化 CMDProjectExpertImp 的实现代码
 */
class CMDExpertProxyMgr
{
public:
    CMDExpertProxyMgr();
    void init(CMDProjectExpertImp* projectExpertImp);
public:
    CMDVariableExpertProxy m_variableProxy;
    CMDEventExpertProxy m_eventProxy;
    CMDActionExpertProxy m_actionProxy;
    CMDHistoryExpertProxy m_historyProxy;
    CMDHMIExpertProxy m_hmiProxy;
};

#endif // MDPROJECTMODULEEXPERTPROXY_H

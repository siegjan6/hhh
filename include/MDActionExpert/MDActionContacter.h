/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDActionExpert::init。
模块的使用者必须实现所有的重载接口。

*/

#ifndef MDACTIONCONTACTER_H
#define MDACTIONCONTACTER_H

#include <QString>
#include "MDActionExpert_Global.h"
#include <QVariant>

class CMDAction;
class CMDProjectModuleExpert;
class CMDActionSPSession;
class CMDLogMessage;
class IMDVariable;
class MDACTIONEXPERTSHARED_EXPORT CMDActionContacter
{
public:
    CMDActionContacter();
    virtual ~CMDActionContacter();
public:
    virtual CMDProjectModuleExpert* findActionExecuter(CMDAction* action); //! 获取动作的执行器
    virtual bool executeAction(CMDAction* action);
    virtual bool variable(const QString& varName, IMDVariable* var);
    virtual QVariant variableValue(const QString& varName);
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession); //! 动作执行前的安全检查
public:
    virtual bool log(CMDLogMessage* msg); //! 将动作记录到日志
};

#endif // MDACTIONCONTACTER_H

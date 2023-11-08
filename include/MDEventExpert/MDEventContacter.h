/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDEventExpert::init。
模块的使用者必须实现所有的重载接口。

*/

#ifndef MDEVENTCONTACTER_H
#define MDEVENTCONTACTER_H

#include <QString>
#include "MDEventExpert_Global.h"

class CMDEvent;
class CMDAction;
class CMDLogMessage;
class MDEVENTEXPERTSHARED_EXPORT CMDEventContacter
{
public:
    CMDEventContacter();
    virtual ~CMDEventContacter();
public:
    virtual CMDAction* findAction(const QString& actionName); //! 查找动作对象

    //! 事件被触发后，执行动作
    virtual bool executeAction(CMDEvent* sourceEvent, CMDAction* action);
public:
    virtual bool log(CMDLogMessage* msg); //! 记录到日志
};

#endif // MDEVENTCONTACTER_H

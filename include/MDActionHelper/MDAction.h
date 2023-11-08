/*!

\author dGFuZ3p3

\section 功能

动作数据类，包含配置时属性 CMDActionProperty_Config 和
                        运行时属性 CMDActionProperty_Runtime，
                        并绑定了动作执行器。

*/

#ifndef MDACTION_H
#define MDACTION_H

#include "MDActionHelper_Global.h"
#include "APC.h"
#include "MDActionProperty_Runtime.h"

class CMDActionData;
class CMDProjectModuleExpert;
class MDACTIONHELPERSHARED_EXPORT CMDAction
{
public:
    CMDAction();
    CMDAction(CMDActionProperty_Config* configProperty);
    ~CMDAction();
/*!
\section 属性
*/
public:
    MDActionType type() const;
    CMDActionProperty_Config* configProperty() const;
    CMDActionProperty_Runtime* runtimeProperty() const;

/*!
\section 动作执行器
*/
public:
    CMDProjectModuleExpert* actionExecuter() const;
    void setActionExecuter(CMDProjectModuleExpert* actionExecuter);
protected:
    CMDActionData* m_data;
};

#endif // MDACTION_H

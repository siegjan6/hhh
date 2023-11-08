/*!

\author dGFuZ3p3

\section 功能

动作属性工厂类，生产 CMDActionProperty_Config 及其派生类的实例。

*/

#ifndef APC_H
#define APC_H

#include "APC_PlayTTS.h"
#include "APC_VariableWriteValue.h"
#include "APC_WindowOperation.h"

class MDACTIONHELPERSHARED_EXPORT CAPC_Factory
{
protected:
    CAPC_Factory();
public:
    /*!
     生产 CMDActionProperty_Config 及其派生类的实例。

     \param actionType 动作类型。
     \param CMDActionProperty_Config 源对象，
                  若不为空，新对象将复制源对象的属性，若为空，则新对象的属性都是缺省值。
     \returns 一个实例指针，失败则返回 NULL。
     */
    static CMDActionProperty_Config* newConfig(MDActionType actionType, const CMDActionProperty_Config* sourceProperty = NULL);
};

#endif // APC_H

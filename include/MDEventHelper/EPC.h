/*!

\author dGFuZ3p3

\section 功能

事件属性工厂类，生产 CMDEventProperty_Config 及其派生类的实例。

*/

#ifndef EPC_H
#define EPC_H

#include "EPC_TimeCycle.h"
#include "EPC_VariableValueChangedInRange.h"
#include "EPC_VariableQualityChanged.h"
#include "EPC_VariableValueWrite.h"
#include "EPC_Combined.h"

class MDEVENTHELPERSHARED_EXPORT CEPC_Factory
{
protected:
    CEPC_Factory();
public:
    /*!
     生产 CMDEventProperty_Config 及其派生类的实例。

     \param eventType 事件类型。
     \param CMDEventProperty_Config 源对象，
                  若不为空，新对象将复制源对象的属性，若为空，则新对象的属性都是缺省值。
     \returns 一个实例指针，失败则返回 NULL。
     */
    static CMDEventProperty_Config* newConfig(MDEventType eventType, const CMDEventProperty_Config* sourceProperty = NULL);
};

#endif // EPC_H

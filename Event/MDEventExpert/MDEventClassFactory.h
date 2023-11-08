/*!

\author dGFuZ3p3

\section 功能

事件工厂类，生产事件相关类的实例。

*/

#ifndef MDEVENTCLASSFACTORY_H
#define MDEVENTCLASSFACTORY_H

#include "EPC.h"
#include "MDEventProperty_Runtime.h"
#include "MDEventHandlerEx.h"
#include "MDEventConstant.h"

class CFactoryForEventProperty
{
public:
    static CMDEventProperty_Config* createConfig(CMDEventProperty_Config* configProperty);
public:
    static CMDEventProperty_Runtime* createRuntime(MDEventType nsEventType);
public:
    static CMDEventHandlerEx* createEventHandler(MDEventType nsEventType, CMDEvent* nsEvent);
};

#endif // MDEVENTCLASSFACTORY_H

/*!

\author dGFuZ3p3

\section 功能

事件数据类，包含配置时属性 CMDEventProperty_Config 和
                        运行时属性 CMDEventProperty_Runtime ，
                        并绑定了事件处理器。

*/

#ifndef MDEVENT_H
#define MDEVENT_H

#include <QList>
#include "MDEventConstant.h"
#include "MDEventHelper_Global.h"
#include "MDEventProperty_Config.h"
#include "MDEventProperty_Runtime.h"
#include "MDEventHandler.h"
#include "../include/MDActionHelper/MDAction.h"

class CMDEventHandler;
class CMDEventProperty_Config;
class CMDEventProperty_Runtime;
class CMDAction;
class CMDEventData;
class MDEVENTHELPERSHARED_EXPORT CMDEvent
{
public:
    CMDEvent(CMDEventProperty_Config* configProperty);
    ~CMDEvent();
/*!
\section 属性
*/
public:
    CMDEventProperty_Config* configProperty() const;
    CMDEventProperty_Runtime* runtimeProperty() const;
    MDEventType type();

/*!
\section 事件处理器
*/
public:
    CMDEventHandler* handler() const;
    void setHandler(CMDEventHandler* handler);

/*!
\section 关联事件与关联动作
*/
public:
    QList<CMDEvent*>* relatedEventList();
public:
    QList<CMDAction*>* actionList();
protected:
    CMDEventData* m_data;
};

#endif // MDEVENT_H

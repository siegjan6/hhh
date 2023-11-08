/*!

\author dGFuZ3p3

\section 功能

事件数据类的实现类，参考 CMDEvent 。
事件处理器类的实现类，参考 CMDEventHandler 。

*/

#ifndef MDEVENTDATA_H
#define MDEVENTDATA_H

#include <QList>
#include "EPC.h"
#include "MDEventProperty_Runtime.h"
#include "MDEventConstant.h"

class CMDEventHandler;
class CMDRawEvent;
class CMDAction;
class CMDEvent;
/*!
 * \brief 事件数据类的实现类
 */
class CMDEventData
{
public:
    CMDEventData(CMDEventProperty_Config* configProperty);
    ~CMDEventData();
public:
    CMDEventProperty_Config* m_configProperty ;// NULL;
    CMDEventProperty_Runtime* m_runtimeProperty ;// NULL;
public:
    CMDEventHandler* m_handler ;// NULL;
public:
    QList<CMDEvent*> m_relatedEventList;
public:
    QList<CMDAction*> m_actionList;
};

/*!
 * \brief 事件处理器类的实现类
 */
class CMDEventHandlerData
{
public:
    CMDEventHandlerData(CMDEvent* nsEvent);
    virtual ~CMDEventHandlerData();
public:
    CMDEvent* m_event ;// NULL;
};

#endif // MDEVENTDATA_H

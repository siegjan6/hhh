/*!

\author dGFuZ3p3

\section 功能

事件处理器基类。

*/

#ifndef MDEVENTHANDLER_H
#define MDEVENTHANDLER_H

#include "MDEventHelper_Global.h"
#include "MDEvent.h"

class CMDEvent;
class CMDEventHandlerData;
class MDEVENTHELPERSHARED_EXPORT CMDEventHandler
{
public:
    CMDEventHandler(CMDEvent* event);
    virtual ~CMDEventHandler();
public:
    CMDEvent* nsEvent() const;
    void setMDEvent(CMDEvent* event);
protected:
    CMDEventHandlerData* m_data;
};

#endif // MDEVENTHANDLER_H

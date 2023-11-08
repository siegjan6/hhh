/*!

\author dGFuZ3p3

\section 功能

事件运行时的属性类，这些属性在配置时是没有的，如触发次数，最后触发时间等。

*/

#ifndef MDEVENTPROPERTY_RUNTIME_H
#define MDEVENTPROPERTY_RUNTIME_H

#include <QDateTime>
#include "MDEventHelper_Global.h"

class CMDEventPropertyData_Runtime;
class MDEVENTHELPERSHARED_EXPORT CMDEventProperty_Runtime
{
public:
    CMDEventProperty_Runtime();
    virtual ~CMDEventProperty_Runtime();
   CMDEventProperty_Runtime(const CMDEventProperty_Runtime& obj);
   CMDEventProperty_Runtime& operator = (const CMDEventProperty_Runtime& obj);

/*!
\section 属性
*/
public:
   //! 表达式状态 ：true - 触发状态，false - 非触发状态。
   bool expFlag() const;
   void setExpFlag(bool flag);
public:
   //! 自动重置表达式状态
   bool autoResetExpFlag() const;
   void setAutoResetExpFlag(bool flag);
public:
   long triggerCount() const;
   void setTriggerCount(long count);

   QDateTime lastTriggerTime() const;
   void setLastTriggerTime(const QDateTime& lastTriggerTime);
protected:
    CMDEventPropertyData_Runtime* m_data;
};

#endif // MDEVENTPROPERTY_RUNTIME_H

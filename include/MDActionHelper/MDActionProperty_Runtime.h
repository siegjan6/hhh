/*!

\author dGFuZ3p3

\section 功能

动作运行时的属性类，这些属性在配置时是没有的，如执行次数，最后执行时间等。

*/
#ifndef MDACTIONPROPERTY_RUNTIME_H
#define MDACTIONPROPERTY_RUNTIME_H

#include <QDateTime>
#include "MDActionHelper_Global.h"

class CMDActionPropertyData_Runtime;
class CMDActionSP;
class MDACTIONHELPERSHARED_EXPORT CMDActionProperty_Runtime
{
public:
    CMDActionProperty_Runtime();
    virtual ~CMDActionProperty_Runtime();
   CMDActionProperty_Runtime(const CMDActionProperty_Runtime& obj);
   CMDActionProperty_Runtime& operator = (const CMDActionProperty_Runtime& obj);
/*!
\section 属性
*/
public:
    //! 执行次数
    long exeCount() const;
    void setExeCount(long exeCount);

    //! 最后执行时间
    QDateTime lastExeTime() const;
    void setLastExeTime(const QDateTime& lastExeTime);

    //! 启用“执行”安全策略
    CMDActionSP* enabledExeSP() const;
    void setEnabledExeSP(CMDActionSP* sp);
protected:
    CMDActionPropertyData_Runtime* m_data;
};

#endif // MDACTIONPROPERTY_RUNTIME_H

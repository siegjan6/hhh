/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDAlarmExpert::init。
模块的使用者必须实现所有的重载接口。

*/

#ifndef MDALARMCONTACTER_H
#define MDALARMCONTACTER_H

#include <QVariant>
#include "MDAlarmExpert_Global.h"

class IMDVariable;
class IMDRTDBVariable;
class CMDActionSPSession;
class MDALARMEXPERTSHARED_EXPORT CMDAlarmContacter
{
public:
    CMDAlarmContacter();
    virtual ~CMDAlarmContacter();
public:
    virtual bool variable(const QString& varName, IMDVariable* var); //! 获取变量信息
    virtual IMDRTDBVariable* getVariable(const QString& varName);
    //! 获取变量数值，若返回 isNull，则该数值不可使用
    virtual QVariant variableValue(const QString& varName);
public:
    virtual QString currentAccount();
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession); //! 安全检查
public:
    virtual bool handleAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime);
};

#endif // MDALARMCONTACTER_H

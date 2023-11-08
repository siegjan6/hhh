/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDVariableExpert::init。
模块的使用者必须实现所有的重载接口。

*/

#ifndef MDVARIABLECONTACTER_H
#define MDVARIABLECONTACTER_H

#include <QVariant>
#include "MDVariableExpert_Global.h"
#include "../../include/MDRTDBInterfaces/IMDChangeVariable.h"

class CMDActionSPSession;
class MDVARIABLEEXPERTSHARED_EXPORT CMDVariableContacter
{
public:
    CMDVariableContacter();
    virtual ~CMDVariableContacter();
public:
    virtual bool onVariableChanged(IMDChangeVariable* changedVariable);
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession);
public:
    virtual void  onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList);
    virtual void  onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList);
    virtual void  onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList);
};

#endif // MDVARIABLECONTACTER_H

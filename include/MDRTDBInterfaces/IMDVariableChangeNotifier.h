/*!
\author zyb 2015.01.08
\section 变量变化通知
*/
#ifndef IMDVARIABLECHANGENOTIFIER
#define IMDVARIABLECHANGENOTIFIER

#include "IMDChangeVariable.h"
class  IMDVariableChangeNotifier
{
public:
    virtual ~IMDVariableChangeNotifier(){}
public:
    virtual void  onChange(IMDChangeVariable *changedVar)=0;
/*!
\section 运行时动态添加设备
*/
public:
    /*!
     设备多个变量变化通知
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    virtual void  onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList)=0;
    virtual void  onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList)=0;

    virtual void  onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList)=0;
};
#endif // IMDVARIABLECHANGENOTIFIER

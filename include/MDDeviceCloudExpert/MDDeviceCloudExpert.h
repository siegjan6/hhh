/*!

\author dGFuZ3p3

\section 功能

设备云专家类，通过 redis 客户端与服务器交互。

*/

#ifndef MDDEVICECLOUDEXPERT_H
#define MDDEVICECLOUDEXPERT_H

#include "MDDeviceCloudExpert_Global.h"
#include <QVariant>
#include "../../include/interface/Common/IMDDataType.h"

class CMDDeviceCloudContacter;
class CMDDeviceCloudExpertImp;
class IMDChangeVariable;
class MDDEVICECLOUDEXPERTSHARED_EXPORT CMDDeviceCloudExpert
{
public:
    CMDDeviceCloudExpert();
    ~CMDDeviceCloudExpert();

/*!
\section 初始化与释放
*/
public:
    void preLoadPara();
public:
    void setParaBeforeLoad(const QString& pid, CMDDeviceCloudContacter* contacter);
    bool load();//! true - 启用了设备云客户端
    void unload();
public:
    bool isEnabled();
    bool isEnabledKV();

/*!
\section 发送数据
*/
public:
    bool sendAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime);
public:
    bool sendVariable(IMDChangeVariable* changedVariable);
    bool sendDeviceVariableChanged(const QList<QByteArray>& jsonMsgList);
    bool sendVariableBatch(const QList<QByteArray>& redisCmdList);
    bool sendVariableBatchToKVCache(const QStringList& varNameList, const QList<QVariant>& varValueList);
protected:
    CMDDeviceCloudExpertImp* m_imp;
};

#endif // MDDEVICECLOUDEXPERT_H

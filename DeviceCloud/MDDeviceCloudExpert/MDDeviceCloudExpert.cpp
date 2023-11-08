#include "MDDeviceCloudExpert.h"
#include "MDDeviceCloudExpertImp.h"

CMDDeviceCloudExpert::CMDDeviceCloudExpert()
{
    m_imp = new CMDDeviceCloudExpertImp;
}

CMDDeviceCloudExpert::~CMDDeviceCloudExpert()
{
    if(m_imp)
        delete m_imp;
}


/*!
\section 初始化与释放
*/
void CMDDeviceCloudExpert::preLoadPara()
{
    if(m_imp)
        m_imp->preLoadPara();
}

void CMDDeviceCloudExpert::setParaBeforeLoad(const QString& pid, CMDDeviceCloudContacter* contacter)
{
    m_imp->setParaBeforeLoad(pid, contacter);
}

bool CMDDeviceCloudExpert::load()
{
    return (m_imp ? m_imp->load() : false);
}

void CMDDeviceCloudExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

bool CMDDeviceCloudExpert::isEnabled()
{
    return (m_imp ? m_imp->isEnabled() : false);
}

bool CMDDeviceCloudExpert::isEnabledKV()
{
    return (m_imp ? m_imp->isEnabledKV() : false);
}

/*!
\section 发送数据
*/
bool CMDDeviceCloudExpert::sendAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime)
{
    return (m_imp ? m_imp->m_cloudClient.sendAlarm(type, name, msg, startTime, endTime) : false);
}

bool CMDDeviceCloudExpert::sendVariable(IMDChangeVariable* changedVariable)
{
    return (m_imp ? m_imp->m_cloudClient.sendVariable(changedVariable) : false);
}

bool CMDDeviceCloudExpert::sendDeviceVariableChanged(const QList<QByteArray>& jsonMsgList)
{
    return (m_imp ? m_imp->m_cloudClient.sendDeviceVariableChanged(jsonMsgList) : false);
}

bool CMDDeviceCloudExpert::sendVariableBatch(const QList<QByteArray>& redisCmdList)
{
    return (m_imp ? m_imp->m_cloudClient.sendVariableBatch(redisCmdList) : false);
}

bool CMDDeviceCloudExpert::sendVariableBatchToKVCache(const QStringList& varNameList, const QList<QVariant>& varValueList)
{
    return (m_imp ? m_imp->m_cloudClient.sendVariableBatchToKVCache(varNameList, varValueList) : false);
}

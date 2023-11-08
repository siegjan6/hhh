#include "MDAlarmExpert.h"
#include "MDAlarmExpertImp.h"
#include <QUaServer>

CMDAlarmExpert::CMDAlarmExpert()
{
    m_imp = new CMDAlarmExpertImp;
}

CMDAlarmExpert::~CMDAlarmExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDAlarmExpert::setParaBeforeLoad(const QString& pid, CMDAlarmConfiger* configer, CMDAlarmContacter* contacter,
                                       short source, CMDHistoryExpert* historyExpert, unsigned long timeout)
{
    m_imp->setParaBeforeLoad(pid, configer, contacter, source, historyExpert, timeout);
}

void CMDAlarmExpert::load()
{
    m_imp->load();
}

void CMDAlarmExpert::load(void* ptr)
{
	m_imp->load(ptr);
}

void CMDAlarmExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

CMDAlarmConfiger* CMDAlarmExpert::configer()
{
    return (m_imp ? m_imp->m_configer : NULL);
}

/*!
\section 报警触发；缓存最近报警到内存；获取最近报警
*/
bool CMDAlarmExpert::submitRawAlarmEvent(CMDRawVariableEvent* rawEvent)
{
    return m_imp->submitRawAlarmEvent(rawEvent);
}

QDateTime CMDAlarmExpert::recentUpdateTime()
{
    return (m_imp ? m_imp->readUpdateTime() : QDateTime());
}

bool CMDAlarmExpert::alarmRecordSet(CMDDatabaseRecordset* dataRecordset)
{
    return (m_imp ? m_imp->alarmRecordSet(dataRecordset) : false);
}

/*!
\section 报警条件改变
*/
bool CMDAlarmExpert::submitDeviceAlarm(const QString& deviceID, const QVariantList &deviceAlarmList)
{
    return (m_imp ? m_imp->submitDeviceAlarm(deviceID, deviceAlarmList) : false);
}

bool CMDAlarmExpert::acknowledgeRequired(const QString& alarmName)
{
    return (m_imp ? m_imp->acknowledgeRequired(alarmName) : false);
}
bool CMDAlarmExpert::acknowledge(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    return (m_imp ? m_imp->acknowledge(alarmID, comment, mode) : false);
}

void CMDAlarmExpert::acknowledgeAll(const int mode/* = 0*/)
{
    if(m_imp)
        m_imp->acknowledgeAll(mode);
}

/*!
\section 报警确认
*/
bool CMDAlarmExpert::confirmRequired(const QString& alarmName)
{
    return (m_imp ? m_imp->confirmRequired(alarmName) : false);
}
bool CMDAlarmExpert::confirm(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    return (m_imp ? m_imp->confirm(alarmID, comment, mode) : false);
}

void CMDAlarmExpert::confirmAll(const int mode/* = 0*/)
{
    if(m_imp)
        m_imp->confirmAll(mode);
}

bool CMDAlarmExpert::suppress(const QString& alarmID, const QString& comment/* = ""*/)
{
    return (m_imp ? m_imp->suppress(alarmID, comment) : false);
}

bool CMDAlarmExpert::shelve(const QString& alarmID, const QString& comment/* = ""*/)
{
    return (m_imp ? m_imp->shelve(alarmID, comment) : false);
}

bool CMDAlarmExpert::addComment(const QString& alarmID, const QString& comment/* = ""*/)
{
    return (m_imp ? m_imp->addComment(alarmID, comment) : false);
}

int CMDAlarmExpert::getAlarmCount()
{
    return (m_imp ? m_imp->getAlarmCount() : -1);
}

QString CMDAlarmExpert::getAlarmByIndex(int index)
{
    return (m_imp ? m_imp->getAlarmByIndex(index) : "");
}

QString CMDAlarmExpert::getAlarmById(QString id)
{
    return (m_imp ? m_imp->getAlarmById(id) : "");
}

QString CMDAlarmExpert::getAlarmConfById(QString id)
{
    return (m_imp ? m_imp->getAlarmConfById(id) : "");
}

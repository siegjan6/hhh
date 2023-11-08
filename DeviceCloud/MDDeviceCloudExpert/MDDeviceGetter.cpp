#include "MDDeviceGetter.h"
#include "MDDeviceCloudClient.h"
#include <QThread>
#include "MDSwitchFactory/MDLogSwitchFactory.h"

const int TIMER_LENGTH = 300;

CMDDeviceGetter::CMDDeviceGetter(QObject *parent) :
    QObject(parent)
{
    m_cloudClient = NULL;
    m_expectToComplete = 0;
    m_completed = 0;
    m_accumTotal = 0;

    connect(&m_timerForCheckTimeout, SIGNAL(timeout()), this, SLOT(checkTimeout()));
}

/*!
\section 启动与停止
*/
bool CMDDeviceGetter::start(const QVariantList& deviceIDList)
{
    if(!prepareStarting(deviceIDList))
        return false;

    getHeadDevice();
    doStat();
    return true;
}

bool CMDDeviceGetter::prepareStarting(const QVariantList& deviceIDList)
{
    QMutexLocker ml(&m_getterMutex);

    //! 如果在本次提取还没完成时，再次收到 deviceIDList，拒绝之！
    //! ---------------------------------------------------------------------
    if(!m_waitingDeviceMap.isEmpty())
    {
        m_cloudClient->m_lsDeviceFlow->LOG_ERROR("收到服务器发来的设备列表，但本次提取任务还未完成，拒绝处理该列表！");
        return false;
    }

    foreach (const QVariant deviceID, deviceIDList)
    {
        QString id = deviceID.toString();
        m_waitingDeviceMap.insert(id.toUpper(), id);
        m_accumTotalDeviceMap.insert(id.toUpper(), id);
    }
    m_expectToComplete = m_waitingDeviceMap.size();
    m_completed = 0;
    m_accumTotal = m_accumTotalDeviceMap.size();
    m_startTime = QDateTime::currentDateTime();
    return true;
}

void CMDDeviceGetter::stop()
{
    QMutexLocker ml(&m_getterMutex);

    m_timerForCheckTimeout.stop();
    m_waitingDeviceMap.clear();
}

void CMDDeviceGetter::bindCloudClient(CMDDeviceCloudClient* cloudClient)
{
    m_cloudClient = cloudClient;
}

/*!
\section 重试定时器（确保取完全部设备）
*/
void CMDDeviceGetter::checkTimeout()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    qint64 timeSpan = m_lastSendTime.msecsTo(curDateTime);
    if(timeSpan < TIMER_LENGTH*3)
        return;

    if(m_cloudClient->isPublishRedisConnected())
    {
        m_cloudClient->m_lsDeviceFlow->LOG_ERROR("获取设备信息【超时】，重发命令 ...");
        getHeadDevice();
    }
    else
        m_cloudClient->m_lsDeviceFlow->LOG_ERROR("获取设备信息【超时】，连接已经断开，等待连接建立 ...");

    doStat();
}

/*!
\section 管理设备；获取设备；进度统计
*/
void CMDDeviceGetter::getHeadDevice()
{
    QMutexLocker ml(&m_getterMutex);

    if(m_waitingDeviceMap.count() > 0)
    {
        m_curDeviceID = m_waitingDeviceMap.first();
//        m_cloudClient->sendCommand_GetDeviceInfo(m_curDeviceID);
        m_cloudClient->sendCommand_GetDeviceInfo_ByEngine(m_curDeviceID);

        m_lastSendTime = QDateTime::currentDateTime();
        m_timerForCheckTimeout.start(TIMER_LENGTH);
    }
    else
    {
        m_timerForCheckTimeout.stop();
    }
}

bool CMDDeviceGetter::completeDevice(const QString& deviceID)
{
    bool result = removeDevice(deviceID);
    getHeadDevice();
    doStat();

    return result;
}

bool CMDDeviceGetter::removeDevice(const QString& deviceID)
{
    QMutexLocker ml(&m_getterMutex);

    QMap<QString, QString>::iterator iPosition = m_waitingDeviceMap.find(deviceID.toUpper());
    if(iPosition == m_waitingDeviceMap.end())
        return false;

    m_waitingDeviceMap.erase(iPosition);
    m_completed ++;
    return true;
}

bool CMDDeviceGetter::findDevice(const QString& deviceID)
{
    QMutexLocker ml(&m_getterMutex);

    QMap<QString, QString>::iterator iPosition = m_waitingDeviceMap.find(deviceID.toUpper());
    return (iPosition != m_waitingDeviceMap.end());
}

bool CMDDeviceGetter::isDeviceProcessed(const QString& deviceID)
{
    // 正常流程下，待处理的设备必须首先位于等待队列中，处理完成后删除；
    // 如果在队列中找不到该设备，即认为设备已经处理过，后续不应该重复处理
    // ---------------------------------------------------------------------------------
    return (!findDevice(deviceID));
}

void CMDDeviceGetter::doStat()
{
    QString totalTime;
    qint64 timeSpan = m_startTime.msecsTo(QDateTime::currentDateTime());
    if(timeSpan < 1000)
        totalTime = QString("%1 毫秒").arg(timeSpan);
    else
        totalTime = QString("%1 秒").arg(timeSpan/1000);

    QString statusMsg;
    if(m_completed < m_expectToComplete)
    {
        statusMsg = "设备总数 %1，已完成 %2，正在获取 %3 ... [%4]";
        statusMsg = statusMsg.arg(m_expectToComplete).arg(m_completed).arg(m_curDeviceID).arg(totalTime);
    }
    else
    {
        if(m_accumTotal <= m_expectToComplete)
        {
            statusMsg = "全部完成：设备总数 %1，已完成 %2 ！ - [%3]";
            statusMsg = statusMsg.arg(m_expectToComplete).arg(m_completed).arg(totalTime);
        }
        else
        {
            statusMsg = "更新完成：设备总数 %1，已更新 %2 ！<%3> - [%4]";
            statusMsg = statusMsg.arg(m_accumTotal).arg(m_completed).arg(m_curDeviceID).arg(totalTime);
        }
        m_cloudClient->m_lsDeviceFlow->LOG_INFO(statusMsg);
    }
    m_cloudClient->handleStatusChanged(statusMsg);
}

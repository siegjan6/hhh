/*!

\author dGFuZ3p3

\section 功能

设备信息获取类，负责将设备信息逐个获取到客户端。

该类的输入是一个【设备ID列表】，之后就逐个获取设备，
收到某个设备信息之后，即将该设备从列表删除，直到所有的
设备信息都到达客户端。

如果某个设备信息没有到达客户端，该设备ID将始终存在于
【设备ID列表】中， CGetterTimer 将不断尝试获取，
这就避免了因为异常情况导致设备信息无法到达客户端。
（例如，某次通信错误导致信息丢失）

*/

#ifndef MDDEVICEGETTER_H
#define MDDEVICEGETTER_H

#include <QString>
#include <QMap>

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QMutex>

class CMDDeviceCloudClient;
class CMDDeviceGetter : public QObject
{
    Q_OBJECT
public:
    CMDDeviceGetter(QObject *parent = 0);
    virtual ~CMDDeviceGetter(){};

/*!
\section 启动与停止
*/
public:
    bool start(const QVariantList& deviceIDList);
    void stop();
    void bindCloudClient(CMDDeviceCloudClient* cloudClient);
protected:
    bool prepareStarting(const QVariantList& deviceIDList);
    CMDDeviceCloudClient* m_cloudClient;

/*!
\section 逐个获取设备；进度统计
*/
public:
    bool completeDevice(const QString& deviceID);
    bool removeDevice(const QString& deviceID);
    bool findDevice(const QString& deviceID);
    bool isDeviceProcessed(const QString& deviceID);
protected:
    void getHeadDevice();
    QMap<QString, QString> m_waitingDeviceMap;
protected:
    void doStat();
    int m_expectToComplete;
    int m_completed;
    QString m_curDeviceID;
    QDateTime m_startTime;
protected:
    //! 记录累加的设备，包括了初始化获取的设备，
    //! 以及后续更新的设备
    QMap<QString, QString> m_accumTotalDeviceMap;
    int m_accumTotal;

/*!
\section 定时器，用于检测提取某个设备是否超时，
             如果超时，则重新发送提取命令，确保取完全部设备
*/
protected:
    QTimer m_timerForCheckTimeout;
    QDateTime m_lastSendTime; //! 最近一次发送命令的时间
protected slots:
    void checkTimeout();

/*!
\section 保护资源的锁（主要是 m_waitingDeviceMap 和 m_timerForCheckTimeout）
*/
protected:
    QMutex m_getterMutex;
};

#endif // MDDEVICEGETTER_H

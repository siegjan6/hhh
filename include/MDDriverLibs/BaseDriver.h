/*!
\author enli 2015.01.08
\section 驱动实现基类
*/
#ifndef BASEDRIVER_H
#define BASEDRIVER_H
#include "MDDriverLibs_global.h"
#include "IDriver.h"
#include "BaseChannel.h"
#include "BaseDataBlock.h"

class CMDLogSwitch;
class MDDRIVERLIBSSHARED_EXPORT CBaseDriver :public IDriver
{
public:
    CBaseDriver();
    ~CBaseDriver();
/*!
\section 项目相关
*/
public:
    void setProjectPath(const QString & projectPath);
private:
    QString m_projectPath;
/*!
\section 初始化与释放接口
*/
public:
    void initial();
    void release();
/*!
\section 设备管理
*/
public:
    IDevice *createDevice();
    bool addDevice(IDevice* dev);
    bool removeDevice(const QString &devName);
    void removeAllDevices();
    IDevice * getDevice(const QString &devName);
private:
    QList<IDevice*> m_deviceList;//设备链表
    QMutex m_deviceListMutex;
/*!
\section 通信控制
*/
public:
    bool startDevice(const QString &devName);
    bool startDevice(IDevice * dev);
    void startAllDevices();
    bool stopDevice(const QString &devName);
    bool stopDevice(IDevice * dev);
    void stopAllDevices();
/*!
\section Channel管理
*/
public:
    //!绑定设备到通信信道
    bool bindDeviceToChannel(IDevice *dev,QString&error);
    bool removeDeviceFromChannel(IDevice *dev);
private:
    CBaseChannel *createChannel();
    bool addChannel(CBaseChannel *channel);
    bool removeChannel(CBaseChannel *channel);
    //!串口通信驱动，同一驱动下设备可以合并使用一个串口:RS485
    CBaseChannel *getSerialChannel(const QString &serialParameter);
    CBaseChannel *getChannel(int iChannelIndex);
    QList<CBaseChannel*> m_channelList;
    QMutex             m_channelListMutex;
/*!
\section 数据块管理
*/
private:
    CBaseDataBlock *createDataBlock();
public:
    //!关联的类库
    void setDriverLib(QLibrary *driverLib);
    QLibrary *getDriverLib();
private:
    QLibrary *m_driverLib;
    //!驱动实例函数
    typedef CBaseChannel* (*createChannelInstance)();
    typedef CBaseDevice* (*createDeviceInstance)();
    typedef CBaseDataBlock* (*createDataBlockInstance)();

    createChannelInstance m_createChannelInstance;
    createDeviceInstance m_createDeviceInstance;
    createDataBlockInstance m_createDataBlockInstance;

    //!日志开关
    CMDLogSwitch* m_logSwitch;
};

#endif // BASEDRIVER_H

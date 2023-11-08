/*!
\author zyb 2015.01.08
\section 驱动接口类 增加、删除、运行、停止device
*/
#ifndef IDRIVER_H
#define IDRIVER_H

#include <QString>
#include <QLibrary>
#include "IDevice.h"
#include "IMDRTDBVariable.h"

class IDriver
{
public:
    virtual ~IDriver(){}
    /*!
    \section 项目相关
    */
public:
    virtual void setProjectPath(const QString & projectPath) = 0;
public:
    /*!
    \section 驱动属性
    */
    //驱动ID eg：NetSCADA6.RTDB.Drivers.MDModbusTCP
    virtual QString driverID()=0;
    //驱动描述，用于枚举驱动
    virtual QString driverDescription()=0;
    /*!
    \section  驱动初始化、释放
    */
    //!初始化
    virtual void initial()=0;
    //!释放
    virtual void release()=0;

    /*!
    \section 设备操作
    */
    //!添加设备
    virtual IDevice* createDevice()=0;
    virtual bool addDevice(IDevice *dev)=0;
    //!删除设备
    virtual bool removeDevice(const QString &devName)=0;
    //!删除所有设备
    virtual void removeAllDevices()=0;
    //!启动设备通信
    virtual bool startDevice(const QString & devName)=0;
    virtual void startAllDevices()=0;
    //!停止设备通信
    virtual bool stopDevice(const QString & devName)=0;
    virtual void stopAllDevices()=0;

    /*!
    \section 关联的类库
    */
    virtual void setDriverLib(QLibrary *driverLib)=0;
    virtual QLibrary *getDriverLib()=0;
};


#endif // IDRIVER_H


/*!
\author zyb 2015.01.08
\section  当前未使用
*/
#ifndef IMDRTDBDEVICEMANAGER_H
#define IMDRTDBDEVICEMANAGER_H

#include <QString>
#include "IMDTreeItem.h"
class IDevice;
class IMDDeviceManager:public IMDTreeItem
{
public:
    virtual ~IMDDeviceManager(){}
//关联变量
public:
    //添加设备
    virtual bool addDevice(QString name,QString driver,QString description,QString&error) = 0;
    virtual bool addDevice(IDevice* device,QString&error) = 0;
    //移除设备
    virtual bool removeDevice(IDevice* device) = 0;
    virtual bool removeDevice(QString devName) = 0;
    //删除所有设备
    virtual void removeAllDevice() = 0;
    //获取设备
    virtual IDevice* getDeviceAt(int index) = 0;
    //获取设备
    virtual IDevice *getDevice(QString devName) = 0;
    //获取设备
    virtual int getDeviceCount() = 0;
};

#endif // IMDRTDBDEVICEMANAGER_H

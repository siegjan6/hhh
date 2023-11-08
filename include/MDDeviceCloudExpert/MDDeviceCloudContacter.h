/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDDeviceCloudExpert::init。
模块的使用者必须实现所有的重载接口。

*/

#ifndef MDDEVICECLOUDCONTACTER_H
#define MDDEVICECLOUDCONTACTER_H

#include "MDDeviceCloudExpert_Global.h"
#include <QVariant>

class MDDEVICECLOUDEXPERTSHARED_EXPORT CMDDeviceCloudContacter
{
public:
    CMDDeviceCloudContacter();
    virtual ~CMDDeviceCloudContacter();
public:
    virtual bool handleDeviceData(const QString& deviceID,
                           const QVariantList& driverList,  const QVariantList& varList,
                            const QVariantList& alarmInfoList,
                            const QVariantList& dataArchiveList); //! 处理来自云的设备数据
public:
    virtual void handleStatusChanged(const QString& statusMsg); //! 将内部状态通知到外部
};

#endif // MDDEVICECLOUDCONTACTER_H

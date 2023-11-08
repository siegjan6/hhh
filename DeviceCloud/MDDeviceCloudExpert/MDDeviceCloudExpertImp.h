/*!

\author dGFuZ3p3

\section 功能

设备云专家类的实现类，通过 redis 客户端与服务器交互。

*/

#ifndef MDDEVICECLOUDEXPERTIMP_H
#define MDDEVICECLOUDEXPERTIMP_H

#include "MDDeviceCloudClient.h"

///
/// \brief 客户端参数管理类
///
class CDeviceCloudClientPara
{
public:
    CDeviceCloudClientPara();
public:
    bool load();
    void save();
    QString paraFileName();
public:
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
public:
    bool m_enable;
    bool m_enableKV;
    QString m_clientID;
    QString m_serverIP;
    int m_serverPort;
};

class CMDDeviceCloudContacter;
///
/// \brief 设备云专家类的实现类
///
class CMDDeviceCloudExpertImp
{
public:
    CMDDeviceCloudExpertImp();

/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDDeviceCloudContacter* contacter);
    bool load();//! true - 启用了设备云客户端
    void unload();
    bool isEnabled();
    bool isEnabledKV();
public:
    void preLoadPara();
    QString prjPath();
protected:
    bool m_paraLoaded;
    QString m_pid;
    CMDDeviceCloudContacter*  m_contacter;

/*!
\section 通过 redis 客户端与服务器交互
*/
public:
    CMDDeviceCloudClient m_cloudClient;
    CDeviceCloudClientPara m_para;
public:
    bool handleDeviceData(const QString& deviceID,
                           const QVariantList& driverList,  const QVariantList& varList,
                            const QVariantList& alarmInfoList,
                            const QVariantList& dataArchiveList); //! 处理来自云的设备数据
public:
    void handleStatusChanged(const QString& statusMsg); //! 将内部状态通知到外部
};

#endif // MDDEVICECLOUDEXPERTIMP_H

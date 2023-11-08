#ifndef IMDPROJECTREMOTECONFIGER_H
#define IMDPROJECTREMOTECONFIGER_H

#include <qvariant.h>

//设备报警Json配置
struct MDDeviceAlarmJsonConfig
{
    QString m_deviceID;
    QVariantList m_alarmConfigList;
    const MDDeviceAlarmJsonConfig& operator=( const MDDeviceAlarmJsonConfig& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_deviceID= other.m_deviceID;
        m_deviceAlarmInfoList= other.m_deviceAlarmInfoList;
        return other;
    }
};
//驱动Json配置
struct MDDeviceDriverJsonConfig
{
    QString m_deviceID;
    QVariantList m_driverConfigList;
    const MDDeviceDriverJsonConfig& operator=( const MDDeviceDriverJsonConfig& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_deviceID= other.m_deviceID;
        m_driverConfigList= other.m_driverConfigList;
        return other;
    }
};
//变量Json配置
struct MDDeviceVariableJsonConfig
{
    QString m_deviceID;
    QVariantList m_variableConfigList;
    const MDDeviceVariableJsonConfig& operator=( const MDDeviceVariableJsonConfig& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_deviceID= other.m_deviceID;
        m_variableConfigList= other.m_variableConfigList;
        return other;
    }
};

//数据归档Json配置
struct MDDeviceDataArchiveJsonConfig
{
    QString m_deviceID;
    QVariantList m_dataArchiveConfigList;
    const MDDeviceDataArchiveJsonConfig& operator=( const MDDeviceDataArchiveJsonConfig& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_deviceID= other.m_deviceID;
        m_dataArchiveConfigList= other.m_dataArchiveConfigList;
        return other;
    }
};

class IMDProjectRemoteConfiger
{
public:
    virtual ~IMDProjectRemoteConfiger(){}
public:
public:
    virtual void start()= 0;
    virtual void stop()= 0;
    //存储变量数据
    virtual void triggerChangeDataArchives(const QString & varName,const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp)= 0;
    //获取变量数据： dataRecordset包含两个字：RecordTime、Value
    virtual bool getData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset *dataRecordset)= 0;
    virtual bool getAllData(const QString & varName,CMDDatabaseRecordset  *dataRecordset)= 0;
    //删除变量数据
    virtual void deleteData(const QString & varName,const QDateTime &startTime)= 0;
    virtual void deleteData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime)= 0;
    virtual void deleteAllData(const QString & varName)= 0;


};

#endif // IMDPROJECTREMOTECONFIGER_H

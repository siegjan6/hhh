#include "MDDeviceCloudExpertImp.h"
#include "MDDeviceCloudContacter.h"
#include "MDDeviceCloudClient.h"

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>

CDeviceCloudClientPara::CDeviceCloudClientPara()
{

}

bool CDeviceCloudClientPara::load()
{
    QFile loadFile(paraFileName());
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());
    return true;
}

void CDeviceCloudClientPara::save()
{
    QFile saveFile(paraFileName());
    saveFile.open(QIODevice::WriteOnly);

    QJsonObject jsDeviceMgr;
    write(jsDeviceMgr);
    QJsonDocument saveDoc(jsDeviceMgr);
    saveFile.write(saveDoc.toJson());
}

QString CDeviceCloudClientPara::paraFileName()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString file;
    #ifdef Q_OS_LINUX
        file = appPath + "/device_cloud.json";
    #else // windows
        file = appPath + "\\device_cloud.json";
    #endif
    return file;
}

void CDeviceCloudClientPara::read(const QJsonObject &json)
{
//    QJsonObject head = json["HEAD"].toObject();

    m_enable = (0 == json["@Enable"].toString().compare("1") ? true : false);
    m_enableKV = (0 == json["@EnableKV"].toString().compare("1") ? true : false);
    m_clientID = json["@ClientID"].toString();
    m_serverIP = json["@ServerIP"].toString();
    m_serverPort = QVariant(json["@ServerPort"].toString()).toInt();
}

void CDeviceCloudClientPara::write(QJsonObject &json) const
{
    QString enableStr = (m_enable ? "1":"0");
    QString enableKVStr = (m_enableKV ? "1":"0");
    json["@Enable"] = enableStr;
    json["@enableKV"] = enableKVStr;
    json["@ClientID"] = m_clientID;
    json["@ServerIP"] = m_serverIP;
    json["@ServerPort"] = QVariant(m_serverPort).toString();

    QJsonObject jsHead;
//    json["HEAD"] = jsHead;
}

///
/// \brief CMDDeviceCloudExpertImp::CMDDeviceCloudExpertImp
///
CMDDeviceCloudExpertImp::CMDDeviceCloudExpertImp()
{
    m_contacter = NULL;
    m_paraLoaded = false;

    QString rtCmd, rtDeviceCmd, arcCmd;
    //m_cloudClient.buildAlarmCmd(0, "beijing.alarmtemp", "温度高", QDateTime::currentDateTime(), QDateTime::currentDateTime(), rtCmd, rtDeviceCmd, arcCmd);
//    m_cloudClient.sendVariable("device1.v1", QVariant(41), 1, QDateTime::currentDateTime(), MDDataType::Int16, true);
}

/*!
\section 初始化与释放
*/
void CMDDeviceCloudExpertImp::preLoadPara()
{
    if(false == m_paraLoaded)
    {
        m_para.load();
        m_paraLoaded = true;
    }
}

void CMDDeviceCloudExpertImp::setParaBeforeLoad(const QString& pid, CMDDeviceCloudContacter* contacter)
{
    m_contacter = contacter;
    m_pid = pid;
    m_cloudClient.createSwitchs(pid, true);
}

QString CMDDeviceCloudExpertImp::prjPath()
{
    return m_pid;
}

bool CMDDeviceCloudExpertImp::load()
{
    preLoadPara();
    if(m_para.m_enable)
    {
        m_cloudClient.load(this, &m_para);
    }
    if(m_para.m_enableKV)
    {
        m_cloudClient.loadKV(this, &m_para);
    }
    return m_para.m_enable;
}

void CMDDeviceCloudExpertImp::unload()
{
    m_paraLoaded = false;
    m_cloudClient.unload();
}

bool CMDDeviceCloudExpertImp::isEnabled()
{
    return m_para.m_enable;
}

bool CMDDeviceCloudExpertImp::isEnabledKV()
{
    return m_para.m_enableKV;
}

/*!
\section 通过 redis 客户端与服务器交互
*/
bool CMDDeviceCloudExpertImp::handleDeviceData(const QString& deviceID,
                       const QVariantList& driverList,  const QVariantList& varList,
                        const QVariantList& alarmInfoList,
                        const QVariantList& dataArchiveList)
{
    return (m_contacter ? m_contacter->handleDeviceData(deviceID, driverList, varList, alarmInfoList, dataArchiveList) : false);
}

void CMDDeviceCloudExpertImp::handleStatusChanged(const QString& statusMsg)
{
    if(m_contacter)
        m_contacter->handleStatusChanged(statusMsg);
}

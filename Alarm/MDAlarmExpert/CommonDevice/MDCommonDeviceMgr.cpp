#include "MDCommonDeviceMgr.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>

CMDCommonDeviceMgr::CMDCommonDeviceMgr()
{
}

bool CMDCommonDeviceMgr::load()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString file;
    #ifdef Q_OS_LINUX
        file = appPath + "/device_alarm.json";
    #else // windows
        file = appPath + "\\device_alarm.json";
    #endif

    QFile loadFile(file);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    read(loadDoc.object());

    save();

    return true;
}

void CMDCommonDeviceMgr::save()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString file;
    #ifdef Q_OS_LINUX
        file = appPath + "/测试.json";
    #else // windows
        file = appPath + "\\测试.json";
    #endif

    QFile saveFile(file);
    saveFile.open(QIODevice::WriteOnly);

    QJsonObject jsDeviceMgr;
    write(jsDeviceMgr);
    QJsonDocument saveDoc(jsDeviceMgr);
    saveFile.write(saveDoc.toJson());
}

void CMDCommonDeviceMgr::read(const QJsonObject &json)
{
    QJsonObject head = json["HEAD"].toObject();

    m_devices.clear();

    QJsonArray deviceArray = head["MDDevices"].toArray();
    for (int index = 0; index < deviceArray.size(); ++index)
    {
        QJsonObject jsDevice = deviceArray[index].toObject();
        CMDCommonDevice device;
        device.read(jsDevice);
        m_devices.append(device);
    }
}

void CMDCommonDeviceMgr::write(QJsonObject &json) const
{
    QJsonObject jsHead;
    QJsonArray deviceArray;
    foreach (const CMDCommonDevice device, m_devices)
    {
        QJsonObject jsDevice;
        device.write(jsDevice);
        deviceArray.append(jsDevice);
    }
    jsHead["MDDevices"] = deviceArray;

    json["HEAD"] = jsHead;
}

void CMDCommonDeviceMgr::deviceAlarmToConditions(QList<CMDAlarmCondition*>& conditionList)
{
    foreach (const CMDCommonDevice device, m_devices)
    {
        device.alarmToConditions(conditionList);
    }
}

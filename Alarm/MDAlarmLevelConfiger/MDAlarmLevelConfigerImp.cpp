#include "MDAlarmLevelConfigerImp.h"
#include "MDAlarmLevel.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDAlarmLevelConfigerImp::CMDAlarmLevelConfigerImp()
{
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAlarmLevelConfigerImp::loadConfig(const QString& projectPath)
{
    m_ConfigFileName = makePath(projectPath) + "AlarmLevel.dat";
    return loadFromFile();
}

void CMDAlarmLevelConfigerImp::unloadConfig()
{
    QMap<QString, CMDAlarmLevel*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        delete i.value();
    m_configedMap.clear();
}

QString CMDAlarmLevelConfigerImp::makePath(const QString& projectPath)
{
    QString modulePath;
    #ifdef Q_OS_LINUX
    modulePath = projectPath + "/Alarm/";
    #else
    modulePath = projectPath + "\\Alarm\\";
    #endif

    QDir dir;
    dir.mkpath(modulePath);
    return modulePath;
}

bool CMDAlarmLevelConfigerImp::loadFromFile()
{
    QFile configFile(m_ConfigFileName);
    if(!configFile.exists())
        return false;

    configFile.open(QIODevice::ReadOnly);
    QByteArray ba;
    QTextStream stream(&configFile);
    stream.setCodec("UTF-8");
    ba = stream.readAll().toUtf8();
    configFile.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return false;

    deserialize(doc.object());
    return true;

    // 2 进制序列化
//    bool result = true;
//    configFile.open(QIODevice::ReadOnly);
//    quint16 count;
//    QDataStream readDS(&configFile);
//    readDS >> count;
//    for(quint16 i = 0; i < count; i++)
//    {
//        CMDAlarmLevel* configProperty = new CMDAlarmLevel;
//        if(configProperty)
//        {
//            configProperty->deserialize(readDS);

//            addToList(configProperty);
//        }
//        else
//        {
//            result = false;
//            break;
//        }
//    }
//    return result;
}

bool CMDAlarmLevelConfigerImp::saveToFile()
{
    QFile configFile(m_ConfigFileName);
    if(!configFile.exists())
    {
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    if(!configFile.exists())
        return false;

    configFile.open(QIODevice::WriteOnly);
    QJsonObject json;
    serialize(json);
    QJsonDocument doc;
    doc.setObject(json);
    QByteArray ba = doc.toJson(/*QJsonDocument::Compact*/);
    QTextStream stream(&configFile);
    stream.setCodec("UTF-8");
    stream << ba;
    configFile.close();
    return true;

    // 2 进制序列化
//    configFile.open(QIODevice::WriteOnly);
    //    quint16 count = (quint16)m_configedMap.size();
    //    QDataStream writeDS(&configFile);
    //    writeDS << count;
    //    QMap<QString, CMDAlarmLevel*> ::const_iterator i;
    //    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    //    {
    //        CMDAlarmLevel* configProperty = i.value();
    //        configProperty->serialize(writeDS);
    //    }
}

QString CMDAlarmLevelConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDAlarmLevelConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("AlarmLevel"));

    QJsonArray ContentJson;
    foreach (CMDAlarmLevel* configProperty, m_configedMap)
    {
        QJsonObject ConditionJson;
        configProperty->serialize(ConditionJson);
        ContentJson.append(ConditionJson);
    }

    json.insert("Content", ContentJson);
    json.insert("Head", HeadJson);
}

void CMDAlarmLevelConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonArray ContentJson = json.value("Content").toArray();
    foreach (const QJsonValue& ConditionJson, ContentJson)
    {
        CMDAlarmLevel* configProperty = new CMDAlarmLevel;
        if(configProperty)
        {
            configProperty->deserialize(ConditionJson.toObject());

            addToList(configProperty);
        }
    }
}

/*!
\section 管理报警等级
*/
bool CMDAlarmLevelConfigerImp::addToList(CMDAlarmLevel* configProperty)
{
    QMap<QString, CMDAlarmLevel*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
    {
        //CMDAlarmLevel* independentAction = iPosition.value();
        delete configProperty;
        return false;
    }
    m_configedMap.insert(configProperty->name().toUpper(), configProperty);
    return true;
}

void CMDAlarmLevelConfigerImp::configedList(QList<CMDAlarmLevel*>& accountList)
{
    accountList.clear();

    QMap<QString, CMDAlarmLevel*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        accountList.append(i.value());
}

int CMDAlarmLevelConfigerImp::count()
{
    return m_configedMap.size();
}

bool CMDAlarmLevelConfigerImp::add(const CMDAlarmLevel* configProperty)
{
    QMap<QString, CMDAlarmLevel*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
        return false;

    CMDAlarmLevel* newProperty = new CMDAlarmLevel(*configProperty);
    if(newProperty)
        m_configedMap.insert(configProperty->name().toUpper(), newProperty);
    return true;
}

bool CMDAlarmLevelConfigerImp::delete1(const QString& name)
{
    QMap<QString, CMDAlarmLevel*>::iterator iPosition = m_configedMap.find(name.toUpper());
    if(iPosition == m_configedMap.end())
        return false;

    delete iPosition.value();
    m_configedMap.erase(iPosition);
    return true;
}

CMDAlarmLevel* CMDAlarmLevelConfigerImp::find(const QString& name)
{
    QMap<QString, CMDAlarmLevel*>::const_iterator iPosition = m_configedMap.find(name.toUpper());
    return (iPosition != m_configedMap.end() ? iPosition.value() : NULL);
}

CMDAlarmLevel* CMDAlarmLevelConfigerImp::find(int index)
{
    int loopIndex = 0;
    QMap<QString, CMDAlarmLevel*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    {
        if(loopIndex == index)
            return i.value();

        loopIndex ++;
    }
    return NULL;
}

bool CMDAlarmLevelConfigerImp::modify(const CMDAlarmLevel* sourceConfig,
                                const CMDAlarmLevel* destConfig)
{
    bool result = false;
    bool nameChanged = (0 != sourceConfig->name().compare(
                                                    destConfig->name(), Qt::CaseSensitivity::CaseInsensitive));
    if(nameChanged)
    {
        if(NULL == find(destConfig->name()))  // 改名后，没有重名对象
        {
            delete1(sourceConfig->name());     // key变化，删除源对象
            add(destConfig);                             // 增加新对象
            result = true;
        }
    }
    else
    {
        QMap<QString, CMDAlarmLevel*>::const_iterator iSource = m_configedMap.find(sourceConfig->name().toUpper());
        if(iSource != m_configedMap.end())
        {
            iSource.value()->copy(destConfig);
            result = true;
        }
    }

    return result;
}

#include "MDAlarmConditionConfigerImp.h"
#include "MDAlarmCondition.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDAlarmConditionConfigerImp::CMDAlarmConditionConfigerImp()
{
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAlarmConditionConfigerImp::loadConfig(const QString& projectPath)
{
    m_ConfigFileName = makePath(projectPath) + "AlarmCondition.dat";
    return loadFromFile();
}

void CMDAlarmConditionConfigerImp::unloadConfig()
{
    deleteAll();
}

QString CMDAlarmConditionConfigerImp::makePath(const QString& projectPath)
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

bool CMDAlarmConditionConfigerImp::loadFromFile()
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
//    configFile.open(QIODevice::ReadOnly);
//    quint16 count;
//    QDataStream readDS(&configFile);
//    readDS >> count;
//    for(quint16 i = 0; i < count; i++)
//    {
//        CMDAlarmCondition* configProperty = new CMDAlarmCondition;
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
//    return true;
}

bool CMDAlarmConditionConfigerImp::saveToFile()
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
    //    QMap<QString, CMDAlarmCondition*> ::const_iterator i;
    //    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    //    {
    //        CMDAlarmCondition* configProperty = i.value();
    //        configProperty->serialize(writeDS);
    //    }
//    return true;
}

QString CMDAlarmConditionConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDAlarmConditionConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("AlarmCondition"));

    QJsonArray ContentJson;
    foreach (CMDAlarmCondition* configProperty, m_configedMap)
    {
        QJsonObject ConditionJson;
        configProperty->serialize(ConditionJson);
        ContentJson.append(ConditionJson);
    }

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDAlarmConditionConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonArray ContentJson = json.value("Content").toArray();
    foreach (const QJsonValue& ConditionJson, ContentJson)
    {
        CMDAlarmCondition* configProperty = new CMDAlarmCondition;
        if(configProperty)
        {
            configProperty->deserialize(ConditionJson.toObject());
            addToList(configProperty);
        }
    }
}

/*!
\section 管理报警条件
*/
bool CMDAlarmConditionConfigerImp::addToList(CMDAlarmCondition* configProperty)
{
    QMap<QString, CMDAlarmCondition*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
    {
        //CMDAlarmCondition* independentAction = iPosition.value();
        delete configProperty;
        return false;
    }
    m_configedMap.insert(configProperty->name().toUpper(), configProperty);
    return true;
}

void CMDAlarmConditionConfigerImp::configedList(QList<CMDAlarmCondition*>& acList)
{
    acList.clear();

    QMap<QString, CMDAlarmCondition*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        acList.append(i.value());
}

int CMDAlarmConditionConfigerImp::count()
{
    return m_configedMap.size();
}

bool CMDAlarmConditionConfigerImp::add(const CMDAlarmCondition* configProperty)
{
    QMap<QString, CMDAlarmCondition*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
        return false;

    CMDAlarmCondition* newProperty = new CMDAlarmCondition(*configProperty);
    if(newProperty)
        m_configedMap.insert(configProperty->name().toUpper(), newProperty);
    return true;
}

bool CMDAlarmConditionConfigerImp::addNew(CMDAlarmCondition* new_configProperty)
{
    QMap<QString, CMDAlarmCondition*>::const_iterator iPosition = m_configedMap.find(new_configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
    {
        delete new_configProperty;
        return false;
    }

    m_configedMap.insert(new_configProperty->name().toUpper(), new_configProperty);
    return true;
}

bool CMDAlarmConditionConfigerImp::delete1(const QString& name)
{
    QMap<QString, CMDAlarmCondition*>::iterator iPosition = m_configedMap.find(name.toUpper());
    if(iPosition == m_configedMap.end())
        return false;

    delete iPosition.value();
    m_configedMap.erase(iPosition);
    return true;
}

void CMDAlarmConditionConfigerImp::deleteAll()
{
    QMap<QString, CMDAlarmCondition*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        delete i.value();
    m_configedMap.clear();
}

CMDAlarmCondition* CMDAlarmConditionConfigerImp::find(const QString& name)
{
    QMap<QString, CMDAlarmCondition*>::const_iterator iPosition = m_configedMap.find(name.toUpper());
    return (iPosition != m_configedMap.end() ? iPosition.value() : NULL);
}

CMDAlarmCondition* CMDAlarmConditionConfigerImp::find(int index)
{
    int loopIndex = 0;
    QMap<QString, CMDAlarmCondition*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    {
        if(loopIndex == index)
            return i.value();

        loopIndex ++;
    }
    return NULL;
}

bool CMDAlarmConditionConfigerImp::modify(const CMDAlarmCondition* sourceConfig,
                                const CMDAlarmCondition* destConfig)
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
        QMap<QString, CMDAlarmCondition*>::const_iterator iSource = m_configedMap.find(sourceConfig->name().toUpper());
        if(iSource != m_configedMap.end())
        {
            iSource.value()->copy(destConfig);
            result = true;
        }
    }

    return result;
}

void CMDAlarmConditionConfigerImp::deletePrefixMatched(const QString& prefix)
{
    QMap<QString, CMDAlarmCondition*> ::iterator iPosition;
    QMap<QString, CMDAlarmCondition*> ::iterator iPosition1;
    for (iPosition = m_configedMap.begin(); iPosition != m_configedMap.end();)
    {
        iPosition1 = iPosition;
        iPosition++;

        QString namePrefix = iPosition1.value()->name().left(prefix.length());
        if(0 == namePrefix.compare(prefix, Qt::CaseInsensitive))
        {
            delete iPosition1.value();
            m_configedMap.erase(iPosition1);
        }
    }
}

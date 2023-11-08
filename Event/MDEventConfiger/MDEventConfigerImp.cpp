#include "MDEventConfigerImp.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDEventConfigerImp::CMDEventConfigerImp()
{
    m_ConfigFileName = "";
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDEventConfigerImp::loadConfig(const QString& projectPath)
{
    m_ConfigFileName = makePath(projectPath) + "Event.dat";
    return loadFromFile();
}

void CMDEventConfigerImp::unloadConfig()
{
    QMap<QString, CMDEventProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        delete i.value();
    m_configedMap.clear();
}

QString CMDEventConfigerImp::makePath(const QString& projectPath)
{
    QString modulePath;
    #ifdef Q_OS_LINUX
    modulePath = projectPath + "/Event/";
    #else
    modulePath = projectPath + "\\Event\\";
    #endif

    QDir dir;
    dir.mkpath(modulePath);
    return modulePath;
}

bool CMDEventConfigerImp::loadFromFile()
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
//    quint16 count;
//    QDataStream readDS(&configFile);
//    readDS >> count;
//    for(quint16 i = 0; i < count; i++)
//    {
//        quint16 eventType;
//        readDS >> eventType;
//        CMDEventProperty_Config* configProperty = CEPC_Factory::newConfig((MDEventType)eventType);
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

bool CMDEventConfigerImp::saveToFile()
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
//    quint16 count = (quint16)m_configedMap.size();
//    QDataStream writeDS(&configFile);
//    writeDS << count;
//    QMap<QString, CMDEventProperty_Config*> ::const_iterator i;
//    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
//    {
//        CMDEventProperty_Config* configProperty = i.value();
//        quint16 eventType = (quint16)configProperty->type();
//        writeDS << eventType;
//        configProperty->serialize(writeDS);
//    }
//    return true;
}

QString CMDEventConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDEventConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("Event"));

    QJsonArray ContentJson;
    foreach (CMDEventProperty_Config* configProperty, m_configedMap)
    {
        QJsonObject ConditionJson;
        configProperty->serialize(ConditionJson);
        ContentJson.append(ConditionJson);
    }

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDEventConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonArray ContentJson = json.value("Content").toArray();
    foreach (const QJsonValue& ConditionJson, ContentJson)
    {
        // 解析基本属性，获取类型，根据类型创建具体对象
        CMDEventProperty_Config baseProp;
        baseProp.deserialize(ConditionJson.toObject());
        MDEventType eType = (MDEventType)baseProp.type();

        CMDEventProperty_Config* configProperty = CEPC_Factory::newConfig(eType);
        if(configProperty)
        {
            configProperty->deserialize(ConditionJson.toObject());
            if(!configProperty->name().isEmpty())
            {
                addToList(configProperty);
            }
            else
                delete configProperty;
        }
    }
}

/*!
\section 管理事件
*/
bool CMDEventConfigerImp::addToList(CMDEventProperty_Config* configProperty)
{
    QMap<QString, CMDEventProperty_Config*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
    {
        //CMDEventProperty_Config* independentEvent = iPosition.value();
        delete configProperty;
        return false;
    }
    m_configedMap.insert(configProperty->name().toUpper(), configProperty);
    return true;
}

void CMDEventConfigerImp::configedEventList(QList<CMDEventProperty_Config*>& eventList)
{
    eventList.clear();

    QMap<QString, CMDEventProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        eventList.append(i.value());
}

int CMDEventConfigerImp::count()
{
    return m_configedMap.size();
}

bool CMDEventConfigerImp::addEvent(const CMDEventProperty_Config* configProperty)
{
    QMap<QString, CMDEventProperty_Config*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
        return false;

    CMDEventProperty_Config* newEvent = CEPC_Factory::newConfig(configProperty->type(), configProperty);
    if(newEvent)
        m_configedMap.insert(configProperty->name().toUpper(), newEvent);
    return true;
}

bool CMDEventConfigerImp::deleteEvent(const QString& name)
{
    QMap<QString, CMDEventProperty_Config*>::iterator iPosition = m_configedMap.find(name.toUpper());
    if(iPosition == m_configedMap.end())
        return false;

    delete iPosition.value();
    m_configedMap.erase(iPosition);
    return true;
}

CMDEventProperty_Config* CMDEventConfigerImp::findEvent(const QString& name)
{
    QMap<QString, CMDEventProperty_Config*>::const_iterator iPosition = m_configedMap.find(name.toUpper());
    return (iPosition != m_configedMap.end() ? iPosition.value() : NULL);
}

CMDEventProperty_Config* CMDEventConfigerImp::findEvent(int index)
{
    int loopIndex = 0;
    QMap<QString, CMDEventProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    {
        if(loopIndex == index)
            return i.value();

        loopIndex ++;
    }
    return NULL;
}

bool CMDEventConfigerImp::modifyEvent(const CMDEventProperty_Config* sourceConfig,
                                const CMDEventProperty_Config* destConfig)
{
    bool result = false;
    bool nameChanged = (0 != sourceConfig->name().compare(
                                                    destConfig->name(), Qt::CaseSensitivity::CaseInsensitive));
    if(nameChanged)
    {
        if(NULL == findEvent(destConfig->name()))  // 改名后，没有重名对象
        {
            deleteEvent(sourceConfig->name());     // key变化，删除源对象
            addEvent(destConfig);                             // 增加新对象
            result = true;
        }
    }
    else
    {
        QMap<QString, CMDEventProperty_Config*>::const_iterator iSource = m_configedMap.find(sourceConfig->name().toUpper());
        if(iSource != m_configedMap.end())
        {
            iSource.value()->copy(destConfig);
            result = true;
        }
    }

    return result;
}

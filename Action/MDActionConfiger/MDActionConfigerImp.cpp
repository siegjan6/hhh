#include "MDActionConfigerImp.h"
#include "../../include/MDActionHelper/MDAction.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDActionConfigerImp::CMDActionConfigerImp()
{
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDActionConfigerImp::loadConfig(const QString& projectPath)
{
    m_ConfigFileName = makePath(projectPath) + "Action.dat";
    return loadFromFile();
}

void CMDActionConfigerImp::unloadConfig()
{
    QMap<QString, CMDActionProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        delete i.value();
    m_configedMap.clear();
}

QString CMDActionConfigerImp::makePath(const QString& projectPath)
{
    QString modulePath;
    #ifdef Q_OS_LINUX
    modulePath = projectPath + "/Action/";
    #else
    modulePath = projectPath + "\\Action\\";
    #endif

    QDir dir;
    dir.mkpath(modulePath);
    return modulePath;
}

bool CMDActionConfigerImp::loadFromFile()
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
//        quint16 actionType;
//        readDS >> actionType;
//        CMDActionProperty_Config* configProperty = CAPC_Factory::newConfig((MDActionType)actionType);
//        if(configProperty)
//        {
//            configProperty->deserialize(readDS);
//            if(!configProperty->name().isEmpty())
//            {
//                addToList(configProperty);
//            }
//            else
//                delete configProperty;
//        }
//        else
//        {
//            result = false;
//            break;
//        }
//    }
//    return result;
}

bool CMDActionConfigerImp::saveToFile()
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
//    QMap<QString, CMDActionProperty_Config*> ::const_iterator i;
//    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
//    {
//        CMDActionProperty_Config* configProperty = i.value();
//        quint16 actionType = (quint16)configProperty->actionID().type();
//        writeDS << actionType;
//        configProperty->serialize(writeDS);
//    }
//    return true;
}

QString CMDActionConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDActionConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("Action"));

    QJsonArray ContentJson;
    foreach (CMDActionProperty_Config* configProperty, m_configedMap)
    {
        QJsonObject ConditionJson;
        configProperty->serialize(ConditionJson);
        ContentJson.append(ConditionJson);
    }

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDActionConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonArray ContentJson = json.value("Content").toArray();
    foreach (const QJsonValue& ConditionJson, ContentJson)
    {
        // 解析基本属性，获取类型，根据类型创建具体对象
        CMDActionProperty_Config baseProp;
        baseProp.deserialize(ConditionJson.toObject());
        MDActionType aType = (MDActionType)baseProp.actionID().type();

        CMDActionProperty_Config* configProperty = CAPC_Factory::newConfig(aType);
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
\section 管理动作
*/
bool CMDActionConfigerImp::addToList(CMDActionProperty_Config* configProperty)
{
    QMap<QString, CMDActionProperty_Config*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
    {
        //CMDActionProperty_Config* independentAction = iPosition.value();
        delete configProperty;
        return false;
    }
    m_configedMap.insert(configProperty->name().toUpper(), configProperty);
    return true;
}

void CMDActionConfigerImp::configedActionList(QList<CMDActionProperty_Config*>& actionList)
{
    actionList.clear();

    QMap<QString, CMDActionProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        actionList.append(i.value());
}

int CMDActionConfigerImp::count()
{
    return m_configedMap.size();
}

bool CMDActionConfigerImp::addAction(const CMDActionProperty_Config* configProperty)
{
    QMap<QString, CMDActionProperty_Config*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
        return false;

    CMDActionProperty_Config* newAction = CAPC_Factory::newConfig(configProperty->actionID().type(), configProperty);
    if(newAction)
        m_configedMap.insert(configProperty->name().toUpper(), newAction);
    return true;
}

bool CMDActionConfigerImp::deleteAction(const QString& name)
{
    QMap<QString, CMDActionProperty_Config*>::iterator iPosition = m_configedMap.find(name.toUpper());
    if(iPosition == m_configedMap.end())
        return false;

    delete iPosition.value();
    m_configedMap.erase(iPosition);
    return true;
}

CMDActionProperty_Config* CMDActionConfigerImp::findAction(const QString& name)
{
    QMap<QString, CMDActionProperty_Config*>::const_iterator iPosition = m_configedMap.find(name.toUpper());
    return (iPosition != m_configedMap.end() ? iPosition.value() : NULL);
}

CMDActionProperty_Config* CMDActionConfigerImp::findAction(int index)
{
    int loopIndex = 0;
    QMap<QString, CMDActionProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    {
        if(loopIndex == index)
            return i.value();

        loopIndex ++;
    }
    return NULL;
}

bool CMDActionConfigerImp::modifyAction(const CMDActionProperty_Config* sourceConfig,
                                const CMDActionProperty_Config* destConfig)
{
    bool result = false;
    bool nameChanged = (0 != sourceConfig->name().compare(
                                                    destConfig->name(), Qt::CaseSensitivity::CaseInsensitive));
    if(nameChanged)
    {
        if(NULL == findAction(destConfig->name()))  // 改名后，没有重名对象
        {
            deleteAction(sourceConfig->name());     // key变化，删除源对象
            addAction(destConfig);                             // 增加新对象
            result = true;
        }
    }
    else
    {
        QMap<QString, CMDActionProperty_Config*>::const_iterator iSource = m_configedMap.find(sourceConfig->name().toUpper());
        if(iSource != m_configedMap.end())
        {
            iSource.value()->copy(destConfig);
            result = true;
        }
    }

    return result;
}

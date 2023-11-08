#include "MDPowerGroupConfigerImp.h"
#include "../../include/MDSecurityPolicyHelper/MDPowerGroup.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDPowerGroupConfigerImp::CMDPowerGroupConfigerImp()
{
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDPowerGroupConfigerImp::loadConfig(const QString& projectPath)
{
    m_ConfigFileName = makePath(projectPath) + "PowerGroup.dat";
    return loadFromFile();
}

void CMDPowerGroupConfigerImp::unloadConfig()
{
    QMap<QString, CMDPowerGroup*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        delete i.value();
    m_configedMap.clear();
}

QString CMDPowerGroupConfigerImp::makePath(const QString& projectPath)
{
    QString modulePath;
    #ifdef Q_OS_LINUX
    modulePath = projectPath + "/SecurityPolicy/";
    #else
    modulePath = projectPath + "\\SecurityPolicy\\";
    #endif

    QDir dir;
    dir.mkpath(modulePath);
    return modulePath;
}

bool CMDPowerGroupConfigerImp::loadFromFile()
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
//    quint16 count;
//    QDataStream readDS(&configFile);
//    readDS >> count;
//    for(quint16 i = 0; i < count; i++)
//    {
//        CMDPowerGroup* configProperty = new CMDPowerGroup;
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

bool CMDPowerGroupConfigerImp::saveToFile()
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
    //    QMap<QString, CMDPowerGroup*> ::const_iterator i;
    //    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    //    {
    //        CMDPowerGroup* configProperty = i.value();
    //        configProperty->serialize(writeDS);
    //    }
}

QString CMDPowerGroupConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDPowerGroupConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("PowerGroup"));

    QJsonArray ContentJson;
    foreach (CMDPowerGroup* configProperty, m_configedMap)
    {
        QJsonObject ConditionJson;
        configProperty->serialize(ConditionJson);
        ContentJson.append(ConditionJson);
    }

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDPowerGroupConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonArray ContentJson = json.value("Content").toArray();
    foreach (const QJsonValue& ConditionJson, ContentJson)
    {
        CMDPowerGroup* configProperty = new CMDPowerGroup;
        if(configProperty)
        {
            configProperty->deserialize(ConditionJson.toObject());

            addToList(configProperty);
        }
    }
}

/*!
\section 管理权限组
*/
bool CMDPowerGroupConfigerImp::addToList(CMDPowerGroup* configProperty)
{
    QMap<QString, CMDPowerGroup*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
    {
        //CMDPowerGroup* independentAction = iPosition.value();
        delete configProperty;
        return false;
    }
    m_configedMap.insert(configProperty->name().toUpper(), configProperty);
    return true;
}

void CMDPowerGroupConfigerImp::configedList(QList<CMDPowerGroup*>& accountList)
{
    accountList.clear();

    QMap<QString, CMDPowerGroup*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        accountList.append(i.value());
}

bool CMDPowerGroupConfigerImp::add(const CMDPowerGroup* configProperty)
{
    QMap<QString, CMDPowerGroup*>::const_iterator iPosition = m_configedMap.find(configProperty->name().toUpper());
    if(iPosition != m_configedMap.end())
        return false;

    CMDPowerGroup* newProperty = new CMDPowerGroup(*configProperty);
    if(newProperty)
        m_configedMap.insert(configProperty->name().toUpper(), newProperty);
    return true;
}

bool CMDPowerGroupConfigerImp::delete1(const QString& name)
{
    QMap<QString, CMDPowerGroup*>::iterator iPosition = m_configedMap.find(name.toUpper());
    if(iPosition == m_configedMap.end())
        return false;

    delete iPosition.value();
    m_configedMap.erase(iPosition);
    return true;
}

CMDPowerGroup* CMDPowerGroupConfigerImp::find(const QString& name)
{
    QMap<QString, CMDPowerGroup*>::const_iterator iPosition = m_configedMap.find(name.toUpper());
    return (iPosition != m_configedMap.end() ? iPosition.value() : NULL);
}

CMDPowerGroup* CMDPowerGroupConfigerImp::find(int index)
{
    int loopIndex = 0;
    QMap<QString, CMDPowerGroup*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    {
        if(loopIndex == index)
            return i.value();

        loopIndex ++;
    }
    return NULL;
}

bool CMDPowerGroupConfigerImp::modify(const CMDPowerGroup* sourceConfig,
                                const CMDPowerGroup* destConfig)
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
        QMap<QString, CMDPowerGroup*>::const_iterator iSource = m_configedMap.find(sourceConfig->name().toUpper());
        if(iSource != m_configedMap.end())
        {
            iSource.value()->copy(destConfig);
            result = true;
        }
    }

    return result;
}

int CMDPowerGroupConfigerImp::count()
{
    return m_configedMap.size();
}


QStringList CMDPowerGroupConfigerImp::getGroups()
{
    //return m_configedMap.keys();
    QStringList retList;
    QMap<QString, CMDPowerGroup*> ::const_iterator itor;
    for (itor = m_configedMap.constBegin(); itor != m_configedMap.constEnd(); ++itor)
    {
        CMDPowerGroup* c = itor.value();
        retList.append(c->name());
    }
    return retList;
}
QStringList CMDPowerGroupConfigerImp::getAccountsInGroup(QString group)
{
    CMDPowerGroup* c = find(group);
    if(c)
    {
        return c->accountList();
    }
    return QStringList();
}
QStringList CMDPowerGroupConfigerImp::getDisabledGroups()
{
    QStringList retList;
    QMap<QString, CMDPowerGroup*> ::const_iterator itor;
    for (itor = m_configedMap.constBegin(); itor != m_configedMap.constEnd(); ++itor)
    {
        CMDPowerGroup* c = itor.value();
        if(!c->isEnable())
            retList.append(c->name());
    }
    return retList;
}
bool CMDPowerGroupConfigerImp::enableGroup(QString name)
{
    CMDPowerGroup* c = find(name);
    if(c)
        c->setEnable(true);
    return true;
}
bool CMDPowerGroupConfigerImp::disableGroup(QString name)
{
    CMDPowerGroup* c = find(name);
    if(c)
        c->setEnable(false);
    return true;
}
bool CMDPowerGroupConfigerImp::deleteGroup(QString name)
{
    return delete1(name);
}
// modifyGroupByUI()
bool CMDPowerGroupConfigerImp::addAccountToGroup(QString account,QString group)
{
    CMDPowerGroup* pg = find(group);
    if(pg)
    {
        QStringList l = pg->accountList();
        if(!l.contains(account, Qt::CaseSensitivity::CaseInsensitive))
        {
            l.append(account);
            pg->setAccountList(l);
        }
        return true;
    }
    return false;
}
bool CMDPowerGroupConfigerImp::removeAccountFromGroup(QString account,QString group)
{
    CMDPowerGroup* pg = find(group);
    if(pg)
    {
        QStringList l = pg->accountList();
        if(l.contains(account, Qt::CaseSensitivity::CaseInsensitive))
        {
            l.removeAll(account);
            pg->setAccountList(l);
        }
        return true;
    }
    return false;
}
bool CMDPowerGroupConfigerImp::isAccountInGroup(QString account,QString group)
{
    CMDPowerGroup* pg = find(group);
    if(pg)
    {
        QStringList l = pg->accountList();
        if(l.contains(account, Qt::CaseSensitivity::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

bool CMDPowerGroupConfigerImp::upsert(QString name, QString comment, bool enable)
{
    CMDPowerGroup* pg = find(name);
    if(pg)
    {
        // modify
        pg->setComment(comment);
        pg->setEnable(enable);
    }
    else
    {
        // add
        CMDPowerGroup* newProperty = new CMDPowerGroup();
        if(newProperty)
        {
            newProperty->setName(name);
            newProperty->setComment(comment);
            newProperty->setEnable(enable);
            m_configedMap.insert(newProperty->name().toUpper(), newProperty);
        }
    }
    return true;
}

QString CMDPowerGroupConfigerImp::getGroup(QString name)
{
    CMDPowerGroup* c = find(name);
    if(c){
        QJsonObject json;
        c->serialize(json);
        QJsonDocument doc(json);
        return QString(doc.toJson().data());
    }
    return "";
}

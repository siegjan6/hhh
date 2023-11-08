#include "MDAccountConfigerImp.h"
#include "../../include/MDSecurityPolicyHelper/MDAccount.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDAccountConfigerImp::CMDAccountConfigerImp()
{
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAccountConfigerImp::loadConfig(const QString& projectPath)
{
    m_ConfigFileName = makePath(projectPath) + "Account.dat";
    return loadFromFile();
}

void CMDAccountConfigerImp::unloadConfig()
{
    QMap<QString, CMDAccountProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        delete i.value();
    m_configedMap.clear();
}

QString CMDAccountConfigerImp::makePath(const QString& projectPath)
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

bool CMDAccountConfigerImp::loadFromFile()
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
//        CMDAccountProperty_Config* configProperty = new CMDAccountProperty_Config;
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

bool CMDAccountConfigerImp::saveToFile()
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
//    QMap<QString, CMDAccountProperty_Config*> ::const_iterator i;
//    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
//    {
//        CMDAccountProperty_Config* configProperty = i.value();
//        configProperty->serialize(writeDS);
//    }
//    return true;
}

QString CMDAccountConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDAccountConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("Account"));

    QJsonArray ContentJson;
    foreach (CMDAccountProperty_Config* configProperty, m_configedMap)
    {
        QJsonObject ConditionJson;
        configProperty->serialize(ConditionJson);
        ContentJson.append(ConditionJson);
    }

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDAccountConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonArray ContentJson = json.value("Content").toArray();
    foreach (const QJsonValue& ConditionJson, ContentJson)
    {
        CMDAccountProperty_Config* configProperty = new CMDAccountProperty_Config;
        if(configProperty)
        {
            configProperty->deserialize(ConditionJson.toObject());

            addToList(configProperty);
        }
    }
}

/*!
\section 管理帐户
*/
bool CMDAccountConfigerImp::addToList(CMDAccountProperty_Config* configProperty)
{
    QMap<QString, CMDAccountProperty_Config*>::const_iterator iPosition = m_configedMap.find(configProperty->name());
    if(iPosition != m_configedMap.end())
    {
        //CMDAccountProperty_Config* independentAction = iPosition.value();
        delete configProperty;
        return false;
    }
    m_configedMap.insert(configProperty->name(), configProperty);
    return true;
}

void CMDAccountConfigerImp::configedList(QList<CMDAccountProperty_Config*>& accountList)
{
    accountList.clear();

    QMap<QString, CMDAccountProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
        accountList.append(i.value());
}

bool CMDAccountConfigerImp::add(const CMDAccountProperty_Config* configProperty)
{
    QMap<QString, CMDAccountProperty_Config*>::const_iterator iPosition = m_configedMap.find(configProperty->name());
    if(iPosition != m_configedMap.end())
        return false;

    CMDAccountProperty_Config* newProperty = new CMDAccountProperty_Config(*configProperty);
    if(newProperty)
        m_configedMap.insert(configProperty->name(), newProperty);
    return true;
}

bool CMDAccountConfigerImp::delete1(const QString& name)
{
    QMap<QString, CMDAccountProperty_Config*>::iterator iPosition = m_configedMap.find(name);
    if(iPosition == m_configedMap.end())
        return false;

    delete iPosition.value();
    m_configedMap.erase(iPosition);
    return true;
}

CMDAccountProperty_Config* CMDAccountConfigerImp::find(const QString& name)
{
    QMap<QString, CMDAccountProperty_Config*>::const_iterator iPosition = m_configedMap.find(name);
    return (iPosition != m_configedMap.end() ? iPosition.value() : NULL);
}

CMDAccountProperty_Config* CMDAccountConfigerImp::find(int index)
{
    int loopIndex = 0;
    QMap<QString, CMDAccountProperty_Config*> ::const_iterator i;
    for (i = m_configedMap.constBegin(); i != m_configedMap.constEnd(); ++i)
    {
        if(loopIndex == index)
            return i.value();

        loopIndex ++;
    }
    return NULL;
}

bool CMDAccountConfigerImp::modify(const CMDAccountProperty_Config* sourceConfig,
                                const CMDAccountProperty_Config* destConfig)
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
        QMap<QString, CMDAccountProperty_Config*>::const_iterator iSource = m_configedMap.find(sourceConfig->name());
        if(iSource != m_configedMap.end())
        {
            iSource.value()->copy(destConfig);
            result = true;
        }
    }

    return result;
}

int CMDAccountConfigerImp::count()
{
    return m_configedMap.size();
}

QStringList CMDAccountConfigerImp::getAccounts()
{
    //return m_configedMap.keys();
    QStringList retList;
    QMap<QString, CMDAccountProperty_Config*> ::const_iterator itor;
    for (itor = m_configedMap.constBegin(); itor != m_configedMap.constEnd(); ++itor)
    {
        CMDAccountProperty_Config* c = itor.value();
        retList.append(c->name());
    }
    return retList;
}

QString CMDAccountConfigerImp::getAccount(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c){
        QJsonObject json;
        c->serialize(json);
        QJsonDocument doc(json);
        QString s(doc.toJson().data());
        return s;
    }
    return "";
}

int CMDAccountConfigerImp::modifyPassword(QString name, QString oldPassword, QString newPassword)
{

//    enum class MDSecurityResultCode
//    {
//        no_result                      = -1,
//        success                        = 0, //! 成功
//        account_not_exist      = 1, //! 帐户不存在
//        invalid_account          = 2, //! 无效的账户
//        invalid_password       = 3, //! 错误的密码
//        account_has_no_power = 4, //! 帐户不具有权限
//        account_disabled            = 5 //! 帐户被禁用
//    };

    CMDAccountProperty_Config* c = find(name);
    if(c){
        // 强制修改密码（此时忽略旧密码验证） 秘钥为 *MD1588501#
        if(c->password() == oldPassword || "*MD1588501#" == oldPassword) {
            c->setPassword(newPassword);
            return 0;
        }
        else
        {
            return 3;
        }
    }
    else
        return 1;
    return -1;
}

QStringList CMDAccountConfigerImp::getDisabledAccounts()
{
    QStringList retList;
    QMap<QString, CMDAccountProperty_Config*> ::const_iterator itor;
    for (itor = m_configedMap.constBegin(); itor != m_configedMap.constEnd(); ++itor)
    {
        CMDAccountProperty_Config* c = itor.value();
        if(!c->isEnable())
            retList.append(c->name());
    }
    return retList;
}

QStringList CMDAccountConfigerImp::getLockedAccounts()
{
    QStringList retList;
    QMap<QString, CMDAccountProperty_Config*> ::const_iterator itor;
    for (itor = m_configedMap.constBegin(); itor != m_configedMap.constEnd(); ++itor)
    {
        CMDAccountProperty_Config* c = itor.value();
        if(!c->isLocked())
            retList.append(c->name());
    }
    return retList;
}

bool CMDAccountConfigerImp::lockAccount(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        c->setLocked(true);
    }
    return true;
}
bool CMDAccountConfigerImp::unlockAccount(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        c->setLocked(false);
        return true;
    }
    return false;
}
bool CMDAccountConfigerImp::enableAccount(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        c->setEnable(true);
        return true;
    }
    return false;
}
bool CMDAccountConfigerImp::disableAccount(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        c->setEnable(false);
        return true;
    }
    return false;
}
bool CMDAccountConfigerImp::isAccountEnabled(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        return c->isEnable();
    }
    return false;
}
bool CMDAccountConfigerImp::isAccountLocked(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        return c->isLocked();
    }
    return false;
}
bool CMDAccountConfigerImp::isAccountLogin(QString name)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        return c->isLogin();
    }
    return false;
}
void CMDAccountConfigerImp::setAccountLogin(QString name, bool logined)
{
    CMDAccountProperty_Config* c = find(name);
    if(c)
    {
        c->setLogined(logined);
    }
}
bool CMDAccountConfigerImp::upsert(QString name, QString comment, QString password, bool enable, bool lock)
{
    CMDAccountProperty_Config* ag = find(name);
    if(ag)
    {
        // modify
        ag->setComment(comment);
        ag->setPassword(password);
        ag->setEnable(enable);
        ag->setLocked(lock);
    }
    else
    {
        // add
        CMDAccountProperty_Config* newProperty = new CMDAccountProperty_Config();
        if(newProperty)
        {
            newProperty->setName(name);
            newProperty->setComment(comment);
            newProperty->setPassword(password);
            newProperty->setEnable(enable);
            newProperty->setLocked(lock);
            m_configedMap.insert(newProperty->name(), newProperty);
        }
    }
    return true;
}

#include "MDSecurityPolicyConfigerImp.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDSecurityPolicyConfigerImp::CMDSecurityPolicyConfigerImp()
{
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDSecurityPolicyConfigerImp::loadConfig(const QString& projectPath)
{
    m_accountConfiger.loadConfig(projectPath);
    m_powerGroupConfiger.loadConfig(projectPath);
    m_accountStrategyConfiger.loadConfig(projectPath);

    m_ConfigFileName = makePath(projectPath) + "SecurityPolicyPara.dat";
    return loadFromFile();
}

void CMDSecurityPolicyConfigerImp::unloadConfig()
{
    m_accountConfiger.unloadConfig();
    m_powerGroupConfiger.unloadConfig();
    m_accountStrategyConfiger.unloadConfig();

    m_para = CMDSecurityPolicyPara();
}

QString CMDSecurityPolicyConfigerImp::makePath(const QString& projectPath)
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

bool CMDSecurityPolicyConfigerImp::loadFromFile()
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
    //    QDataStream readDS(&configFile);
    //    m_para.deserialize(readDS);
}

bool CMDSecurityPolicyConfigerImp::saveToFile()
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
    //    QDataStream writeDS(&configFile);
    //    m_para.serialize(writeDS);
}

QString CMDSecurityPolicyConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDSecurityPolicyConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("SecurityPolicy"));

    QJsonObject ContentJson;
    m_para.serialize(ContentJson);

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDSecurityPolicyConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonObject ContentJson = json.value("Content").toObject();
    m_para.deserialize(ContentJson);
}

/*!
\section 管理安全策略参数
*/
CMDSecurityPolicyPara* CMDSecurityPolicyConfigerImp::para()
{
    return &m_para;
}

void CMDSecurityPolicyConfigerImp::setPara(const CMDSecurityPolicyPara* destConfig)
{
    m_para = *destConfig;
}

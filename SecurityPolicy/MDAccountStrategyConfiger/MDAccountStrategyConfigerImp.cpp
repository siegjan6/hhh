#include "MDAccountStrategyConfigerImp.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDAccountStrategyConfigerImp::CMDAccountStrategyConfigerImp()
{
}

// 配置器
bool CMDAccountStrategyConfigerImp::loadConfig(const QString& projectPath)
{
    m_ConfigFileName = makePath(projectPath) + "AccountStrategy.dat";
    return loadFromFile();
}

void CMDAccountStrategyConfigerImp::unloadConfig()
{
}

QString CMDAccountStrategyConfigerImp::makePath(const QString& projectPath)
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

bool CMDAccountStrategyConfigerImp::loadFromFile()
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
    //    m_accountStrategy.deserialize(readDS);
    //    return true;
}

bool CMDAccountStrategyConfigerImp::saveToFile()
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
    //    m_accountStrategy.serialize(writeDS);
    //    configFile.close();
}

QString CMDAccountStrategyConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDAccountStrategyConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("AccountStrategy"));

    QJsonObject ContentJson;
    m_accountStrategy.serialize(ContentJson);

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDAccountStrategyConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonObject ContentJson = json.value("Content").toObject();
    m_accountStrategy.deserialize(ContentJson);
}

﻿#include "MDAlarmConfigerImp.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>

CMDAlarmConfigerImp::CMDAlarmConfigerImp()
{
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAlarmConfigerImp::loadConfig(const QString& projectPath)
{
    m_levelConfiger.loadConfig(projectPath);
    m_conditionConfiger.loadConfig(projectPath);

    m_ConfigFileName = makePath(projectPath) + "AlarmPara.dat";
    return loadFromFile();
}

void CMDAlarmConfigerImp::unloadConfig()
{
    m_levelConfiger.unloadConfig();
    m_conditionConfiger.unloadConfig();

    m_para = CMDAlarmPara();
}

QString CMDAlarmConfigerImp::makePath(const QString& projectPath)
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

bool CMDAlarmConfigerImp::loadFromFile()
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
//    QDataStream readDS(&configFile);
//    m_para.deserialize(readDS);
//    return true;
}

bool CMDAlarmConfigerImp::saveToFile()
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
//    QDataStream writeDS(&configFile);
//    m_para.serialize(writeDS);
}

QString CMDAlarmConfigerImp::configFileName()
{
    return m_ConfigFileName;
}

void CMDAlarmConfigerImp::serialize(QJsonObject& json)
{
    QJsonObject HeadJson;
    HeadJson.insert("Version", 1);
    HeadJson.insert("Type", QString("AlarmPara"));

    QJsonObject ContentJson;
    m_para.serialize(ContentJson);

    json.insert("Head", HeadJson);
    json.insert("Content", ContentJson);
}

void CMDAlarmConfigerImp::deserialize(const QJsonObject& json)
{
    QJsonObject HeadJson = json.value("Head").toObject();
    int version = HeadJson.value("Version").toInt();
    QString type = HeadJson.value("Type").toString();

    QJsonObject ContentJson = json.value("Content").toObject();
    m_para.deserialize(ContentJson);
}

/*!
\section 管理报警参数
*/
CMDAlarmPara* CMDAlarmConfigerImp::alarmPara()
{
    return &m_para;
}

void CMDAlarmConfigerImp::setAlarmPara(const CMDAlarmPara* destConfig)
{
    m_para = *destConfig;
}

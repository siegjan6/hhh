/*!

\author dGFuZ3p3

\section 功能

报警等级配置类，负责存储到文件及从文件加载。

*/

#ifndef MDALARMLEVELCONFIGERIMP_H
#define MDALARMLEVELCONFIGERIMP_H

#include <QString>
#include <QMap>
#include <QJsonObject>

class CMDAlarmLevel;
class CMDAlarmLevelConfigerImp
{
    friend class CMDAlarmLevelConfiger;
public:
    CMDAlarmLevelConfigerImp();
/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString makePath(const QString& projectPath);
public:
    bool loadFromFile();
    bool saveToFile();
    QString configFileName();
    QString m_ConfigFileName;
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

/*!
\section 管理报警等级
*/
public:
    bool addToList(CMDAlarmLevel* configProperty);
    void configedList(QList<CMDAlarmLevel*>& accountList);
    int count();
public:
    bool add(const CMDAlarmLevel* configProperty);
    bool delete1(const QString& name);
    CMDAlarmLevel* find(const QString& name);
    CMDAlarmLevel* find(int index);
    bool modify(const CMDAlarmLevel* sourceConfig,
                                    const CMDAlarmLevel* destConfig);
protected:
    QMap<QString, CMDAlarmLevel*> m_configedMap;
};

#endif // MDALARMLEVELCONFIGERIMP_H

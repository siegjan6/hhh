/*!

\author dGFuZ3p3

\section 功能

报警条件配置类，负责存储到文件及从文件加载。

*/

#ifndef MDALARMCONDITIONCONFIGERIMP_H
#define MDALARMCONDITIONCONFIGERIMP_H

#include <QString>
#include <QMap>
#include <QJsonObject>

class CMDAlarmCondition;
class CMDAlarmConditionConfigerImp
{
    friend class CMDAlarmConditionConfiger;
public:
    CMDAlarmConditionConfigerImp();
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
\section 管理报警条件
*/
public:
    bool addToList(CMDAlarmCondition* configProperty);
    void configedList(QList<CMDAlarmCondition*>& acList);
    int count();
public:
    bool add(const CMDAlarmCondition* configProperty);
    bool addNew(CMDAlarmCondition* new_configProperty);
    bool delete1(const QString& name);
    void deleteAll();
    CMDAlarmCondition* find(const QString& name);
    CMDAlarmCondition* find(int index);
    bool modify(const CMDAlarmCondition* sourceConfig,
                                    const CMDAlarmCondition* destConfig);
public:
    void deletePrefixMatched(const QString& prefix);

protected:
    QMap<QString, CMDAlarmCondition*> m_configedMap;
};

#endif // MDALARMCONDITIONCONFIGERIMP_H

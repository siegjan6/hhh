/*!

\author dGFuZ3p3

\section 功能

报警参数配置类，负责存储到文件及从文件加载，
可获取【报警条件配置类】和【报警等级配置类】的对象实例。

*/

#ifndef MDALARMCONFIGERIMP_H
#define MDALARMCONFIGERIMP_H

#include "MDAlarmLevelConfiger.h"
#include "MDAlarmConditionConfiger.h"
#include "MDAlarmPara.h"
#include <QJsonObject>

class CMDAlarmConfigerImp
{
    friend class CMDAlarmConfiger;
public:
    CMDAlarmConfigerImp();
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
\section 管理报警参数
*/
public:
    CMDAlarmPara* alarmPara();
    void setAlarmPara(const CMDAlarmPara* destConfig);
protected:
    CMDAlarmPara m_para;

/*!
\section 管理【报警条件配置类】和【报警等级配置类】的对象实例
*/
public:
    CMDAlarmLevelConfiger m_levelConfiger;
    CMDAlarmConditionConfiger m_conditionConfiger;
};

#endif // MDALARMCONFIGERIMP_H

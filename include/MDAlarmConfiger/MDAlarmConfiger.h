/*!

\author dGFuZ3p3

\section 功能

报警参数配置类，负责存储到文件及从文件加载，
可获取【报警条件配置类】和【报警等级配置类】的对象实例。

*/

#ifndef MDALARMCONFIGER_H
#define MDALARMCONFIGER_H

#include "MDAlarmLevelConfiger.h"
#include "MDAlarmConditionConfiger.h"
#include "MDAlarmConfiger_Global.h"
#include "../../include/MDAlarmHelper/MDAlarmPara.h"

class CMDAlarmPara;
class CMDAlarmConfigerImp;
class MDALARMCONFIGERSHARED_EXPORT CMDAlarmConfiger
{
public:
    CMDAlarmConfiger();
    ~CMDAlarmConfiger();
/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString configFileName();
    bool saveToFile();

/*!
\section 管理报警参数
*/
public:
    CMDAlarmPara* alarmPara();
    void setAlarmPara(const CMDAlarmPara* destConfig); //! 设置参数，但不保存到文件

/*!
\section 获取【报警条件配置类】和【报警等级配置类】的对象实例
*/
public:
    CMDAlarmLevelConfiger* levelConfiger();
    CMDAlarmConditionConfiger* conditionConfiger();
protected:
    CMDAlarmConfigerImp* m_imp;
};

#endif // MDALARMCONFIGER_H

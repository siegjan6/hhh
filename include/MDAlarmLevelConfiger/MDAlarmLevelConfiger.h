/*!

\author dGFuZ3p3

\section 功能

报警等级配置类，负责存储到文件及从文件加载。

*/

#ifndef MDALARMLEVELCONFIGER_H
#define MDALARMLEVELCONFIGER_H

#include <QString>
#include "MDAlarmLevelConfiger_Global.h"

class CMDAlarmLevel;
class CMDAlarmLevelConfigerImp;
class MDALARMLEVELCONFIGERSHARED_EXPORT CMDAlarmLevelConfiger
{
public:
    CMDAlarmLevelConfiger();
    ~CMDAlarmLevelConfiger();
/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString configFileName();
    bool saveToFile();

/*!
\section 管理报警等级
*/
public:
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
    CMDAlarmLevelConfigerImp* m_imp;
};

#endif // MDALARMLEVELCONFIGER_H

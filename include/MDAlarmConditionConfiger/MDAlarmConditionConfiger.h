/*!

\author dGFuZ3p3

\section 功能

报警条件配置类，负责存储到文件及从文件加载。

*/

#ifndef MDALARMCONDITIONCONFIGER_H
#define MDALARMCONDITIONCONFIGER_H

#include <QString>
#include "MDAlarmConditionConfiger_Global.h"

class CMDAlarmCondition;
class CMDAlarmConditionConfigerImp;
class MDALARMCONDITIONCONFIGERSHARED_EXPORT CMDAlarmConditionConfiger
{
public:
    CMDAlarmConditionConfiger();
    ~CMDAlarmConditionConfiger();
/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString configFileName();
    bool saveToFile();

/*!
\section 管理报警条件
*/
public:
    void configedList(QList<CMDAlarmCondition*>& acList);
    int count();
public:
    /*!
     添加一个报警条件对象（对象信息从 configProperty 复制）。

     \param configProperty 源对象。
     \returns true - 成功添加；
                   false - 同名对象已经存在，不能添加。
     */
    bool add(const CMDAlarmCondition* configProperty);
    /*!
     添加报警条件对象 new_configProperty 到此配置器（new_configProperty 必须是通过 new 创建的）。

     \param new_configProperty 源对象，必须是通过 new 创建的。
     \returns true - 成功添加；
                   false - 同名对象已经存在，不能添加，new_configProperty 已经被 delete。
     */
    bool addNew(CMDAlarmCondition* new_configProperty);
public:
    bool delete1(const QString& name);
    void deleteAll();
    CMDAlarmCondition* find(const QString& name);
    CMDAlarmCondition* find(int index);
    bool modify(const CMDAlarmCondition* sourceConfig,
                                    const CMDAlarmCondition* destConfig);
public:
    void deletePrefixMatched(const QString& prefix);

protected:
    CMDAlarmConditionConfigerImp* m_imp;
};

#endif // MDALARMCONDITIONCONFIGER_H

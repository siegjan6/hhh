/*!

\author dGFuZ3p3

\section 功能 - 项目配置类：

管理项目总体参数；
管理项目子模块的配置器。

*/

#ifndef MDPROJECTCONFIGER_H
#define MDPROJECTCONFIGER_H

#include "MDProjectConfiger_Global.h"
#include "../../include/MDProjectHelper/MDProjectPara.h"

class CMDEventConfiger;
class CMDActionConfiger;
class CMDProjectConfigerImp;
class IMDDataArchives;
class CMDDataArchivesEngine;
class CMDSecurityPolicyConfiger;
class CMDAccountConfiger;
class CMDPowerGroupConfiger;
class IMDRTDBManager;
class CMDAlarmConfiger;
class CMDAlarmLevelConfiger;
class CMDAlarmConditionConfiger;
class CMDRecipeConfiger;
class CMDProjectPara;
class MDPROJECTCONFIGERSHARED_EXPORT CMDProjectConfiger
{
public:
    CMDProjectConfiger();
    ~CMDProjectConfiger();

    static bool parseProjectName(const QString& projectName, QString& path, QString& nameOnly);

/*!
\section 管理项目总体参数
*/
public:
    bool load(const QString & strProjectName) ;
    bool save(const QString & strProjectName) ;
    bool save();
    bool unLoad();
public:
    //! 完整的项目文件名称
    QString fullFileName() ;
    //! 名称
    QString name() ;
    void setName(const QString &name) ;
    //! 目录
     QString path() ;
     void setPath(const QString &projectPath) ;
    //! 描述
    QString description() ;
    void setDescription(const QString &description);

    static QString projectFileExt();    //! 项目文件扩展名称，形如 .devx
public:
    //! 存储项目参数的文件名称
    QString paraFileName();
    bool saveToParaFile();
public:
    //! 项目参数
    CMDProjectPara* projectPara();
    void setProjectPara(const CMDProjectPara* destConfig);

/*!
\section 管理项目子模块的配置器
*/
public:
    CMDSecurityPolicyConfiger* securityConfiger(); //! 安全策略
    CMDAccountConfiger* accountConfiger(); //! 帐户管理
    CMDPowerGroupConfiger* powerGroupConfiger(); //! 权限组管理
    CMDEventConfiger *eventConfiger(); //! 事件
    CMDActionConfiger *actionConfiger(); //! 动作
    CMDAlarmConfiger* alarmConfiger(); //! 报警
    CMDAlarmLevelConfiger* alarmLevelConfiger(); //! 报警等级
    CMDAlarmConditionConfiger* conditionConfiger(); //! 报警条件
    CMDRecipeConfiger* recipeConfiger(); //! 配方

/*!
\section 管理实时数据库模块
*/
public:
    void loadRTDB();
    void unloadRTDB();
    IMDRTDBManager *nsRTDBManager(); //!实时数据库
    IMDDataArchives *dataArchives(); //! 数据归档
protected:
    CMDProjectConfigerImp* m_imp;
};

#endif // MDPROJECTCONFIGER_H

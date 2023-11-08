/*!

\author dGFuZ3p3

\section 功能 - 项目配置类的实现类：

管理项目总体参数；
管理项目子模块的配置器。

*/

#ifndef MDPROJECTCONFIGERIMP_H
#define MDPROJECTCONFIGERIMP_H

#include "MDEventConfiger.h"
#include "MDActionConfiger.h"
#include "MDRTDBManagerFactory.h"
#include "MDDataArchivesFactory.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDAlarmConfiger.h"
#include "MDRecipeConfiger.h"
#include "MDProjectPara.h"

class CMDProjectPara;
class CMDProjectConfigerImp
{
public:
    CMDProjectConfigerImp();
    ~CMDProjectConfigerImp();

    static bool parseProjectName(const QString& projectName, QString& path, QString& nameOnly);

/*!
\section 管理项目总体参数
*/
public:
    bool load(const QString& projectName);
    bool unLoad();
    bool save(const QString& projectName);
    bool save();
public:
    //! 完整的项目文件名称
    QString fullFileName();
    // 名称
    QString name() ;
    void setName(const QString &name) ;
    // 目录
     QString path() ;
     void setPath(const QString &projectPath) ;
    // 描述
    QString description() ;
    void setDescription(const QString &description) ;
public:
    QString paraFileName();
    bool loadFromParaFile();
    bool saveToParaFile();
    CMDProjectPara* projectPara();
    void setProjectPara(const CMDProjectPara* destConfig);
protected:
    CMDProjectPara m_para;
    QString m_paraFileName;
private:
    QString m_name;
    QString m_projectPath;
    QString m_description;
    QString m_projectFullName;

/*!
\section 管理项目子模块的配置器
*/
public:
    CMDEventConfiger m_eventConfiger;
    CMDActionConfiger m_actionConfiger;
    CMDSecurityPolicyConfiger m_securityConfiger;
    CMDAlarmConfiger m_alarmConfiger;
    CMDRecipeConfiger m_recipeConfiger;

/*!
\section 实时数据库模块的初始化与释放
*/
public:
    void loadRTDB();
    void unloadRTDB();
public:
    IMDRTDBManager *m_mdRTDBManager ;// NULL;
    IMDDataArchives *m_mdDataArchives ;// NULL;
};

#endif // MDPROJECTCONFIGERIMP_H

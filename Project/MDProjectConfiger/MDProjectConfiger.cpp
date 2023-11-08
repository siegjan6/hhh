#include "MDProjectConfiger.h"
#include "MDProjectConfigerImp.h"

CMDProjectConfiger::CMDProjectConfiger()
{
    m_imp = new CMDProjectConfigerImp;
}

CMDProjectConfiger::~CMDProjectConfiger()
{
    if(m_imp)
        delete m_imp;
}

bool CMDProjectConfiger::parseProjectName(const QString& projectName, QString& path, QString& nameOnly)
{
    return CMDProjectConfigerImp::parseProjectName(projectName, path, nameOnly);
}

/*!
\section 管理项目总体参数
*/
bool CMDProjectConfiger::load(const QString& strProjectName)
{
    return (m_imp ? m_imp->load(strProjectName) : false);
}
bool CMDProjectConfiger::unLoad()
{
    return (m_imp ? m_imp->unLoad() : false);
}
bool CMDProjectConfiger::save(const QString& strProjectName)
{
    return (m_imp ? m_imp->save(strProjectName) : false);
}
bool CMDProjectConfiger::save()
{
    return (m_imp ? m_imp->save() : false);
}

QString CMDProjectConfiger::fullFileName()
{
    return (m_imp ? m_imp->fullFileName() : "");
}

// 名称
QString CMDProjectConfiger::name()
{
     return (m_imp ? m_imp->name() : "");
}
void CMDProjectConfiger::setName(const QString &name)
{
     if(m_imp)
         m_imp->setName(name) ;
}
// 目录
 QString CMDProjectConfiger::path()
 {
     return (m_imp ? m_imp->path() : "");
 }
 void CMDProjectConfiger::setPath(const QString &projectPath)
 {
      if(m_imp)
          m_imp->setPath(projectPath) ;
 }
// 描述
QString  CMDProjectConfiger::description()
{
    return (m_imp ? m_imp->description() : "");
}
void CMDProjectConfiger:: setDescription(const QString &description)
{
     if(m_imp)
         m_imp->setDescription(description) ;
}

QString  CMDProjectConfiger::projectFileExt()
{
    return ".devx";
}

// 项目参数,例如,加载/卸载的安全策略等
QString CMDProjectConfiger::paraFileName()
{
    return (m_imp ? m_imp->paraFileName() : "");
}

bool CMDProjectConfiger::saveToParaFile()
{
    return (m_imp ? m_imp->saveToParaFile() : false);
}

void CMDProjectConfiger::setProjectPara(const CMDProjectPara* destConfig)
{
    if(m_imp)
        m_imp->setProjectPara(destConfig);
}

CMDProjectPara* CMDProjectConfiger::projectPara()
{
    return (m_imp ? m_imp->projectPara() : NULL);
}

/*!
\section 管理项目子模块的配置器
*/
CMDEventConfiger* CMDProjectConfiger::eventConfiger()
{
    return (m_imp ? &m_imp->m_eventConfiger : NULL);
}

CMDActionConfiger* CMDProjectConfiger::actionConfiger()
{
    return (m_imp ? &m_imp->m_actionConfiger : NULL);
}

CMDSecurityPolicyConfiger* CMDProjectConfiger::securityConfiger()
{
    return (m_imp ? &m_imp->m_securityConfiger : NULL);
}

CMDAccountConfiger* CMDProjectConfiger::accountConfiger()
{
    return (m_imp ? m_imp->m_securityConfiger.accountConfiger() : NULL);
}

CMDPowerGroupConfiger* CMDProjectConfiger::powerGroupConfiger()
{
    return (m_imp ? m_imp->m_securityConfiger.powerGroupConfiger() : NULL);
}

CMDAlarmConfiger* CMDProjectConfiger::alarmConfiger()
{
    return (m_imp ? &m_imp->m_alarmConfiger : NULL);
}

CMDAlarmLevelConfiger* CMDProjectConfiger::alarmLevelConfiger()
{
    return (m_imp ? m_imp->m_alarmConfiger.levelConfiger() : NULL);
}

CMDAlarmConditionConfiger* CMDProjectConfiger::conditionConfiger()
{
    return (m_imp ? m_imp->m_alarmConfiger.conditionConfiger() : NULL);
}

CMDRecipeConfiger* CMDProjectConfiger::recipeConfiger()
{
    return &m_imp->m_recipeConfiger;
}

/*!
\section 管理实时数据库模块
*/
void CMDProjectConfiger::loadRTDB()
{
    m_imp->loadRTDB();
}

void CMDProjectConfiger::unloadRTDB()
{
    m_imp->unloadRTDB();
}

IMDRTDBManager* CMDProjectConfiger::nsRTDBManager()
{
    return (m_imp ? m_imp->m_mdRTDBManager : NULL);
}
IMDDataArchives* CMDProjectConfiger::dataArchives()
{
    return (m_imp ? m_imp->m_mdDataArchives : NULL);
}

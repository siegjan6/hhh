#include "MDProjectExpert.h"
#include "MDProjectExpertImp.h"

CMDProjectExpert::CMDProjectExpert()
{
    m_imp = new CMDProjectExpertImp;
}

CMDProjectExpert::~CMDProjectExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 加载/卸载
*/
bool CMDProjectExpert::load(const QString& projectName, CMDProjectContacter* projectContacter)
{
    return (m_imp ? m_imp->load(projectName, projectContacter) : false);
}

bool CMDProjectExpert::unload()
{
    return (m_imp ? m_imp->unload() : false);
}

MDProjectLoadStatus CMDProjectExpert::loadStatus()
{
    return (m_imp ? m_imp->loadStatus() : MDProjectLoadStatus::loaded);
}

bool CMDProjectExpert::resultOnLoaded()
{
    return (m_imp ? m_imp->resultOnLoaded() : true);
}

bool CMDProjectExpert::canLoad()
{
    return (m_imp ? m_imp->canLoad() : false);
}

bool CMDProjectExpert::canUnload()
{
    return (m_imp ? m_imp->canUnload() : false);
}

/*!
\section 项目接口类；项目配置器
*/
IMDProjectExpert* CMDProjectExpert::IProjectExpert()
{
    return (m_imp ? &m_imp->m_IProjectExpert : NULL);
}

CMDProjectConfiger* CMDProjectExpert::projectConfiger()
{
    return (m_imp ? &m_imp->m_projectConfiger : NULL);
}

/*!
\section 管理子模块专家
*/
CMDEventExpert* CMDProjectExpert::eventExpert()
{
    return (m_imp ? &m_imp->m_eventExpert : NULL);
}

CMDActionExpert* CMDProjectExpert::actionExpert()
{
    return (m_imp ? &m_imp->m_actionExpert : NULL);
}

CMDVariableExpert* CMDProjectExpert::variableExpert()
{
    return (m_imp ? &m_imp->m_variableExpert : NULL);
}

CMDSecurityPolicyExpert* CMDProjectExpert::securityExpert()
{
    return (m_imp ? &m_imp->m_securityExpert : NULL);
}

CMDRecipeExpert* CMDProjectExpert::recipeExpert()
{
    return &m_imp->m_recipeExpert;
}

/*!
\section 开关管理
*/
CMDSwitchFactory* CMDProjectExpert::funcSwitchFactory()
{
    return m_imp->funcSwitchFactory();
}

CMDSwitchFactory* CMDProjectExpert::logSwitchFactory()
{
    return m_imp->logSwitchFactory();
}

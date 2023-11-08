#include "MDSecurityPolicyConfiger.h"
#include "MDSecurityPolicyConfigerImp.h"

CMDSecurityPolicyConfiger::CMDSecurityPolicyConfiger()
{
    m_imp = new CMDSecurityPolicyConfigerImp;
}

CMDSecurityPolicyConfiger::~CMDSecurityPolicyConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDSecurityPolicyConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDSecurityPolicyConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

QString CMDSecurityPolicyConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDSecurityPolicyConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

void CMDSecurityPolicyConfiger::setPara(const CMDSecurityPolicyPara* destConfig)
{
    if(m_imp)
        m_imp->setPara(destConfig);
}

/*!
\section 管理安全策略参数
*/
CMDSecurityPolicyPara* CMDSecurityPolicyConfiger::para()
{
    return (m_imp ? m_imp->para() : NULL);
}

/*!
\section 获取【帐户配置类】、【帐户策略配置类】和 【权限组配置类】的对象实例
*/
CMDAccountConfiger* CMDSecurityPolicyConfiger::accountConfiger()
{
    return (m_imp ? &m_imp->m_accountConfiger : NULL);
}

CMDPowerGroupConfiger* CMDSecurityPolicyConfiger::powerGroupConfiger()
{
    return (m_imp ? &m_imp->m_powerGroupConfiger : NULL);
}

CMDAccountStrategyConfiger* CMDSecurityPolicyConfiger::accountStrategyConfiger()
{
    return (m_imp ? &m_imp->m_accountStrategyConfiger : NULL);
}

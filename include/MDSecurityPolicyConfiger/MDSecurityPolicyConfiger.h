/*!

\author dGFuZ3p3

\section 功能

安全策略参数配置类，负责存储到文件及从文件加载，
可获取【帐户配置类】、【帐户策略配置类】和 【权限组配置类】的对象实例。

*/

#ifndef MDSECURITYPOLICYCONFIGER_H
#define MDSECURITYPOLICYCONFIGER_H

#include "MDAccountConfiger.h"
#include "MDPowerGroupConfiger.h"
#include "MDAccountStrategyConfiger.h"
#include "MDSecurityPolicyPara.h"
#include "MDSecurityPolicyConfiger_Global.h"

class CMDSecurityPolicyPara;
class CMDSecurityPolicyConfigerImp;
class MDSECURITYPOLICYCONFIGERSHARED_EXPORT CMDSecurityPolicyConfiger
{
public:
    CMDSecurityPolicyConfiger();
    ~CMDSecurityPolicyConfiger();

/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString configFileName();
    bool saveToFile();

/*!
\section 管理安全策略参数
*/
public:
    CMDSecurityPolicyPara* para();
    void setPara(const CMDSecurityPolicyPara* destConfig);

/*!
\section 获取【帐户配置类】、【帐户策略配置类】和 【权限组配置类】的对象实例
*/
public:
    CMDAccountConfiger* accountConfiger();
    CMDPowerGroupConfiger* powerGroupConfiger();
    CMDAccountStrategyConfiger* accountStrategyConfiger();
protected:
    CMDSecurityPolicyConfigerImp* m_imp;
};

#endif // MDSECURITYPOLICYCONFIGER_H

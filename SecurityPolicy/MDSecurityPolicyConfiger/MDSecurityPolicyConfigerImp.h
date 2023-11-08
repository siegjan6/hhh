/*!

\author dGFuZ3p3

\section 功能

安全策略参数配置类的实现类，负责存储到文件及从文件加载，
可获取【帐户配置类】、【帐户策略配置类】和 【权限组配置类】的对象实例。

*/

#ifndef MDSECURITYPOLICYCONFIGERIMP_H
#define MDSECURITYPOLICYCONFIGERIMP_H

#include "MDAccountConfiger.h"
#include "MDPowerGroupConfiger.h"
#include "MDAccountStrategyConfiger.h"
#include "MDSecurityPolicyPara.h"
#include <QJsonObject>

class CMDSecurityPolicyPara;
class CMDSecurityPolicyConfigerImp
{
    friend class CMDSecurityPolicyConfiger;
public:
    CMDSecurityPolicyConfigerImp();

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
\section 管理安全策略参数
*/
public:
    CMDSecurityPolicyPara* para();
    void setPara(const CMDSecurityPolicyPara* destConfig);
protected:
    CMDSecurityPolicyPara m_para;

/*!
\section 管理【帐户配置类】、【帐户策略配置类】和 【权限组配置类】的对象实例
*/
public:
    CMDAccountConfiger m_accountConfiger;
    CMDPowerGroupConfiger m_powerGroupConfiger;
    CMDAccountStrategyConfiger m_accountStrategyConfiger;
};

#endif // MDSECURITYPOLICYCONFIGERIMP_H

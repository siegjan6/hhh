/*!

\author dGFuZ3p3

\section 功能

帐户策略配置类，负责存储到文件及从文件加载。

*/

#ifndef MDACCOUNTSTRATEGYCONFIGER_H
#define MDACCOUNTSTRATEGYCONFIGER_H

#include "MDAccountStrategyConfiger_Global.h"
#include "../../include/MDSecurityPolicyHelper/MDAccountStrategy.h"

class CMDAccountStrategy;
class CMDAccountStrategyConfigerImp;
class MDACCOUNTSTRATEGYCONFIGERSHARED_EXPORT CMDAccountStrategyConfiger
{
public:
    CMDAccountStrategyConfiger();
    ~CMDAccountStrategyConfiger();

public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString configFileName();
    bool saveToFile();

    QString getAccountOption();
    bool setAccountOption(QString option);

public:
    CMDAccountStrategy* accountStrategy();
protected:
    CMDAccountStrategyConfigerImp* m_imp;
};

#endif // MDACCOUNTSTRATEGYCONFIGER_H

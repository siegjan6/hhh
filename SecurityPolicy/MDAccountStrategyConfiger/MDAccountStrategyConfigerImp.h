/*!

\author dGFuZ3p3

\section 功能

帐户策略配置类的实现类，负责存储到文件及从文件加载。

*/

#ifndef MDACCOUNTSTRATEGYCONFIGERIMP_H
#define MDACCOUNTSTRATEGYCONFIGERIMP_H

#include <QString>
#include "../../include/MDSecurityPolicyHelper/MDAccountStrategy.h"
#include <QJsonObject>

class CMDAccountStrategyConfigerImp
{
    friend class CMDAccountStrategyConfiger;
public:
    CMDAccountStrategyConfigerImp();

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
protected:
    CMDAccountStrategy m_accountStrategy;
};

#endif // MDACCOUNTSTRATEGYCONFIGERIMP_H

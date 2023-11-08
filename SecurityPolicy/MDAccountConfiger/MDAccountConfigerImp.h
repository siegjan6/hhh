/*!

\author dGFuZ3p3

\section 功能

帐户配置类的实现类，负责存储到文件及从文件加载。

*/

#ifndef MDACCOUNTCONFIGERIMP_H
#define MDACCOUNTCONFIGERIMP_H

#include <QString>
#include <QMap>
#include <QJsonObject>

class CMDAccountProperty_Config;
class CMDAccountConfigerImp
{
    friend class CMDAccountConfiger;
public:
    CMDAccountConfigerImp();

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
\section 管理帐户
*/
public:
    bool addToList(CMDAccountProperty_Config* configProperty);
    void configedList(QList<CMDAccountProperty_Config*>& accountList);
public:
    bool add(const CMDAccountProperty_Config* configProperty);
    bool delete1(const QString& name);
    CMDAccountProperty_Config* find(const QString& name);
    CMDAccountProperty_Config* find(int index);
    bool modify(const CMDAccountProperty_Config* sourceConfig,
                                    const CMDAccountProperty_Config* destConfig);

/*!
\section interface for script and ua
*/
public:
    int count();
    QString getAccount(QString name);
    QStringList getAccounts();
    QStringList getDisabledAccounts();
    QStringList getLockedAccounts();
    int modifyPassword(QString name, QString oldPassword, QString newPassword);
    bool enableAccount(QString name);
    bool disableAccount(QString name);
    bool lockAccount(QString name);
    bool unlockAccount(QString name);
    bool isAccountEnabled(QString name);
    bool isAccountLocked(QString name);
    bool isAccountLogin(QString name);
    void setAccountLogin(QString name, bool logined);

    bool upsert(QString name, QString comment, QString password, bool enable, bool lock);


protected:
    QMap<QString, CMDAccountProperty_Config*> m_configedMap;
};

#endif // MDACCOUNTCONFIGERIMP_H

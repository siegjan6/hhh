/*!

\author dGFuZ3p3

\section 功能

权限组配置类的实现类，负责存储到文件及从文件加载。

*/

#ifndef MDPOWERGROUPCONFIGERIMP_H
#define MDPOWERGROUPCONFIGERIMP_H

#include <QString>
#include <QMap>
#include <QJsonObject>

class CMDPowerGroup;
class CMDPowerGroupConfigerImp
{
    friend class CMDPowerGroupConfiger;
public:
    CMDPowerGroupConfigerImp();

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
\section 管理权限组
*/
public:
    bool addToList(CMDPowerGroup* configProperty);
    void configedList(QList<CMDPowerGroup*>& accountList);
public:
    bool add(const CMDPowerGroup* configProperty);
    bool delete1(const QString& name);
    CMDPowerGroup* find(const QString& name);
    CMDPowerGroup* find(int index);
    bool modify(const CMDPowerGroup* sourceConfig,
                                    const CMDPowerGroup* destConfig);

/*!
\section interface for script and ua
*/
public:
    int count();
    QString getGroup(QString name);
    QStringList getGroups();
    QStringList getAccountsInGroup(QString group);
    QStringList getDisabledGroups();
    bool enableGroup(QString name);
    bool disableGroup(QString name);
    bool deleteGroup(QString name);
    // modifyGroupByUI()
    bool addAccountToGroup(QString account,QString group);
    bool removeAccountFromGroup(QString account,QString group);
    bool isAccountInGroup(QString account,QString group);
    bool upsert(QString name, QString comment, bool enable);
protected:
    QMap<QString, CMDPowerGroup*> m_configedMap;
};

#endif // MDPOWERGROUPCONFIGERIMP_H

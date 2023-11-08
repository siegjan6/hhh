/*!

\author dGFuZ3p3

\section 功能

权限组配置类，负责存储到文件及从文件加载。

*/

#ifndef MDPOWERGROUPCONFIGER_H
#define MDPOWERGROUPCONFIGER_H

#include "MDPowerGroupConfiger_Global.h"
#include <QStringList>

class CMDPowerGroup;
class CMDPowerGroupConfigerImp;
class MDPOWERGROUPCONFIGERSHARED_EXPORT CMDPowerGroupConfiger
{
public:
    CMDPowerGroupConfiger();
    ~CMDPowerGroupConfiger();

/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    void configedList(QList<CMDPowerGroup*>& powerGroupList);
    QString configFileName();
    bool saveToFile();

/*!
\section 管理权限组
*/
public:
    bool add(const CMDPowerGroup* configProperty);
    bool delete1(const QString& name);
    CMDPowerGroup* find(const QString& name);
    CMDPowerGroup* find(int index);
    bool modify(const CMDPowerGroup* sourceConfig,
                                    const CMDPowerGroup* destConfig);
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
    bool upsertGroup(QString name, QString comment, bool enable);

protected:
    CMDPowerGroupConfigerImp* m_imp;
};

#endif // MDPOWERGROUPCONFIGER_H

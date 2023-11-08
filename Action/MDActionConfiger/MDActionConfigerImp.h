/*!

\author dGFuZ3p3

\section 功能

动作配置类，负责存储到文件及从文件加载。

*/

#ifndef MDACTIONCONFIGERIMP_H
#define MDACTIONCONFIGERIMP_H

#include <QString>
#include <QList>
#include <QMap>
#include <QJsonObject>

class CMDActionProperty_Config;
class CMDActionConfigerImp
{
    friend class CMDActionConfiger;
public:
    CMDActionConfigerImp();
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
\section 管理动作
*/
public:
    bool addToList(CMDActionProperty_Config* configProperty);
    void configedActionList(QList<CMDActionProperty_Config*>& actionList);
    int count();
public:
    bool addAction(const CMDActionProperty_Config* configProperty);
    bool deleteAction(const QString& name);
    CMDActionProperty_Config* findAction(const QString& name);
    CMDActionProperty_Config* findAction(int index);
    bool modifyAction(const CMDActionProperty_Config* sourceConfig,
                                    const CMDActionProperty_Config* destConfig);
protected:
    QMap<QString, CMDActionProperty_Config*> m_configedMap;
};

#endif // MDACTIONCONFIGERIMP_H

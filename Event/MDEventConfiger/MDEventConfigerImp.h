/*!

\author dGFuZ3p3

\section 功能

事件配置类，负责存储到文件及从文件加载。

*/

#ifndef MDEVENTCONFIGERIMP_H
#define MDEVENTCONFIGERIMP_H

#include "EPC.h"
#include <QList>
#include <QJsonObject>

class CMDEventConfigerImp
{
    friend class CMDEventConfiger;
public:
    CMDEventConfigerImp();

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
\section 管理事件
*/
public:
    bool addToList(CMDEventProperty_Config* configProperty);
    void configedEventList(QList<CMDEventProperty_Config*>& eventList);
    int count();
public:
    bool addEvent(const CMDEventProperty_Config* configProperty);
    bool deleteEvent(const QString& name);
    CMDEventProperty_Config* findEvent(const QString& name);
    CMDEventProperty_Config* findEvent(int index);
    bool modifyEvent(const CMDEventProperty_Config* sourceConfig,
                                    const CMDEventProperty_Config* destConfig);
protected:
    QMap<QString, CMDEventProperty_Config*> m_configedMap;
};

#endif // MDEVENTCONFIGERIMP_H

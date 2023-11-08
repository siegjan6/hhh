/*!

\author dGFuZ3p3

\section 功能

事件配置类，负责存储到文件及从文件加载。

*/

#ifndef MDEVENTCONFIGER_H
#define MDEVENTCONFIGER_H

#include "MDEventConfiger_Global.h"
#include "../MDEventHelper/MDEventProperty_Config.h"

class CMDEventProperty_Config;
class CMDEventConfigerImp;
class MDEVENTCONFIGERSHARED_EXPORT CMDEventConfiger
{
public:
    CMDEventConfiger();
    ~CMDEventConfiger();

/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString configFileName();
    bool saveToFile();

/*!
\section 管理事件
*/
public:
    bool addEvent(const CMDEventProperty_Config* configProperty);
    bool deleteEvent(const QString& name);
    CMDEventProperty_Config* findEvent(const QString& name);
    CMDEventProperty_Config* findEvent(int index);
    bool modifyEvent(const CMDEventProperty_Config* sourceConfig,
                                    const CMDEventProperty_Config* destConfig);
public:
    void configedEventList(QList<CMDEventProperty_Config*>& eventList);
    int count();
protected:
    CMDEventConfigerImp* m_imp;
};

#endif // MDEVENTCONFIGER_H

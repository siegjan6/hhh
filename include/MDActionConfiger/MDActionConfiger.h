/*!

\author dGFuZ3p3

\section 功能

动作配置类，负责存储到文件及从文件加载。

*/

#ifndef MDACTIONCONFIGER_H
#define MDACTIONCONFIGER_H

#include "MDActionConfiger_Global.h"
#include "../MDActionHelper/APC.h"

class CMDActionProperty_Config;
class CMDActionConfigerImp;
class MDACTIONCONFIGERSHARED_EXPORT CMDActionConfiger
{
public:
    CMDActionConfiger();
    ~CMDActionConfiger();
/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QString configFileName();
    bool saveToFile();

/*!
\section 管理动作
*/
public:
    bool addAction(const CMDActionProperty_Config* configProperty);
    bool deleteAction(const QString& name);
    CMDActionProperty_Config* findAction(const QString& name);
    CMDActionProperty_Config* findAction(int index);
    bool modifyAction(const CMDActionProperty_Config* sourceConfig,
                                    const CMDActionProperty_Config* destConfig);
public:
    void configedActionList(QList<CMDActionProperty_Config*>& actionList);
    int count();
protected:
    CMDActionConfigerImp* m_imp;
};

#endif // MDACTIONCONFIGER_H

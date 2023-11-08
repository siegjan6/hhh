/*!

\author dGFuZ3p3

\section 功能

脚本专家类，实现运行时的项目全局脚本处理。

*/

#ifndef MDSCRIPTEXPERT_H
#define MDSCRIPTEXPERT_H

#include "MDScriptExpert_Global.h"
#include "MDScriptContacter.h"

class CMDScriptContacter;
class CMDScriptExpertImp;
class MDSCRIPTEXPERTSHARED_EXPORT CMDScriptExpert
{
public:
    CMDScriptExpert();
    ~CMDScriptExpert();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, const QString& projectPath, CMDScriptContacter* scriptContacter);
    void load();
    void unload();

/*!
\section 控制脚本运行
*/
public:
    bool runScript(const QString& nameOnly);
    bool stopScript(const QString& nameOnly);
    void runAll();
    void stopAll();
protected:
    CMDScriptExpertImp* m_imp;
};

#endif // MDSCRIPTEXPERT_H

/*!

\author dGFuZ3p3

\section 功能 - 项目专家类，实现运行时的项目管理。

加载/卸载项目；
管理子模块专家。

*/

#ifndef MDPROJECTEXPERT_H
#define MDPROJECTEXPERT_H

#include <QString>
#include "MDProjectExpert_Global.h"
#include "../../include/MDEventExpert/MDEventExpert.h"
#include "../../include/MDActionExpert/MDActionExpert.h"
#include "../../include/MDSecurityPolicyExpert/MDSecurityPolicyExpert.h"
#include "../../include/MDVariableExpert/MDVariableExpert.h"
#include "IMDProjectExpert.h"

enum class MDProjectLoadStatus
{
    ready = 0,
    loading  = 1,
    loaded    = 2,
    unloading = 3,
    unloaded  = 4
};

class CMDProjectExpertImp;
class CMDEventExpert;
class CMDActionExpert;
class CMDSecurityPolicyExpert;
class CMDProjectContacter;
class CMDProjectConfiger;
class CMDSwitchFactory;
class CMDRecipeExpert;
class MDPROJECTEXPERTSHARED_EXPORT CMDProjectExpert
{
public:
    CMDProjectExpert();
    ~CMDProjectExpert();
/*!
\section 加载/卸载
*/
public:
    bool load(const QString& projectName, CMDProjectContacter* projectContacter);
    bool unload();
    MDProjectLoadStatus loadStatus();
    bool canLoad();
    bool canUnload();
public:
    bool resultOnLoaded();

public:
    void showRecipeRuntime();
    void hideRecipeRuntime();

/*!
\section 项目接口类；项目配置器
*/
public:
   IMDProjectExpert* IProjectExpert();
   CMDProjectConfiger* projectConfiger();

/*!
\section 管理子模块专家
*/
public:
    CMDEventExpert* eventExpert();
    CMDActionExpert* actionExpert();
    CMDVariableExpert* variableExpert();
    CMDSecurityPolicyExpert* securityExpert();
    CMDRecipeExpert* recipeExpert();

/*!
\section 开关管理
*/
public:
    CMDSwitchFactory* funcSwitchFactory();
    CMDSwitchFactory* logSwitchFactory();

protected:
    CMDProjectExpertImp* m_imp;
};

#endif // MDPROJECTEXPERT_H

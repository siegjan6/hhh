/*!

\author dGFuZ3p3

\section 功能

动作专家类，实现运行时的动作处理。

*/

#ifndef MDACTIONEXPERT_H
#define MDACTIONEXPERT_H

#include <QString>
#include "MDActionExpert_Global.h"
#include "../../include/MDActionHelper/MDAction.h"
#include "../../include/MDActionConfiger/MDActionConfiger.h"

class CMDActionContacter;
class CMDAction;
class CMDActionConfiger;
class CMDCommandQueueEngine;
class CMDActionExpertImp;
class MDACTIONEXPERTSHARED_EXPORT CMDActionExpert
{
public:
    CMDActionExpert();
    ~CMDActionExpert();
/*!
\section 初始化与释放
*/
public:
    /*!
     初始化动作专家，并启动【动作处理引擎】。

     \param configer 动作配置类实例。
     \param actionContacter CMDActionContacter派生类的实例，通过该实例获取需要的外界信息。
     \returns 成功则返回 true，否则 false。
     */
    void setParaBeforeLoad(const QString& pid, CMDActionConfiger* configer, CMDActionContacter* actionContacter, unsigned long timeout = 0);
    bool load();
    void unload();
public:
    CMDActionConfiger* configer();

/*!
\section 启动与停止【动作处理引擎】
*/
public:
    bool start();
    void stop();
public:
    CMDCommandQueueEngine* actionEngine();

/*!
\section 处理动作
*/
public:
    CMDAction* findAction(const QString& actionName);
    bool asyncProcessAction(const QString& actionName); //! 异步处理动作，不阻塞调用线程
    bool asyncProcessAction(CMDAction* action); //! 异步处理动作，不阻塞调用线程
protected:
    CMDActionExpertImp* m_imp;
};

#endif // MDACTIONEXPERT_H

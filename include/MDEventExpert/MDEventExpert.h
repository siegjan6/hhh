/*!

\author dGFuZ3p3

\section 功能

事件专家类，实现运行时的事件处理。

*/

#ifndef MDEVENTEXPERT_H
#define MDEVENTEXPERT_H

#include <QString>
#include "MDEventExpert_Global.h"
#include "../../include/MDEventConfiger/MDEventConfiger.h"

class CMDRawEvent;
class CMDEventContacter;
class CMDEventConfiger;
class CMDEventExpertImp;
class MDEVENTEXPERTSHARED_EXPORT CMDEventExpert
{
public:
    CMDEventExpert();
    ~CMDEventExpert();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDEventConfiger* configer, CMDEventContacter* actionContacter, unsigned long timeout = 0);
    bool load();
    void unload();
public:
    CMDEventConfiger* configer();

/*!
\section 启动与停止【事件处理引擎】
*/
public:
    bool start();
    void stop();

/*!
\section 处理事件
*/
public:
    bool asyncProcessRawEvent(CMDRawEvent* rawEvent);
protected:
    CMDEventExpertImp* m_imp;
};

#endif // MDEVENTEXPERT_H

/*!

\author dGFuZ3p3

\section 功能

写变量队列引擎类，管理写变量前需要进行安全检查的动作。
该引擎一定是在主线程处理动作，因为安全检查时会显示验证界面，只能在主线程执行。

*/

#ifndef COMMANDENGINEFORCHECKVARIABLESP_H
#define COMMANDENGINEFORCHECKVARIABLESP_H

#include <QVariant>
#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDVariableExpertImp;
class CMDActionSP;
class CCommandEngineForCheckVariableSP : public CMDCommandQueueEngine
{
public:
    CCommandEngineForCheckVariableSP();
public:
    CMDVariableExpertImp* m_expert ;// NULL;
public:
    void submitWriteVarWithSP(const QString &varName, QVariant varValue, CMDActionSP* sp);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // COMMANDENGINEFORCHECKVARIABLESP_H

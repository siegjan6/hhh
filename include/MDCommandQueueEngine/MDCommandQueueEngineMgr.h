/*!

\author dGFuZ3p3

\section 功能

命令引擎管理类，便于了解引擎的数量、用途等。

*/

#ifndef MDCOMMANDQUEUEENGINEMGR_H
#define MDCOMMANDQUEUEENGINEMGR_H

#include "MDCommandQueueEngine_Global.h"

class CMDCommandQueueEngine;
class CMDCommandQueueEngineMgrImp;
class MDCOMMANDQUEUEENGINESHARED_EXPORT CMDCommandQueueEngineMgr
{
    friend class CMDCommandQueueEngine;
public:
    static void engines(QList<CMDCommandQueueEngine*>& engineList);
    static CMDCommandQueueEngine* engineAt(int index);
    static int count();
protected:
    CMDCommandQueueEngineMgr(){};
private:
    static void reg(CMDCommandQueueEngine* engine);
    static void unreg(CMDCommandQueueEngine* engine);
    static CMDCommandQueueEngineMgrImp* m_imp;
};

#endif // MDCOMMANDQUEUEENGINEMGR_H

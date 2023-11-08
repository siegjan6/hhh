#include "MDCommandQueueEngineMgr.h"
#include "MDCommandQueueEngineMgrImp.h"

CMDCommandQueueEngineMgrImp* CMDCommandQueueEngineMgr::m_imp = new CMDCommandQueueEngineMgrImp;

void CMDCommandQueueEngineMgr::engines(QList<CMDCommandQueueEngine*>& engineList)
{
    if(m_imp)
        m_imp->engines(engineList);
}

CMDCommandQueueEngine* CMDCommandQueueEngineMgr::engineAt(int index)
{
    return m_imp->engineAt(index);
}

int CMDCommandQueueEngineMgr::count()
{
    return m_imp->count();
}

void CMDCommandQueueEngineMgr::reg(CMDCommandQueueEngine* engine)
{
    if(m_imp)
        m_imp->reg(engine);
}

void CMDCommandQueueEngineMgr::unreg(CMDCommandQueueEngine* engine)
{
    if(m_imp)
        m_imp->unreg(engine);
}

#include "MDCommandQueueEngineMgrImp.h"

CMDCommandQueueEngineMgrImp::CMDCommandQueueEngineMgrImp()
{
}

void CMDCommandQueueEngineMgrImp::reg(CMDCommandQueueEngine* engine)
{
    QMutexLocker engineListLocker(&m_engineListMutex);
    m_engineList.append(engine);
}

void CMDCommandQueueEngineMgrImp::unreg(CMDCommandQueueEngine* engine)
{
    QMutexLocker engineListLocker(&m_engineListMutex);

    int index = m_engineList.indexOf(engine);
    if(index != -1)
        m_engineList.removeAt(index);
}

void CMDCommandQueueEngineMgrImp::engines(QList<CMDCommandQueueEngine*>& engineList)
{
    QMutexLocker engineListLocker(&m_engineListMutex);

    engineList.append(m_engineList);
}

CMDCommandQueueEngine* CMDCommandQueueEngineMgrImp::engineAt(int index)
{
    QMutexLocker engineListLocker(&m_engineListMutex);

    if(index < 0 || index >= m_engineList.count())
        return NULL;

    return m_engineList.at(index);
}

int CMDCommandQueueEngineMgrImp::count()
{
    QMutexLocker engineListLocker(&m_engineListMutex);

    return m_engineList.count();
}

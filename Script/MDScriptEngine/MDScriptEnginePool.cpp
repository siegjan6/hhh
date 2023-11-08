#include "MDScriptEnginePool.h"
#include "MDScriptEngineImp.h"

/*!
 * \brief 引擎池类，管理运行的多个脚本引擎
 */
CMDScriptEnginePool::CMDScriptEnginePool()
{
}

CMDScriptEnginePool::~CMDScriptEnginePool()
{
    unregAll();
}

void CMDScriptEnginePool::regEngine(CMDScriptEngineImp* engine)
{
    QMap<CMDScriptEngineImp*, CMDScriptEngineImp*>::const_iterator iPosition = m_engineMap.find(engine);
    if(m_engineMap.constEnd() == iPosition)
        m_engineMap.insert(engine, engine);
}

void CMDScriptEnginePool::unregEngine(CMDScriptEngineImp* engine)
{
    m_engineMap.remove(engine);
}

void CMDScriptEnginePool::unregAll()
{
    m_engineMap.clear();
}

void CMDScriptEnginePool::callFunc(const QString& funcName, QList<QVariant>& paras)
{
    QMap<CMDScriptEngineImp*, CMDScriptEngineImp*>::const_iterator iPosition = m_engineMap.constBegin();
    while(m_engineMap.constEnd() != iPosition)
    {
        iPosition.value()->callFunc(funcName, paras);
        ++ iPosition;
    }
}

/*!
 * \brief 引擎池类的管理类
 */
CMDScriptEnginePoolMgr* CMDScriptEnginePoolMgr::m_single;
QMap<QString, CMDScriptEnginePool*> CMDScriptEnginePoolMgr::m_poolMap;
CMDScriptEnginePoolMgr* CMDScriptEnginePoolMgr::instance()
{
    if(!m_single)
        m_single = new CMDScriptEnginePoolMgr;
    return m_single;
}

void CMDScriptEnginePoolMgr::release()
{
    deleteAllPools();

    if(m_single)
        delete m_single;
    m_single = NULL;
}

// 引擎操作
CMDScriptEnginePool* CMDScriptEnginePoolMgr::regToPool(CMDScriptEngineImp* engine, const QString& PID)
{
    CMDScriptEnginePool* enginePool = instance()->addPool(PID);
    if(enginePool)
        enginePool->regEngine(engine);
    return enginePool;
}

bool CMDScriptEnginePoolMgr::unregFromPool(CMDScriptEngineImp* engine, const QString& PID)
{
    // TODO: 如果 pool 中的引擎全部注销，则可以释放 pool

    CMDScriptEnginePool* enginePool = instance()->findPool(PID);
    if(enginePool)
        enginePool->unregEngine(engine);
    return true;
}

bool CMDScriptEnginePoolMgr::callFuncInPool(const QString& funcName, QList<QVariant>& paras, const QString& PID)
{
    CMDScriptEnginePool* enginePool = instance()->findPool(PID);
    if(enginePool)
        enginePool->callFunc(funcName, paras);
    return (enginePool != NULL);
}

// pool 操作
CMDScriptEnginePool* CMDScriptEnginePoolMgr::addPool(const QString& PID)
{
    QString pid = PID;
    pid.trimmed();

    CMDScriptEnginePool* pool = findPool(pid);
    if(pool)
        return pool;

    CMDScriptEnginePool* newPool = new CMDScriptEnginePool;
    m_poolMap.insert(pid, newPool);
    return newPool;
}

CMDScriptEnginePool* CMDScriptEnginePoolMgr::findPool(const QString& PID)
{
    QString pid = PID;
    pid.trimmed();

    QMap<QString, CMDScriptEnginePool*>::const_iterator iPosition = m_poolMap.find(pid);
   return (m_poolMap.constEnd() != iPosition ? iPosition.value() : NULL);
}

bool CMDScriptEnginePoolMgr::deletePool(const QString& PID)
{
    QString pid = PID;
    pid.trimmed();

    QMap<QString, CMDScriptEnginePool*>::const_iterator iPosition = m_poolMap.find(pid);
    if(m_poolMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        return true;
    }
    return false;
}

void CMDScriptEnginePoolMgr::deleteAllPools()
{
    QMap<QString, CMDScriptEnginePool*>::const_iterator iPosition = m_poolMap.constBegin();
    while(m_poolMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_poolMap.clear();
}

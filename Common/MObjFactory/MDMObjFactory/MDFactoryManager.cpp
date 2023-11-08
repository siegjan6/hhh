#include "MDFactoryManager.h"
#include "MDMObjFactory.h"

CMDFactoryManager::CMDFactoryManager()
{
}

CMDFactoryManager::~CMDFactoryManager()
{
    deleteAll();
}

CMDMObj* CMDFactoryManager::createMObj(CMDMObjFactory* staticFactoryInstance, const CMDMObjPara& para)
{
    CMDMObjFactory* f = createFactory(staticFactoryInstance, para);
    if(NULL == f)
        f = staticFactoryInstance;

    CMDMObj* mobj = f->newMObj(para);
    if(f->add(mobj))
        return mobj;
    else
        return f->theMObj();
}

CMDMObjFactory* CMDFactoryManager::createFactory(CMDMObjFactory* staticFactoryInstance, const CMDMObjPara& para)
{
    QMutexLocker locker(&m_mutex);

    CMDMObjFactory* f = NULL;
    QString fName = para.name_from_pid_type();
    QMap<QString, CMDMObjFactory*>::const_iterator it = m_fMap.find(fName);
    if(it != m_fMap.constEnd())
    {
        f = it.value();
    }
    else
    {
        CMDMObjPara fPara(para);
        fPara.setName(fName);
        f = staticFactoryInstance->newFactory(fPara);
        if(f)
            m_fMap.insert(fName, f);
    }
    return f;
}

CMDMObjFactory* CMDFactoryManager::findFactory(const CMDMObjPara& para)
{
    QMutexLocker locker(&m_mutex);

    CMDMObjFactory* f = NULL;
    QString fName = para.name_from_pid_type();
    QMap<QString, CMDMObjFactory*>::const_iterator it = m_fMap.find(fName);
    if(it != m_fMap.constEnd())
    {
        f = it.value();
    }
    return f;
}

void CMDFactoryManager::deleteSome(const QString& pid)
{
    QMutexLocker locker(&m_mutex);

    QMap<QString, CMDMObjFactory*>::iterator it = m_fMap.begin();
    while(it != m_fMap.end())
    {
        CMDMObjFactory* f = it.value();
        QString fName = f->para().name_from_pid_type();
        if(fName.contains(pid, Qt::CaseInsensitive))
        {
            delete f;
            it = m_fMap.erase(it);
        }
        else it++;
    }
}

void CMDFactoryManager::deleteAll()
{
    QMutexLocker locker(&m_mutex);

    qDeleteAll(m_fMap);
    m_fMap.clear();
}

void CMDFactoryManager::enumAll(QList<CMDMObjFactory*>& fList)
{
    QMutexLocker locker(&m_mutex);

    foreach (CMDMObjFactory* f, m_fMap) {
        fList.append(f);
    }
}

void CMDFactoryManager::saveAll()
{
    QMutexLocker locker(&m_mutex);

    foreach (CMDMObjFactory* f, m_fMap) {
        f->save();
    }
}

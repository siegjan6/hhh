#include "MDMObjFactoryImp.h"
#include "MDMObj.h"

CMDMObjFactoryImp::CMDMObjFactoryImp(CMDMObj* theMObj)
{
    m_theMObj = theMObj;
}

CMDMObjFactoryImp::CMDMObjFactoryImp(const CMDMObjPara& para, CMDMObj* theMObj)
{
    m_para =para;
    m_theMObj = theMObj;
}

CMDMObjFactoryImp::~CMDMObjFactoryImp()
{
    deleteAll();
}

const int MAX_MOBJ_COUNT = 1024 * 1024 * 4;
bool CMDMObjFactoryImp::add(CMDMObj* mobj)
{
    QMutexLocker locker(&m_mutex);

    bool result = false;
    if(mobj && m_mobjList.count() < MAX_MOBJ_COUNT)
    {
        m_mobjList.append(mobj);
        result = true;
    }
    else
    {
        if(mobj)
            delete mobj;
    }
    return result;
}

bool CMDMObjFactoryImp::delete1(CMDMObj* mobj)
{
    QMutexLocker locker(&m_mutex);

    // 只删除由此工厂管理的对象
    bool result = m_mobjList.removeOne(mobj);
    if(result)
        delete mobj;

    return result;
}

void CMDMObjFactoryImp::deleteAll()
{
    QMutexLocker locker(&m_mutex);

    foreach (CMDMObj* mobj, m_mobjList)
        delete mobj;
    m_mobjList.clear();
}

void CMDMObjFactoryImp::enumAll(QList<CMDMObj*>& sList)
{
    QMutexLocker locker(&m_mutex);

    foreach (CMDMObj* mobj, m_mobjList)
    {
        sList.append(mobj);
    }
}

int CMDMObjFactoryImp::count()
{
    QMutexLocker locker(&m_mutex);
    return m_mobjList.count();
}

CMDMObj* CMDMObjFactoryImp::find(int index)
{
    QMutexLocker locker(&m_mutex);

    CMDMObj* mo = NULL;
    if(index >= 0 && index < m_mobjList.count())
        mo = m_mobjList[index];
    return mo;
}

CMDMObj* CMDMObjFactoryImp::findByGroup(const QString& group)
{
    QMutexLocker locker(&m_mutex);

    foreach (CMDMObj* mobj, m_mobjList)
    {
        if(0 == mobj->para().group().compare(group, Qt::CaseInsensitive))
            return mobj;
    }
    return NULL;
}

bool CMDMObjFactoryImp::save(CMDMObjFactory* f)
{
    CMDFactoryFile fFile;
    return fFile.save(f);
}

bool CMDMObjFactoryImp::load(CMDMObjFactory* f)
{
    return m_fFile.load(f);
}

QString CMDMObjFactoryImp::groupValue(const QString& group)
{
    return m_fFile.groupValue(group);
}


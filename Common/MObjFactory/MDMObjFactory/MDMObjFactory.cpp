#include "MDMObjFactory.h"
#include "MDMObjFactoryImp.h"
#include "MDFactoryManager.h"

CMDMObjFactory::CMDMObjFactory()
{
    CMDMObjPara para;
    m_imp = new CMDMObjFactoryImp(newMObj(para));
}

CMDMObjFactory::CMDMObjFactory(const CMDMObjPara& para)
{
    m_imp = new CMDMObjFactoryImp(para, newMObj(para));
}

CMDMObjFactory::~CMDMObjFactory()
{
    if(m_imp)
        delete m_imp;
}

static CMDFactoryManager g_fManager;
CMDMObj* CMDMObjFactory::createMObj(CMDMObjFactory* staticFactoryInstance, const CMDMObjPara& para)
{
    return g_fManager.createMObj(staticFactoryInstance, para);
}

static CMDMObjFactory g_staticMObjFactory;
CMDMObj* CMDMObjFactory::createMObj(const CMDMObjPara& para)
{
    return g_fManager.createMObj(&g_staticMObjFactory, para);
}

bool CMDMObjFactory::deleteMObj(CMDMObj* mobj)
{
    CMDMObjFactory* f = (mobj ? mobj->factory() : NULL);
    return (f ? f->delete1(mobj) : false);
}

void CMDMObjFactory::enumFactories(QList<CMDMObjFactory*>& fList)
{
    g_fManager.enumAll(fList);
}

CMDMObjFactory* CMDMObjFactory::findFactory(const CMDMObjPara& para)
{
    return g_fManager.findFactory(para);
}

void CMDMObjFactory::deleteFactories(const QString& pid)
{
    g_fManager.deleteSome(pid);
}

void CMDMObjFactory::deleteAllFactories()
{
    g_fManager.deleteAll();
}

void CMDMObjFactory::saveAllFactories()
{
    g_fManager.saveAll();
}

bool CMDMObjFactory::add(CMDMObj* mobj)
{
   return m_imp->add(mobj);
}

CMDMObj* CMDMObjFactory::theMObj()
{
   return m_imp->m_theMObj;
}

bool CMDMObjFactory::delete1(CMDMObj* mobj)
{
   return m_imp->delete1(mobj);
}

void CMDMObjFactory::deleteAll()
{
    m_imp->deleteAll();
}

void CMDMObjFactory::enumAll(QList<CMDMObj*>& sList)
{
    m_imp->enumAll(sList);
}

int CMDMObjFactory::count()
{
    return m_imp->count();
}

CMDMObj* CMDMObjFactory::find(int index)
{
    return m_imp->find(index);
}

CMDMObj* CMDMObjFactory::findByGroup(const QString& group)
{
    return m_imp->findByGroup(group);
}

const CMDMObjPara& CMDMObjFactory::para() const
{
    return m_imp->m_para;
}

bool CMDMObjFactory::save()
{
    return m_imp->save(this);
}

bool CMDMObjFactory::load()
{
    return m_imp->load(this);
}

QString CMDMObjFactory::groupValue(const QString& group)
{
    return m_imp->groupValue(group);
}

CMDMObj* CMDMObjFactory::newMObj(const CMDMObjPara& para)
{
    CMDMObj* m = new CMDMObj(para, this);
    return m;
}

CMDMObjFactory* CMDMObjFactory::newFactory(const CMDMObjPara& para)
{
    CMDMObjFactory* f = new CMDMObjFactory(para);
    return f;
}

QString CMDMObjFactory::subPathForFactoryFile()
{
    return "mFactory";
}

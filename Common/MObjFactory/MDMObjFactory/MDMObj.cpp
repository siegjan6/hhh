#include "MDMObj.h"
#include "MDMObjImp.h"

CMDMObj::CMDMObj()
{
    m_imp = new CMDMObjImp;
}

CMDMObj::CMDMObj(const CMDMObjPara& para, CMDMObjFactory* factory)
{
    m_imp = new CMDMObjImp(para, factory);
}

CMDMObj::~CMDMObj()
{
    if(m_imp)
        delete m_imp;
}

const CMDMObjPara& CMDMObj::para() const
{
    return m_imp->m_para;
}

CMDMObjFactory* CMDMObj::factory() const
{
    return m_imp->m_factory;
}

QString CMDMObj::groupValue()
{
    return "";
}

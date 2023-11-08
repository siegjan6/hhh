#ifndef MDMOBJ_H
#define MDMOBJ_H

#include "MDMObjFactory_Global.h"
#include "MDMObjPara.h"

class CMDMObjImp;
class CMDMObjFactory;
class MDMOBJFACTORYSHARED_EXPORT CMDMObj
{
    friend class CMDMObjFactoryImp;
    friend class CMDMObjFactory;
protected:
    CMDMObj();
    CMDMObj(const CMDMObjPara& para, CMDMObjFactory* factory);
    virtual ~CMDMObj();
public:
    const CMDMObjPara& para() const;
public:
    CMDMObjFactory* factory() const;
public:
    virtual QString groupValue();
protected:
    CMDMObjImp* m_imp;
};

#endif // MDMOBJ_H

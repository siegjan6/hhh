#ifndef MDMOBJFACTORY_H
#define MDMOBJFACTORY_H

#include "MDMObjFactory_Global.h"
#include "MDMObj.h"

class CMDMObjFactoryImp;
class MDMOBJFACTORYSHARED_EXPORT CMDMObjFactory
{
    friend class CMDFactoryManager;
public:
    CMDMObjFactory();
    CMDMObjFactory(const CMDMObjPara& para);
    virtual~CMDMObjFactory();
public:
    bool delete1(CMDMObj* mobj);
    void deleteAll();
    void enumAll(QList<CMDMObj*>& sList);
    int count();
    CMDMObj* find(int index);
    CMDMObj* findByGroup(const QString& group);
    const CMDMObjPara& para() const;
protected:
    bool add(CMDMObj* mobj);
    CMDMObj* theMObj();
public:
    static CMDMObj* createMObj(const CMDMObjPara& para);
    static bool deleteMObj(CMDMObj* mobj);
public:
    static void enumFactories(QList<CMDMObjFactory*>& fList);
    static CMDMObjFactory* findFactory(const CMDMObjPara& para);
    static void deleteFactories(const QString& pid);
    static void deleteAllFactories();
    static void saveAllFactories();
protected:
    static CMDMObj* createMObj(CMDMObjFactory* staticFactoryInstance, const CMDMObjPara& para);

public:
    bool save();
    bool load();
    QString groupValue(const QString& group);

public:
    virtual CMDMObj* newMObj(const CMDMObjPara& para);
    virtual CMDMObjFactory* newFactory(const CMDMObjPara& para);
    virtual QString subPathForFactoryFile();
protected:
    CMDMObjFactoryImp* m_imp;
};

#endif // MDMOBJFACTORY_H

#ifndef MDSWITCHFACTORY_H
#define MDSWITCHFACTORY_H

#include "MDSwitchFactory_Global.h"
#include "../MDMObjFactory/MDMObjFactory.h"

class CMDSwitch;
class MDSWITCHFACTORYSHARED_EXPORT CMDSwitchFactory : public CMDMObjFactory
{
public:
    CMDSwitchFactory();
    CMDSwitchFactory(const CMDMObjPara& para);
    virtual~CMDSwitchFactory();
public:
    void turnOff();
    void turnOn();
    void enumOn(QList<CMDSwitch*>& switchList);
    void enumOnNames(QStringList& switchNameList);
    void enumOff(QList<CMDSwitch*>& switchList);
    void enumOffNames(QStringList& switchNameList);
public:
    static bool deleteSwitch(CMDSwitch* s);
protected:
    static CMDSwitch* createSwitch(CMDMObjFactory* factoryCreator, const CMDMObjPara& para, bool on = true);
protected:
    virtual CMDMObj* newMObj(const CMDMObjPara& para);
    virtual CMDMObjFactory* newFactory(const CMDMObjPara& para);
    virtual QString subPathForFactoryFile();
};

class CMDSwitchImp;
class MDSWITCHFACTORYSHARED_EXPORT CMDSwitch : public CMDMObj
{
    friend class CMDSwitchFactory;
protected:
    CMDSwitch();
    CMDSwitch(const CMDMObjPara& para, CMDMObjFactory* factory);
    virtual~CMDSwitch();
public:
    bool isOn();
    bool turnOn();
    bool turnOff();
public:
    virtual QString groupValue();
protected:
    CMDSwitchImp* m_imp;
};

#endif // MDSWITCHFACTORY_H

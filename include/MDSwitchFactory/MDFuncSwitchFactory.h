#ifndef MDFUNCSWITCHFACTORY_H
#define MDFUNCSWITCHFACTORY_H

#include "MDSwitchFactory_Global.h"
#include "MDSwitchFactory.h"

class CMDSwitch;
class MDSWITCHFACTORYSHARED_EXPORT CMDFuncSwitchFactory : public CMDSwitchFactory
{
public:
    CMDFuncSwitchFactory();
    CMDFuncSwitchFactory(const CMDMObjPara& para);
    virtual~CMDFuncSwitchFactory();
public:
    static CMDSwitch* createSwitch(const QString& pid, const QString& group, const QString& comment);
public:
    static CMDSwitch* findByGroup(const QString& pid, const QString& group);
};

#endif // MDFUNCSWITCHFACTORY_H

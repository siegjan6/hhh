#ifndef MDPROJECTREMOTECONFIGERFACTORY_H
#define MDPROJECTREMOTECONFIGERFACTORY_H
#include "nsprojectremoteconfiger_global.h"
#include "IMDProjectRemoteConfiger.h"

class MDPROJECTREMOTECONFIGERSHARED_EXPORT CMDProjectRemoteConfigerFactory
{
public:
    CMDProjectRemoteConfigerFactory();
    ~CMDProjectRemoteConfigerFactory();
public:
    static IMDProjectRemoteConfiger *createProjectRemoteConfiger();
};
#endif // MDPROJECTREMOTECONFIGERFACTORY_H

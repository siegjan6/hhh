#ifndef MDVARIABLECONFIGER_H
#define MDVARIABLECONFIGER_H

#include "MDVariableConfiger_Global.h"
#include "../../include/MDVariableHelper/MDVariableProperty.h"

class CMDVariableProperty_Config;
class CMDVariableConfigerImp;
class MDVARIABLECONFIGERSHARED_EXPORT CMDVariableConfiger
{
public:
    CMDVariableConfiger();
    ~CMDVariableConfiger();

public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    QList<CMDVariableProperty_Config*>* configedVariableList();
protected:
    CMDVariableConfigerImp* m_imp;
};

#endif // MDVARIABLECONFIGER_H

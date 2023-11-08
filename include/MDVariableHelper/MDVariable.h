#ifndef MDVARIABLE_H
#define MDVARIABLE_H

#include "MDVariableHelper_Global.h"
#include "MDVariableProperty.h"

class CMDVariableData;
class CMDProjectModuleExpert;
class CMDVariableProperty_Config;
class CMDVariableProperty_Runtime;
class MDVARIABLEHELPERSHARED_EXPORT CMDVariable
{
public:
    CMDVariable();
    CMDVariable(const CMDVariableProperty_Config& configProperty);
    ~CMDVariable();
    CMDVariable(const CMDVariable& obj);
    CMDVariable& operator = (const CMDVariable& obj);
public:
    CMDVariableProperty_Config* configProperty() const;
    bool setConfigProperty(const CMDVariableProperty_Config& configProperty);
public:
    bool enableRuntime();
public:
    CMDVariableProperty_Runtime* runtimeProperty() const;
protected:
    CMDVariableData* m_data;
};

#endif // MDVARIABLE_H

#ifndef MDRECIPEDATA_H
#define MDRECIPEDATA_H

#include "MDRecipeProperty_Config.h"
#include "MDRecipeProperty_Runtime.h"

class CMDRecipeData
{
public:
    CMDRecipeData();
    ~CMDRecipeData();
public:
    CMDRecipeProperty_Config m_configProperty;
    CMDRecipeProperty_Runtime m_runtimeProperty;
};

#endif // MDRECIPEDATA_H

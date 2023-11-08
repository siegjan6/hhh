#ifndef MDACCOUNTDATA_H
#define MDACCOUNTDATA_H

#include "MDAccountProperty_Config.h"
#include "MDAccountProperty_Runtime.h"

class CMDAccountData
{
public:
    CMDAccountData();
public:
    CMDAccountProperty_Config m_configProperty;
    CMDAccountProperty_Runtime m_runtimeProperty;
};

#endif // MDACCOUNTDATA_H

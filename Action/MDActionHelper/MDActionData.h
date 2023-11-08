/*!

\author dGFuZ3p3

\section 功能

动作数据类的实现类，参考 CMDAction。

*/

#ifndef MDACTIONDATA_H
#define MDACTIONDATA_H

#include "MDActionConstant.h"

class CMDActionProperty_Config;
class CMDActionProperty_Runtime;
class CMDProjectModuleExpert;
class CMDActionData
{
public:
    CMDActionData();
    CMDActionData(CMDActionProperty_Config* configProperty);
    ~CMDActionData();
public:
    CMDActionProperty_Config* m_configProperty;
    CMDActionProperty_Runtime* m_runtimeProperty;
public:
    CMDProjectModuleExpert* m_actionExecuter;
};

#endif // MDACTIONDATA_H

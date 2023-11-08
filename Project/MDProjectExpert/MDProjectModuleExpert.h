/*!

\author dGFuZ3p3

\section 功能

子模块专家代理类，定义子模块的通用行为，例如，都要完成动作执行。

*/

#ifndef MDPROJECTMODULEEXPERT_H
#define MDPROJECTMODULEEXPERT_H

#include <QtGlobal>

class CMDAction;
class CMDProjectModuleExpert
{
public:
    CMDProjectModuleExpert();
    virtual ~CMDProjectModuleExpert();
public:
    virtual qint8 executeAction(CMDAction* action);
};

#endif // MDPROJECTMODULEEXPERT_H

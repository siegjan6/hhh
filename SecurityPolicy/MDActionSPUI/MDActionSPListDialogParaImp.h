/*!

\author dGFuZ3p3

\section 功能

CMDActionSP 链表配置对话类的辅助参数类。

*/

#ifndef MDACTIONSPLISTDIALOGPARAIMP_H
#define MDACTIONSPLISTDIALOGPARAIMP_H

#include "MDActionSP.h"
class CMDPowerGroupConfiger;
class CMDActionSPListDialogParaImp
{
public:
    CMDActionSPListDialogParaImp();
    ~CMDActionSPListDialogParaImp();
public:
    CMDActionSPList m_destList;
    CMDPowerGroupConfiger* m_pgConfiger;
};

#endif // MDACTIONSPLISTDIALOGPARAIMP_H

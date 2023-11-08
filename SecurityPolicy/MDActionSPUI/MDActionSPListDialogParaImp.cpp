#include "MDActionSPListDialogParaImp.h"

CMDActionSPListDialogParaImp::CMDActionSPListDialogParaImp()
{
    m_pgConfiger = NULL;
}

CMDActionSPListDialogParaImp::~CMDActionSPListDialogParaImp()
{
   m_destList.deleteAll();
}

#include "MDObjectTreeWindowPara.h"
#include "MDObjectTreeWindowParaImp.h"

CMDObjectTreeWindowPara::CMDObjectTreeWindowPara()
{
    m_imp = new CMDObjectTreeWindowParaImp;
}

CMDObjectTreeWindowPara::~CMDObjectTreeWindowPara()
{
    if(m_imp)
        delete m_imp;
}

void CMDObjectTreeWindowPara::setAddObjOnSelect(bool enable)
{
    m_imp->m_addObjOnSelect = enable;
}

bool CMDObjectTreeWindowPara::isAddObjOnSelect()
{
    return m_imp->m_addObjOnSelect;
}

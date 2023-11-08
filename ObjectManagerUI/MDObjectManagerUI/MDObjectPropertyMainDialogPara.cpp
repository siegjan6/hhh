#include "MDObjectPropertyMainDialogPara.h"
#include "MDObjectPropertyMainDialogParaImp.h"

CMDObjectPropertyMainDialogPara::CMDObjectPropertyMainDialogPara()
{
    m_imp = new CMDObjectPropertyMainDialogParaImp;
}

CMDObjectPropertyMainDialogPara::~CMDObjectPropertyMainDialogPara()
{
    if(m_imp)
        delete m_imp;
}

void* CMDObjectPropertyMainDialogPara::configer()
{
    return (m_imp ? m_imp->m_configer : NULL);
}

void* CMDObjectPropertyMainDialogPara::destProperty()
{
    return m_imp->m_destProperty;
}

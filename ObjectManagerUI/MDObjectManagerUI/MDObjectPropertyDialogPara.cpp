#include "MDObjectPropertyDialogPara.h"
#include "MDObjectPropertyDialogParaImp.h"

CMDObjectPropertyDialogPara::CMDObjectPropertyDialogPara()
{
    m_imp = new CMDObjectPropertyDialogParaImp;
}

CMDObjectPropertyDialogPara::~CMDObjectPropertyDialogPara()
{
    if(m_imp)
        delete m_imp;
}

void* CMDObjectPropertyDialogPara::objectUI()
{
    return (m_imp ? m_imp->m_objectUI : NULL);
}

ITranslateWidgetTool* CMDObjectPropertyDialogPara::twTool()
{
    return (m_imp ? m_imp->m_twTool : NULL);
}

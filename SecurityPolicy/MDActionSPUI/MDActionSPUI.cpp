#include "MDActionSPUI.h"
#include "MDActionSPUIImp.h"

CMDActionSPUI::CMDActionSPUI()
{
    m_imp = new CMDActionSPUIImp;
}

CMDActionSPUI::~CMDActionSPUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化
*/
bool CMDActionSPUI::init(CMDPowerGroupConfiger* configer)
{
    return (m_imp ? m_imp->init(configer) : false);
}

/*!
\section 显示模态界面
*/
bool CMDActionSPUI::doModalActionSPDialog(CMDActionSP *sourceProperty)
{
    return (m_imp ? m_imp->doModalActionSPDialog(sourceProperty) : false);
}

bool CMDActionSPUI::doModalActionSPListDialog(CMDActionSPList& sourceList, CMDActionSPList& destList)
{
    return (m_imp ? m_imp->doModalActionSPListDialog(sourceList, destList) : false);
}

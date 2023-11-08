#include "MDCommandQueueEngineUI.h"
#include "MDEngineMgrDialog.h"

CMDCommandQueueEngineUI::CMDCommandQueueEngineUI()
{
    m_dlg = new CMDEngineMgrDialog;
}

CMDCommandQueueEngineUI::~CMDCommandQueueEngineUI()
{
    if(m_dlg)
        delete m_dlg;
}

void CMDCommandQueueEngineUI::release()
{
    if(m_dlg)
    {
        delete m_dlg;
        m_dlg = NULL;
    }
}

void CMDCommandQueueEngineUI::showMgrUI(bool show)
{
    if(!m_dlg)
        return;

    if(show)
    {
        m_dlg->showUI();
    }
    else
    {
        m_dlg->closeUI();
    }
}

#include "MDEventActionUIImp.h"

CMDEventActionUIImp::CMDEventActionUIImp()
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_eventUIContacter.m_uiImp = this;
    m_eventUI.setContacter(&m_eventUIContacter);

    m_actionUIContacter.m_uiImp = this;
    m_actionUI.setContacter(&m_actionUIContacter);
}

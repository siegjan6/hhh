#include "MDAlarmUIImp.h"

CMDAlarmUIImp::CMDAlarmUIImp()
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;
    m_conditionUI.setContacter(&m_conditionContacter);
    m_conditionContacter.m_uiImp = this;
}


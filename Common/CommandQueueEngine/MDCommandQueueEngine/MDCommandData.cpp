#include "MDCommandData.h"

CMDCommandData::CMDCommandData()
{
    m_type = -1;
    m_timeOnCreated = QDateTime::currentDateTime();
    m_para = NULL;
}

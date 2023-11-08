#include "MDActionData.h"
#include "APC.h"
#include "MDActionProperty_Runtime.h"

CMDActionData::CMDActionData()
{
    m_configProperty = CAPC_Factory::newConfig(MDActionType::null_action, NULL);
    m_runtimeProperty = NULL;

    m_actionExecuter = NULL;
}

CMDActionData::CMDActionData(CMDActionProperty_Config* configProperty)
{
    m_configProperty = CAPC_Factory::newConfig(MDActionType::null_action, configProperty);

    m_runtimeProperty = (m_configProperty ? new CMDActionProperty_Runtime() : NULL);

    m_actionExecuter = NULL;
}

CMDActionData::~CMDActionData()
{
    if(m_runtimeProperty)
        delete m_runtimeProperty;

    if(m_configProperty)
        delete m_configProperty;
}

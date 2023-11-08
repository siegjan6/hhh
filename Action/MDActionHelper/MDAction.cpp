#include "MDAction.h"
#include "MDActionData.h"

CMDAction::CMDAction()
{
    m_data = new CMDActionData();
}

CMDAction::CMDAction(CMDActionProperty_Config* configProperty)
{
    m_data = new CMDActionData(configProperty);
}

CMDAction::~CMDAction()
{
    if(m_data)
        delete m_data;
}

/*!
\section 属性
*/
MDActionType CMDAction::type() const
{
    CMDActionProperty_Config* cfp = configProperty();
    return (cfp ? cfp->actionID().type() : MDActionType::null_action);
}

CMDActionProperty_Config* CMDAction::configProperty() const
{
    return (m_data ? m_data->m_configProperty : NULL);
}

CMDActionProperty_Runtime* CMDAction::runtimeProperty() const
{
    return (m_data ? m_data->m_runtimeProperty : NULL);
}

/*!
\section 动作执行器
*/
CMDProjectModuleExpert* CMDAction::actionExecuter() const
{
    return (m_data ? m_data->m_actionExecuter : NULL);
}
void CMDAction::setActionExecuter(CMDProjectModuleExpert* actionExecuter)
{
    if(m_data)
        m_data->m_actionExecuter = actionExecuter;
}

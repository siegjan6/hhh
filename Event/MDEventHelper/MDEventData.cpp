#include "MDEventData.h"
#include "MDEventHandler.h"

/*!
 * \brief 事件数据类的实现类
 */
CMDEventData::CMDEventData(CMDEventProperty_Config* configProperty)
{
    m_handler = NULL;

    m_configProperty = CEPC_Factory::newConfig(MDEventType::null_event, configProperty);
    m_runtimeProperty = (m_configProperty ? new CMDEventProperty_Runtime() : NULL);
}

CMDEventData::~CMDEventData()
{
    if(m_handler)
        delete m_handler;

    if(m_runtimeProperty)
        delete m_runtimeProperty;

    if(m_configProperty)
        delete m_configProperty;

    m_relatedEventList.clear();

    m_actionList.clear();
}

/*!
 * \brief 事件处理器类的实现类
 */
CMDEventHandlerData::CMDEventHandlerData(CMDEvent* event)
{
    m_event = event;
}

CMDEventHandlerData::~CMDEventHandlerData()
{

}

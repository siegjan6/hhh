#include "MDEventClassFactory.h"

CMDEventProperty_Config* CFactoryForEventProperty::createConfig(CMDEventProperty_Config* configProperty)
{
    return CEPC_Factory::newConfig(MDEventType::null_event, configProperty);
}

CMDEventProperty_Runtime* CFactoryForEventProperty::createRuntime(MDEventType nsEventType)
{
    CMDEventProperty_Runtime* newProperty = NULL;
    newProperty = new CMDEventProperty_Runtime();
//    switch(nsEventType)
//    {
//        case MDEventType::variable_ValueChanged :
//            newProperty = new CMDEventProperty_Config();
//            break;
//        case MDEventType::variable_ValueChangedInRange :
//            newProperty = new CMDEventProperty_Config_VariantValueChangedInRange();
//            break;
//        case MDEventType::variable_QualityChanged :
//            newProperty = new CMDEventProperty_Config_VariantQualityChanged();
//            break;
//        case MDEventType::variable_ValueRead :
//            newProperty = new CMDEventProperty_Config();
//            break;
//        case MDEventType::variable_ValueWrite :
//            newProperty = new CMDEventProperty_Config_VariantValueWrite();
//            break;
//     default:;
//    }

    return newProperty;
}

CMDEventHandlerEx* CFactoryForEventProperty::createEventHandler(MDEventType nsEventType, CMDEvent* nsEvent)
{
    CMDEventHandlerEx* newHandler = NULL;
    switch(nsEventType)
    {
        case MDEventType::variable_ValueChanged :
        case MDEventType::variable_ValueChangedInRange :
            newHandler = new CMDVariantValueEventHandler(nsEvent);
        break;
    case MDEventType::timer_Cycle :
        newHandler = new CMDTimerCycleEventHandler(nsEvent);
    break;
    case MDEventType::variable_QualityChanged :
    case MDEventType::variable_ValueRead :
    case MDEventType::variable_ValueWrite :
        newHandler = new CMDVariantQualityEventHandler(nsEvent);
    break;
        case MDEventType::combined :
            newHandler = new CMDEventHandlerEx(nsEvent);
        break;
     default:;
    }

    return newHandler;
}

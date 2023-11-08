#include "EPC.h"

// 工厂
CEPC_Factory::CEPC_Factory()
{

}

CMDEventProperty_Config* CEPC_Factory::newConfig(MDEventType eventType, const CMDEventProperty_Config* sourceProperty)
{
    CMDEventProperty_Config* newProperty = NULL;
    MDEventType theEventType = (sourceProperty ? sourceProperty->type() : eventType);
    switch (theEventType)
    {
    case MDEventType::variable_ValueChangedInRange:
        newProperty = new CEPC_VariableValueChangedInRange();
    break;

    case MDEventType::variable_ValueChanged:
        newProperty = new CMDEventProperty_Config(theEventType);
    break;

    case MDEventType::variable_QualityChanged:
        newProperty = new CEPC_VariableQualityChanged();
    break;

    case MDEventType::variable_ValueWrite:
        newProperty = new CEPC_VariableValueWrite();
    break;

    case MDEventType::timer_Cycle:
        newProperty = new CEPC_TimeCycle();
    break;

    case MDEventType::variable_ValueRead:
        newProperty = new CMDEventProperty_Config(theEventType);
    break;

    case MDEventType::combined:
        newProperty = new CEPC_Combined();
    break;

    default:
        break;
    }

    if(newProperty && sourceProperty)
    {
        newProperty->copy(sourceProperty);
    }
    return newProperty;
}

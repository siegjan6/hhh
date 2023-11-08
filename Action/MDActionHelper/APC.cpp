#include "APC.h"

// 工厂类
CAPC_Factory::CAPC_Factory()
{

}

CMDActionProperty_Config* CAPC_Factory::newConfig(MDActionType actionType, const CMDActionProperty_Config* sourceProperty)
{
    CMDActionProperty_Config* newProperty = NULL;
    MDActionType theActionType = (sourceProperty ? sourceProperty->actionID().type() : actionType);
    switch (theActionType)
    {
    case MDActionType::variable_WriteValue:
        newProperty = new CAPC_VariableWriteValue();
    break;

    case MDActionType::tts_Play:
        newProperty = new CAPC_PlayTTS();
    break;

    case MDActionType::historyGroup_Save:
        newProperty = new CMDActionProperty_Config(theActionType);
    break;

    case MDActionType::window_Open:
    case MDActionType::window_Close:
    case MDActionType::window_Move:
    case MDActionType::window_Hide:
    case MDActionType::window_RunScript:
    case MDActionType::window_ShowByPid:
    case MDActionType::window_HideByPid:
    case MDActionType::window_CloseByPid:
    {
        newProperty = new CAPC_WindowOperation();
        newProperty->actionID().setType(theActionType);
    }
    break;
    default:
        newProperty = new CMDActionProperty_Config(theActionType);
        break;
    }

    if(newProperty && sourceProperty)
    {
        newProperty->copy(sourceProperty);
    }
    return newProperty;
}

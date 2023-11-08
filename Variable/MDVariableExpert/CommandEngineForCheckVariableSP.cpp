#include "CommandEngineForCheckVariableSP.h"
#include "MDVariableExpertImp.h"
#include "APC_VariableWriteValue.h"
#include "MDActionSP.h"

CCommandEngineForCheckVariableSP::CCommandEngineForCheckVariableSP()
{
    setComment("VariableExpert.CommandEngineForCheckVariableSP");
    m_expert = NULL;
}

void CCommandEngineForCheckVariableSP::submitWriteVarWithSP(const QString &varName, QVariant varValue, CMDActionSP* sp)
{
    CAPC_VariableWriteValue* wv = new CAPC_VariableWriteValue;
    wv->actionID().setObjName(varName);
    wv->setValue(varValue);
    wv->spList()->append(new CMDActionSP(*sp));

    CMDCommand* cmd = new CMDCommand(wv);
    submitCommand(cmd);
}

void CCommandEngineForCheckVariableSP::onProcessCommand(CMDCommand* command)
{
    CAPC_VariableWriteValue* wv = (CAPC_VariableWriteValue*)command->cmdPara();

    if(m_expert)
    {
        CMDActionSP* sp = wv->spList()->find(MDActionType::variable_WriteValue);
        m_expert->writeVariableValue_AfterCheckSP(wv->actionID().objName(), wv->value(), sp);
    }

    delete wv;
}

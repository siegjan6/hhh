#include "scriptcasegroupmgr.h"

ScriptCaseGroupMgr * ScriptCaseGroupMgr::m_Inst = 0;

ScriptCaseGroupMgr * ScriptCaseGroupMgr::GetInst()
{
    if(!m_Inst)
    {
        m_Inst = new ScriptCaseGroupMgr();
        m_Inst->SetFilePostfix("js");
    }

    return m_Inst;
}

ScriptCaseGroupMgr::ScriptCaseGroupMgr()
{
}

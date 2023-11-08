#ifndef SCRIPTCASEGROUPMGR_H
#define SCRIPTCASEGROUPMGR_H
#include "CaseGroupMgr.h"

class ScriptCaseGroupMgr : public CaseGroupMgr
{
 public:
    static ScriptCaseGroupMgr * GetInst();
private:
    ScriptCaseGroupMgr();
 private:
    static ScriptCaseGroupMgr * m_Inst;
};

#endif // SCRIPTCASEGROUPMGR_H

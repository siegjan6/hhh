#ifndef MDCOMMANDQUEUEENGINEUI_H
#define MDCOMMANDQUEUEENGINEUI_H

#include "MDCommandQueueEngineUI_Global.h"

class CMDEngineMgrDialog;
class MDCOMMANDQUEUEENGINEUISHARED_EXPORT CMDCommandQueueEngineUI
{
public:
    CMDCommandQueueEngineUI();
    ~CMDCommandQueueEngineUI();
public:
    void showMgrUI(bool show);
    void release();
private:
    CMDEngineMgrDialog* m_dlg;
};

#endif // MDCOMMANDQUEUEENGINEUI_H

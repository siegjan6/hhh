#ifndef MDSWITCHFACTORYUI_H
#define MDSWITCHFACTORYUI_H

#include "MDSwitchFactoryUI_Global.h"

class CMDSwitchFactory;
class MDSWITCHFACTORYUISHARED_EXPORT CMDSwitchFactoryUI
{
public:
    CMDSwitchFactoryUI();
    ~CMDSwitchFactoryUI();
public:
    void configFactory(CMDSwitchFactory* f);
};

#endif // MDSWITCHFACTORYUI_H

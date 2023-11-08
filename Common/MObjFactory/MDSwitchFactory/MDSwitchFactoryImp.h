#ifndef MDSWITCHFACTORYIMP_H
#define MDSWITCHFACTORYIMP_H

class CMDSysLogger;
class CMDSwitchImp
{
public:
    CMDSwitchImp();
    ~CMDSwitchImp();
    bool m_isOn;
    CMDSysLogger* m_logger;
};

class CMDSwitchFactoryImp
{
public:
    CMDSwitchFactoryImp();
};

#endif // MDSWITCHFACTORYIMP_H

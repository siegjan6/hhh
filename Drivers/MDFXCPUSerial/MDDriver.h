#ifndef MDDRIVER_H
#define MDDRIVER_H
#include "BaseDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"

class CMDDriver : public CBaseDriver
{
public:
    CMDDriver();
    ~CMDDriver();
public:
    //!获取驱动ID
    QString driverID();
    //!获取驱动描述
    QString driverDescription();
};


#endif // MDDRIVER_H

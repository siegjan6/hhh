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
    QString driverID();//!获取驱动ID
    QString driverDescription();
};


#endif // MDDRIVER_H

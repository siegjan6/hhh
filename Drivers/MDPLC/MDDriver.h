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
    //驱动ID
    QString driverID();
    QString driverDescription();
};


#endif // MDDRIVER_H

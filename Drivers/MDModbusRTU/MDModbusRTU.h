#ifndef MDMODBUSRTU_H
#define MDMODBUSRTU_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDMODBUSRTU_LIBRARY)
#  define MDMODBUSRTUSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDMODBUSRTUSHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDMODBUSRTUSHARED_EXPORT  CBaseDriver* createDriverInstance()
{

    return  new CMDDriver();
}

extern "C" MDMODBUSRTUSHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDMODBUSRTUSHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDMODBUSRTUSHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDMODBUSRTU_H

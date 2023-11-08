#ifndef MDEPLC_H
#define MDEPLC_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDPLC_LIBRARY)
#  define MDEPLCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDEPLCSHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDEPLCSHARED_EXPORT  CBaseDriver* createDriverInstance()
{
    return  new CMDDriver();
}

extern "C" MDEPLCSHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDEPLCSHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{
    return  new CMDDataBlock();
}

extern "C" MDEPLCSHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDEPLC_H

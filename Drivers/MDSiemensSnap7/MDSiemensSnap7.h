#ifndef MDSIEMENSSNAP7_H
#define MDSIEMENSSNAP7_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDSIEMENSSNAP7_LIBRARY)
#  define MDSIEMENSSNAP7SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSIEMENSSNAP7SHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDSIEMENSSNAP7SHARED_EXPORT  CBaseDriver* createDriverInstance()
{

    return  new CMDDriver();
}

extern "C" MDSIEMENSSNAP7SHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDSIEMENSSNAP7SHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDSIEMENSSNAP7SHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDSIEMENSSNAP7_H

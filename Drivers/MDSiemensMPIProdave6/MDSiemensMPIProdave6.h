#ifndef MDSIEMENSMPIPRODAVE6_H
#define MDSIEMENSMPIPRODAVE6_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDSIEMENSMPIPRODAVE6_LIBRARY)
#  define MDSIEMENSMPIPRODAVE6SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSIEMENSMPIPRODAVE6SHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDSIEMENSMPIPRODAVE6SHARED_EXPORT  CBaseDriver* createDriverInstance()
{
    return  new CMDDriver();
}

extern "C" MDSIEMENSMPIPRODAVE6SHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDSIEMENSMPIPRODAVE6SHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDSIEMENSMPIPRODAVE6SHARED_EXPORT  CBaseDevice* createDeviceInstance()
{

    return  new CMDDevice();
}


#endif // MDSIEMENSMPIPRODAVE6_H

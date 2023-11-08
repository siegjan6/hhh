#ifndef MDMITSUBISHIFX3_H
#define MDMITSUBISHIFX3_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDMITSUBISHIFX3_LIBRARY)
#  define MDMITSUBISHIFX3SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDMITSUBISHIFX3SHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDMITSUBISHIFX3SHARED_EXPORT  CBaseDriver* createDriverInstance()
{

    return  new CMDDriver();
}

extern "C" MDMITSUBISHIFX3SHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDMITSUBISHIFX3SHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDMITSUBISHIFX3SHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDMITSUBISHIFX3_H

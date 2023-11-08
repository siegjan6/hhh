#ifndef MDMITSUBISHIFX2_H
#define MDMITSUBISHIFX2_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDMITSUBISHIFX2_LIBRARY)
#  define MDMITSUBISHIFX2SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDMITSUBISHIFX2SHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDMITSUBISHIFX2SHARED_EXPORT  CBaseDriver* createDriverInstance()
{

    return  new CMDDriver();
}

extern "C" MDMITSUBISHIFX2SHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDMITSUBISHIFX2SHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDMITSUBISHIFX2SHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDMITSUBISHIFX2_H

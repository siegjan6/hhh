#ifndef MDMITSUBISHIE71_H
#define MDMITSUBISHIE71_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDMITSUBISHIE71_LIBRARY)
#  define MDMITSUBISHIE71SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDMITSUBISHIE71SHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDMITSUBISHIE71SHARED_EXPORT  CBaseDriver* createDriverInstance()
{
    return  new CMDDriver();
}

extern "C" MDMITSUBISHIE71SHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDMITSUBISHIE71SHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDMITSUBISHIE71SHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDMITSUBISHIE71_H

#ifndef MDOPCUA_H
#define MDOPCUA_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDOPCUA_LIBRARY)
#  define MDOPCUASHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDOPCUASHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDOPCUASHARED_EXPORT  CBaseDriver* createDriverInstance()
{
    return  new CMDDriver();
}

extern "C" MDOPCUASHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDOPCUASHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDOPCUASHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDOPCUA_H

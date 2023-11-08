#ifndef MDMODBUSTCP_H
#define MDMODBUSTCP_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDMODBUSTCP_LIBRARY)
#  define MDMODBUSTCPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDMODBUSTCPSHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDMODBUSTCPSHARED_EXPORT  CBaseDriver* createDriverInstance()
{
    return  new CMDDriver();
}

extern "C" MDMODBUSTCPSHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDMODBUSTCPSHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDMODBUSTCPSHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDMODBUSTCP_H

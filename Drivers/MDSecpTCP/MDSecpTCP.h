#ifndef MDSECPTCP_H
#define MDSECPTCP_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDSECPTCP_LIBRARY)
#  define MDSECPTCPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSECPSHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDSECPTCPSHARED_EXPORT  CBaseDriver* createDriverInstance()
{

    return  new CMDDriver();
}

extern "C" MDSECPTCPSHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDSECPTCPSHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDSECPTCPSHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDMODBUSTCP_H

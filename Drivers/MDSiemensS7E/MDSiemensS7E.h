#ifndef MDSIEMENSS7E_H
#define MDSIEMENSS7E_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDSIEMENSS7E_LIBRARY)
#  define MDSIEMENSS7ESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSIEMENSS7ESHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDSIEMENSS7ESHARED_EXPORT  CBaseDriver* createDriverInstance()
{

    return  new CMDDriver();
}

extern "C" MDSIEMENSS7ESHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDSIEMENSS7ESHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDSIEMENSS7ESHARED_EXPORT  CBaseDevice* createDeviceInstance()
{

    return  new CMDDevice();
}


#endif // MDSIEMENSS7E_H

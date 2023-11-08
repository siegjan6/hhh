#ifndef MDOMRONHOSTLINK_H
#define MDOMRONHOSTLINK_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDOMRONFINS_LIBRARY)
#  define MDOMRONFINSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDOMRONFINSSHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDOMRONFINSSHARED_EXPORT  CBaseDriver* createDriverInstance()
{
    return  new CMDDriver();
}

extern "C" MDOMRONFINSSHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDOMRONFINSSHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDOMRONFINSSHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDOMRONFINS_H

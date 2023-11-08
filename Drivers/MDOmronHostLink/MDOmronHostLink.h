#ifndef MDOMRONHOSTLINK_H
#define MDOMRONHOSTLINK_H

#include "MDDriver.h"
#include "MDChannel.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#if defined(MDOMRONHOSTLINK_LIBRARY)
#  define MDOMRONHOSTLINKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDOMRONHOSTLINKSHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C" MDOMRONHOSTLINKSHARED_EXPORT  CBaseDriver* createDriverInstance()
{
    return  new CMDDriver();
}

extern "C" MDOMRONHOSTLINKSHARED_EXPORT  CBaseChannel* createChannelInstance()
{
    return  new CMDChannel();
}

extern "C" MDOMRONHOSTLINKSHARED_EXPORT  CBaseDataBlock* createDataBlockInstance()
{

    return  new CMDDataBlock();
}

extern "C" MDOMRONHOSTLINKSHARED_EXPORT  CBaseDevice* createDeviceInstance()
{
    return  new CMDDevice();
}


#endif // MDOMRONHOSTLINK_H

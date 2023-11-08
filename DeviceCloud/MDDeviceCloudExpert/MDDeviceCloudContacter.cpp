#include "MDDeviceCloudContacter.h"
#include <QVariant>

CMDDeviceCloudContacter::CMDDeviceCloudContacter()
{
}

CMDDeviceCloudContacter::~CMDDeviceCloudContacter()
{

}

bool CMDDeviceCloudContacter::handleDeviceData(const QString& deviceID,
                                       const QVariantList& driverList,  const QVariantList& varList,
                                        const QVariantList& alarmInfoList,
                                        const QVariantList& dataArchiveList)
{
    return false;
}

void CMDDeviceCloudContacter::handleStatusChanged(const QString& statusMsg)
{

}

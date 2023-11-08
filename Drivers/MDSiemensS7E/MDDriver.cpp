#include "MDDriver.h"

CMDDriver::CMDDriver()
{

}
CMDDriver::~CMDDriver()
{

}
QString CMDDriver::driverID()
{
    return "MDDRV.SiemensTCPIPC";
}
QString CMDDriver::driverDescription()
{
    return "Siemens TCP/IP Ethernet";
}

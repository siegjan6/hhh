#include "MDDriver.h"

CMDDriver::CMDDriver()
{

}
CMDDriver::~CMDDriver()
{

}
QString CMDDriver::driverID()
{
    return "MDDRV.OmronFINSEthernetC";
}
QString CMDDriver::driverDescription()
{
    return "Omron FINS Ethernet";
}

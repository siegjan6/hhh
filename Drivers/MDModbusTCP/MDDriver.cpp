#include "MDDriver.h"

CMDDriver::CMDDriver()
{

}
CMDDriver::~CMDDriver()
{

}
QString CMDDriver::driverID()
{
//    return "Mind.RTDB.Drivers.MDModbusTCP";
    return "MDDRV.ModBusTCPC";
}
QString CMDDriver::driverDescription()
{
    return "Modbus Tcp/Ip";
}

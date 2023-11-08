#include "MDDriver.h"

CMDDriver::CMDDriver()
{

}
CMDDriver::~CMDDriver()
{

}
QString CMDDriver::driverID()
{
    return "Mind.RTDB.Drivers.MDModbusRTU";
}
QString CMDDriver::driverDescription()
{
    return "Modbus RTU";
}

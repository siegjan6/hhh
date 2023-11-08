#include "MDDriver.h"

CMDDriver::CMDDriver()
{

}
CMDDriver::~CMDDriver()
{

}
QString CMDDriver::driverID()
{
//    return "Mind.RTDB.Drivers.MindPLC";
    return "MDDRV.MindTechPLCC";
}
QString CMDDriver::driverDescription()
{
    return "MindPLC";
}

#include <MDSwitchFactory/MDLogSwitchFactory.h>
#include <QCoreApplication>

namespace MDDBC
{
static CMDLogSwitch * pLogSwitch = 0;

void WriteInfoLog(const QString& msg)
{
    if(!pLogSwitch)
    {
        QString runPath = QCoreApplication::applicationDirPath();
        pLogSwitch = CMDLogSwitchFactory::createSwitchWithSubFileDir(runPath, "MDDBConnection", "", "");
        pLogSwitch->turnOn();
    }

    pLogSwitch->LOG_INFO(msg);
}

void WriteErrorLog(const QString& msg)
{
    if(!pLogSwitch)
    {
        QString runPath = QCoreApplication::applicationDirPath();
        pLogSwitch = CMDLogSwitchFactory::createSwitchWithSubFileDir(runPath, "DBQureyEngine", "", "");
        pLogSwitch->turnOn();
    }

    pLogSwitch->LOG_ERROR(msg);
}
}

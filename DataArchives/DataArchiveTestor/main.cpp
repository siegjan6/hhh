#include "mainwidget.h"
#include <QApplication>
#include "TestorGlobalInst.h"
#include <MDSysLogExpert/MDSysLogExpert.h>

#define LOG_SOURCE "DATestor"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CMDSysLoggerPara loggerPara;
    //loggerPara.setName("Testor");
    //loggerPara.setFileMainName("log");
    CMDSysLogExpert::init(loggerPara);

    MDLOG_INFO_TO_LOGGER("Test Log---+++***");

    TestorGlobalInst::Init();

    MainWidget w;
    w.show();
    w.DisplayReSize();

    return a.exec();
}

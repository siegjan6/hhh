#include "TestorDialog.h"
#include <QApplication>
#include <IMDDBQueryEngine.h>
#include <MDDBQueryEngineFactory.h>
#include <assert.h>

IMDDBQueryEngine * pGlobalEngine = 0;
IMDDBQueryEngine * GetGlobalDBQueryEngine()
{
    return pGlobalEngine;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pGlobalEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pGlobalEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case8");
    assert(b);
    TestorDialog w;
    w.show();

    return a.exec();
}

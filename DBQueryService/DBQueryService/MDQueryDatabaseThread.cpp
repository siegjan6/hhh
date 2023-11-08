#include "MDQueryDatabaseThread.h"
#include "DBQueryCommand.h"

CMDQueryDatabaseThread::CMDQueryDatabaseThread()
{
    setComment("databaseQuery");
}

void CMDQueryDatabaseThread::onProcessCommand(CMDCommand *command)
{
    CDBQueryCommand *pQueryCmd = reinterpret_cast<CDBQueryCommand*>(command->cmdPara());
    if (NULL == pQueryCmd || !pQueryCmd->isStarted())
    {
        return;
    }

//    MDLOG_INFO_TO_LOGGER(QString("query---exec cmd, sql: %1...").arg(pQueryCmd->getQueryCommand()));
    pQueryCmd->execQuery();

    delete pQueryCmd;
    pQueryCmd = NULL;
}

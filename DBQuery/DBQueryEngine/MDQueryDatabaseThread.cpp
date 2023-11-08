#include "MDQueryDatabaseThread.h"
#include "DBQueryCommand.h"
#include "utils.h"

namespace DBQE
{
CMDQueryDatabaseThread::CMDQueryDatabaseThread()
{
    setComment("databaseQuery");
}

void CMDQueryDatabaseThread::onProcessCommand(CMDCommand *command)
{
    CDBQueryCommand *pQueryCmd = reinterpret_cast<CDBQueryCommand*>(command->cmdPara());
    if (NULL == pQueryCmd || !pQueryCmd->isStarted())
    {
        DBQE::WriteInfoLog("CMDQueryDatabaseThread::onProcessCommand:pQueryCmd is null");
        return;
    }

    pQueryCmd->execQuery();

    delete pQueryCmd;
    pQueryCmd = NULL;
}
}

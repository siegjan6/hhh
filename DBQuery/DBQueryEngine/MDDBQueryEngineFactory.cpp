#include "MDDBQueryEngineFactory.h"
#include "DBQueryEngine.h"

CMDDBQueryEngineFactory::CMDDBQueryEngineFactory()
{

}

CMDDBQueryEngineFactory::~CMDDBQueryEngineFactory()
{
}

IMDDBQueryEngine *CMDDBQueryEngineFactory::createDBQueryEngine()
{
    IMDDBQueryEngine* pEngine = new DBQE::CMDDBQueryEngine;
    return pEngine;
}


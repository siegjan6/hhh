#include "MDDBQueryServiceFactory.h"
#include "MDDBQueryService.h"

CMDDBQueryServiceFactory::CMDDBQueryServiceFactory()
{

}

CMDDBQueryServiceFactory::~CMDDBQueryServiceFactory()
{
}

IMDDBQueryService *CMDDBQueryServiceFactory::createDBQueryService()
{
    IMDDBQueryService* pService = new CMDDBQueryService;
    return pService;
}

IMDDBQueryParam *CMDDBQueryServiceFactory::createDBQueryParam()
{
    IMDDBQueryParam *pQueryParam = new CMDDBQueryParam;
    return pQueryParam;
}

IMDConditionItem *CMDDBQueryServiceFactory::createConditionItem()
{
    IMDConditionItem *pCondItem = new CMDConditionItem;
    return pCondItem;
}

IMDCondition *CMDDBQueryServiceFactory::createCondition()
{
    IMDCondition *pCondition = new CMDCondition;
    return pCondition;
}

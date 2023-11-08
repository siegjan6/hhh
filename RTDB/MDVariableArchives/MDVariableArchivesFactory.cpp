#include "MDVariableArchivesFactory.h"
#include "MDVariableArchivesManager.h"

CMDVariableArchivesFactory::CMDVariableArchivesFactory()
{
}
CMDVariableArchivesFactory::~CMDVariableArchivesFactory()
{

}
IMDVariableArchivesManager *CMDVariableArchivesFactory::createVariableArchivesManager()
{
    CMDVariableArchivesManager *pMDVariableArchives=new CMDVariableArchivesManager();
    return (IMDVariableArchivesManager*)pMDVariableArchives;
}

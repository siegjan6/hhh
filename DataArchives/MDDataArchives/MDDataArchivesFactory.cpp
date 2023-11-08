#include "MDDataArchivesFactory.h"
#include "MDDataArchives.h"

CMDDataArchivesFactory::CMDDataArchivesFactory()
{
}

/*!
\section 静态创建数据档
*/
IMDDataArchives *CMDDataArchivesFactory::createDataArchives()
{
    IMDDataArchives *pDataArchives = new CMDDataArchives();
    return pDataArchives;
}


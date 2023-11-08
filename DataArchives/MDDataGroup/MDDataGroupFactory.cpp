#include "MDDataGroupFactory.h"
#include "MDDataGroupManager.h"
#include "MDDataGroup.h"
#include "MDDataItem.h"

CMDDataGroupFactory::CMDDataGroupFactory()
{
}

/*!
\section 静态创建数据组管理器
*/
IMDDataGroupManager *CMDDataGroupFactory::createDataGroupManager()
{
    IMDDataGroupManager *pGroupManager = new CMDDataGroupManager();
    return pGroupManager;
}

/*!
\section 静态创建数据组
*/
IMDDataGroup *CMDDataGroupFactory::createDataGroup()
{
    IMDDataGroup *pDataGroup = new CMDDataGroup();
    return pDataGroup;
}

/*!
\section 静态创建数据项
*/
IMDDataItem *CMDDataGroupFactory::createDataItem()
{
    IMDDataItem *pDataItem = new CMDDataItem();
    return pDataItem;
}

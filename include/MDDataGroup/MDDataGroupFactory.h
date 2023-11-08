#ifndef MDDATAARCHIVESFACTORY_H
#define MDDATAARCHIVESFACTORY_H

/*!

\author bHVvY2hhbw==

\section 功能
数据组工厂类，对外接口，用于创建以下相关子模块。
1.静态创建数据组管理器。
2.静态创建数据组。
3.静态创建数据项。

*/

#include "mddatagroup_global.h"

class IMDDataGroupManager;
class IMDDataGroup;
class IMDDataItem;

class MDDATAGROUPSHARED_EXPORT CMDDataGroupFactory
{
public:
    CMDDataGroupFactory();
public:
    //! 静态创建数据组管理器
    static IMDDataGroupManager *createDataGroupManager();

    //! 静态创建数据组
    static IMDDataGroup *createDataGroup();

    //! 静态创建数据项
    static IMDDataItem *createDataItem();
};


#endif // MDDATAARCHIVESFACTORY_H

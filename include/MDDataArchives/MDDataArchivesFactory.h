#ifndef MDDATAARCHIVESFACTORY_H
#define MDDATAARCHIVESFACTORY_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据归档工厂类，对外接口类。

*/

#include "MDDataArchives_global.h"

class IMDDataArchives;
class IMDDataGroup;
class IMDDataItem;

class MDDATAARCHIVESSHARED_EXPORT CMDDataArchivesFactory
{
public:
    CMDDataArchivesFactory();

/*!
\section 静态创建数据档
*/
public:
    static IMDDataArchives *createDataArchives();
};


#endif // MDDATAARCHIVESFACTORY_H

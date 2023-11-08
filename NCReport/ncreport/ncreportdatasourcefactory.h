#ifndef NCREPORTDATASOURCEFACTORY_H
#define NCREPORTDATASOURCEFACTORY_H

#include "ncreport_global.h"

class NCReportDataSource;

/*!

section 功能

数据源管理工厂类

*/
class NCREPORTSHARED_EXPORT NCReportDataSourceFactory
{
public:
    NCReportDataSourceFactory();
	virtual ~NCReportDataSourceFactory();
	virtual NCReportDataSource * createCustomDataSource( const QString& dsID ) = 0;
};

#endif // NCREPORTDATASOURCEFACTORY_H

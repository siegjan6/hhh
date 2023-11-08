#ifndef NCREPORTDUMMYDATASOURCEFACTORY_H
#define NCREPORTDUMMYDATASOURCEFACTORY_H

#include "ncreportdatasourcefactory.h"
#include "ncreport_global.h"

/*!

section 功能

数据源虚管理工厂类
【公有继承自数据源工厂类NCReportDataSourceFactory】

*/
class NCREPORTSHARED_EXPORT NCReportDummyDataSourceFactory : public NCReportDataSourceFactory
{
public:
    NCReportDummyDataSourceFactory();
	virtual NCReportDataSource * createCustomDataSource( const QString& dsID );
};

#endif // NCREPORTDUMMYDATASOURCEFACTORY_H

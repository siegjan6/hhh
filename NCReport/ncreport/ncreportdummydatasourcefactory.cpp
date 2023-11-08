#include "ncreportdummydatasourcefactory.h"

NCReportDummyDataSourceFactory::NCReportDummyDataSourceFactory()
{
}

NCReportDataSource * NCReportDummyDataSourceFactory::createCustomDataSource( const QString& )
{
	return 0;
}

#include "DataSourcesStorage.h"

namespace DBQ
{
//TODO:使用DataSource_GetDBType代替
const char * DataSourcesStorage::GetTypeString(DBSOURCE_TYPE type)
{
    switch(type)
    {
        case DBSOURCE_TYPE_MYSQL: return "mysql";
        case DBSOURCE_TYPE_SQL_SERVER: return "sqlserver";
        case DBSOURCE_TYPE_ORACLE: return "oracle";
        case DBSOURCE_TYPE_KINGBASE: return "kingbase";
        default:
            return "unknown";
    }
    return "unknown";
}
}

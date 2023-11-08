#ifndef DATASOURCESSTORAGE_H
#define DATASOURCESSTORAGE_H
#include <QString>
#include "MDDBQueryConfigure.h"

namespace DBQ
{
class DataSourcesStorage
{
public:
    DataSourcesStorage(){}
    virtual ~DataSourcesStorage(){}
    virtual bool Load(const QString & fileName, QList<DBSource *> & dbSourcesOut, QString & errOut) = 0;
    virtual bool Save(const QString & fileName, QList<DBSource *> & dbSrcsIn, QString & errStrOut) = 0;
protected:
    const char * GetTypeString(DBSOURCE_TYPE type);
};
}
#endif // DATASOURCESSTORAGE_H

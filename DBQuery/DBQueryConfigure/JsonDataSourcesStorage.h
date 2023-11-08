#ifndef JSONDATASOURCESSTORAGE_H
#define JSONDATASOURCESSTORAGE_H
#include "DataSourcesStorage.h"
#include <QByteArray>
#include <QJsonArray>

namespace DBQ
{
class JsonDataSourcesStorage : public DataSourcesStorage
{
public:
    JsonDataSourcesStorage();
    bool Load(const QString & fileName, QList<DBSource *> & dbSourcesOut, QString & errOut);
    virtual bool Save(const QString & fileName, QList<DBSource *> & dbSrcsIn, QString & errStrOut);
private:
    //bool GetFileContent(QString fullFileName, QByteArray & fileContentOut);
    bool ParseDB(QJsonArray & dbArray, DBSource * pDBSrc);
    void WriteDatabasesNode(QJsonObject & ele, QList<struct DB> & dbs);
};
}
#endif // JSONDATASOURCESSTORAGE_H

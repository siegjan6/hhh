#ifndef XMLDATASOURCESSTORAGE_H
#define XMLDATASOURCESSTORAGE_H
#include "DataSourcesStorage.h"
#include <QFile>
#include <QtXml/QDomDocument>
#include <QTextStream>

namespace DBQ
{
class XMLDataSourcesStorage : public DataSourcesStorage
{
public:
    XMLDataSourcesStorage();
    virtual bool Load(const QString & fileName, QList<DBSource *> & dbSourcesOut, QString & errOut);
    virtual bool Save(const QString & fileName, QList<DBSource *> & dbSrcsIn, QString & errStrOut);
protected:
    bool GetFileContent(const QString &  fullFileName, QString & fileContentOut);
    bool ParseDB(QDomElement & child, DBSource * pDBSrc);
    void AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText);
    void WriteDatabasesNode(QDomDocument & doc, QDomElement & dbSrcNode, QList<struct DB> & dbs);
};
}
#endif // XMLDATASOURCESSTORAGE_H

#ifndef JSONVIEWSTORAGE_H
#define JSONVIEWSTORAGE_H
#include "ViewStorage.h"
#include <QtXml/QDomDocument>
#include <QJsonArray>

namespace  DBQ
{
class JsonViewStorage : public ViewStorage
{
public:
    JsonViewStorage();
    virtual bool Load(const QString &  fileName, View ** ppViewOut, QString & errOut);
    virtual bool Save(const QString &  fileName, View * pViewIn, QString & errStrOut);
private:
    //bool GetFileContent(QString fullFileName, QByteArray & fileContentOut);
    bool ParseDBSources(QJsonArray & dbsourcesArray, View * pView);
    bool ParseFields(QJsonArray & fieldsArray, View * pView);
    bool ParseFilter(QString & filterXMLStr, View * pView);
    void WriteDBSources(QJsonArray & dbSourceArray, QList<ViewDBSource> & dbSources);
    void WriteFields(QJsonArray & fieldArray, QList<ViewField *> & fields);
    bool WriteFilter(QString & xmlFilterStrOut, ViewFilterNode * pFilter, QString & errStrOut);
    void WriteFilterTree(QDomDocument & doc, QDomElement & parentNode, ViewFilterNode * pFilter);
    void AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText);
};
}
#endif // JSONVIEWSTORAGE_H

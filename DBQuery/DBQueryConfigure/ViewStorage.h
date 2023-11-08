#ifndef VIEWSTORAGE_H
#define VIEWSTORAGE_H
#include <QString>
#include "MDDBQueryConfigure.h"
#include <QtXml/QDomDocument>

namespace DBQ
{
class ViewStorage
{
public:
    ViewStorage();
    virtual bool Load(const QString & fileName, View ** ppViewOut, QString & errOut) = 0;
    virtual bool Save(const QString &  fileName, View * pViewIn, QString & errStrOut) = 0;
protected:
    VIEW_FIELD_TYPE GetFieldTypeFromString(QString & typeStr);
    bool ParserFilterTree(QDomElement & itemNode, ViewFilterNode * pFilter);
    QString GetFiledTypeStr(VIEW_FIELD_TYPE type);
private:
    bool ParseItem(QDomElement & itemNode, ViewFilterNode * pChildNode);
};
}

#endif // VIEWSTORAGE_H

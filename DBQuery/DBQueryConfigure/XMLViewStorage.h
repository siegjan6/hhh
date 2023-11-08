#ifndef XMLVIEWSTORAGE_H
#define XMLVIEWSTORAGE_H
#include "ViewStorage.h"
#include <QtXml/QDomDocument>

namespace DBQ
{
class XMLViewStorage : public ViewStorage
{
public:
    XMLViewStorage();
    virtual bool Load(const QString &  fileName, View ** ppViewOut, QString & errOut);
    virtual bool Save(const QString &  fileName, View * pViewIn, QString & errStrOut);
private:
    bool GetFileContent(const QString &  fullFileName, QString & fileContentOut);
    bool ParseDBSources(QDomElement & root, struct View * pViewOut);
    bool ParseFields(QDomElement & root,  struct View * pViewOut);
    bool ParseFilter(QDomElement & root,  struct View * pViewOut);
    void WriteDBSources(QDomDocument & doc, QDomElement & viewNode, QList<ViewDBSource> & dbSources);
    void WriteFields(QDomDocument & doc, QDomElement & viewNode, QList<ViewField *> & fields);
    void AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText);
    bool WriteFilter(QDomDocument & doc, QDomElement & viewNode, ViewFilterNode * pFilter, QString & errStrOut);
    void WriteFilterTree(QDomDocument & doc, QDomElement & parentNode, ViewFilterNode * pFilter);
};
}
#endif // XMLVIEWSTORAGE_H

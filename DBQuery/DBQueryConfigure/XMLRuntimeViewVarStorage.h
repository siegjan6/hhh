#ifndef XMLRUNTIMEVIEWVARSTORAGE_H
#define XMLRUNTIMEVIEWVARSTORAGE_H
#include "RuntimeViewVarStorage.h"
#include <QtXml/QDomDocument>

namespace DBQ
{
class XMLRuntimeViewVarStorage : public RuntimeViewVarStorage
{
public:
    XMLRuntimeViewVarStorage();
    virtual bool Load(const QString &  fileName, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut);
    virtual bool Save(const QString &  fileName, QList<RuntimeViewVariable *> & rtVarsIn, QString & errStrOut);
protected:
    bool GetFileContent(const QString &  fullFileName, QString & fileContentOut);
    void AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText);
};
}
#endif // XMLRUNTIMEVIEWVARSTORAGE_H

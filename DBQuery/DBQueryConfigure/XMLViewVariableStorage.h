#ifndef XMLVIEWVARIABLESTORAGE_H
#define XMLVIEWVARIABLESTORAGE_H
#include "ViewVariableStorage.h"
#include <QtXml/QDomDocument>

namespace DBQ
{
class XMLViewVariableStorage : public ViewVariableStorage
{
public:
    XMLViewVariableStorage();
    virtual bool Load(const QString &  fileName, QList<ViewVariable *> & viewVarsOut, QString & errStrOut);
    virtual bool Save(const QString &  fileName, QList<ViewVariable *> & viewVarsIn, QString & errStrOut);
protected:
    bool GetFileContent(const QString &  fullFileName, QString & fileContentOut);
    bool ParseEnumValue(QDomElement & varNode, ViewVariable * pVarOut);
    void AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText);
    void WriteEnumNode(QDomDocument & doc, QDomElement & parentNode, QList<ViewVariableEnumValue> & evs);
};
}
#endif // XMLVIEWVARIABLESTORAGE_H

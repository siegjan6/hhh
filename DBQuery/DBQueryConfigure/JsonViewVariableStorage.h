#ifndef JSONVIEWVARIABLESTORAGE_H
#define JSONVIEWVARIABLESTORAGE_H
#include "ViewVariableStorage.h"
#include <QJsonObject>
namespace DBQ
{
class JsonViewVariableStorage : public ViewVariableStorage
{
public:
    JsonViewVariableStorage();
    virtual bool Load(const QString &  fileName, QList<ViewVariable *> & viewVarsOut, QString & errStrOut);
    virtual bool Save(const QString &  fileName, QList<ViewVariable *> & viewVarsIn, QString & errStrOut);
private:
    //bool GetFileContent(const QString & fullFileName, QByteArray & fileContentOut);
    //bool WriteJSonToFile(const QString & fullFileName, QJsonObject & root);
    bool ParseEnumValue(QJsonObject & varObj, ViewVariable * pVarOut);
    void WriteEnumNode(QJsonObject & ele, QList<ViewVariableEnumValue> & evs);
};
}
#endif // JSONVIEWVARIABLESTORAGE_H

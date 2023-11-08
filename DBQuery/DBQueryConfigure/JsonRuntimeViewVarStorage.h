#ifndef JSONRUNTIMEVIEWVARSTORAGE_H
#define JSONRUNTIMEVIEWVARSTORAGE_H
#include "RuntimeViewVarStorage.h"

namespace DBQ
{
class JsonRuntimeViewVarStorage : public RuntimeViewVarStorage
{
public:
    JsonRuntimeViewVarStorage();
    virtual bool Load(const QString &  fileName, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut);
    virtual bool Save(const QString &  fileName, QList<RuntimeViewVariable *> & rtVarsIn, QString & errStrOut);
};
}
#endif // JSONRUNTIMEVIEWVARSTORAGE_H

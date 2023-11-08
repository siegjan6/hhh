#ifndef RUNTIMEVIEWVARSTORAGE_H
#define RUNTIMEVIEWVARSTORAGE_H
#include "MDDBQueryConfigure.h"

namespace DBQ
{
class RuntimeViewVarStorage
{
public:
    RuntimeViewVarStorage();
    virtual bool Load(const QString &  fileName, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut) = 0;
    virtual bool Save(const QString &  fileName, QList<RuntimeViewVariable *> & rtVarsIn, QString & errStrOut) = 0;
};
}

#endif // RUNTIMEVIEWVARSTORAGE_H

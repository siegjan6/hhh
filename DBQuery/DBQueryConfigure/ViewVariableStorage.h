#ifndef VIEWVARIABLESTORAGE_H
#define VIEWVARIABLESTORAGE_H
#include <QString>
#include "MDDBQueryConfigure.h"

namespace DBQ
{
class ViewVariableStorage
{
public:
    ViewVariableStorage();
    virtual bool Load(const QString &  fileName, QList<ViewVariable *> & viewVarsOut, QString & errStrOut) = 0;
    virtual bool Save(const QString &  fileName, QList<ViewVariable *> & viewVarsIn, QString & errStrOut) = 0;
protected:
    VIEW_VARIABLE_TYPE GetTypeFromString(const QString & typeStr);
    //VIEW_VARIABLE_VALUE_TYPE GetValueTypeFromString(const QString & typeStr);
    const char * GetTypeString(VIEW_VARIABLE_TYPE type);
    //const char * GetValueTypeString(VIEW_VARIABLE_VALUE_TYPE type);
};
}
#endif // VIEWVARIABLESTORAGE_H

#include "ViewVariableStorage.h"

namespace DBQ
{
ViewVariableStorage::ViewVariableStorage()
{
}

VIEW_VARIABLE_TYPE ViewVariableStorage::GetTypeFromString(const QString & typeStr)
{
    if(typeStr.compare("const", Qt::CaseInsensitive) == 0)
        return VIEW_VARIABLE_TYPE_CONST;
    else if(typeStr.compare("enum", Qt::CaseInsensitive) == 0)
        return VIEW_VARIABLE_TYPE_ENUM;
    else if(typeStr.compare("field", Qt::CaseInsensitive) == 0)
        return VIEW_VARIABLE_TYPE_FIELD;
    else if(typeStr.compare("function", Qt::CaseInsensitive) == 0)
        return VIEW_VARIABLE_TYPE_FUNC;
    //else

    return VIEW_VARIABLE_TYPE_UNKNOWN;
}

//不要使用查表法 不安全
const char * ViewVariableStorage::GetTypeString(VIEW_VARIABLE_TYPE type)
{
    switch(type)
    {
        case VIEW_VARIABLE_TYPE_CONST: return "const";
        case VIEW_VARIABLE_TYPE_ENUM: return "enum";
        case VIEW_VARIABLE_TYPE_FIELD: return "field";
        case VIEW_VARIABLE_TYPE_FUNC: return "function";
        default:
            return "unknown";
    }

    return "unknown";
}
}

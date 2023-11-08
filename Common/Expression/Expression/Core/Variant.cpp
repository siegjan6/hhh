#include "Variant.h"

CVariant::CVariant():QVariant()
{
}

CVariant::~CVariant()
{

}

bool CVariant::inHerits(const QString &typeName)
{
    bool b = "CVariant" == typeName;
    if(b)
        return true;
    return this->typeName() == typeName;
}

QString CVariant::toString()
{
    QString str = QVariant::toString();
    if(str == "true")
        str = "1";
    else if(str == "false")
        str = "0";
    return str;
}


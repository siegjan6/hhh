#ifndef VARIANT_H
#define VARIANT_H

#include <QVariant>
#include "./Interface/IEmpty.h"
class CVariant : public QVariant,public IEmpty
{
public:
    CVariant();
    virtual ~CVariant();
    virtual bool inHerits(const QString& typeName);
    virtual QString toString();
};


#endif // VARIANT_H

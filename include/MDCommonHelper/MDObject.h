/*!

\author dGFuZ3p3

\section 功能

通用的 MindSCADA 对象类。

*/

#ifndef MDOBJECT_H
#define MDOBJECT_H

#include <QString>
#include "MDCommonConstant.h"
#include "MDCommonHelper_Global.h"

class CMDObjectData;
class MDCOMMONHELPERSHARED_EXPORT CMDObject
{
public:
    CMDObject();
    CMDObject(const QString& name, MDObjectType type);
    virtual ~CMDObject();
    CMDObject(const CMDObject& obj);
    CMDObject& operator = (const CMDObject& obj);
public:
    virtual bool copy(const CMDObject* source);
public:
    QString name() const;
    void setName(const QString& name);

    MDObjectType type() const;
    void setType(MDObjectType type);

    QString comment() const;
    void setComment(const QString& comment);
protected:
    CMDObjectData* m_data;
};

#endif // MDOBJECT_H

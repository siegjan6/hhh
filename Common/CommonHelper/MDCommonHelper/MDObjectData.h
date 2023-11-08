/*!

\author dGFuZ3p3

\section 功能

通用的 MindSCADA 对象类的实现类。

*/

#ifndef MDOBJECTDATA_H
#define MDOBJECTDATA_H

#include <QString>
#include "../../../include/MDCommonHelper/MDCommonConstant.h"

class CMDObjectData
{
public:
    CMDObjectData();
    CMDObjectData(const QString& name, MDObjectType type);
    virtual ~CMDObjectData();
   CMDObjectData(const CMDObjectData& obj);
   CMDObjectData& operator = (const CMDObjectData& obj);
public:
    bool copy(const CMDObjectData* source);
public:
    QString m_name;
    MDObjectType m_objectType;// = MDObjectType::null_obj;
    QString m_comment;
};

#endif // MDOBJECTDATA_H

#include "MDObjectData.h"

CMDObjectData::CMDObjectData()
{
    m_objectType = MDObjectType::null_obj;
}

CMDObjectData::CMDObjectData(const QString& name, MDObjectType type)
{
    m_name = name;
    m_objectType = type;
}

CMDObjectData::~CMDObjectData()
{

}

CMDObjectData::CMDObjectData(const CMDObjectData& obj)
{
    copy(&obj);
}

CMDObjectData& CMDObjectData::operator = (const CMDObjectData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDObjectData::copy(const CMDObjectData* source)
{
    m_name = source->m_name;
    m_comment = source->m_comment;
    m_objectType = source->m_objectType;

    return true;
}

#include "../../../include/MDCommonHelper/MDObject.h"
#include "MDObjectData.h"

CMDObject::CMDObject()
{
    m_data = new CMDObjectData;
}

CMDObject::CMDObject(const QString& name, MDObjectType type)
{
    m_data = new CMDObjectData(name, type);
}

CMDObject::~CMDObject()
{
    if(m_data)
        delete m_data;
}

CMDObject::CMDObject(const CMDObject& obj)
{
    m_data = new CMDObjectData(obj.name(), obj.type());

    copy(&obj);
}

CMDObject& CMDObject::operator = (const CMDObject& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDObject::copy(const CMDObject* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

QString CMDObject::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDObject::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

MDObjectType CMDObject::type() const
{
    return (m_data ? m_data->m_objectType : MDObjectType::null_obj);
}
void CMDObject::setType(MDObjectType type)
{
    if(m_data)
        m_data->m_objectType = type;
}

QString CMDObject::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDObject::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

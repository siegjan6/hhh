#include "MDMObjImp.h"

CMDMObjImp::CMDMObjImp()
{
    m_factory = NULL;
}

CMDMObjImp::CMDMObjImp(const CMDMObjPara& para, CMDMObjFactory* factory)
{
    m_factory = factory;
    m_para = para;
}

CMDMObjImp::~CMDMObjImp()
{
}

CMDMObjParaImp::CMDMObjParaImp()
{
    m_ptr = NULL;
}

CMDMObjParaImp::CMDMObjParaImp(const QString& pid, const QString& type, const QString& group, const QString& comment)
{
    m_group = group;
    m_comment = comment;
    m_pid = pid;
    m_type = type;
    m_ptr = NULL;
}

CMDMObjParaImp::CMDMObjParaImp(const CMDMObjParaImp& obj)
{
    copy(&obj);
}

CMDMObjParaImp& CMDMObjParaImp::operator = (const CMDMObjParaImp& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDMObjParaImp::copy(const CMDMObjParaImp* source)
{
    m_group = source->m_group;
    m_comment = source->m_comment;
    m_pid = source->m_pid;
    m_type = source->m_type;
    m_name = source->m_name;
    m_ptr = source->m_ptr;

    return true;
}

CMDMObjParaImp::~CMDMObjParaImp()
{

}

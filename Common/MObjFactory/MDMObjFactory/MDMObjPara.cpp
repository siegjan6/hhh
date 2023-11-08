#include "MDMObjPara.h"
#include "MDMObjImp.h"

CMDMObjPara::CMDMObjPara()
{
    m_data = new CMDMObjParaImp;
}

CMDMObjPara::CMDMObjPara(const QString& pid, const QString& type)
{
    m_data = new CMDMObjParaImp(pid, type, "", "");
}

CMDMObjPara::CMDMObjPara(const QString& pid, const QString& type, const QString& group, const QString& comment)
{
    m_data = new CMDMObjParaImp(pid, type, group, comment);
}

CMDMObjPara::CMDMObjPara(const CMDMObjPara& obj)
{
    m_data = new CMDMObjParaImp(*obj.m_data);
}

CMDMObjPara& CMDMObjPara::operator = (const CMDMObjPara& obj)
{
    if( this != &obj )
    {
        *m_data = *obj.m_data;
    }
    return *this;
}

CMDMObjPara::~CMDMObjPara()
{
    if(m_data)
        delete m_data;
}

QString CMDMObjPara::group() const
{
    return m_data->m_group;
}
void CMDMObjPara::setGroup(const QString& group)
{
    m_data->m_group = group;
}

QString CMDMObjPara::comment() const
{
    return m_data->m_comment;
}
void CMDMObjPara::setComment(const QString& comment)
{
    m_data->m_comment = comment;
}

QString CMDMObjPara::pid() const
{
    return m_data->m_pid;
}
void CMDMObjPara::setPid(const QString& pid)
{
    m_data->m_pid = pid;
}

QString CMDMObjPara::pidToPath(const QString& pid)
{
    QString pathSymbol1;
    QString pathSymbol2;
    #ifdef Q_OS_LINUX
        pathSymbol1 = "/";
        pathSymbol2 = "\\";
    #else
        pathSymbol1 = "\\";
        pathSymbol2 = "/";
    #endif

    QString pPath;
    if(pid.right(1) == pathSymbol1 ||
        pid.right(1) == pathSymbol2)
    {
        pPath = pid;
    }
    else
    {
        QString symbol = pathSymbol1;
        int index = pid.lastIndexOf(pathSymbol1);
        if(-1 == index)
             index = pid.lastIndexOf(pathSymbol2);
        if(index != -1)
            symbol = pid.mid(index, 1);
        pPath = pid + symbol;
    }
    return pPath;
}

QString CMDMObjPara::pidToPath() const
{
    return pidToPath(pid());
}

QString CMDMObjPara::type() const
{
    return m_data->m_type;
}
void CMDMObjPara::setType(const QString& type)
{
    m_data->m_type = type;
}

QString CMDMObjPara::name_from_pid_type() const
{
    return pidToPath() + ":" + type();
}

QString CMDMObjPara::name() const
{
    return m_data->m_name;
}
void CMDMObjPara::setName(const QString& name)
{
    m_data->m_name = name;
}

void* CMDMObjPara::ptr() const
{
    return m_data->m_ptr;
}
void CMDMObjPara::setPtr(void* ptr)
{
    m_data->m_ptr = ptr;
}

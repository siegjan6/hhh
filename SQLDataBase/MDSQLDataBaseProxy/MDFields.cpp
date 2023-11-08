#include "MDFields.h"

CMDField::CMDField()
{
    m_name ="";
}

CMDField::~CMDField()
{

}

QString CMDField::name()
{
    return m_name;
}
void CMDField::setName(const QString &name)
{
    m_name = name;
}
//------------------------------------------------------------------------------


CMDFields::CMDFields()
{
}

CMDFields::~CMDFields()
{

}
int CMDFields::size()
{
    return m_fieldList.count();
}
bool CMDFields::addFiled(const QString &name)
{
    if(name.isEmpty()||name.isNull())
    {
        return false;
    }
    if(getFiledIndex(name)<=0)
    {
        CMDField *field = new CMDField();
        field->setName(name);
        m_fieldList.append(field);
        return true;
    }
    return false;
}
void CMDFields::deleteFiled(const QString &name)
{
    int index = getFiledIndex(name);
    if(index>=0)
    {
        CMDField *field =m_fieldList[index];
        m_fieldList.removeAt(index);
        delete field;
    }
}
void CMDFields::deleteAllFiled()
{
    for(int i=0;i<m_fieldList.count();i++)
    {
        delete m_fieldList[i];
    }
    m_fieldList.clear();
}
int CMDFields::getFiledIndex(const QString &name)
{
    for(int i=0;i<m_fieldList.count();i++)
    {
        if(0==QString::compare(m_fieldList[i]->name(), name,Qt::CaseInsensitive))
        {
            return  i;
        }
    }
    return -1;
}
QString CMDFields::getFiledName(int index)
{
    if(index>=0 && index<m_fieldList.count())
    {
        return m_fieldList[index]->name();
    }
    return "";
}

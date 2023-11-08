#include "MDRowData.h"

CMDRowData::CMDRowData()
{
    m_size = 0;
    m_data = NULL;
}

CMDRowData::~CMDRowData()
{
   if(m_data != NULL)
   {
       delete [] m_data;
   }
}
CMDRowData::CMDRowData(int size)
{
    m_size = size;
    if(m_size>0)
    {
        m_data = new QVariant[m_size];
    }
}
void CMDRowData::setSize(int size)
{
    m_size = size;
    if(m_size>0)
    {
        m_data = new QVariant[m_size];
    }
}
int  CMDRowData::size()
{
    return m_size;
}
//赋值，判断索引的有效性
void CMDRowData::setValue(int index,const QVariant & data)
{
    if(index>=0 && index<m_size)
    {
        m_data[index] = data;
    }
}
//赋值，不判断索引的有效性
void CMDRowData::setValueEx(int index,const QVariant & data)
{
   m_data[index] = data;
}
//取值，判断索引的有效性
QVariant CMDRowData::value(int index)
{
    if(index>=0 && index<m_size)
    {
        return m_data[index];
    }
    return QVariant();
}
//取值，不判断索引的有效性
QVariant CMDRowData::valueEx(int index)
{
    return m_data[index];
}

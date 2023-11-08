#ifndef CMDROWDATA_H
#define CMDROWDATA_H

#include <QVariant>
#include<QList>
class CMDRowData
{
public:
    CMDRowData();
    CMDRowData(int size);
    ~CMDRowData();
public:
    void setSize(int size);
    int  size();
    //赋值，判断索引的有效性
    void setValue(int index,const QVariant & data);
    //赋值，不判断索引的有效性
    void setValueEx(int index,const QVariant & data);
    //取值，判断索引的有效性
    QVariant value(int index);
    //取值，不判断索引的有效性
    QVariant valueEx(int index);
private:
    QVariant *m_data;
    int m_size;
};

#endif // CMDROWDATA_H

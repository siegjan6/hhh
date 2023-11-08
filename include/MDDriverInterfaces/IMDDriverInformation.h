/*!
\author zyb 2015.01.08
\section 驱动信息描述接口类，bin/dirvers/
        \<Driver>
        \    <Name>Modbus TCP</Name>
        \    <ID>NetSCADA6.RTDB.Drivers.MDModbusTCP</ID>
        \    <Description>Modbus TCP</Description>
        \</Driver>
*/
#ifndef IMDDRIVERINFORMATION_H
#define IMDDRIVERINFORMATION_H
#include <QString>
class IMDDriverInformation
{
public:
    virtual ~IMDDriverInformation(){}
public:
    /*!
    \section 驱动属性
    */
    //!ID
    virtual QString id()=0;
    virtual void setID(QString id)=0;
    //!name
    virtual QString name()=0;
    virtual void setName(QString name)=0;
    //!Description
    virtual QString description()=0;
    virtual void setDescription(QString description)=0;
};


#endif // IMDDRIVERINFORMATION_H


#include "MDWriteVariable.h"

CMDWriteVariable::CMDWriteVariable(IMDDriverVariable *nsVar)
{
    //m_name = nsVar.getName();
    //m_curValue = nsVar.getCurValue();
    m_deviceName= nsVar->getDeviceName();
    m_address=nsVar->getAddress();
    m_dataBlockIndexID=nsVar->getDataBlockIndexID();
    m_dataType=nsVar->getDataType();
    m_originalValue=nsVar->getOriginalValue();
    m_quality=nsVar->getQuality();
    m_timeStamp=nsVar->getTimeStamp();
    m_internalAddress=nsVar->getInternalAddress();
    m_internalSubAddress=nsVar->getInternalSubAddress();

}
CMDWriteVariable::~CMDWriteVariable()
{

}

//名称
QString CMDWriteVariable::getName()
{
    return m_name;
}

void CMDWriteVariable::setName(QString name)
{
    m_name = name;
}

//地址
QString CMDWriteVariable::getAddress()
{
    return m_address;
}
void CMDWriteVariable::setAddress(QString address)
{
    m_address = address;
}

//品质
MDVariableDataQuality CMDWriteVariable::getQuality()
{
     return m_quality;
}
void CMDWriteVariable::setQuality(MDVariableDataQuality quality)
{
    m_quality = quality;
}

//设备名称
QString CMDWriteVariable::getDeviceName()
{
     return m_deviceName;
}
void CMDWriteVariable::setDeviceName(QString deviceName)
{
    m_deviceName= deviceName;
}

//时间戳
QDateTime CMDWriteVariable::getTimeStamp()
{
     return m_timeStamp;
}
void CMDWriteVariable::setTimeStamp(QDateTime timeStamp)
{
    m_timeStamp = timeStamp;
}

//原始值
QVariant CMDWriteVariable::getOriginalValue()
{
     return m_originalValue;
}
void CMDWriteVariable::setOriginalValue(QVariant value)
{
    m_originalValue = value;
}

//数据类型
MDVariableDataType CMDWriteVariable::getDataType()
{
     return m_dataType;
}
void CMDWriteVariable::setDataType(MDVariableDataType dataType)
{
    m_dataType = dataType;
}

//关联数据块编号
int CMDWriteVariable::getDataBlockIndexID()
{
     return m_dataBlockIndexID;
}
void CMDWriteVariable::setDataBlockIndexID(int dataBlockIndexID)
{
    m_dataBlockIndexID = dataBlockIndexID;
}

//关联数据区地址
int CMDWriteVariable::getInternalAddress()
{
     return m_internalAddress;
}
void CMDWriteVariable::setInternalAddress(int internalAddress)
{
    m_internalAddress = internalAddress;
}

//关联数据区子地址
int CMDWriteVariable::getInternalSubAddress()
{
     return m_internalSubAddress;
}
void CMDWriteVariable::setInternalSubAddress(int internalSubAddress)
{
    m_internalSubAddress = internalSubAddress;
}

//更新数据
void CMDWriteVariable::refreshData(QVariant dataValue,QDateTime TimeStamp,MDVariableDataQuality quality,bool OnlyChangedQuality)
{

}

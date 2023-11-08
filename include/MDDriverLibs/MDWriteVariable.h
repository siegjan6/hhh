#ifndef MDWRITEVARIABLE_H
#define MDWRITEVARIABLE_H


#include "MDDriverLibs_global.h"
#include "IMDRTDBVariable.h"

class CMDWriteVariable : public IMDRTDBVariable
{
public:
    CMDWriteVariable(IMDRTDBVariable *nsVar);
    ~CMDWriteVariable();
public:
    //名称
    QString getName();
    void setName(QString name);

    //地址
    QString getAddress();
    void setAddress(QString address);

    //品质
    MDVariableDataQuality  getQuality();
    void setQuality(MDVariableDataQuality quality);

    //设备名称
    QString getDeviceName();
    void setDeviceName(QString deviceName);

    //时间戳
    QDateTime getTimeStamp();
    void setTimeStamp(QDateTime timeStamp);

    //原始值
    QVariant getOriginalValue();
    void setOriginalValue(QVariant value);

    //数据类型
    MDDataType getDataType();
    void setDataType(MDDataType dataType);

    //关联数据块编号
    int getDataBlockIndexID();
    void setDataBlockIndexID(int dataBlockIndexID);

    //关联数据区地址
    int getInternalAddress();
    void setInternalAddress(int internalAddress);

    //关联数据区子地址
    int getInternalSubAddress();
    void setInternalSubAddress(int internalSubAddress);

    //更新数据
    bool refreshData(QVariant dataValue,QDateTime TimeStamp,MDVariableDataQuality quality,bool OnlyChangedQuality=false);

private:
    QString m_name;
    QVariant m_curValue;
    QVariant m_originalValue;

    QString m_address;
    QString m_deviceName;
    int m_dataBlockIndexID;
    int m_internalAddress;
    int m_internalSubAddress;

    MDDataType m_dataType;
    MDVariableDataQuality m_quality;
    QDateTime m_timeStamp;
};

#endif // MDWRITEVARIABLE_H

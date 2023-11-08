#ifndef MDDRIVERPROPERTY_H
#define MDDRIVERPROPERTY_H

#include <QList>
#include <QString>
#include <QObject>
#include "IDevice.h"
#include "IMDRTDBVariable.h"
/*
class CMDDriverProperty
{
    QString m_name;
    QString m_description;
    QString m_dataType;
    QList<QString> m_enumValues;
    QVariant m_maxValue;
    QVariant m_minValue;
    QVariant m_defaultValue;
    QVariant m_value;
    struct MDDriverProperty *next;
};
enum class MDDriverPropertyType
{
    String = 0,
    BOOL = 1,
    DWORD = 2,
    Enum = 3,
    StringList = 4,
};

class  IMDDriverProperty
{
 public:
    virtual ~IMDDriverProperty(){}
public:
    virtual QString name()=0;
    virtual QString description()=0;


    virtual void addDevices(QList<IDevice*> devList)=0;
    virtual void removeAllDevices()=0;
    virtual void startDevices(QList<QString> devList)=0;
    virtual void startAllDevices()=0;
    virtual void stopDevices(QList<QString> devList)=0;
    virtual void stopAllDevices()=0;
    virtual bool checkValidVarAddress(IDevice *device,QString varAddress,MDDataType dataType,string resgister,QString StartAddr,QString endAddr,QString addressList)=0;
    virtual void monitorDataBlock(bool bEnable,int dataBlockID,QString devName,QString dbName,int nFlag)=0;
    virtual bool asyncWriteVariables(QList<IMDRTDBVariable*> nsVarList)=0;
    virtual bool asyncWriteVariable(IMDRTDBVariable *nsVar,QVariant data)=0;
};
*/

#endif // MDDRIVERPROPERTY_H

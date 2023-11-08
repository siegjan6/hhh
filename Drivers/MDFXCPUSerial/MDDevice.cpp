#include "MDDevice.h"
#include "MDDataBlock.h"
#include "MDDeviceConfigDialog.h"
#include "MDDataBlockDialog.h"

CMDDevice::CMDDevice()
{
    m_commParameter = "Com1,9600,7,2,2";
    setCommType(CommunicationType::Serial);
    setReplyTimeout(1000);
    setRetries(3);
    setEnabled(true);
    m_PLCType = 3;
}
CMDDevice::~CMDDevice()
{
}

void CMDDevice::copy(IDevice *otherDevice)
{
    if(NULL== otherDevice)
    {
        return;
    }
    CBaseDevice::copy(otherDevice);
    m_commType = ((CMDDevice*)otherDevice)->commType ();
    m_commParameter = ((CMDDevice*)otherDevice)->commParameter();
    m_PLCType = ((CMDDevice*)otherDevice)->PLCType();
 }

void CMDDevice::configDecice(void *parent)
{
    CMDDeviceConfigDialog dlg((QWidget *)parent);
    dlg.setDevice(this);
    dlg.exec();
}

IDataBlock *CMDDevice::createDataBlock(void *parent,bool config)
{
    CMDDataBlock *dataBlock = new CMDDataBlock();
    dataBlock->setParent(this,MDTreeItemType::Device);
    if(config)
    {
        CMDDataBlockDialog  dlg((QWidget *)parent);
        dlg.setDataBlock(dataBlock);
        dlg.exec();
        if(QDialog::Accepted == dlg.result())
        {
            addDataBlock(dataBlock);
        }
        else
        {
            delete dataBlock;
            dataBlock = NULL;
        }
    }
    return dataBlock;
}
bool CMDDevice::configDataBlock(IDataBlock *dataBlock,void *parent)
{
    CMDDataBlockDialog dlg((QWidget *)parent);
    dlg.setDataBlock((CMDDataBlock*)dataBlock);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        return true;
    }
    return false;
}
qint16 CMDDevice::PLCType()
{
    return m_PLCType;
}

void CMDDevice::setPLCType(qint16 PLCType)
{
    m_PLCType = PLCType;
}
//通信类型
CommunicationType CMDDevice::commType()
{
    return m_commType;
}

void CMDDevice::setCommType(CommunicationType commType)
{
    m_commType = commType;
}

//通信参数
QString CMDDevice::commParameter()
{
    return m_commParameter;
}

void CMDDevice::setCommParameter(const QString &commParameter)
{
    m_commParameter = commParameter;
}
//添加变量
bool CMDDevice::addVariable(IMDRTDBVariable* nsVar,QString &error)
{
    if(nsVar==NULL)
    {
        return false;
    }
    QString address = nsVar->address();

    int pos = address.indexOf(".");
    if(pos<=0)
    {
         return false;
    }
    QString dbName = address.left(pos);
    IDataBlock * db = getDataBlock(dbName);
    if(db==NULL)
    {
        return false;
    }
    return db->addVariable(nsVar,error);
}

//移除变量
bool CMDDevice::removeVariable(IMDRTDBVariable* nsVar)
{
    if(nsVar==NULL)
    {
        return false;
    }
    QString address = nsVar->address();

    int pos = address.indexOf(".");
    if(pos<=0)
    {
         return false;
    }
    QString dbName = address.left(pos);
    IDataBlock * db = getDataBlock(dbName);
    if(db==NULL)
    {
        return false;
    }
    return db->removeVariable(nsVar);
}
//验证变量地址
bool CMDDevice::checkVarAddress(IMDRTDBVariable* nsVar,QString &error)
{
    if(nsVar==NULL)
    {
        return false;
    }
    QString address = nsVar->address();

    int pos = address.indexOf(".");
    if(pos<=0)
    {
         return false;
    }
    QString dbName = address.left(pos);
    IDataBlock * db = getDataBlock(dbName);
    if(db==NULL)
    {
        return false;
    }
    return db->checkVarAddress(nsVar,error);
}

void CMDDevice::getVariableAddressList(QStringList &varAddressList)
{
        for (int index =0;index< this->getDataBlockCount();index++)
        {
            CMDDataBlock* dataBlock = (CMDDataBlock*)this->getDataBlockAt(index);
            dataBlock->getVariableAddressList(varAddressList);
        }
}

void CMDDevice::saveConfiguration(QDomDocument &doc,QDomElement &parentElement)
{
    if(doc.isNull()||parentElement.isNull())
    {
        return;
    }
    QDomElement deviceElement = doc.createElement("Device");
    QDomAttr driverIDDomAttr = doc.createAttribute("DriverID");
    driverIDDomAttr.setValue(this->driver()->driverID());
    deviceElement.setAttributeNode(driverIDDomAttr);
    parentElement.appendChild(deviceElement);


    QDomElement element = doc.createElement("Name");
    deviceElement.appendChild(element);
    QDomText elementText = doc.createTextNode(this->name());
    element.appendChild(elementText);

    element = doc.createElement("Description");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(this->description());
    element.appendChild(elementText);

    element = doc.createElement("Enabled");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(this->isEnabled()?"1":"0");
    element.appendChild(elementText);

    element = doc.createElement("CommType");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(int(this->commType())));
    element.appendChild(elementText);


    element = doc.createElement("CommParameter");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(this->commParameter());
    element.appendChild(elementText);

    element = doc.createElement("ReplyTimeout");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->replyTimeout()));
    element.appendChild(elementText);

    element = doc.createElement("Retries");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->retries()));
    element.appendChild(elementText);

    element = doc.createElement("PLCType");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->PLCType()));
    element.appendChild(elementText);

    if(this->getDataBlockCount()>0)
    {
        QDomElement dataBlocksElement = doc.createElement("DataBlocks");
        deviceElement.appendChild(dataBlocksElement);
        for (int index =0;index< this->getDataBlockCount();index++)
        {
            CMDDataBlock* dataBlock = (CMDDataBlock*)this->getDataBlockAt(index);
            dataBlock->saveConfiguration(doc,dataBlocksElement);
        }
    }
}

void CMDDevice::readConfiguration(QDomElement &domElement)
{
    if(domElement.isNull())
    {
        return;
    }

    QDomElement element = domElement.firstChildElement();
    while ( !element.isNull() )
    {
        if(0==QString::compare(element.tagName(),"Name", Qt::CaseInsensitive))
        {
            this->setName(element.text());
        }
        else if(0==QString::compare(element.tagName(),"Description", Qt::CaseInsensitive))
        {
            this->setDescription(element.text());
        }
        else if(0==QString::compare(element.tagName(),"Enabled", Qt::CaseInsensitive))
        {
            if(element.text().toInt()!=0)
            {
                this->setEnabled(true);
            }
            else
            {
                this->setEnabled(false);
            }
        }
        else if(0==QString::compare(element.tagName(),"CommType", Qt::CaseInsensitive))
        {
            this->setCommType((CommunicationType)element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"CommParameter", Qt::CaseInsensitive))
        {
            this->setCommParameter(element.text());
        }
        else if(0==QString::compare(element.tagName(),"ReplyTimeout", Qt::CaseInsensitive))
        {
            this->setReplyTimeout(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"Retries", Qt::CaseInsensitive))
        {
            this->setRetries(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"PLCType", Qt::CaseInsensitive))
        {
            this->setPLCType(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"DataBlocks", Qt::CaseInsensitive))
        {
            QDomElement dataBlockElement = element.firstChildElement();
            while ( !dataBlockElement.isNull() )
            {
                CMDDataBlock *dataBlock = new CMDDataBlock();
                dataBlock->setParent(this,MDTreeItemType::Device);
                dataBlock->readConfiguration(dataBlockElement);
                if(false == addDataBlock(dataBlock))
                {
                    delete dataBlock;
                    dataBlock = NULL;
                }
                dataBlockElement = dataBlockElement.nextSiblingElement();
            }
        }
        element = element.nextSiblingElement();
    }
}

void CMDDevice::dynamicSetParameter(const QVariantMap &deviceParameterMap)
{
    QString deviceID =deviceParameterMap["@DriverDeviceID"].toString();

    deviceID.trimmed();
    if(deviceID.isNull()||deviceID.isEmpty())
    {
        return;
    }
    this->setName(deviceID);

    QVariantList commParameterList =  deviceParameterMap["MDDevicesCommunicateParameter"].toList();
    for(int i=0;i<commParameterList.count();i++)
    {
        QVariantMap commParameterMap = commParameterList[i].toMap();
        QString parameterName =commParameterMap["@ParameterName"].toString();
        QString parameterValue =commParameterMap["@ParameterValue"].toString();
        parameterName.trimmed();
        parameterValue.trimmed();

        if(0==QString::compare(parameterName,"Description", Qt::CaseInsensitive))
        {
            this->setDescription(parameterValue);
        }
        else if(0==QString::compare(parameterName,"Enabled", Qt::CaseInsensitive))
        {
            if(parameterValue.toInt()!=0)
            {
                this->setEnabled(true);
            }
            else
            {
                if(0==QString::compare(parameterValue,"true", Qt::CaseInsensitive))
                {
                    this->setEnabled(true);
                }
                else
                {
                    this->setEnabled(false);
                }
            }
        }
        else if(0==QString::compare(parameterName,"CommType", Qt::CaseInsensitive))
        {
            this->setCommType((CommunicationType)parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"CommParameter", Qt::CaseInsensitive))
        {
            this->setCommParameter(parameterValue);
        }
        else if(0==QString::compare(parameterName,"ReplyTimeout", Qt::CaseInsensitive))
        {
            this->setReplyTimeout(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"PLCType", Qt::CaseInsensitive))
        {
            this->setPLCType(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"Retries", Qt::CaseInsensitive))
        {
            this->setRetries(parameterValue.toInt());
        }
    }

    QVariantList dataBlockList=  deviceParameterMap["MDDevicesCommunicateDataBlock"].toList();
    for(int i=0;i<dataBlockList.count();i++)
    {
        QVariantMap dataBlockMap = dataBlockList[i].toMap();
        CMDDataBlock *dataBlock = new CMDDataBlock();
        dataBlock->setParent(this,MDTreeItemType::Device);
        dataBlock->dynamicSetParameter(dataBlockMap);
        if(false == addDataBlock(dataBlock))
        {
            delete dataBlock;
            dataBlock = NULL;
        }
    }
}

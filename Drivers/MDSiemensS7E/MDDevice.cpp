#include "MDDevice.h"
#include "MDDataBlock.h"
#include "MDDeviceConfigDialog.h"
#include "MDDataBlockDialog.h"
#include "MDProtocolCommand.h"
#include "PropertyBrowser.h"

#ifdef Q_OS_WIN
    //#pragma execution_character_set("utf-8")   //解决 VS编译器下中文乱码
#endif

CMDDevice::CMDDevice()
{
    m_station = 0;
    m_nPLCComInitialStatus = -1;
    m_PLCType = 0;
    m_LocalTSAP = "10.11";
    m_RemoteTSAP = "3.01";
    m_commType = CommunicationType::TCP;
    m_commParameter = "Client,127.0.0.1,102,,";
    setReplyTimeout(1000);
    setRetries(3);
    setEnabled(true);
    //************************属性框变量-通信参数***********************//
    this->initTranslateNameAndPropertyType();
    m_communicationModel = (MDCommunicationType_Pro)m_commType;
    //!串口
    m_serialPort = "COM0";
    m_serialBaud = MDSerialBaud_Pro::Baud_115200;
    m_serialDataBit = MDSerialDataBit_Pro::DataBit_5;
    m_serialStopBit = MDSerialStopBit_Pro::StopBit_1;
    m_serialParityBit = MDSerialParityBit_Pro::NONE;
    //!以太
    m_clientOrServer = MDClientOrServer_Pro::Client;
    m_tcpOrUdpIP = "127.0.0.1";
    m_tcpOrUdpLocalPort = "";
    m_tcpOrUdpRemotePort = "102";
    //!USB
    m_usbParameter = "";
    //!板卡
    m_boardParameter = "";
    //!其他
    m_otherParameter = "127.0.0.1";
    //!初始隐藏标志位
    m_bHide = 0;
    //***************************************************************//
    //************************属性框变量-设备参数***********************//
    m_commProtocol_Pro = "SiemensSnap7";
    m_replyTimeout_Pro = "1000";
    m_retries_Pro = "3";
    m_plcType_Pro = MDPLCType_Pro::S7_1200;
    m_cpuRack_Pro = "0";
    m_cpuSlot_Pro = "2";
    m_localTSAP_Pro = "10.11";
    m_remoteTSAP_Pro = "3.01";
    //***************************************************************//
}
CMDDevice::~CMDDevice()
{
}
//***************PKX*************//
bool CMDDevice::start()
{
    m_nPLCComInitialStatus = -1;//PKX
    return CBaseDevice::start();
}
bool CMDDevice::stop()
{
    m_nPLCComInitialStatus = -1;//PKX
    return CBaseDevice::stop();
}
//*******************************//
void CMDDevice::copy(IDevice*otherDevice)
{
    CBaseDevice::copy(otherDevice);
    this->setReplyTimeout_Pro(QString::number(((CMDDevice*)otherDevice)->replyTimeout(),10));
    this->setRetries_Pro(QString::number(((CMDDevice*)otherDevice)->retries(),10));
    this->setPLCType(((CMDDevice*)otherDevice)->PLCType());
    this->setLocalTSAP(((CMDDevice*)otherDevice)->LocalTSAP());
    this->setRemoteTSAP(((CMDDevice*)otherDevice)->RemoteTSAP());

    this->setCommType(((CMDDevice*)otherDevice)->commType());
    this->setCommParameter(((CMDDevice*)otherDevice)->commParameter());

}
//*************************PKX************************//
void CMDDevice::getVariableAddressList(QStringList &varAddressList)
{
    varAddressList.append("CommStatus");//!状态显示变量
    if(this->getDataBlockCount()>0)
    {
        for (int index =0;index< this->getDataBlockCount();index++)
        {
            CMDDataBlock* dataBlock = (CMDDataBlock*)this->getDataBlockAt(index);
            dataBlock->variablesAddressReflect(varAddressList);
        }
    }

}
//*******************************************************//
qint16 CMDDevice::station()
{
    return m_station;
}

void CMDDevice::setStation(qint16 station)
{
    m_station = station;
}
//***********************PKX**********************//
void CMDDevice::setLocalTSAP(QString LocalTSAP)
{
    m_LocalTSAP = LocalTSAP;
    if(m_plcType_Pro == MDPLCType_Pro::S7_300 || m_plcType_Pro == MDPLCType_Pro::S7_400)
    {
        m_cpuRack_Pro = m_LocalTSAP;
    }
    else if(m_plcType_Pro == MDPLCType_Pro::S7_1200 || m_plcType_Pro == MDPLCType_Pro::S7_1500)
    {
        m_localTSAP_Pro = m_LocalTSAP;
    }
}
QString CMDDevice::LocalTSAP()
{
    return m_LocalTSAP;
}
void CMDDevice::setRemoteTSAP(QString RemoteTSAP)
{
    m_RemoteTSAP = RemoteTSAP;
    if(m_plcType_Pro == MDPLCType_Pro::S7_300 || m_plcType_Pro == MDPLCType_Pro::S7_400)
    {
        m_cpuSlot_Pro = m_RemoteTSAP;
    }
    else if(m_plcType_Pro == MDPLCType_Pro::S7_1200 || m_plcType_Pro == MDPLCType_Pro::S7_1500)
    {
        m_remoteTSAP_Pro = m_RemoteTSAP;
    }
}
QString CMDDevice::RemoteTSAP()
{
    return m_RemoteTSAP;
}
void CMDDevice::setPLCType(qint16 PLCType)
{
    m_PLCType = PLCType;
    m_plcType_Pro = (MDPLCType_Pro)m_PLCType;
}
qint16 CMDDevice::PLCType()
{
    return m_PLCType;
}
//**************************************************//
void CMDDevice::configDecice(void *parent)
{
    CMDDeviceConfigDialog dlg((QWidget *)parent);
    dlg.setDevice(this);
    dlg.exec();
}
IDataBlock *CMDDevice::createDataBlock(void *parent,bool config)
{
    CMDDataBlock *dataBlock = new CMDDataBlock();
    dataBlock->setParentItem(this,MDTreeItemType::Device);
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
//通信类型
CommunicationType CMDDevice::commType()
{
    return m_commType;
}

void CMDDevice::setCommType(CommunicationType commType)
{
    m_commType = commType;
    m_communicationModel = (MDCommunicationType_Pro)m_commType;//!property
}

//通信参数
QString CMDDevice::commParameter()
{
    return m_commParameter;
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
        if(address == "CommStatus")
        {
           m_qualityVariable = nsVar;
           return true;
        }
        else
        {
           error = tr("地址验证错误!");
           return false;
        }
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
        if(address == "CommStatus")
        {
           m_qualityVariable = NULL;
           return true;
        }
        else
        {
           return false;
        }
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
        if(address == "CommStatus")
        {
           m_qualityVariable = nsVar;
           return true;
        }
        else
        {
           error = tr("地址验证错误!");
           return false;
        }
    }
    QString dbName = address.left(pos);
    IDataBlock * db = getDataBlock(dbName);
    if(db==NULL)
    {
        return false;
    }
    return db->checkVarAddress(nsVar,error);
}
void CMDDevice::setCommParameter(const QString &   commParameter)
{
    m_commParameter = commParameter;
    if(m_commType==CommunicationType::Serial)
    {
        QStringList strListCommParameter = m_commParameter.split(",");
        if(strListCommParameter.count()>=5)
        {
            m_serialPort = strListCommParameter[0];
            if(0==QString::compare(strListCommParameter[1],"115200",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_115200;
            }
            else if(0==QString::compare(strListCommParameter[1],"57600",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_57600;
            }
            else if(0==QString::compare(strListCommParameter[1],"38400",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_38400;
            }
            else if(0==QString::compare(strListCommParameter[1],"19200",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_19200;
            }
            else if(0==QString::compare(strListCommParameter[1],"9600",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_9600;
            }
            else if(0==QString::compare(strListCommParameter[1],"4800",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_4800;
            }
            else if(0==QString::compare(strListCommParameter[1],"2400",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_2400;
            }
            else if(0==QString::compare(strListCommParameter[1],"1200",Qt::CaseInsensitive))
            {
                m_serialBaud = MDSerialBaud_Pro::Baud_1200;
            }
            else
            {
                m_serialBaud = (MDSerialBaud_Pro)strListCommParameter[1].toInt();
            }

            if(0==QString::compare(strListCommParameter[2],"5",Qt::CaseInsensitive))
            {
                m_serialDataBit = MDSerialDataBit_Pro::DataBit_5;
            }
            else if(0==QString::compare(strListCommParameter[2],"6",Qt::CaseInsensitive))
            {
                m_serialDataBit = MDSerialDataBit_Pro::DataBit_6;
            }
            else if(0==QString::compare(strListCommParameter[2],"7",Qt::CaseInsensitive))
            {
                m_serialDataBit = MDSerialDataBit_Pro::DataBit_7;
            }
            else if(0==QString::compare(strListCommParameter[2],"8",Qt::CaseInsensitive))
            {
                m_serialDataBit = MDSerialDataBit_Pro::DataBit_8;
            }
            else
            {
                m_serialDataBit = (MDSerialDataBit_Pro)strListCommParameter[2].toInt();
            }
            m_serialStopBit = (MDSerialStopBit_Pro)strListCommParameter[3].toInt();
            m_serialParityBit = (MDSerialParityBit_Pro)strListCommParameter[4].toInt();
        }
    }
    else if(m_commType == CommunicationType::TCP||m_commType == CommunicationType::UDP)
    {
        QStringList strListCommParameter = m_commParameter.split(",");
        if(strListCommParameter.count()>=5)
        {
            if(0==QString::compare(strListCommParameter[0],"Client",Qt::CaseInsensitive))
            {
                m_clientOrServer = MDClientOrServer_Pro::Client;
            }
            else
            {
                m_clientOrServer = MDClientOrServer_Pro::Server;
            }

            m_tcpOrUdpIP = strListCommParameter[1];
            m_tcpOrUdpLocalPort = strListCommParameter[4];
            m_tcpOrUdpRemotePort = strListCommParameter[2];
        }
    }
    else if(m_commType == CommunicationType::USB)
    {
        m_usbParameter = m_commParameter;
    }
    else if(m_commType == CommunicationType::Board)
    {
        m_boardParameter = m_commParameter;
    }
    else if(m_commType == CommunicationType::Other)
    {
        m_otherParameter = m_commParameter;
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

    element = doc.createElement("Station");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->station()));
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

    element = doc.createElement("LocalTSAP");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->LocalTSAP()));
    element.appendChild(elementText);

    element = doc.createElement("RemoteTSAP");
    deviceElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->RemoteTSAP()));
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
void CMDDevice::serialize(CSortJsonObject &json)
{
    json.insertSimpleValue("Name",this->name());
    json.insertSimpleValue("Description",this->description());
    json.insertSimpleValue("Enabled",this->isEnabled()?QString("1"):QString("0"));
    json.insertSimpleValue("CommType",this->commTypeEnumToString(this->commType()));
    json.insertSimpleValue("CommParameter",this->commParameterIndexToString(this->commParameter()));
    json.insertSimpleValue("ReplyTimeout",QString("%1").arg(this->replyTimeout()));
    json.insertSimpleValue("Retries",QString("%1").arg(this->retries()));
    json.insertSimpleValue("PLCType",this->PLCTypeIndexToString(this->PLCType()));
    json.insertSimpleValue("LocalTSAP",this->LocalTSAP());
    json.insertSimpleValue("RemoteTSAP",this->RemoteTSAP());

    CSortJsonArray array_Block;
    if(this->getDataBlockCount()>0)
    {
        for (int index =0;index< this->getDataBlockCount();index++)
        {
            CSortJsonObject block_json;
            CMDDataBlock* dataBlock = (CMDDataBlock*)this->getDataBlockAt(index);
            dataBlock->serialize(block_json);
            array_Block.insertObject(block_json);
        }
        json.insertArray("DataBlocks",array_Block);
    }
}
void CMDDevice::readConfiguration(QDomElement &domElement)
{
    //QString TempLocal,TempRemote;
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
        else if(0==QString::compare(element.tagName(),"Station", Qt::CaseInsensitive))
        {
            this->setStation(element.text().toInt());
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
        else if(0==QString::compare(element.tagName(),"LocalTSAP", Qt::CaseInsensitive))
        {
            this->setLocalTSAP(element.text());
        }
        else if(0==QString::compare(element.tagName(),"RemoteTSAP", Qt::CaseInsensitive))
        {
            this->setRemoteTSAP(element.text());
        }
        else if(0==QString::compare(element.tagName(),"DataBlocks", Qt::CaseInsensitive))
        {
            QDomElement dataBlockElement = element.firstChildElement();
            while ( !dataBlockElement.isNull() )
            {
                CMDDataBlock *dataBlock = new CMDDataBlock();
                dataBlock->setParentItem(this,MDTreeItemType::Device);
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
void CMDDevice::deserialize(const QJsonObject &json)
{
    if(json.isEmpty()==1)
    {
        return;
    }
    this->setName(json.value("Name").toString(""));
    this->setDescription(json.value("Description").toString(""));
    this->setEnabled(json.value("Enabled").toString("1").toInt());
    this->setCommType(this->commTypeStringToEnum(json.value("CommType").toString("1")));
    this->setCommParameter(this->commParameterStringToIndex(json.value("CommParameter").toString("Client,127.0.0.1,102,,")));
    this->setReplyTimeout_Pro(json.value("ReplyTimeout").toString("1000"));
    this->setRetries_Pro(json.value("Retries").toString("3"));
    this->setPLCType(this->PLCTypeStringToIndex(json.value("PLCType").toString("0")));
    this->setLocalTSAP(json.value("LocalTSAP").toString("10.11"));
    this->setRemoteTSAP(json.value("RemoteTSAP").toString("3.01"));
    QJsonArray array_Block = json.value("DataBlocks").toArray();
    for(int i=0;i<array_Block.count();i++)
    {
        CMDDataBlock *dataBlock = new CMDDataBlock();
        dataBlock->setParentItem(this,MDTreeItemType::Device);
        dataBlock->deserialize(array_Block.at(i).toObject());
        if(false == addDataBlock(dataBlock))
        {
            delete dataBlock;
            dataBlock = NULL;
        }
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
        else if(0==QString::compare(parameterName,"Station", Qt::CaseInsensitive))
        {
            this->setStation(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"ReplyTimeout", Qt::CaseInsensitive))
        {
            this->setReplyTimeout_Pro(parameterValue);
        }
        else if(0==QString::compare(parameterName,"Retries", Qt::CaseInsensitive))
        {
            this->setRetries_Pro(parameterValue);
        }
        else if(0==QString::compare(parameterName,"PLCType", Qt::CaseInsensitive))
        {
            this->setPLCType(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"LocalTSAP", Qt::CaseInsensitive))
        {
            this->setLocalTSAP(parameterValue);
        }
        else if(0==QString::compare(parameterName,"RemoteTSAP", Qt::CaseInsensitive))
        {
            this->setRemoteTSAP(parameterValue);
        }
    }

    QVariantList dataBlockList=  deviceParameterMap["MDDevicesCommunicateDataBlock"].toList();
    for(int i=0;i<dataBlockList.count();i++)
    {
        QVariantMap dataBlockMap = dataBlockList[i].toMap();
        CMDDataBlock *dataBlock = new CMDDataBlock();
        dataBlock->setParentItem(this,MDTreeItemType::Device);
        dataBlock->dynamicSetParameter(dataBlockMap);
        if(false == addDataBlock(dataBlock))
        {
            delete dataBlock;
            dataBlock = NULL;
        }
    }
}
QString CMDDevice::commTypeEnumToString(CommunicationType commtype)
{
    QString strCommType = QString("%1").arg((int)commtype);
    if(commtype==CommunicationType::Serial)
    {
        strCommType = "Serial";
    }
    else if(commtype==CommunicationType::TCP)
    {
        strCommType = "TCP";
    }
    else if(commtype==CommunicationType::UDP)
    {
        strCommType = "UDP";
    }
    else if(commtype==CommunicationType::USB)
    {
        strCommType = "USB";
    }
    else if(commtype==CommunicationType::Board)
    {
        strCommType = "Board";
    }
    else if(commtype==CommunicationType::Other)
    {
        strCommType = "Other";
    }
    return strCommType;
}
CommunicationType CMDDevice::commTypeStringToEnum(QString strcommtype)
{
    CommunicationType commType = (CommunicationType)strcommtype.toInt();
    if(0==QString::compare(strcommtype,"Serial",Qt::CaseInsensitive))
    {
        commType = CommunicationType::Serial;
    }
    else if(0==QString::compare(strcommtype,"TCP",Qt::CaseInsensitive))
    {
        commType = CommunicationType::TCP;
    }
    else if(0==QString::compare(strcommtype,"TCP",Qt::CaseInsensitive))
    {
        commType = CommunicationType::UDP;
    }
    else if(0==QString::compare(strcommtype,"USB",Qt::CaseInsensitive))
    {
        commType = CommunicationType::USB;
    }
    else if(0==QString::compare(strcommtype,"Board",Qt::CaseInsensitive))
    {
        commType = CommunicationType::Board;
    }
    else if(0==QString::compare(strcommtype,"Other",Qt::CaseInsensitive))
    {
        commType = CommunicationType::Other;
    }
    return commType;
}
QString CMDDevice::commParameterIndexToString(QString parameter)
{
    QString strCommIndexToString=parameter;
    if(this->commType()==CommunicationType::Serial)
    {
        QStringList strListCommIndexToString = strCommIndexToString.split(",");
        if(strListCommIndexToString.count()>=5)
        {
            if(strListCommIndexToString[3] == "0")
            {
                strListCommIndexToString.replace(3,"1");
            }
            else if(strListCommIndexToString[3] == "1")
            {
                strListCommIndexToString.replace(3,"1.5");
            }
            else if(strListCommIndexToString[3] == "2")
            {
                strListCommIndexToString.replace(3,"2");
            }

            if(strListCommIndexToString[4] == "0")
            {
                strListCommIndexToString.replace(4,"NONE");
            }
            else if(strListCommIndexToString[4] == "1")
            {
                strListCommIndexToString.replace(4,"ODD");
            }
            else if(strListCommIndexToString[4] == "2")
            {
                strListCommIndexToString.replace(4,"EVEN");
            }
            else if(strListCommIndexToString[4] == "3")
            {
                strListCommIndexToString.replace(4,"MARK");
            }
            else if(strListCommIndexToString[4] == "4")
            {
                strListCommIndexToString.replace(4,"SPACE");
            }
            strCommIndexToString = strListCommIndexToString.join(",");
        }
        else
        {
            return strCommIndexToString;
        }

    }
    return strCommIndexToString;
}
QString CMDDevice::commParameterStringToIndex(QString parameter)
{
    QString strCommStringToIndex=parameter;
    if(this->commType()==CommunicationType::Serial)
    {
        QStringList strListCommStringToIndex = strCommStringToIndex.split(",");
        if(strListCommStringToIndex.count()>=5)
        {
            if(0==QString::compare(strListCommStringToIndex[3],"1",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(3,"0");
            }
            else if(0==QString::compare(strListCommStringToIndex[3],"1.5",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(3,"1");
            }
            else if(0==QString::compare(strListCommStringToIndex[3],"2",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(3,"2");
            }

            if(0==QString::compare(strListCommStringToIndex[4],"NONE",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(4,"0");
            }
            else if(0==QString::compare(strListCommStringToIndex[4],"ODD",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(4,"1");
            }
            else if(0==QString::compare(strListCommStringToIndex[4],"EVEN",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(4,"2");
            }
            else if(0==QString::compare(strListCommStringToIndex[4],"MARK",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(4,"3");
            }
            else if(0==QString::compare(strListCommStringToIndex[4],"SPACE",Qt::CaseInsensitive))
            {
                strListCommStringToIndex.replace(4,"4");
            }
            strCommStringToIndex = strListCommStringToIndex.join(",");
        }
        else
        {
            return strCommStringToIndex;
        }

    }
    return strCommStringToIndex;
}
QString CMDDevice::PLCTypeIndexToString(qint16 index)
{
    QString strPLCType = QString("%1").arg(index);
    if(index == 0)
    {
        strPLCType = "S7-1200";
    }
    else if(index == 1)
    {
        strPLCType = "S7-1500";
    }
    else if(index == 2)
    {
        strPLCType = "S7-300";
    }
    else if(index == 3)
    {
        strPLCType = "S7-400";
    }
    return strPLCType;
}
qint16 CMDDevice::PLCTypeStringToIndex(QString str)
{
    qint16 nPLCType = (qint16)str.toInt();
    if(0==QString::compare(str,"S7-1200",Qt::CaseInsensitive))
    {
        nPLCType = 0;
    }
    else if(0==QString::compare(str,"S7-1500",Qt::CaseInsensitive))
    {
        nPLCType = 1;
    }
    else if(0==QString::compare(str,"S7-300",Qt::CaseInsensitive))
    {
        nPLCType = 2;
    }
    else if(0==QString::compare(str,"S7-400",Qt::CaseInsensitive))
    {
        nPLCType = 3;
    }
    return nPLCType;
}
//**********************************属性框翻译与提示***************************//
void CMDDevice::initTranslateNameAndPropertyType()
{
    m_translateName.insert(QString("GUID_CommunicationParameter"),QString("通信参数"));
    m_translateName.insert(QString("CommunicationTypeP"),QString("通信类型"));
    m_propertyTip.insert(QString("CommunicationTypeP"),QString("选择通信的类型"));
    //*******************************通信参数********************************//
    //!串口
    m_translateName.insert(QString("SerialPort"),QString("端口"));
    m_translateName.insert(QString("SerialBaud"),QString("波特率"));
    m_translateName.insert(QString("SerialDataBit"),QString("数据位"));
    m_translateName.insert(QString("SerialStopBit"),QString("停止位"));
    m_translateName.insert(QString("SerialParityBit"),QString("校验位"));

    m_propertyTip.insert(QString("SerialPort"),QString("端口(例:Windows通常格式是'COM0',Linux通常格式是'/dev/ttyS0'或'ttyUSB0')"));
    m_propertyTip.insert(QString("SerialBaud"),QString("选择波特率"));
    m_propertyTip.insert(QString("SerialDataBit"),QString("选择数据位"));
    m_propertyTip.insert(QString("SerialStopBit"),QString("选择停止位"));
    m_propertyTip.insert(QString("SerialParityBit"),QString("选择校验位"));
    //!以太
    m_translateName.insert(QString("ClientOrServer"),QString("客户端/服务端"));
    m_translateName.insert(QString("TcpOrUdpIP"),QString("IP地址"));
    m_translateName.insert(QString("TcpOrUdpRemotePort"),QString("服务端口"));
    m_translateName.insert(QString("TcpOrUdpLocalPort"),QString("本地服务端口"));

    m_propertyTip.insert(QString("ClientOrServer"),QString("选择客户端/服务端"));
    m_propertyTip.insert(QString("TcpOrUdpIP"),QString("IP地址(例:格式是127.0.0.1)"));
    m_propertyTip.insert(QString("TcpOrUdpRemotePort"),QString("服务端口"));
    m_propertyTip.insert(QString("TcpOrUdpLocalPort"),QString("本地服务端口"));
    //!USB
    m_translateName.insert(QString("UsbParameter"),QString("USB参数"));
    m_propertyTip.insert(QString("UsbParameter"),QString("USB参数"));
    //!板卡
    m_translateName.insert(QString("BoardParameter"),QString("板卡参数"));
    m_propertyTip.insert(QString("BoardParameter"),QString("板卡参数"));
    //!其他
    m_translateName.insert(QString("OtherParameter"),QString("其他参数"));
    m_propertyTip.insert(QString("OtherParameter"),QString("其他参数"));
    //**********************************************************************//
    //*******************************设备参数********************************//
    m_translateName.insert(QString("GUID_DeviceParameter"),QString("设备参数"));
    //!通信协议
    m_translateName.insert(QString("CommunicationProtocol"),QString("通信协议"));
    //!超时时间
    m_translateName.insert(QString("ReplyTimeout"),QString("超时时间"));
    m_propertyTip.insert(QString("ReplyTimeout"),QString("超时时间"));
    //!重试次数
    m_translateName.insert(QString("Retries"),QString("重试次数"));
    m_propertyTip.insert(QString("Retries"),QString("重试次数"));
    //!Rack/Slot TSAP
    m_translateName.insert(QString("RackAndSlotOrTSAP"),QString("传输模式"));
    m_propertyTip.insert(QString("RackAndSlotOrTSAP"),QString("传输模式"));
    //!机架号
    m_translateName.insert(QString("CPURack"),QString("机架号"));
    m_propertyTip.insert(QString("CPURack"),QString("机架号"));
    //!槽号
    m_translateName.insert(QString("CPUSlot"),QString("槽号"));
    m_propertyTip.insert(QString("CPUSlot"),QString("槽号"));
    //!本地传输服务访问点
    m_translateName.insert(QString("LocalTSAP"),QString("本地传输服务访问点"));
    m_propertyTip.insert(QString("LocalTSAP"),QString("本地传输服务访问点"));
    //!传输服务访问点
    m_translateName.insert(QString("RemoteTSAP"),QString("传输服务访问点"));
    m_propertyTip.insert(QString("RemoteTSAP"),QString("传输服务访问点"));
    //**********************************************************************//

}
QMap<QString,QVariant> CMDDevice::translateName() const//!翻译函数
{
    return m_translateName;
}
QMap<QString, QVariant> CMDDevice::propertyTip() const//!提示框函数
{
    return m_propertyTip;
}
//****************************属性框函数-通信参数***************************//
CMDDevice::MDCommunicationType_Pro CMDDevice::communicationType_Pro()//!通信类型读函数
{
    if(m_bHide == 0)
    {
        connect(getPropertyBrowse(),SIGNAL(propertyFinish()),this,SLOT(propertyLoadFinish_Pro()));
        m_bHide = 1;
    }
    return m_communicationModel;
}
void CMDDevice::setCommunicationType_Pro(CMDDevice::MDCommunicationType_Pro model)//!通信类型写函数
{
    if(m_communicationModel!=model)
    {
        if(!((m_communicationModel==MDCommunicationType_Pro::TCP&&model==MDCommunicationType_Pro::UDP)
             ||(m_communicationModel==MDCommunicationType_Pro::UDP&&model==MDCommunicationType_Pro::TCP)))
        {
            m_communicationModel = model;
            m_commType = (CommunicationType)m_communicationModel;
            m_commParameter = "";
            if(m_commType==CommunicationType::Serial)
            {
                m_commParameter=m_commParameter +m_serialPort+",";
                if(m_serialBaud==MDSerialBaud_Pro::Baud_115200)
                {
                     m_commParameter=m_commParameter +"115200"+",";
                }
                else if(m_serialBaud==MDSerialBaud_Pro::Baud_57600)
                {
                    m_commParameter=m_commParameter +"57600"+",";
                }
                else if(m_serialBaud==MDSerialBaud_Pro::Baud_38400)
                {
                    m_commParameter=m_commParameter +"38400"+",";
                }
                else if(m_serialBaud==MDSerialBaud_Pro::Baud_19200)
                {
                    m_commParameter=m_commParameter +"19200"+",";
                }
                else if(m_serialBaud==MDSerialBaud_Pro::Baud_9600)
                {
                    m_commParameter=m_commParameter +"9600"+",";
                }
                else if(m_serialBaud==MDSerialBaud_Pro::Baud_4800)
                {
                    m_commParameter=m_commParameter +"4800"+",";
                }
                else if(m_serialBaud==MDSerialBaud_Pro::Baud_2400)
                {
                    m_commParameter=m_commParameter +"2400"+",";
                }
                else if(m_serialBaud==MDSerialBaud_Pro::Baud_1200)
                {
                    m_commParameter=m_commParameter +"1200"+",";
                }

                if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_5)
                {
                    m_commParameter=m_commParameter +"5"+",";
                }
                else if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_6)
                {
                    m_commParameter=m_commParameter +"6"+",";
                }
                else if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_7)
                {
                    m_commParameter=m_commParameter +"7"+",";
                }
                else if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_8)
                {
                    m_commParameter=m_commParameter +"8"+",";
                }
                m_commParameter=m_commParameter +QString("%1").arg((int)m_serialStopBit)+",";
                m_commParameter=m_commParameter +QString("%1").arg((int)m_serialParityBit);
            }
            else if(m_commType==CommunicationType::TCP||m_commType==CommunicationType::UDP)
            {
                if(m_clientOrServer == MDClientOrServer_Pro::Client)
                {
                    m_commParameter=m_commParameter +"Client"+",";
                }
                else
                {
                    m_commParameter=m_commParameter +"Server"+",";
                }
                m_commParameter=m_commParameter +m_tcpOrUdpIP+",";
                m_commParameter=m_commParameter +m_tcpOrUdpRemotePort+",";
                m_commParameter=m_commParameter +""+",";
                m_commParameter=m_commParameter +m_tcpOrUdpLocalPort;
            }
            else if(m_commType==CommunicationType::USB)
            {
                m_commParameter = m_usbParameter;
            }
            else if(m_commType==CommunicationType::Board)
            {
                m_commParameter = m_boardParameter;
            }
            else if(m_commType==CommunicationType::Other)
            {
                m_commParameter = m_otherParameter;
            }
            this->hideAllAndShowOne_Pro(m_commType);
        }
    }
}
//!串口
QString CMDDevice::serialPort_Pro()
{
    return m_serialPort;
}
void CMDDevice::setSerialPort_Pro(QString port)
{
    m_serialPort = port;
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(0,m_serialPort);
        m_commParameter = strListTemp.join(",");
    }
}

CMDDevice::MDSerialBaud_Pro CMDDevice::serialBaud_Pro()
{
    return m_serialBaud;
}

void CMDDevice::setSerialBaud_Pro(CMDDevice::MDSerialBaud_Pro baud)
{
    m_serialBaud = baud;
    QString strTemp = "";
    if(m_serialBaud==MDSerialBaud_Pro::Baud_115200)
    {
         strTemp="115200";
    }
    else if(m_serialBaud==MDSerialBaud_Pro::Baud_57600)
    {
        strTemp="57600";
    }
    else if(m_serialBaud==MDSerialBaud_Pro::Baud_38400)
    {
        strTemp="38400";
    }
    else if(m_serialBaud==MDSerialBaud_Pro::Baud_19200)
    {
        strTemp="19200";
    }
    else if(m_serialBaud==MDSerialBaud_Pro::Baud_9600)
    {
        strTemp="9600";
    }
    else if(m_serialBaud==MDSerialBaud_Pro::Baud_4800)
    {
        strTemp="4800";
    }
    else if(m_serialBaud==MDSerialBaud_Pro::Baud_2400)
    {
        strTemp="2400";
    }
    else if(m_serialBaud==MDSerialBaud_Pro::Baud_1200)
    {
        strTemp="1200";
    }
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(1,strTemp);
        m_commParameter = strListTemp.join(",");
    }
}

CMDDevice::MDSerialDataBit_Pro CMDDevice::serialDataBit_Pro()
{
    return m_serialDataBit;
}
void CMDDevice::setSerialDataBit_Pro(CMDDevice::MDSerialDataBit_Pro dataBit)
{
    m_serialDataBit = dataBit;
    QString strTemp = "";
    if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_5)
    {
        strTemp="5";
    }
    else if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_6)
    {
        strTemp="6";
    }
    else if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_7)
    {
        strTemp="7";
    }
    else if(m_serialDataBit == MDSerialDataBit_Pro::DataBit_8)
    {
        strTemp="8";
    }
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(2,strTemp);
        m_commParameter = strListTemp.join(",");
    }
}

CMDDevice::MDSerialStopBit_Pro CMDDevice::serialStopBit_Pro()
{
    return m_serialStopBit;
}
void CMDDevice::setSerialStopBit_Pro(CMDDevice::MDSerialStopBit_Pro stopBit)
{
    m_serialStopBit = stopBit;
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(3,QString("%1").arg((int)m_serialStopBit));
        m_commParameter = strListTemp.join(",");
    }
}

CMDDevice::MDSerialParityBit_Pro CMDDevice::serialParityBit_Pro()
{
    return m_serialParityBit;
}
void CMDDevice::setSerialParityBit_Pro(CMDDevice::MDSerialParityBit_Pro parityBit)
{
    m_serialParityBit = parityBit;
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(4,QString("%1").arg((int)m_serialParityBit));
        m_commParameter = strListTemp.join(",");
    }
}
//!以太
CMDDevice::MDClientOrServer_Pro CMDDevice::clientOrServer_Pro()
{
    return m_clientOrServer;
}
void CMDDevice::setcClientOrServer_Pro(CMDDevice::MDClientOrServer_Pro clientOrServer)
{
    m_clientOrServer = clientOrServer;
    QString strTemp = "";
    if(m_clientOrServer == MDClientOrServer_Pro::Client)
    {
        strTemp="Client";
    }
    else
    {
        strTemp="Server";
    }
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(0,strTemp);
        m_commParameter = strListTemp.join(",");
    }
}
QString CMDDevice::tcpOrUdpIP_Pro()
{
    return m_tcpOrUdpIP;
}
void CMDDevice::setTcpOrUdpIP_Pro(QString ip)
{
    m_tcpOrUdpIP = ip;
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(1,m_tcpOrUdpIP);
        m_commParameter = strListTemp.join(",");
    }
}
QString CMDDevice::tcpOrUdpLocalPort_Pro()
{
    return m_tcpOrUdpLocalPort;
}
void CMDDevice::setTcpOrUdpLocalPort_Pro(QString localPort)
{
    m_tcpOrUdpLocalPort = localPort;
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(4,m_tcpOrUdpLocalPort);
        m_commParameter = strListTemp.join(",");
    }
}
QString CMDDevice::tcpOrUdpRemotePort_Pro()
{
    return m_tcpOrUdpRemotePort;
}
void CMDDevice::setTcpOrUdpRemotePort_Pro(QString remotePort)
{
    m_tcpOrUdpRemotePort = remotePort;
    QStringList strListTemp = m_commParameter.split(",");
    if(strListTemp.count()>=5)
    {
        strListTemp.replace(2,m_tcpOrUdpRemotePort);
        m_commParameter = strListTemp.join(",");
    }
}
//!USB
QString CMDDevice::usbParameter_Pro()
{
    return m_usbParameter;
}
void CMDDevice::setUsbParameter_Pro(QString parameter)
{
    m_usbParameter = parameter;
    m_commParameter = m_usbParameter;
}
//!板卡
QString CMDDevice::boardParameter_Pro()
{
    return m_boardParameter;
}
void CMDDevice::setBoardParameter_Pro(QString parameter)
{
    m_boardParameter = parameter;
    m_commParameter = m_boardParameter;
}
//!其他
QString CMDDevice::otherParameter_Pro()
{
    return m_otherParameter;
}
void CMDDevice::setOtherParameter_Pro(QString parameter)
{
    m_otherParameter = parameter;
    m_commParameter = m_otherParameter;
}
void CMDDevice::hideAllAndShowOne_Pro(CommunicationType comm)
{
    CPropertyBrowser* pProperty = getPropertyBrowse();
    //!串口
    pProperty->setSinglePropertyHide("SerialPort",true);
    pProperty->setSinglePropertyHide("SerialBaud",true);
    pProperty->setSinglePropertyHide("SerialDataBit",true);
    pProperty->setSinglePropertyHide("SerialStopBit",true);
    pProperty->setSinglePropertyHide("SerialParityBit",true);
    //!以太
    pProperty->setSinglePropertyHide("ClientOrServer",true);
    pProperty->setSinglePropertyHide("TcpOrUdpIP",true);
    pProperty->setSinglePropertyHide("TcpOrUdpRemotePort",true);
    pProperty->setSinglePropertyHide("TcpOrUdpLocalPort",true);
    //!USB
    pProperty->setSinglePropertyHide("UsbParameter",true);
    //!板卡
    pProperty->setSinglePropertyHide("BoardParameter",true);
    //!其他
    pProperty->setSinglePropertyHide("OtherParameter",true);
    if(comm==CommunicationType::Serial)
    {
        pProperty->setSinglePropertyHide("SerialPort",false);
        pProperty->setSinglePropertyHide("SerialBaud",false);
        pProperty->setSinglePropertyHide("SerialDataBit",false);
        pProperty->setSinglePropertyHide("SerialStopBit",false);
        pProperty->setSinglePropertyHide("SerialParityBit",false);
    }
    else if(comm==CommunicationType::TCP||comm==CommunicationType::UDP)
    {
        pProperty->setSinglePropertyHide("ClientOrServer",false);
        pProperty->setSinglePropertyHide("TcpOrUdpIP",false);
        pProperty->setSinglePropertyHide("TcpOrUdpRemotePort",false);
        pProperty->setSinglePropertyHide("TcpOrUdpLocalPort",false);
    }
    else if(comm==CommunicationType::USB)
    {
        pProperty->setSinglePropertyHide("UsbParameter",false);
    }
    else if(comm==CommunicationType::Board)
    {
        pProperty->setSinglePropertyHide("BoardParameter",false);
    }
    else if(comm==CommunicationType::Other)
    {
        pProperty->setSinglePropertyHide("OtherParameter",false);
    }

    pProperty->setSinglePropertyHide("CPURack",true);
    pProperty->setSinglePropertyHide("CPUSlot",true);
    pProperty->setSinglePropertyHide("LocalTSAP",true);
    pProperty->setSinglePropertyHide("RemoteTSAP",true);
    if(m_plcType_Pro == MDPLCType_Pro::S7_1200 || m_plcType_Pro == MDPLCType_Pro::S7_1500)
    {
        pProperty->setSinglePropertyHide("LocalTSAP",false);
        pProperty->setSinglePropertyHide("RemoteTSAP",false);
    }
    else if(m_plcType_Pro == MDPLCType_Pro::S7_300 || m_plcType_Pro == MDPLCType_Pro::S7_400)
    {
        pProperty->setSinglePropertyHide("CPURack",false);
        pProperty->setSinglePropertyHide("CPUSlot",false);
    }

}
void CMDDevice::propertyLoadFinish_Pro()
{
    if(this == qobject_cast<CMDDevice*>(getPropertyBrowse()->object()))
    {
        this->hideAllAndShowOne_Pro(m_commType);
    }
}
//***********************************************************************//
//****************************属性框函数-设备参数***************************//
//!超时时间
QString CMDDevice::replyTimeout_Pro()
{
    return m_replyTimeout_Pro;
}
void CMDDevice::setReplyTimeout_Pro(QString timeout)
{
    m_replyTimeout_Pro = timeout;
    setReplyTimeout((quint32)m_replyTimeout_Pro.toUInt());
}
//!重试次数
QString CMDDevice::retries_Pro()
{
    return m_retries_Pro;
}
void CMDDevice::setRetries_Pro(QString retries)
{
    m_retries_Pro = retries;
    setRetries((quint32)m_retries_Pro.toUInt());
}
//!rack or tsap
CMDDevice::MDPLCType_Pro CMDDevice::plcType_Pro()
{
    return m_plcType_Pro;
}
void CMDDevice::setPLCType_Pro(MDPLCType_Pro plc)
{
    if(plc!=m_plcType_Pro)
    {
        CPropertyBrowser* pProperty = getPropertyBrowse();
        if(plc == MDPLCType_Pro::S7_1200 || plc == MDPLCType_Pro::S7_1500)
        {
            pProperty->setSinglePropertyHide("CPURack",true);
            pProperty->setSinglePropertyHide("CPUSlot",true);
            pProperty->setSinglePropertyHide("LocalTSAP",false);
            pProperty->setSinglePropertyHide("RemoteTSAP",false);

            m_LocalTSAP = m_localTSAP_Pro;
            m_RemoteTSAP = m_remoteTSAP_Pro;
        }
        else if(plc == MDPLCType_Pro::S7_300 || plc == MDPLCType_Pro::S7_400)
        {
            pProperty->setSinglePropertyHide("CPURack",false);
            pProperty->setSinglePropertyHide("CPUSlot",false);
            pProperty->setSinglePropertyHide("LocalTSAP",true);
            pProperty->setSinglePropertyHide("RemoteTSAP",true);
            m_LocalTSAP = m_cpuRack_Pro;
            m_RemoteTSAP = m_cpuSlot_Pro;
        }
        m_plcType_Pro = plc;
        m_PLCType = (qint16)m_plcType_Pro;
    }
}
QString CMDDevice::communicationProtocol_Pro()
{
    return m_commProtocol_Pro;
}
//!机架号
QString CMDDevice::cpuRack_Pro()
{
    return m_cpuRack_Pro;
}
void CMDDevice::setCPURack_Pro(QString rack)
{
    m_cpuRack_Pro = rack;
    m_LocalTSAP = m_cpuRack_Pro;
}
//!槽号
QString CMDDevice::cpuSlot_Pro()
{
    return m_cpuSlot_Pro;
}
void CMDDevice::setCPUSlot_Pro(QString slot)
{
    m_cpuSlot_Pro = slot;
    m_RemoteTSAP = m_cpuSlot_Pro;
}
//!本地传输服务访问点
QString CMDDevice::localTSAP_Pro()
{
    return m_localTSAP_Pro;
}
void CMDDevice::setLocalTSAP_Pro(QString ltsap)
{
    m_localTSAP_Pro = ltsap;
    m_RemoteTSAP = m_localTSAP_Pro;
}
//!传输服务访问点
QString CMDDevice::remoteTSAP_Pro()
{
    return m_remoteTSAP_Pro;
}
void CMDDevice::setRemoteTSAP_Pro(QString rtsap)
{
    m_remoteTSAP_Pro = rtsap;
    m_RemoteTSAP = m_remoteTSAP_Pro;
}
//***********************************************************************//

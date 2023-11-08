#include "MDDevice.h"
#include "MDDataBlock.h"
#include "MDDeviceConfigDialog.h"
#include "MDDataBlockDialog.h"
#include "MDDeviceParameterGetConfig.h"
#include "PropertyBrowser.h"
CMDDevice::CMDDevice()
{
    m_pVarTable = NULL;
    m_configFlag = 0;
    m_bFromSlave = 0;
    m_bStatuFlag = 0;
    m_runFlag = 0;


    m_tabNum = 1;
    m_tabCurrentNum = 1;

    m_commType = CommunicationType::TCP;
    m_commParameter = "Client,127.0.0.1,502,,;Client,,,,";
    setReplyTimeout(1000);
    setRetries(3);
    setEnabled(true);
    //************************属性框变量-通信参数***********************//
    this->initTranslateNameAndPropertyType();
    m_communicationModel = (MDCommunicationType_Pro)m_commType;
    //!串口
    m_serialPort = "COM1";
    m_serialBaud = MDSerialBaud_Pro::Baud_9600;
    m_serialDataBit = MDSerialDataBit_Pro::DataBit_7;
    m_serialStopBit = MDSerialStopBit_Pro::StopBit_2;
    m_serialParityBit = MDSerialParityBit_Pro::EVEN;
    //!以太
    m_clientOrServer = MDClientOrServer_Pro::Client;
    m_tcpOrUdpIP = "127.0.0.1";
    m_tcpOrUdpLocalPort = "";
    m_tcpOrUdpRemotePort = "502";
    m_slaveClientOrServer = MDClientOrServer_Pro::Client;
    m_slaveTcpOrUdpIP = "";
    m_slaveTcpOrUdpLocalPort = "";
    m_slaveTcpOrUdpRemotePort = "";
    //!USB
    m_usbParameter = "";
    //!板卡
    m_boardParameter = "";
    //!其他
    m_otherParameter = "";
    //!初始隐藏标志位
    m_bHide = 0;
    //***************************************************************//
    //************************属性框变量-设备参数***********************//
    m_config_Pro = "";
    m_commProtocol_Pro ="SecpTCP";
    m_replyTimeout_Pro = "1000";
    m_retries_Pro = "3";
    //***************************************************************//
}
CMDDevice::~CMDDevice()
{
    if(NULL != m_pVarTable)
    {
        delete m_pVarTable;
        m_pVarTable = NULL;
    }
    for( IDAndPro ::iterator i=idTabMap.begin();i != idTabMap.end();++i)
    {
       MDVarPrope*pVarPrope = i.value();
       if(NULL!=pVarPrope)
       {
           delete pVarPrope;
           pVarPrope = NULL;
        }
    }
    idTabMap.clear();
    nameTabMap.clear();
    tabNumTagMap.clear();
    dataTabNOAndPrope.clear();
    for( IDAndPro ::iterator i=idTabMapRun.begin();i != idTabMapRun.end();++i)
    {
       MDVarPrope*pVarPrope = i.value();
       if(NULL!=pVarPrope)
       {
           delete pVarPrope;
           pVarPrope = NULL;
        }
    }
    idTabMapRun.clear();
    nameTabMapRun.clear();
}

bool CMDDevice::start()
{
    if(NULL != m_pVarTable)
    {
        delete m_pVarTable;
        m_pVarTable = NULL;
    }
    for( IDAndPro ::iterator i=idTabMapRun.begin();i != idTabMapRun.end();++i)
    {
       MDVarPrope*pVarPrope = i.value();
       if(NULL!=pVarPrope)
       {
           delete pVarPrope;
           pVarPrope = NULL;
        }
    }
    idTabMapRun.clear();
    nameTabMapRun.clear();
    m_runFlag = 0;
    m_tabCurrentNum = 1;
    return CBaseDevice::start();
}
bool CMDDevice::stop()
{
    for( IDAndPro ::iterator i=idTabMapRun.begin();i != idTabMapRun.end();++i)
    {
       MDVarPrope*pVarPrope = i.value();
       if(NULL!=pVarPrope)
       {
           delete pVarPrope;
           pVarPrope = NULL;
        }
    }
    idTabMapRun.clear();
    nameTabMapRun.clear();
    m_runFlag = 0;
    m_tabCurrentNum = 1;
    return CBaseDevice::stop();
}

void CMDDevice::copy(IDevice *otherDevice)
{
    if(NULL== otherDevice)
    {
        return;
    }
    CBaseDevice::copy(otherDevice);
    this->setReplyTimeout_Pro(QString::number(((CMDDevice*)otherDevice)->replyTimeout(),10));
    this->setRetries_Pro(QString::number(((CMDDevice*)otherDevice)->retries(),10));
    this->setCommType(((CMDDevice*)otherDevice)->commType());
    this->setCommParameter(((CMDDevice*)otherDevice)->commParameter());
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


void CMDDevice::setCommParameter(const QString &commParameter)
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
        QStringList strListCommParameter = m_commParameter.split(";");
        if(strListCommParameter.count()>=2)
        {
            QStringList strListCommParameterSub = strListCommParameter[0].split(",");
            if(strListCommParameterSub.count()>=5)
            {
                if(0==QString::compare(strListCommParameterSub[0],"Client",Qt::CaseInsensitive))
                {
                    m_clientOrServer = MDClientOrServer_Pro::Client;
                }
                else
                {
                    m_clientOrServer = MDClientOrServer_Pro::Server;
                }

                m_tcpOrUdpIP = strListCommParameterSub[1];
                m_tcpOrUdpLocalPort = strListCommParameterSub[4];
                m_tcpOrUdpRemotePort = strListCommParameterSub[2];
            }
            QStringList strListCommParameterSub1 = strListCommParameter[1].split(",");
            if(strListCommParameterSub1.count()>=5)
            {
                if(0==QString::compare(strListCommParameterSub1[0],"Client",Qt::CaseInsensitive))
                {
                    m_slaveClientOrServer = MDClientOrServer_Pro::Client;
                }
                else
                {
                    m_slaveClientOrServer = MDClientOrServer_Pro::Server;
                }

                m_slaveTcpOrUdpIP = strListCommParameterSub1[1];
                m_slaveTcpOrUdpLocalPort = strListCommParameterSub1[4];
                m_slaveTcpOrUdpRemotePort = strListCommParameterSub1[2];
            }
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
void CMDDevice::getVariableAddressList(QStringList &varAddressList)
{
    varAddressList.append("CommStatus");//!状态显示变量
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

    if(!dataTabNOAndPrope.isEmpty())
    {
        for(DataTabNoAndPrope::iterator dataTabID = dataTabNOAndPrope.begin();dataTabID!=dataTabNOAndPrope.end();++dataTabID)
        {
            QDomElement dataTabAndTagPrope = doc.createElement("DataTabNumAndTagPrope");
            deviceElement.appendChild(dataTabAndTagPrope);

            QDomElement dataTabNO = doc.createElement("DataTabNO");
            dataTabAndTagPrope.appendChild(dataTabNO);
            IDAndPro idTabMapTemp = dataTabID.value();
            qint16 tempKey = dataTabID.key();
            QDomText dataNOValue = doc.createTextNode(QString("%1").arg(tempKey));//
            dataTabNO.appendChild(dataNOValue);

            QDomElement varTables = doc.createElement("VarTables");
            dataTabAndTagPrope.appendChild(varTables);
            for(IDAndPro::iterator mapID= idTabMapTemp.begin();mapID!=idTabMapTemp.end();++mapID)
            {
                MDVarPrope*pVarPrope = mapID.value();

                QDomElement varTableElement = doc.createElement("VarTable");
                varTables.appendChild(varTableElement);

                QDomElement varPrope = doc.createElement("ID");
                varTableElement.appendChild(varPrope);
                QDomText varValue = doc.createTextNode(QString("%1").arg(pVarPrope->m_tagID));//
                varPrope.appendChild(varValue);

                varPrope = doc.createElement("Name");
                varTableElement.appendChild(varPrope);
                varValue = doc.createTextNode(pVarPrope->m_tagName);
                varPrope.appendChild(varValue);

                varPrope = doc.createElement("Type");
                varTableElement.appendChild(varPrope);
                varValue = doc.createTextNode(QString("%1").arg(pVarPrope->m_tagTyp));
                varPrope.appendChild(varValue);

                varPrope = doc.createElement("Length");
                varTableElement.appendChild(varPrope);
                varValue = doc.createTextNode(QString("%1").arg(pVarPrope->m_tagLength));
                varPrope.appendChild(varValue);
            }
        }
    }

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

    if(!dataTabNOAndPrope.isEmpty())
    {
        CSortJsonArray array_TabNoAndPrope;
        for(DataTabNoAndPrope::iterator dataTabID = dataTabNOAndPrope.begin();dataTabID!=dataTabNOAndPrope.end();++dataTabID)
        {
            CSortJsonObject json_TabNoAndPrope;
            IDAndPro idTabMapTemp = dataTabID.value();
            qint16 tempKey = dataTabID.key();
            json_TabNoAndPrope.insertSimpleValue("DataTabNO",QString("%1").arg(tempKey));
            CSortJsonArray array_Vars;
            for(IDAndPro::iterator mapID= idTabMapTemp.begin();mapID!=idTabMapTemp.end();++mapID)
            {
                CSortJsonObject json_Var;

                MDVarPrope*pVarPrope = mapID.value();

                json_Var.insertSimpleValue("ID",QString("%1").arg(pVarPrope->m_tagID));
                json_Var.insertSimpleValue("Name",pVarPrope->m_tagName);
                json_Var.insertSimpleValue("Type",QString("%1").arg(pVarPrope->m_tagTyp));
                json_Var.insertSimpleValue("Length",QString("%1").arg(pVarPrope->m_tagLength));
                array_Vars.insertObject(json_Var);
            }
            json_TabNoAndPrope.insertArray("VarTables",array_Vars);
            array_TabNoAndPrope.insertObject(json_TabNoAndPrope);
        }
        json.insertArray("DataTabNumAndTagPrope",array_TabNoAndPrope);
    }
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

            QStringList  parameterTCP = this->commParameter().split(',');

            //QString strIP=parameterTCP[1];
            //qint16 port =(qint16) parameterTCP[2].toInt();
            //this->getTabVar(strIP,port);
        }
        else if(0==QString::compare(element.tagName(),"ReplyTimeout", Qt::CaseInsensitive))
        {
            this->setReplyTimeout(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"Retries", Qt::CaseInsensitive))
        {
            this->setRetries(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"DataTabNumAndTagPrope", Qt::CaseInsensitive))
        {
            QDomElement tabNOElement = element.firstChildElement();
            qint16 tempTabNO = 0;
            if(0==QString::compare(tabNOElement.tagName(),"DataTabNO", Qt::CaseInsensitive))
            {
                tempTabNO=(qint16)tabNOElement.text().toInt();
            }
            IDAndPro idTabMapTemp;
            idTabMapTemp.clear();

            while(!tabNOElement.isNull())
            {
                QDomElement varElement = tabNOElement.firstChildElement();

                while( !varElement.isNull() )
                {
                     QDomElement varPropeElement = varElement.firstChildElement();
                     qint16 tempID = 0;
                     QString strTempName;
                     qint16 tempType;
                     qint16 tempLength;
                     while(!varPropeElement.isNull())
                     {
                         if(0==QString::compare(varPropeElement.tagName(),"ID", Qt::CaseInsensitive))
                         {
                             tempID = (qint16)varPropeElement.text().toInt();
                         }
                         else if(0==QString::compare(varPropeElement.tagName(),"Name", Qt::CaseInsensitive))
                         {
                             strTempName= varPropeElement.text();
                         }
                         else if(0==QString::compare(varPropeElement.tagName(),"Type", Qt::CaseInsensitive))
                         {
                             tempType=(qint16) varPropeElement.text().toInt();
                         }
                         else if(0==QString::compare(varPropeElement.tagName(),"Length", Qt::CaseInsensitive))
                         {
                             tempLength= (qint16) varPropeElement.text().toInt();
                         }
                         varPropeElement = varPropeElement.nextSiblingElement();
                     }
                     MDVarPrope *pVarPrope = new MDVarPrope();
                     pVarPrope->m_tagID = tempID;
                     pVarPrope->m_tagName= strTempName;
                     pVarPrope->m_tagTyp = tempType;
                     pVarPrope->m_tagLength = tempLength;

                     idTabMap.insert(tempID,pVarPrope);
                     idTabMapTemp.insert(tempID,pVarPrope);
                     varElement = varElement.nextSiblingElement();
                }

                tabNOElement = tabNOElement.nextSiblingElement();
            }

            dataTabNOAndPrope.insert(tempTabNO,idTabMapTemp);
            if(!dataTabNOAndPrope.isEmpty())
            {
                m_configFlag = true;
            }
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
    this->setCommParameter(this->commParameterStringToIndex(json.value("CommParameter").toString("Client,127.0.0.1,502,,;Client,,,,")));
    this->setReplyTimeout_Pro(json.value("ReplyTimeout").toString("1000"));
    this->setRetries_Pro(json.value("Retries").toString("3"));
    QJsonArray array_TabNumAndTagPrope = json.value("DataTabNumAndTagPrope").toArray();
    qint16 tempTabNO;
    for(int i=0;i<array_TabNumAndTagPrope.count();i++)
    {
        QJsonObject json_TabNo = array_TabNumAndTagPrope.at(i).toObject();
        tempTabNO = json_TabNo.value("DataTabNO").toString("-1").toInt();
        IDAndPro idTabMapTemp;
        idTabMapTemp.clear();
        QJsonArray array_Vars = json_TabNo.value("VarTables").toArray();
        for(int j=0;j<array_Vars.count();j++)
        {
            QJsonObject json_Var = array_Vars.at(j).toObject();
            qint16 tempID = (qint16)json_Var.value("ID").toString("-1").toInt();
            QString strTempName = json_Var.value("Name").toString("");
            qint16 tempType = (qint16)json_Var.value("Type").toString("-1").toInt();
            qint16 tempLength = (qint16)json_Var.value("Length").toString("-1").toInt();

            MDVarPrope *pVarPrope = new MDVarPrope();
            pVarPrope->m_tagID = tempID;
            pVarPrope->m_tagName= strTempName;
            pVarPrope->m_tagTyp = tempType;
            pVarPrope->m_tagLength = tempLength;

            idTabMap.insert(tempID,pVarPrope);
            idTabMapTemp.insert(tempID,pVarPrope);
        }
        dataTabNOAndPrope.insert(tempTabNO,idTabMapTemp);
    }
    if(!dataTabNOAndPrope.isEmpty())
    {
        m_configFlag = true;
    }
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
void CMDDevice::getTabVar(QString serverIP,qint16 serverPort,QString serverSlaveIP,qint16 serverSlavePort)
{
    if(NULL!=m_pVarTable)
    {
        delete m_pVarTable;
        m_pVarTable = NULL;
    }

    m_pVarTable = new MDVarTable(this);
    if(NULL!=m_pVarTable)
    {
         for( IDAndPro::iterator i=idTabMap.begin();i != idTabMap.end();++i)
         {
            MDVarPrope*pVarPrope = i.value();
            if(NULL!=pVarPrope)
            {
                delete pVarPrope;
                pVarPrope = NULL;
             }
         }
         idTabMap.clear();
        nameTabMap.clear();
        tabNumTagMap.clear();
        dataTabNOAndPrope.clear();

        m_pVarTable->setParameter(serverIP,serverPort,serverSlaveIP,serverSlavePort);

        m_pVarTable->Connect();

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
    else if(0==QString::compare(strcommtype,"UDP",Qt::CaseInsensitive))
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
    m_propertyTip.insert(QString("SlaveTcpOrUdpRemotePort"),QString("服务端口"));
    m_propertyTip.insert(QString("TcpOrUdpLocalPort"),QString("本地服务端口"));

    m_translateName.insert(QString("SlaveClientOrServer"),QString("从站客户端/服务端"));
    m_translateName.insert(QString("SlaveTcpOrUdpIP"),QString("从站IP地址"));
    m_translateName.insert(QString("SlaveTcpOrUdpRemotePort"),QString("从站服务端口"));
    m_translateName.insert(QString("SlaveTcpOrUdpLocalPort"),QString("从站本地服务端口"));

    m_propertyTip.insert(QString("SlaveClientOrServer"),QString("从站客户端/服务端"));
    m_propertyTip.insert(QString("SlaveTcpOrUdpIP"),QString("从站IP地址(例:格式是127.0.0.1)"));
    m_propertyTip.insert(QString("SlaveTcpOrUdpRemotePort"),QString("从站服务端口"));
    m_propertyTip.insert(QString("SlaveTcpOrUdpLocalPort"),QString("从站本地服务端口"));
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
    //!站号
    m_translateName.insert(QString("Station"),QString("站号"));
    m_propertyTip.insert(QString("Station"),QString("站号"));
    m_translateName.insert(QString("Config"),QString("设备配置"));
    m_propertyTip.insert(QString("Config"),QString("设备配置"));

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
//***********************************************************************//
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
                m_commParameter=m_commParameter +m_tcpOrUdpLocalPort+";";
                if(m_slaveClientOrServer == MDClientOrServer_Pro::Client)
                {
                    m_commParameter=m_commParameter +"Client"+",";
                }
                else
                {
                    m_commParameter=m_commParameter +"Server"+",";
                }
                m_commParameter=m_commParameter +m_slaveTcpOrUdpIP+",";
                m_commParameter=m_commParameter +m_slaveTcpOrUdpRemotePort+",";
                m_commParameter=m_commParameter +""+",";
                m_commParameter=m_commParameter +m_slaveTcpOrUdpLocalPort;
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
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[0].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(0,strTemp);
            strListTempMasterAndSlave[0] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
    }
}
QString CMDDevice::tcpOrUdpIP_Pro()
{
    return m_tcpOrUdpIP;
}
void CMDDevice::setTcpOrUdpIP_Pro(QString ip)
{
    m_tcpOrUdpIP = ip;
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[0].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(1,m_tcpOrUdpIP);
            strListTempMasterAndSlave[0] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
    }
}
QString CMDDevice::tcpOrUdpLocalPort_Pro()
{
    return m_tcpOrUdpLocalPort;
}
void CMDDevice::setTcpOrUdpLocalPort_Pro(QString localPort)
{
    m_tcpOrUdpLocalPort = localPort;
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[0].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(4,m_tcpOrUdpLocalPort);
            strListTempMasterAndSlave[0] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
    }
}
QString CMDDevice::tcpOrUdpRemotePort_Pro()
{
    return m_tcpOrUdpRemotePort;
}
void CMDDevice::setTcpOrUdpRemotePort_Pro(QString remotePort)
{
    m_tcpOrUdpRemotePort = remotePort;
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[0].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(2,m_tcpOrUdpRemotePort);
            strListTempMasterAndSlave[0] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
    }
}

CMDDevice::MDClientOrServer_Pro CMDDevice::slaveClientOrServer_Pro()
{
    return m_slaveClientOrServer;
}
void CMDDevice::setSlaveClientOrServer_Pro(CMDDevice::MDClientOrServer_Pro slaveClientOrServer)
{
    m_slaveClientOrServer = slaveClientOrServer;
    QString strTemp = "";
    if(m_slaveClientOrServer == MDClientOrServer_Pro::Client)
    {
        strTemp="Client";
    }
    else
    {
        strTemp="Server";
    }
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[1].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(0,strTemp);
            strListTempMasterAndSlave[1] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
    }
}
QString CMDDevice::slaveTcpOrUdpIP_Pro()
{
    return m_slaveTcpOrUdpIP;
}
void CMDDevice::setSlaveTcpOrUdpIP_Pro(QString slaveIp)
{
    m_slaveTcpOrUdpIP = slaveIp;
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[1].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(1,m_slaveTcpOrUdpIP);
            strListTempMasterAndSlave[1] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
    }
}
QString CMDDevice::slaveTcpOrUdpLocalPort_Pro()
{
    return m_slaveTcpOrUdpLocalPort;
}
void CMDDevice::setSlaveTcpOrUdpLocalPort_Pro(QString slaveLocalPort)
{
    m_slaveTcpOrUdpLocalPort = slaveLocalPort;
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[1].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(4,m_slaveTcpOrUdpLocalPort);
            strListTempMasterAndSlave[1] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
    }
}

QString CMDDevice::slaveTcpOrUdpRemotePort_Pro()
{
    return m_slaveTcpOrUdpRemotePort;
}
void CMDDevice::setSlaveTcpOrUdpRemotePort_Pro(QString slaveRemotePort)
{
    m_slaveTcpOrUdpRemotePort = slaveRemotePort;
    QStringList strListTempMasterAndSlave = m_commParameter.split(";");
    if(strListTempMasterAndSlave.count()>=2)
    {
        QStringList strListTemp = strListTempMasterAndSlave[1].split(",");
        if(strListTemp.count()>=5)
        {
            strListTemp.replace(2,m_slaveTcpOrUdpRemotePort);
            strListTempMasterAndSlave[1] = strListTemp.join(",");
            m_commParameter = strListTempMasterAndSlave.join(";");
        }
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
    pProperty->setSinglePropertyHide("SlaveClientOrServer",true);
    pProperty->setSinglePropertyHide("SlaveTcpOrUdpIP",true);
    pProperty->setSinglePropertyHide("SlaveTcpOrUdpRemotePort",true);
    pProperty->setSinglePropertyHide("SlaveTcpOrUdpLocalPort",true);
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
        pProperty->setSinglePropertyHide("SlaveClientOrServer",false);
        pProperty->setSinglePropertyHide("SlaveTcpOrUdpIP",false);
        pProperty->setSinglePropertyHide("SlaveTcpOrUdpRemotePort",false);
        pProperty->setSinglePropertyHide("SlaveTcpOrUdpLocalPort",false);
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
QString CMDDevice::communicationProtocol_Pro()
{
    return m_commProtocol_Pro;
}
//!对话框
QString CMDDevice::DILconfig_Pro()
{
    return m_config_Pro;
}
void CMDDevice::setDILconfig_Pro(QString config)
{
    CMDDeviceParameterGetConfig dlg;
    dlg.setDevicePoint(this);
    dlg.exec();
}
//***********************************************************************//

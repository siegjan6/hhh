#include "MDDataBlock.h"
#include <QStringList>
#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include <QJsonObject>
#include "PropertyBrowser.h"
#include "MDRegisterAndAddressConfigTab.h"
#include <QVariant>

#ifdef Q_OS_WIN
    //#pragma execution_character_set("utf-8")   //解决 VS编译器下中文乱码
#endif

CMDDataBlock::CMDDataBlock()
{
     m_dataBufferSize=0;
     m_dataBuffer=NULL;
     m_overallPackage = 0;
     m_currentPackage = 0;
     setEnabled(true);
     m_packDBCount = 0;
     m_currentDBPack = 0;
     //*********************属性框变量*********************//
     this->initTranslateNameAndPropertyType();
     m_registersConfig_Pro = "";
     //**************************************************//
}
CMDDataBlock::~CMDDataBlock()
{
    if(NULL !=m_dataBuffer)
    {
        delete []m_dataBuffer;
    }
    listRegistersCongigClear();
    mapDataRegistersConfig.clear();
    mapInitialPackage.clear();
}

bool CMDDataBlock::start()
{
    //this->setCurrentPackage(0);
    if(verifyFlag() != FLAG_RIGHT)
    {
        setQuality(MDVariableDataQuality::DataBlockConfigError);
        refreshVariablesQuality();
    }
    return CBaseDataBlock::start();
}

bool CMDDataBlock::stop()
{
    return CBaseDataBlock::stop();
}
void CMDDataBlock::copy(IDataBlock *otherDataBlock)
{
    if(NULL== otherDataBlock)
    {
        return;
    }
    CBaseDataBlock::copy(otherDataBlock);
    ((CMDDataBlock*)otherDataBlock)->listRegistersConfig(this->listDataRegistersConfig);
    this->overallVerify(listDataRegistersConfig);
    if(verifyFlag()==FLAG_RIGHT)
    {
        this->setMapRegistersConfig(listDataRegistersConfig);
        this->setInitialMapPackage(listDataRegistersConfig);
    }

 }

bool CMDDataBlock::getDataBufferUShortValue(qint16 internalAddress,  quint16 &data)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);

    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }

    if (internalAddress < m_dataBufferSize-1 && internalAddress>=0)
    {
        quint8 tempbuffer[4];
        tempbuffer[0] = m_dataBuffer[internalAddress];
        tempbuffer[1] =  m_dataBuffer[internalAddress + 1];
        memcpy(&data,tempbuffer,2);
        bRet = true;
    }
    return bRet;
}

//获取数据缓冲区
bool CMDDataBlock::getDataBuffer(quint8* dataBuf, qint32 offset, qint32 length)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);
    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }
    memcpy(dataBuf,m_dataBuffer+offset,length);

    bRet = true;
    return bRet;
}

qint32 CMDDataBlock::getDataBufferSize()
{
    return m_dataBufferSize;
}
void CMDDataBlock::calculateDataBufferSize(qint32 bufferSize)
{
    if(NULL == m_dataBuffer)
    {
        m_dataBufferSize = bufferSize;
        m_dataBuffer = new quint8[m_dataBufferSize];
    }
    else
    {
        if(bufferSize!=m_dataBufferSize)
        {
            delete m_dataBuffer;
            m_dataBuffer = NULL;
            m_dataBufferSize = bufferSize;
            m_dataBuffer = new quint8[m_dataBufferSize];
        }
    }

}

bool CMDDataBlock::checkVarAddress(IMDRTDBVariable* nsVar,QString &error)
{
    bool ret = false;
    if (NULL == nsVar)
    {
        return ret;
    }
    if (nsVar->address().isNull() || nsVar->address().isEmpty())
    {
        error = tr("变量地址不能为空!");
        return ret;
    }
    if(verifyFlag()!=FLAG_RIGHT)
    {
        error = tr("数据块配置错误!");
        return ret;
    }
    QT_TRY
    {
        nsVar->setInternalAddress(-1);
        nsVar->setInternalSubAddress(-1);
        QStringList addressSplit = nsVar->address().split(".");
        if (addressSplit.count() < 2)
        {
            error = tr("变量地址格式错误!");
            return ret;
        }

        QString strRegisterType=addressSplit[1];
        if(strRegisterType.right(1) == "]")
            strRegisterType = strRegisterType.left(strRegisterType.length()-1);
        else
        {
            error = tr("变量地址中寄存器错误!");
            return ret;
        }
        QStringList registerAndAddress = strRegisterType.split("[");
        int nAddressFormat = 0;
        int verifyRegisterResult = this->verifyRegisterFormat(registerAndAddress[0],nAddressFormat);
        if( verifyRegisterResult!=REGISTER_FORMAT_RIGHT)
        {
            error = tr("变量地址中寄存器错误!");
            return ret;
        }
        qint32 varAddress = 0;
        int verifyVarAddressResult = this->verifyAddressFormat(registerAndAddress[1],varAddress,nAddressFormat);

        if(verifyVarAddressResult!=ADDRESS_FORMAT_RIGHT)
        {
            error = tr("变量地址格式错误!");
            return ret;
        }

        if(registerAndAddress[0] == "X"
                || registerAndAddress[0] == "Y"
                    || registerAndAddress[0] == "S"
                        || registerAndAddress[0] == "M"
                            || registerAndAddress[0] == "SPM"
                                || registerAndAddress[0] == "TS"
                                    || registerAndAddress[0] == "CS16"
                                        || registerAndAddress[0] == "CS32")
        {
            if(nsVar->originalDataType() !=MDDataType::Bool)
            {
                error = tr("变量类型错误!");
                return ret;
            }
            else
            {
                int offset = 0;
                int subOffset = 0;
                bool verifyVarAddress = this->verifyVarAddress(registerAndAddress[0],varAddress,offset,subOffset);
                if(verifyVarAddress == 0)
                {
                    error = tr("变量地址超限");
                    return ret;
                }
                else
                {
                    ret = true;
                    nsVar->setInternalAddress(offset);
                    nsVar->setInternalSubAddress(subOffset);
                }
            }

        }
        else if(registerAndAddress[0] == "T"
                    ||registerAndAddress[0] == "C32")
        {
            int offset = 0;
            int subOffset = 0;
            bool verifyVarAddress = this->verifyVarAddress(registerAndAddress[0],varAddress,offset,subOffset);
            if(verifyVarAddress == 0)
            {
                error = tr("变量地址超限");
                return ret;
            }
            if(nsVar->originalDataType() ==MDDataType::Bool)
            {
                if(addressSplit.count()<3)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                else
                {
                    subOffset = addressSplit[2].toInt();
                    if(subOffset>31)
                    {
                        subOffset = 31;
                    }
                    ret = true;
                    nsVar->setInternalAddress(offset);
                    nsVar->setInternalSubAddress(subOffset);
                }
            }
            else if(nsVar->originalDataType() ==MDDataType::Int32||nsVar->originalDataType() ==MDDataType::UInt32)
            {
                ret = true;
                nsVar->setInternalAddress(offset);
            }
            else
            {
                error = tr("变量类型错误!");
                return ret;
            }

        }
        else
        {
            int offset = 0;
            int subOffset = 0;
            bool verifyVarAddress = this->verifyVarAddress(registerAndAddress[0],varAddress,offset,subOffset);
            if(verifyVarAddress == 0)
            {
                error = tr("变量地址超限");
                return ret;
            }
            if(nsVar->originalDataType() ==MDDataType::Bool)
            {
                if(addressSplit.count()<3)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                else
                {
                    subOffset = addressSplit[2].toInt();
                    if(subOffset>15)
                    {
                        subOffset = 15;
                    }
                    ret = true;
                    nsVar->setInternalAddress(offset);
                    nsVar->setInternalSubAddress(subOffset);
                }
            }
            else if(nsVar->originalDataType() ==MDDataType::Int16||nsVar->originalDataType() ==MDDataType::UInt16)
            {
                ret = true;
                nsVar->setInternalAddress(offset);
            }
            else if(nsVar->originalDataType() ==MDDataType::Int32||nsVar->originalDataType() ==MDDataType::UInt32||nsVar->originalDataType() ==MDDataType::Float)
            {
                int offset32 = 0;
                int subOffset32 = 0;
                bool verifyVarAddress32 = this->verifyVarAddress(registerAndAddress[0],varAddress+1,offset32,subOffset32);
                if(verifyVarAddress32==0)
                {
                    error = tr("变量地址超限");
                    return ret;
                }
                ret = true;
                offset = (offset32/2-1)*2;
                nsVar->setInternalAddress(offset);
            }
            else if(nsVar->originalDataType() ==MDDataType::String)
            {
                int offsetStr = 0;
                int subOffsetStr = 0;
                bool verifyVarAddressStr = this->verifyVarAddress(registerAndAddress[0],varAddress+1,offsetStr,subOffsetStr);

                if(verifyVarAddressStr==0)
                {
                    error = tr("变量地址超限");
                    return ret;
                }
                this->insertToVarForStringMap(nsVar->name(),registerAndAddress[0],varAddress,offsetStr);
                ret = true;
                nsVar->setInternalAddress(offsetStr);
            }
            else
            {
                error = tr("变量类型错误!");
                return ret;
            }
        }


    }
    QT_CATCH (...)
    {

    }
    return ret;
}

void CMDDataBlock::initialVariables()
{
    QString error;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        checkVarAddress(getVariableAt(varIndex),error) ;
    }
}

void CMDDataBlock::refreshVariables()
{
    if (m_dataBufferSize<=0)
    {
        return;
    }
    int changeVarCount =0;
    IMDRTDBVariable *lastChangeVar = NULL;
    //!变化变量列表
    QList<IMDRTDBVariable*> changevarList;

    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        IMDRTDBVariable *nsVar = getVariableAt(varIndex);
        if (NULL == nsVar)
        {
            continue;
        }
        if(nsVar->internalAddress()< 0||nsVar->internalAddress()>=m_dataBufferSize)
        {
            continue;
        }
        QVariant varDataValue;
        bool bRightValue = false;
        bool bDataTypeErr = false;//!数据类型标志位
        unsigned char tempBuffer[4];
        QStringList addressSplit = nsVar->address().split(".");

        QString strRegisterType=addressSplit[1];

        QStringList registerAndAddress = strRegisterType.split("[");
        if(registerAndAddress[0] == "X"
                || registerAndAddress[0] == "Y"
                    || registerAndAddress[0] == "S"
                        || registerAndAddress[0] == "M"
                            || registerAndAddress[0] == "SPM"
                                || registerAndAddress[0] == "TS"
                                    || registerAndAddress[0] == "CS16"
                                        || registerAndAddress[0] == "CS32")
        {
            if (nsVar->originalDataType() == MDDataType::Bool)
            {
                quint32 tempValue = 0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],4);
                quint32 dataValue = CBitFuntons::getBit(tempValue,nsVar->internalSubAddress());

                if (dataValue > 0)
                {
                    varDataValue = true;
                }
                else
                {
                    varDataValue = false;
                }
                bRightValue = true;
            }
            else
            {
                bDataTypeErr = true;
            }

        }
        else if(registerAndAddress[0] == "T"
                    ||registerAndAddress[0] == "C32")
        {
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                quint32 tempValue = 0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],4);
                quint32 dataValue = CBitFuntons::getBit(tempValue,nsVar->internalSubAddress());

                if (dataValue > 0)
                {
                    varDataValue = true;
                }
                else
                {
                    varDataValue = false;
                }
                bRightValue = true;
            }
                break;
            case MDDataType::Int32:
            {
                qint32 tempValue=0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::UInt32:
            {
                quint32 tempValue=0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            default:
                bDataTypeErr = true;
                break;
            }
        }
        else
        {
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                quint16 tempValue = 0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],2);
                quint32 dataValue = CBitFuntons::getBit((quint32)tempValue,nsVar->internalSubAddress());

                if (dataValue > 0)
                {
                    varDataValue = true;
                }
                else
                {
                    varDataValue = false;
                }
                bRightValue = true;
            }
                break;
            case MDDataType::Int16:
            {
                qint16 tempValue=0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],2);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::UInt16:
            {
                quint16 tempValue=0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],2);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::Int32:
            {
                qint32 tempValue=0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::Float:
            {
                float tempValue=0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::UInt32:
            {
                quint32 tempValue=0;
                memcpy(&tempValue,&m_dataBuffer[nsVar->internalAddress()],4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::String:
            {
                QString tempValue((char*)&m_dataBuffer[nsVar->internalAddress()+2]);
                QString tempValue1 = "";
                qint32 tempEndAddress = this->endAddressFromVarForStringMap(nsVar->name());
                quint16 defineLength = 0;
                memcpy(&defineLength,&m_dataBuffer[nsVar->internalAddress()],2);
                if(registerAndAddress[1].right(1) == "]")
                    registerAndAddress[1] = registerAndAddress[1].left(registerAndAddress[1].length()-1);
                qint32 varAddress = (qint32)registerAndAddress[1].toInt();
                if(defineLength>(tempEndAddress - varAddress)*2)
                {
                    if(tempValue.size()>(tempEndAddress - varAddress)*2)
                    {
                        tempValue1 = tempValue.left((int)(tempEndAddress - varAddress)*2);
                    }
                    else
                    {
                        tempValue1 = tempValue;
                    }
                }
                else
                {
                    if(tempValue.size()>defineLength)
                    {
                        tempValue1 = tempValue.left((int)(defineLength));
                    }
                    else
                    {
                        tempValue1 = tempValue;
                    }
                }

                varDataValue =tempValue1;
                bRightValue = true;

            }
                break;
            default:
                bDataTypeErr = true;
                break;
            }

        }
        if (true == bRightValue)
        {
           if(nsVar->refreshData(varDataValue, timeStamp(), quality(), false))
           {
               lastChangeVar = nsVar;
               changeVarCount++;
               changevarList.append(nsVar);
           }
        }
        else if(bDataTypeErr == true)
        {
            nsVar->refreshData(varDataValue, timeStamp(), MDVariableDataQuality::DataTypeError, true);
        }
    }
    if(changeVarCount>0 && lastChangeVar!=NULL)
    {
        lastChangeVar->notifyDeviceVariablesChange(changevarList);
        changevarList.clear();
    }
}

void CMDDataBlock::refreshVariablesQuality()
{
    IMDRTDBVariable *lastChangeVar = NULL;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        IMDRTDBVariable *nsVar = getVariableAt(varIndex);
        if (NULL != nsVar)
        {
            lastChangeVar =nsVar;
            nsVar->refreshData(QVariant(), timeStamp(), quality(), true);
        }
    }
    if(lastChangeVar!=NULL)
    {
        QList<IMDRTDBVariable*> changevarList;
        lastChangeVar->notifyDeviceVariablesChange(changevarList,true);
        changevarList.clear();
    }
}


CBaseProtocolCommand *CMDDataBlock::createCommand(ProtocolCommandType cmdType)
{
    CMDProtocolCommand *cmd = new CMDProtocolCommand();
    cmd->setChannel (getChannel());
    cmd->setDataBlock (this);
    cmd->setDevice(device());
    cmd->setCommandType(cmdType);
    cmd->setCommandResult(ProtocolCommandResult::None);
    return cmd;
}
//分析命令，将数据拷贝到数据区，并更新变量
void CMDDataBlock::parseCommand(CBaseProtocolCommand *cmd)
{
    if(NULL == cmd )
    {
        return;
    }
    if(cmd->getCommandType() == ProtocolCommandType::Read)
    {
        if (cmd->getCommandResult() == ProtocolCommandResult::Right)
        {
           CMDDevice *dev = (CMDDevice*)cmd->getDevice();
           if(dev)
           {
               if(dev->communicationProcessFlag() == PROCESS_GETCONFIG)
               {
                    qint32 nMaxPoint = 0;

                    bool verifyFlag = 1;

                    for(int currentRegister = 0;currentRegister<10;currentRegister++)
                    {
                        QString strTempFindRegister ="";
                        QString strTempFindStatusRegister = "";
                        switch (currentRegister)
                        {
                        case ID_REG_RANGE_ORDER_X:
                        {
                            strTempFindRegister = "X";
                        }
                            break;
                        case ID_REG_RANG_ORDER_Y:
                        {
                            strTempFindRegister = "Y";
                        }
                            break;
                        case ID_REG_RANG_ORDER_S:
                        {
                            strTempFindRegister = "S";
                        }
                            break;
                        case ID_REG_RANG_ORDER_M:
                        {
                            strTempFindRegister = "M";
                        }
                            break;
                        case ID_REG_RANG_ORDER_SPM:
                        {
                            strTempFindRegister = "SPM";
                        }
                            break;
                        case ID_REG_RANG_ORDER_T:
                        {
                            strTempFindRegister = "T";
                            strTempFindStatusRegister = "TS";
                        }
                            break;
                        case ID_REG_RANG_ORDER_C16:
                        {
                            strTempFindRegister = "C16";
                            strTempFindStatusRegister = "CS16";
                        }
                            break;
                        case ID_REG_RANG_ORDER_C32:
                        {
                            strTempFindRegister = "C32";
                            strTempFindStatusRegister = "CS32";
                        }
                            break;
                        case ID_REG_RANG_ORDER_D:
                        {
                            strTempFindRegister = "D";
                        }
                            break;
                        case ID_REG_RANG_ORDER_SPD:
                        {
                            strTempFindRegister = "SPD";
                        }
                            break;
                        default:

                            break;
                        }
                        memcpy(&nMaxPoint,cmd->getReceiveData()+44+currentRegister*4,4);
                        verifyFlag = this->verifyRegisterAddress(nMaxPoint,strTempFindRegister,strTempFindStatusRegister);
                        if(verifyFlag == 0)
                        {
                            break;
                        }
                    }
                    if(verifyFlag == 0)
                    {
                        setVerifyFlag(FLAG_REGISTER_ERROR);
                        setQuality(MDVariableDataQuality::DataBlockConfigError);
                        refreshVariablesQuality();

                    }
                    else
                    {
                        dev->setCommunicationProcessFlag(PROCESS_INITIAL);
                        this->onReceiveCommand(true, cmd->getCommandResult());
                    }

               }
               else if(dev->communicationProcessFlag() == PROCESS_GETDATA)
               {
                   QMutexLocker locker(&m_dataBufferMutex);
//                   if(m_dataBuffer==NULL)
//                   {
//                        m_dataBuffer = new quint8[m_dataBufferSize];
//                   }
                   this->onReceiveCommand(true, cmd->getCommandResult());
                   refreshVariables();
               }
           }
        }
        else
        {
           MDVariableDataQuality oldQuality = quality();
           this->onReceiveCommand(true, cmd->getCommandResult());
           if (oldQuality != quality())
           {
               refreshVariablesQuality();
           }
        }
    }
    else
    {
        this->onReceiveCommand(false, cmd->getCommandResult());
    }

}
void CMDDataBlock::getVariableAddressList(QStringList &varAddressList)
{
    if(verifyFlag()==FLAG_RIGHT)
    {
        QString addressTemp = "";
        for(int i = 0;i <listDataRegistersConfig.count();i++)
        {
            addressTemp = QString("%1.%2[%3]--%1.%2[%4]").arg(name())
                    .arg(listDataRegistersConfig.at(i)->m_strRegister)
                    .arg(listDataRegistersConfig.at(i)->m_strStartAddress)
                    .arg(listDataRegistersConfig.at(i)->m_strEndAddress);
            varAddressList.append(addressTemp);
        }
    }
    else
    {
        QString addressTemp = QString("数据块%1配置错误!").arg(name());
        varAddressList.append(addressTemp);
    }

}
void CMDDataBlock::saveConfiguration(QDomDocument &doc,QDomElement &parentElement)
{
    if(doc.isNull()||parentElement.isNull())
    {
        return;
    }
    QDomElement variableElement = doc.createElement("DataBlock");
    parentElement.appendChild(variableElement);

    QDomElement element = doc.createElement("Name");
    variableElement.appendChild(element);
    QDomText elementText = doc.createTextNode(this->name());
    element.appendChild(elementText);

    element = doc.createElement("Description");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->description());
    element.appendChild(elementText);

    element = doc.createElement("Enabled");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->isEnabled()?"1":"0");
    element.appendChild(elementText);

    element = doc.createElement("Registers");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->listRegistersConfigToFile());
    element.appendChild(elementText);

    element = doc.createElement("PollRate");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->pollRate()));
    element.appendChild(elementText);

}
void CMDDataBlock::serialize(CSortJsonObject &json)
{
    json.insertSimpleValue("Name",this->name());
    json.insertSimpleValue("Description",this->description());
    json.insertSimpleValue("Enabled",this->isEnabled()?QString("1"):QString("0"));
    json.insertSimpleValue("PollRate",QString("%1").arg(this->pollRate()));
    QString strConfig = this->listRegistersConfigToFile();
    CSortJsonArray configArray;
    if(strConfig!="")
    {
        QStringList strlistConfig = strConfig.split(";");
        if(strlistConfig.count()>0)
        {

            for(int i=0;i<strlistConfig.count();i++)
            {
                CSortJsonObject subJson;
                QStringList strlistSubConfig = strlistConfig.at(i).split(",");
                subJson.insertSimpleValue("RegisterType",strlistSubConfig.at(0));
                subJson.insertSimpleValue("StartAddress",strlistSubConfig.at(1));
                subJson.insertSimpleValue("EndAddress",strlistSubConfig.at(2));
                configArray.insertObject(subJson);
            }
            json.insertArray("Registers",configArray);
        }
    }
    else
    {
        json.insertArray("Registers",configArray);
    }
}

void CMDDataBlock::readConfiguration(QDomElement &domElement)
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
        else if(0==QString::compare(element.tagName(),"Registers", Qt::CaseInsensitive))
        {
            QString str = element.text();
            str.replace("\r","");
            str.replace("\n","");
            str.replace("\t","");
            str.replace(" ", "");
            this->setListRegistersConfigDownLoad(str);
        }
        else if(0==QString::compare(element.tagName(),"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(element.text().toInt());
        }
        element = element.nextSiblingElement();
    }
    this->overallVerify(listDataRegistersConfig);
    if(verifyFlag()==FLAG_RIGHT)
    {
        this->setMapRegistersConfig(listDataRegistersConfig);
        this->setInitialMapPackage(listDataRegistersConfig);
    }

}
void CMDDataBlock::deserialize(const QJsonObject &json)
{
    this->setName(json.value("Name").toString(""));
    this->setDescription(json.value("Description").toString(""));
    this->setEnabled(json.value("Enabled").toString("1").toInt());
    QString strConfig="";
    QJsonArray configArray = json.value("Registers").toArray();
    for(int i=0;i<configArray.count();i++)
    {
        QJsonObject subJson = configArray.at(i).toObject();
        if(i+1==configArray.count())
        {
            strConfig = strConfig + subJson.value("RegisterType").toString("")+",";
            strConfig = strConfig + subJson.value("StartAddress").toString("")+",";
            strConfig = strConfig + subJson.value("EndAddress").toString("");
        }
        else if(i==0)
        {
            strConfig = strConfig + subJson.value("RegisterType").toString("")+",";
            strConfig = strConfig + subJson.value("StartAddress").toString("")+",";
            strConfig = strConfig + subJson.value("EndAddress").toString("")+";";
        }
        else
        {
            strConfig = strConfig + subJson.value("RegisterType").toString("")+",";
            strConfig = strConfig + subJson.value("StartAddress").toString("")+",";
            strConfig = strConfig + subJson.value("EndAddress").toString("")+";";
        }

    }
    this->setListRegistersConfigDownLoad(strConfig);
    this->setPollRate(json.value("PollRate").toString("1000").toInt());

    this->overallVerify(listDataRegistersConfig);

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->setMapRegistersConfig(listDataRegistersConfig);
        this->setInitialMapPackage(listDataRegistersConfig);
    }
}

void CMDDataBlock::dynamicSetParameter(const QVariantMap &dataBlockMap)
{
    QString dataBlockID =dataBlockMap["@DataBlockID"].toString();
    dataBlockID.trimmed();
    if(dataBlockID.isNull()||dataBlockID.isEmpty())
    {
        return;
    }
    this->setName(dataBlockID);

    QVariantList dataBlockParameterList =  dataBlockMap["MDDevicesCommunicateDataBlockParameter"].toList();
    for(int i=0;i<dataBlockParameterList.count();i++)
    {
        QVariantMap commParameterMap = dataBlockParameterList[i].toMap();
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
        else if(0==QString::compare(parameterName,"Registers", Qt::CaseInsensitive))
        {
            QString str = parameterValue;
            str.replace("\r","");
            str.replace("\n","");
            str.replace("\t","");
            str.replace(" ", "");
            this->setListRegistersConfigDownLoad(str);
        }
        else if(0==QString::compare(parameterName,"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(parameterValue.toInt());
        }
    }
    this->overallVerify(listDataRegistersConfig);
    if(verifyFlag()==FLAG_RIGHT)
    {
        this->setMapRegistersConfig(listDataRegistersConfig);
        this->setInitialMapPackage(listDataRegistersConfig);
    }
}
int CMDDataBlock::verifyRegisterFormat(QString strRegister,int &addressFormat)
{
    if(strRegister == "X" || strRegister == "Y")
    {
        addressFormat = 8;
    }
    else if(strRegister == "S"
                ||strRegister == "M"
                    ||strRegister == "SPM"
                        ||strRegister == "D"
                            ||strRegister == "SPD"
                                ||strRegister == "T"
                                    ||strRegister == "TS"
                                        ||strRegister == "C16"
                                            ||strRegister == "C32"
                                                ||strRegister == "CS16"
                                                    ||strRegister == "CS32")
    {
        addressFormat = 10;
    }
    else
    {
      return REGISTER_FORMAT_ERROR;//!寄存器格式校验错误
    }
    return REGISTER_FORMAT_RIGHT;
}

int CMDDataBlock::verifyAddressFormat(QString strAddress,qint32 &nAddress,qint16 addressFormat)
{
    bool okAddress = false;
    qint32 tempAddress = 0;

    tempAddress = strAddress.toInt(&okAddress,addressFormat);

    if((okAddress == false)||(tempAddress<0))
        return ADDRESS_FORMAT_ERROR;//!地址格式校验错误
    else
        nAddress = tempAddress;
    return ADDRESS_FORMAT_RIGHT;
}
int CMDDataBlock::verifyLength(qint32 startAddress,qint32 endAddress)
{
    if(startAddress > endAddress)
    {
        return LENGTH_NEGATIVENUMBER_ERROR;//!开始地址大于结束地址
    }
    return LENGTH_RIGHT;
}
void CMDDataBlock::overallVerify(listNoOrderRegistersConfig &config)
{
    if(config.isEmpty())
    {
        setVerifyFlag(FLAG_REGISTER_ERROR);
        return;
    }
    for(int i = 0;i<config.count();i++)
    {
        int nAddressFormat;

        int nVerifyRegister = this->verifyRegisterFormat(config.at(i)->m_strRegister,nAddressFormat);
        if(nVerifyRegister!=REGISTER_FORMAT_RIGHT)
        {
            setVerifyFlag(FLAG_REGISTER_ERROR);
            return;
        }
        else
        {
            qint32 nStartAddress = 0;
            int nVerifyStartAddress=this->verifyAddressFormat(config.at(i)->m_strStartAddress,nStartAddress,nAddressFormat);
            if(nVerifyStartAddress!=ADDRESS_FORMAT_RIGHT)
            {
                setVerifyFlag(FLAG_STARTADDRESS_ERROR);
                return;
            }
            qint32 nEndAddress = 0;
            int nVerifyEndAddress=this->verifyAddressFormat(config.at(i)->m_strEndAddress,nEndAddress,nAddressFormat);

            if(nVerifyEndAddress!=ADDRESS_FORMAT_RIGHT)
            {
                setVerifyFlag(FLAG_ENDADDRESS_ERROR);
                return;
            }

            int nVerifyLength = this->verifyLength(nVerifyStartAddress,nVerifyEndAddress);
            if(nVerifyLength==LENGTH_NEGATIVENUMBER_ERROR)
            {
                setVerifyFlag(FLAG_LENGT_NEGATIVE_ERROR);
                return;
            }
        }
    }
    bool bVerifyLoadFlag = verifyLoadSpace(config);
    if(bVerifyLoadFlag == 1)
    {
        setVerifyFlag(FLAG_LENGT_OVERRUN_ERROR);
    }
}
void CMDDataBlock::setListRegistersConfigDownLoad(QString strConfig)
{
    if(strConfig.isEmpty())
    {
        return;
    }

    listRegistersCongigClear();//!清除配置

    QStringList strListAllConfig = strConfig.split(";");
    int nCount = strListAllConfig.count();

    for(int i = 0;i<nCount;i++)
    {
        QStringList strListOneConfig = strListAllConfig.at(i).split(",");
        MDDataBlockConfig *pOneRegisterConfig = new  MDDataBlockConfig;
        pOneRegisterConfig->m_strRegister = strListOneConfig.at(0);
        pOneRegisterConfig->m_strStartAddress = strListOneConfig.at(1);
        pOneRegisterConfig->m_strEndAddress = strListOneConfig.at(2);
        listDataRegistersConfig.append(pOneRegisterConfig);
    }

}

void CMDDataBlock::listRegistersConfig(listNoOrderRegistersConfig &config)
{
    config.clear();
    if(!listDataRegistersConfig.isEmpty())
    {
        for(listNoOrderRegistersConfig::iterator i=listDataRegistersConfig.begin();i!=listDataRegistersConfig.end();++i)
        {
            MDDataBlockConfig *pData = *i;
            config.append(pData);
        }
    }
}

void CMDDataBlock::listRegistersCongigClear()
{
    if(!listDataRegistersConfig.isEmpty())
    {
        for(listNoOrderRegistersConfig::iterator i=listDataRegistersConfig.begin();i!=listDataRegistersConfig.end();++i)
        {
            MDDataBlockConfig *pData = *i;
            if(pData!=NULL)
            {
                delete pData;
                pData =NULL;
            }
        }
    }
    listDataRegistersConfig.clear();
}
QString CMDDataBlock::listRegistersConfigToFile()
{
    if(listDataRegistersConfig.isEmpty())
    {
        return "";
    }
    else
    {
        QString strConfig="";
        for(listNoOrderRegistersConfig::iterator i=listDataRegistersConfig.begin();i!=listDataRegistersConfig.end();++i)
        {
            MDDataBlockConfig *pData = *i;
            listNoOrderRegistersConfig::iterator endFlag = i+1;
            if(endFlag==listDataRegistersConfig.end())
            {
                strConfig = strConfig+pData->m_strRegister+","+pData->m_strStartAddress+","+pData->m_strEndAddress;
            }
            else if(i==listDataRegistersConfig.begin())
            {
                strConfig = strConfig +pData->m_strRegister+","+pData->m_strStartAddress+","+pData->m_strEndAddress+";";
            }
            else
            {
                strConfig = strConfig +pData->m_strRegister+","+pData->m_strStartAddress+","+pData->m_strEndAddress+";";
            }
        }
        return strConfig;
    }

}
void CMDDataBlock::setMapRegistersConfig(listNoOrderRegistersConfig &config)
{
    listNoOrderRegistersConfig listForSingleRegister;
    QString strCompareRegister = "";

    mapDataRegistersConfig.clear();

    qint32 nLastOffset = 0;

    for(int registerCount = 0;registerCount<13;registerCount++)
    {
        listForSingleRegister.clear();
        switch (registerCount) {
        case ID_MEM_ORDER_X:
        {
            strCompareRegister = "X";
        }
            break;
        case ID_MEM_ORDER_Y:
        {
            strCompareRegister = "Y";
        }
            break;
        case ID_MEM_ORDER_S:
        {
            strCompareRegister = "S";
        }
            break;
        case ID_MEM_ORDER_M:
        {
            strCompareRegister = "M";
        }
            break;
        case ID_MEM_ORDER_SPM:
        {
            strCompareRegister = "SPM";
        }
            break;
        case ID_MEM_ORDER_TS:
        {
            strCompareRegister = "TS";
        }
            break;
        case ID_MEM_ORDER_CS16:
        {
            strCompareRegister = "CS16";
        }
            break;
        case ID_MEM_ORDER_CS32:
        {
            strCompareRegister = "CS32";
        }
            break;
        case ID_MEM_ORDER_T:
        {
            strCompareRegister = "T";
        }
            break;
        case ID_MEM_ORDER_C32:
        {
            strCompareRegister = "C32";
        }
            break;
        case ID_MEM_ORDER_D:
        {
            strCompareRegister = "D";
        }
            break;
        case ID_MEM_ORDER_SPD:
        {
            strCompareRegister = "SPD";
        }
            break;
        case ID_MEM_ORDER_C16:
        {
            strCompareRegister = "C16";
        }
            break;
        default:
            break;
        }

        for(int count = 0;count<config.count();count++)
        {
            if(config.at(count)->m_strRegister == strCompareRegister)
            {
                MDDataBlockConfig *pTempConfig = config.at(count);
                int nAddressFormat;

                this->verifyRegisterFormat(pTempConfig->m_strRegister,nAddressFormat);

                qint32 nStartAddress = 0;
                this->verifyAddressFormat(pTempConfig->m_strStartAddress,nStartAddress,nAddressFormat);

                qint32 nEndAddress = 0;
                this->verifyAddressFormat(pTempConfig->m_strEndAddress,nEndAddress,nAddressFormat);


                switch (registerCount)
                {
                case ID_MEM_ORDER_X:
                {
                    pTempConfig->m_nRegister = 1;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;

                }
                    break;
                case ID_MEM_ORDER_Y:
                {
                    pTempConfig->m_nRegister = 2;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;
                }
                    break;
                case ID_MEM_ORDER_S:
                {
                    pTempConfig->m_nRegister = 3;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;
                }
                    break;
                case ID_MEM_ORDER_M:
                {
                    pTempConfig->m_nRegister = 5;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;

                }
                    break;
                case ID_MEM_ORDER_SPM:
                {
                    pTempConfig->m_nRegister = 6;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;
                }
                    break;
                case ID_MEM_ORDER_TS:
                {
                    pTempConfig->m_nRegister = 7;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;
                }
                    break;
                case ID_MEM_ORDER_CS16:
                {
                    pTempConfig->m_nRegister = 9;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;
                }
                    break;
                case ID_MEM_ORDER_CS32:
                {
                    pTempConfig->m_nRegister = 10;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress/32 - nStartAddress/32 +1);
                    pTempConfig->m_nArraySubScript = nStartAddress/32;
                }
                    break;
                case ID_MEM_ORDER_T:
                {
                    pTempConfig->m_nRegister = 14;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress - nStartAddress +1);
                    pTempConfig->m_nArraySubScript = nStartAddress;
                }
                    break;
                case ID_MEM_ORDER_C32:
                {
                    pTempConfig->m_nRegister = 17;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress - nStartAddress +1);
                    pTempConfig->m_nArraySubScript = nStartAddress;
                }
                    break;
                case ID_MEM_ORDER_D:
                {
                    pTempConfig->m_nRegister = 11;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress - nStartAddress +1);
                    pTempConfig->m_nArraySubScript = nStartAddress;
                }
                    break;
                case ID_MEM_ORDER_SPD:
                {
                    pTempConfig->m_nRegister = 12;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress - nStartAddress +1);
                    pTempConfig->m_nArraySubScript = nStartAddress;
                }
                    break;
                case ID_MEM_ORDER_C16:
                {
                    pTempConfig->m_nRegister = 16;
                    pTempConfig->m_mdtartAddress = nStartAddress;
                    pTempConfig->m_nEndAddress = nEndAddress;
                    pTempConfig->m_nArrayLength= (qint16)(nEndAddress - nStartAddress +1);
                    pTempConfig->m_nArraySubScript = nStartAddress;
                }
                    break;
                default:
                    break;
                }
                listForSingleRegister.append(pTempConfig);
            }
        }
        if(!listForSingleRegister.isEmpty())
        {
            for(int listCount = 0;listCount<listForSingleRegister.count();listCount++)
            {
                MDDataBlockConfig *pTempConfig = listForSingleRegister.at(listCount);
                pTempConfig->m_nOffSet = nLastOffset;
                if(pTempConfig->m_strRegister == "D"
                        ||pTempConfig->m_strRegister == "SPD"
                            ||pTempConfig->m_strRegister == "C16")
                {
                    nLastOffset = nLastOffset + pTempConfig->m_nArrayLength*2;
                }
                else
                {
                    nLastOffset = nLastOffset + pTempConfig->m_nArrayLength*4;
                }


            }
            mapDataRegistersConfig.insert(listForSingleRegister.at(0)->m_strRegister,listForSingleRegister);
        }
    }
    calculateDataBufferSize(nLastOffset);
}
bool CMDDataBlock::verifyRegisterAddress(qint32 maxOrVarPoint,QString strFindRegister,QString strFindStatusRegister)
{
    if(mapDataRegistersConfig.isEmpty())
    {
        return 0;
    }
    if(mapDataRegistersConfig.contains(strFindRegister))
    {
        mapOrderRegistersConfig::iterator iteratorFindRegister = mapDataRegistersConfig.find(strFindRegister);
        for(int i = 0;i<iteratorFindRegister.value().count();i++)
        {
            if( iteratorFindRegister.value().at(i)->m_nEndAddress>maxOrVarPoint)
            {
                return 0;
            }
        }
    }
    if(!strFindStatusRegister.isEmpty())
    {
        if(mapDataRegistersConfig.contains(strFindStatusRegister))
        {
            mapOrderRegistersConfig::iterator iteratorFindStatusRegister = mapDataRegistersConfig.find(strFindRegister);
            for(int j = 0;j<iteratorFindStatusRegister.value().count();j++)
            {
                if( iteratorFindStatusRegister.value().at(j)->m_nEndAddress > maxOrVarPoint)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}
void CMDDataBlock::setInitialMapPackage(listNoOrderRegistersConfig &config)
{
    int overallPackage =config.count()/128;
    if(config.count()%128>0)
    {
        overallPackage ++;
    }
    this->setOverallPackage(overallPackage);
    QString strCompareRegister = "";
    listNoOrderRegistersConfig listTempConfig;
    listTempConfig.clear();
    for(int registerCount = 0;registerCount<13;registerCount++)
    {
        switch (registerCount) {
        case ID_MEM_ORDER_X:
        {
            strCompareRegister = "X";
        }
            break;
        case ID_MEM_ORDER_Y:
        {
            strCompareRegister = "Y";
        }
            break;
        case ID_MEM_ORDER_S:
        {
            strCompareRegister = "S";
        }
            break;
        case ID_MEM_ORDER_M:
        {
            strCompareRegister = "M";
        }
            break;
        case ID_MEM_ORDER_SPM:
        {
            strCompareRegister = "SPM";
        }
            break;
        case ID_MEM_ORDER_TS:
        {
            strCompareRegister = "TS";
        }
            break;
        case ID_MEM_ORDER_CS16:
        {
            strCompareRegister = "CS16";
        }
            break;
        case ID_MEM_ORDER_CS32:
        {
            strCompareRegister = "CS32";
        }
            break;
        case ID_MEM_ORDER_T:
        {
            strCompareRegister = "T";
        }
            break;
        case ID_MEM_ORDER_C32:
        {
            strCompareRegister = "C32";
        }
            break;
        case ID_MEM_ORDER_D:
        {
            strCompareRegister = "D";
        }
            break;
        case ID_MEM_ORDER_SPD:
        {
            strCompareRegister = "SPD";
        }
            break;
        case ID_MEM_ORDER_C16:
        {
            strCompareRegister = "C16";
        }
            break;
        default:
            break;
        }
        for(int count = 0;count<config.count();count++)
        {
            if(config.at(count)->m_strRegister == strCompareRegister)
            {
                listTempConfig.append(config.at(count));
            }
        }
    }
    listNoOrderRegistersConfig listTempConfigToMap;
    mapInitialPackage.clear();
    for(int package = 0;package<this->overallPackage();package++)
    {
        listTempConfigToMap.clear();
        if(package == this->overallPackage()-1)
        {
            for(int i = package*128;i<listTempConfig.count();i++)
            {
                listTempConfigToMap.append(listTempConfig.at(i));
            }
        }
        else
        {
            for(int i = package*128;i<(package+1)*128;i++)
            {
                listTempConfigToMap.append(listTempConfig.at(i));
            }
        }
        mapInitialPackage.insert(package,listTempConfigToMap);
    }
}
void CMDDataBlock::InitialMapPackage(listNoOrderRegistersConfig &config,int currentPackage)
{
    if(mapInitialPackage.contains(currentPackage))
    {
        mapInitialCommandPackage::iterator iteratorInitial = mapInitialPackage.find(currentPackage);
        for(int i = 0;i<iteratorInitial.value().count();++i)
        {
            config.append(iteratorInitial.value().at(i));
        }
    }
}
void CMDDataBlock::setOverallPackage(int package)
{
    m_overallPackage = package;
}
int CMDDataBlock::overallPackage()
{
    return m_overallPackage;
}

void CMDDataBlock::setCurrentPackage(int currentPackage)
{
    m_currentPackage = currentPackage;
}
int CMDDataBlock::currentPackage()
{
    return m_currentPackage;
}
quint8 *CMDDataBlock::pointerOfDataBuffer()
{
    return m_dataBuffer;
}
bool CMDDataBlock::verifyVarAddress(QString reg,qint32 address,qint32 &offset,qint32 &subOffset)
{
    if(mapDataRegistersConfig.contains(reg))
    {
        mapOrderRegistersConfig::iterator iteratorFindRegister = mapDataRegistersConfig.find(reg);
        for(int i = 0;i<iteratorFindRegister.value().count();i++)
        {
            if( address<=iteratorFindRegister.value().at(i)->m_nEndAddress && address>=iteratorFindRegister.value().at(i)->m_mdtartAddress)
            {
                if(reg == "X"
                        || reg == "Y"
                            || reg == "S"
                                || reg == "M"
                                    || reg == "SPM"
                                        || reg == "TS"
                                            || reg == "CS16"
                                                || reg == "CS32")
                {
                    offset = iteratorFindRegister.value().at(i)->m_nOffSet+
                                    (address/32-iteratorFindRegister.value().at(i)->m_mdtartAddress/32)*4;
                    subOffset = address%32;
                }
                else if(reg == "T"
                            ||reg == "C32")
                {
                    offset = iteratorFindRegister.value().at(i)->m_nOffSet+
                                    (address-iteratorFindRegister.value().at(i)->m_mdtartAddress)*4;
                }
                else
                {
                    offset = iteratorFindRegister.value().at(i)->m_nOffSet+
                                    (address-iteratorFindRegister.value().at(i)->m_mdtartAddress)*2;
                }
                return 1;
            }
        }
    }
    return 0;
}
void CMDDataBlock::insertToVarForStringMap(QString mapKey,QString reg,qint32 address,qint32 &offset)
{
    qint32 maxEndAddress = 0;
    if(mapVarForString.contains(mapKey))
    {
        mapVarForString.remove(mapKey);
    }

    if(mapDataRegistersConfig.contains(reg))
    {
        mapOrderRegistersConfig::iterator iteratorFindRegister = mapDataRegistersConfig.find(reg);
        for(int i = 0;i<iteratorFindRegister.value().count();i++)
        {
            if( address<=iteratorFindRegister.value().at(i)->m_nEndAddress && address>=iteratorFindRegister.value().at(i)->m_mdtartAddress)
            {
                if(iteratorFindRegister.value().at(i)->m_nEndAddress>maxEndAddress)
                {
                    maxEndAddress = iteratorFindRegister.value().at(i)->m_nEndAddress;
                    offset = iteratorFindRegister.value().at(i)->m_nOffSet+
                                    (address-iteratorFindRegister.value().at(i)->m_mdtartAddress)*2;
                }

            }
        }
    }
    mapVarForString.insert(mapKey,maxEndAddress);
}

qint32 CMDDataBlock::endAddressFromVarForStringMap(QString mapKey)
{
    if(mapVarForString.contains(mapKey))
    {
        QMap<QString,quint32>::iterator iteratorFindEndAddress = mapVarForString.find(mapKey);
        return iteratorFindEndAddress.value();
    }
    return 0;
}
bool CMDDataBlock::verifyLoadSpace(listNoOrderRegistersConfig &config)
{
    int nBytes = 0;
    for(int i = 0;i<config.count();i++)
    {
        if(config.at(i)->m_strRegister == "X"
                || config.at(i)->m_strRegister == "Y"
                    || config.at(i)->m_strRegister == "S"
                        || config.at(i)->m_strRegister == "M"
                            || config.at(i)->m_strRegister == "SPM"
                                || config.at(i)->m_strRegister == "TS"
                                    || config.at(i)->m_strRegister == "CS16"
                                        || config.at(i)->m_strRegister == "CS32")
        {
            if(config.at(i)->m_strRegister == "X"||config.at(i)->m_strRegister == "Y")
            {
                bool ok=0;
                int startAddress = config.at(i)->m_strStartAddress.toInt(&ok,8);
                int endAddress = config.at(i)->m_strEndAddress.toInt(&ok,8);
                nBytes = nBytes + (endAddress/32 - startAddress/32+1)*4;
            }
            else
            {
                int startAddress = config.at(i)->m_strStartAddress.toInt();
                int endAddress = config.at(i)->m_strEndAddress.toInt();
                nBytes = nBytes + (endAddress/32 - startAddress/32+1)*4;
            }

        }
        else if(config.at(i)->m_strRegister == "T"
                ||config.at(i)->m_strRegister == "C32")
        {
            int startAddress = config.at(i)->m_strStartAddress.toInt();
            int endAddress = config.at(i)->m_strEndAddress.toInt();
            nBytes = nBytes + (endAddress - startAddress+1)*4;
        }
        else
        {
            int startAddress = config.at(i)->m_strStartAddress.toInt();
            int endAddress = config.at(i)->m_strEndAddress.toInt();
            nBytes = nBytes + (endAddress - startAddress+1)*2;
        }
    }
    if(nBytes>8192)
    {
       return 1;//超限
    }
    else
    {
       return 0;
    }
}
void CMDDataBlock::setPackBlockWrite(uint allpack)
{
    m_packDBCount = allpack;
}
uint  CMDDataBlock::packBlockWrite()
{
    return m_packDBCount;
}
void CMDDataBlock::cpyTempDBWriteBuffer(quint8 * pbuffer,uint cpyLength,int nwei)
{
    memcpy(m_DBWriteArray[nwei],pbuffer,cpyLength);
}
quint8 * CMDDataBlock::tempDBWtiteBuffer(int nwei)
{
    return m_DBWriteArray[nwei];
}
void CMDDataBlock::setCurrentPackBlockWrite(uint currentpack)
{
    m_currentDBPack = currentpack;
}
uint  CMDDataBlock::currentPackBlockWrite()
{
    return m_currentDBPack;
}
//**********************************属性框翻译与提示***************************//
void CMDDataBlock::initTranslateNameAndPropertyType()
{
    m_translateName.insert(QString("GUID_DataBlock"),QString("数据块"));
    m_translateName.insert(QString("RegistersConfig"),QString("寄存器与地址配置"));
    m_propertyTip.insert(QString("RegistersConfig"),QString("寄存器地址配置"));
}
QMap<QString,QVariant> CMDDataBlock::translateName() const//!翻译函数
{
    return m_translateName;
}
QMap<QString, QVariant> CMDDataBlock::propertyTip() const//!提示框函数
{
    return m_propertyTip;
}
QString CMDDataBlock::registersConfig_Pro()
{
    return m_registersConfig_Pro;
}
void CMDDataBlock::setRegistersConfig_Pro(QString registerConfig)
{
    CMDRegisterAndAddressConfigTab dlg;
    dlg.setDataBlock(this);
    dlg.exec();
}
//***************************************************************************//

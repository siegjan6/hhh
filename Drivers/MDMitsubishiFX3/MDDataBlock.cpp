#include "MDDataBlock.h"
#include <QStringList>
#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "PropertyBrowser.h"
CMDDataBlock::CMDDataBlock()
{
     m_packCount = 0;
     m_currentPack = 0;
     m_registerType=0;
     m_startAddress=0;
     m_endAddress=0;
     m_dataBufferSize=0;
     m_strRegisterType = "0";
     m_strStartAddress = "0";
     m_strEndAddress = "0";
     m_dataBuffer=NULL;
     setEnabled(true);
     //*********************属性框变量*********************//
     this->initTranslateNameAndPropertyType();
     m_registerType_Pro = (MDRegister_Pro)0;
     m_startAddress_Pro = "0";
     m_endAddress_Pro = "0";
     //!初始报错标志位
     m_bError = 0;
     //**************************************************//
}
CMDDataBlock::~CMDDataBlock()
{
    if(NULL !=m_dataBuffer)
    {
        delete []m_dataBuffer;
    }
}

bool CMDDataBlock::start()
{
    if(verifyFlag() != FLAG_RIGHT)
    {
        setQuality(MDVariableDataQuality::DataBlockConfigError);
        refreshVariablesQuality();
    }
    return CBaseDataBlock::start();//PKX20151119
}

bool CMDDataBlock::stop()
{
    return CBaseDataBlock::stop();//PKX20151119
}

void CMDDataBlock::copy(IDataBlock *otherDataBlock)
{
    if(NULL== otherDataBlock)
    {
        return;
    }
    CBaseDataBlock::copy(otherDataBlock);
    this->setStrStartAddress(((CMDDataBlock*)otherDataBlock)->strStartAddress());
    this->setStrEndAddress(((CMDDataBlock*)otherDataBlock)->strEndAddress ());
    this->setStrRegisterType(((CMDDataBlock*)otherDataBlock)->strRegisterType());
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }
 }

//开始地址
void CMDDataBlock::setStartAddress(qint32 address)
{
    m_startAddress = address;
    if(m_bError == 1)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
        }
    }

}

qint32 CMDDataBlock::startAddress()
{
    return m_startAddress;
}

//结束地址
void CMDDataBlock::setEndAddress(qint32 address)
{
    m_endAddress = address;
    if(m_bError == 1)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
        }
    }
}

qint32 CMDDataBlock::endAddress()
{
    return m_endAddress;
}
void CMDDataBlock::setRegisterType(qint16 registerType)
{
    m_registerType = registerType;
    m_registerType_Pro = (MDRegister_Pro)m_registerType;//!属性框
    if(m_bError == 1)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::white);
        }
    }
}

qint16 CMDDataBlock::registerType()
{
    return m_registerType;
}
void CMDDataBlock::setStrStartAddress(QString address)
{

    m_strStartAddress = address;
    m_startAddress_Pro = m_strStartAddress;

}
QString CMDDataBlock::strStartAddress()
{
    return m_strStartAddress;
}
void CMDDataBlock::setStrEndAddress(QString address)
{
    m_strEndAddress = address;
    m_endAddress_Pro = m_strEndAddress;
}
QString CMDDataBlock::strEndAddress()
{
    return m_strEndAddress;
}
void CMDDataBlock::setStrRegisterType(QString registerType)
{
    m_strRegisterType = registerType;
}
QString CMDDataBlock::strRegisterType()
{
    return m_strRegisterType;
}
void CMDDataBlock::getVariableAddressList(QStringList &varAddressList)
{
    if(verifyFlag()==FLAG_RIGHT)
    {
        struRegisterArea* RegisterArea = GetRegisterStruct(m_registerType);
        QString str_Register_Name(RegisterArea->m_strName);
        QString strStartAddress = "";
        QString strEndAddress ="";
        if(m_registerType == ID_MEM_Y
                ||m_registerType == ID_MEM_X)
        {
            strStartAddress = QString::number(m_startAddress,8);
            strEndAddress = QString::number(m_endAddress,8);
        }
        else
        {
            strStartAddress = QString::number(m_startAddress,10);
            strEndAddress = QString::number(m_endAddress,10);
        }

        varAddressList.append
                (QString("%1.%2%3---%1.%2%4(*hint:StartAddress---EndAddress)")
                 .arg(name())
                 .arg(str_Register_Name)
                 .arg(strStartAddress)
                 .arg(strEndAddress));
    }
    else
    {
        QString addressTemp = QString("数据块%1配置错误!").arg(name());
        varAddressList.append(addressTemp);
    }
}
bool CMDDataBlock::getDataBufferUShortValue(int internalAddress, ushort& data)
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
        tempbuffer[0] = m_dataBuffer[internalAddress+1];
        tempbuffer[1] =  m_dataBuffer[internalAddress];
        memcpy(&data,tempbuffer,2);
        bRet = true;
    }
    return bRet;
}

bool CMDDataBlock::getDataBufferULongValue(int internalAddress, ulong& data)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);

    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }

    if (internalAddress < m_dataBufferSize-3 && internalAddress>=0)
    {
        quint8 tempbuffer[4];
        tempbuffer[0] = m_dataBuffer[internalAddress+3];
        tempbuffer[1] =  m_dataBuffer[internalAddress+2];
        tempbuffer[2] = m_dataBuffer[internalAddress+1];
        tempbuffer[3] =  m_dataBuffer[internalAddress];
        memcpy(&data,tempbuffer,4);
        bRet = true;
    }
    return bRet;
}

//获取数据缓冲区
bool CMDDataBlock::getDataBuffer(quint8* dataBuf, qint16 offset, qint16 length)
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

void CMDDataBlock::calculateDataBufferSize()
{
    qint32 tempDataBufferSize = 0;
    if (m_registerType<=5)
    {
        tempDataBufferSize=(m_endAddress - m_startAddress+1)*1;
    }
    else if (m_registerType>5)
    {
        if (m_registerType==ID_MEM_CN && m_startAddress>=200)
        {
            tempDataBufferSize=(m_endAddress - m_startAddress+1)*4;
        }
        else
        {
            tempDataBufferSize=(m_endAddress - m_startAddress+1)*2;
        }
    }
    if(tempDataBufferSize!=m_dataBufferSize)
    {
        if(m_dataBuffer == NULL)
        {
            m_dataBufferSize = tempDataBufferSize;
        }
        else
        {
            delete m_dataBuffer;
            m_dataBuffer = NULL;
            m_dataBufferSize = tempDataBufferSize;
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
    if(verifyFlag() != FLAG_RIGHT)
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

        struRegisterArea* pRegisterArea = GetRegisterStruct(m_registerType);
        QString strRegisterType(pRegisterArea->m_strName);

        QString strRegisterTypeFromUI=addressSplit[1];
        strRegisterTypeFromUI.truncate(strRegisterType.size());

        if(strRegisterType!=strRegisterTypeFromUI)
        {
            error = tr("变量地址中寄存器错误!");
            return ret;
        }
        QString strAddress = addressSplit[1];
        QString strAddress1 =  strAddress.remove(0,strRegisterType.size());
        qint32 address = 0;
        if(m_registerType == ID_MEM_X
                ||m_registerType == ID_MEM_Y)
        {
            bool ok = 0;
            address = (qint32)strAddress1.toInt(&ok,8);
            if(ok==0)
            {
                error = tr("变量地址格式错误!");
                return ret;
            }
        }
        else
        {
            address = (qint32)strAddress1.toInt();
        }
        if(address <m_startAddress||address>m_endAddress)
        {
            error = tr("变量地址超限");
            return ret;
        }

        switch (m_registerType)
        {
        case ID_MEM_X:
        case ID_MEM_Y:
        case ID_MEM_M:
        case ID_MEM_S:
        case ID_MEM_TS:
        case ID_MEM_CS:
        {
            qint32 internalAddress = address - m_startAddress;
            if (nsVar->originalDataType() == MDDataType::Bool)
            {
                if(internalAddress>=m_dataBufferSize)
                {
                    internalAddress = -1;
                    error = tr("变量地址超限!");
                }
                else
                {
                    ret = true;
                }
                nsVar->setInternalAddress(internalAddress);
            }
            else
            {
                error = tr("变量类型错误!");
            }
         }
            break;
        case ID_MEM_TN:
        case ID_MEM_D:
        case ID_MEM_R:
        case ID_MEM_CN:
        {
            qint32 internalAddress = (address - m_startAddress)*2;
            qint32 internalSubAddress = -1;
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                if(m_registerType == ID_MEM_CN
                        &&m_startAddress>=200)
                {
                    internalAddress = (address - m_startAddress)*4;
                    if (addressSplit.count() > 2)
                    {
                        internalSubAddress=addressSplit[2].toUInt();
                    }
                    else
                    {
                        error = tr("变量地址格式错误!");
                        return ret;
                    }
                    if ((internalAddress+4) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限!");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                    if (internalSubAddress > 31)
                    {
                        internalSubAddress = 31;
                    }
                }
                else
                {
                    if (addressSplit.count() > 2)
                    {
                        internalSubAddress=(qint16)addressSplit[2].toUInt();
                    }
                    else
                    {
                        error = tr("变量地址格式错误!");
                        return ret;
                    }
                    if ((internalAddress+2) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限!");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                    if (internalSubAddress > 15)
                    {
                        internalSubAddress = 15;
                    }
                }

            }
                break;
            case MDDataType::Int16:
            case MDDataType::UInt16:
            {
                if(m_registerType == ID_MEM_CN
                        &&m_startAddress>=200)
                {
                    error = tr("变量类型错误!");
                    return ret;
                }
                else
                {
                    if (addressSplit.count() > 2)
                    {
                        error = tr("变量地址格式错误!");
                        return ret;
                    }
                    if ((internalAddress+2) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限!");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                }
            }
                break;
            case MDDataType::Int32:
            case MDDataType::UInt32:
            case MDDataType::Float:
            {
                if(m_registerType == ID_MEM_CN
                        &&m_startAddress>=200)
                {
                    internalAddress = (address - m_startAddress)*4;
                }
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                if ((internalAddress+4) > m_dataBufferSize)
                {
                    error = tr("变量地址超限!");
                    internalAddress = -1;
                }
                else
                {
                    ret = true;
                }
            }
                break;
            case MDDataType::Double:
            {
                if(m_registerType == ID_MEM_CN
                        &&m_startAddress>=200)
                {
                    internalAddress = (address - m_startAddress)*4;
                }
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                if ((internalAddress+8) > m_dataBufferSize)
                {
                    error = tr("变量地址超限!");
                    internalAddress = -1;
                }
                else
                {
                    ret = true;
                }
            }
                break;
            case MDDataType::String:
                {
                    if(m_registerType!=ID_MEM_D&&m_registerType!=ID_MEM_R)
                    {
                        error = tr("变量类型错误!");
                        return ret;
                    }
                    if ((internalAddress+2) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限!");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                }
                break;
            default:
                error = tr("变量类型错误!");
                return ret;
                break;
            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);
        }
            break;
        default:
            break;
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
        bool bDataType = false;
        unsigned char tempBuffer[8];
        switch (m_registerType)
        {
        case ID_MEM_X:
        case ID_MEM_Y:
        case ID_MEM_M:
        case ID_MEM_S:
        case ID_MEM_TS:
        case ID_MEM_CS:
        {
            tempBuffer[0] = m_dataBuffer[nsVar->internalAddress()];
            if (nsVar->originalDataType() == MDDataType::Bool)
            {
                if (tempBuffer[0] == 0x31)
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
                bDataType = true;
            }
        }
            break;
        case ID_MEM_TN:
        case ID_MEM_CN:
        case ID_MEM_D:
        case ID_MEM_R:
        {
            tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
            tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
            tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 3];
            tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 2];
            tempBuffer[4] = m_dataBuffer[nsVar->internalAddress() + 5];
            tempBuffer[5] = m_dataBuffer[nsVar->internalAddress() + 4];
            tempBuffer[6] = m_dataBuffer[nsVar->internalAddress() + 7];
            tempBuffer[7] = m_dataBuffer[nsVar->internalAddress() + 6];
            if(m_registerType == ID_MEM_CN&&m_startAddress>=200)
            {
                tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                tempBuffer[4] = m_dataBuffer[nsVar->internalAddress() + 7];
                tempBuffer[5] = m_dataBuffer[nsVar->internalAddress() + 6];
                tempBuffer[6] = m_dataBuffer[nsVar->internalAddress() + 5];
                tempBuffer[7] = m_dataBuffer[nsVar->internalAddress() + 4];
            }
            switch (nsVar->originalDataType())
            {
            case MDDataType::Int16:
            {
                if(m_registerType == ID_MEM_CN&&m_startAddress>=200)
                {
                    bDataType = true;
                }
                else
                {
                    qint16 tempValue=0;
                    memcpy(&tempValue,tempBuffer,2);
                    varDataValue =tempValue;
                    bRightValue = true;
                }

            }
                break;
            case MDDataType::UInt16:
            {
                if(m_registerType == ID_MEM_CN&&m_startAddress>=200)
                {
                    bDataType = true;
                }
                else
                {
                    quint16 tempValue=0;
                    memcpy(&tempValue,tempBuffer,2);
                    varDataValue =tempValue;
                    bRightValue = true;
                }
            }
                break;
            case MDDataType::Bool:
            {
                if(m_registerType == ID_MEM_CN&&m_startAddress>=200)
                {
                    quint32 tempValue=0;
                    memcpy(&tempValue,tempBuffer,4);

                    uint dataValue=CBitFuntons::getBit(tempValue,nsVar->internalSubAddress());
                    if (dataValue > 0)
                    {
                        varDataValue =true;
                    }
                    else
                    {
                        varDataValue = false;
                    }
                    bRightValue = true;
                }
                else
                {
                    quint32 tempValue=0;
                    memcpy(&tempValue,tempBuffer,4);

                    uint dataValue=CBitFuntons::getBit(tempValue,nsVar->internalSubAddress());
                    if (dataValue > 0)
                    {
                        varDataValue =true;
                    }
                    else
                    {
                        varDataValue = false;
                    }
                    bRightValue = true;
                }
            }
                break;
            case MDDataType::Int32:
            {
                qint32 tempValue=0;
                memcpy(&tempValue,tempBuffer,4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::UInt32:
            {
                quint32 tempValue=0;
                memcpy(&tempValue,tempBuffer,4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::Float:
            {
                float tempValue=0;
                memcpy(&tempValue,tempBuffer,4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::Double:
            {
                double tempValue=0;
                memcpy(&tempValue,tempBuffer,8);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::String:
               {
                    char strTempBuffer[1024];
                    memset(strTempBuffer,0,1024);
                    quint16 nRegulationSize = 0;
                    char tempbuffer[2];
                    tempbuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                    tempbuffer[1] =  m_dataBuffer[nsVar->internalAddress()];
                    memcpy(&nRegulationSize,tempbuffer,2);
                    if(nRegulationSize>=(m_endAddress-nsVar->internalAddress()/2 -m_startAddress)*2)
                    {
                        nRegulationSize = (m_endAddress-nsVar->internalAddress()/2 -m_startAddress)*2;
                    }
                    for(int i = 0; i<nRegulationSize;i++)
                    {
                        if(i%2 == 0)
                        {
                            strTempBuffer[i] = m_dataBuffer[i+1+nsVar->internalAddress()+2];
                        }
                        else
                        {
                            strTempBuffer[i] = m_dataBuffer[i-1+nsVar->internalAddress()+2];
                        }
                    }
                    QString string(strTempBuffer);
                    varDataValue =string;
                    bRightValue = true;
               }
                break;
            default:
                bDataType = true;
                break;
            }
        }
            break;
        default:
            break;
        }
        if (true == bRightValue)
        {
           bool bChangeFlag = nsVar->refreshData(varDataValue, timeStamp(), quality(), false);
           if(bChangeFlag)
           {
               lastChangeVar = nsVar;
               changeVarCount++;
               changevarList.append(nsVar);
           }
        }
        else if(bDataType == true)
        {
            nsVar->refreshData(varDataValue, timeStamp(), MDVariableDataQuality::DataTypeError, false);
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
    cmd->setDataBlock (this);
    cmd->setDevice(device());
    cmd->setCommandType(cmdType);
    return cmd;
}
//分析命令，将数据拷贝到数据区，并更新变量
void CMDDataBlock::parseCommand(CBaseProtocolCommand *cmd)
{
    if(NULL == cmd)
    {
        return;
    }
    if(cmd->getCommandType() == ProtocolCommandType::Read)
    {
        if (cmd->getCommandResult() == ProtocolCommandResult::RightAndWithoutRet)
        {
           QMutexLocker locker(&m_dataBufferMutex);
           if(m_dataBufferSize>0)
           {
               if (NULL == m_dataBuffer)
               {
                   m_dataBuffer = new quint8[m_dataBufferSize];
               }
           }
           char chTempDataBuffer[1024];
           memset(chTempDataBuffer,0,1024);

           if (m_registerType<=5)
           {
                memcpy(m_dataBuffer,cmd->getReceiveData()+5,m_dataBufferSize);
           }
           else if (m_registerType>5)
           {
               memcpy(chTempDataBuffer,cmd->getReceiveData()+5,m_dataBufferSize*2);
               QString strTempDataBuffer(chTempDataBuffer);
               for(int i = 0;i<m_dataBufferSize;i++)
               {
                   QString stTtemp = strTempDataBuffer.mid(2*i,2);
                   bool ok = false;
                   unsigned char uchTemp = (unsigned char)stTtemp.toUInt(&ok,16);
                   if(ok)
                   {
                       *(m_dataBuffer+i) = uchTemp;
                   }
                   else
                   {
                       continue;
                   }
               }
           }
           this->onReceiveCommand(true, cmd->getCommandResult());
           refreshVariables();
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

    element = doc.createElement("RegisterType");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->strRegisterType());
    element.appendChild(elementText);

    element = doc.createElement("StartAddress");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->strStartAddress());
    element.appendChild(elementText);

    element = doc.createElement("EndAddress");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->strEndAddress());
    element.appendChild(elementText);

    element = doc.createElement("PollRate");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->pollRate()));
    element.appendChild(elementText);

}
void CMDDataBlock::serialize(CSortJsonObject& json)
{
    json.insertSimpleValue("Name",this->name());
    json.insertSimpleValue("Description",this->description());
    json.insertSimpleValue("Enabled",this->isEnabled()?QString("1"):QString("0"));
    json.insertSimpleValue("RegisterType",this->registerIndexToString(this->strRegisterType()));
    json.insertSimpleValue("StartAddress",this->strStartAddress());
    json.insertSimpleValue("EndAddress",this->strEndAddress());
    json.insertSimpleValue("PollRate",QString("%1").arg(this->pollRate()));
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
        else if(0==QString::compare(element.tagName(),"RegisterType", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setStrRegisterType(str);
        }
        else if(0==QString::compare(element.tagName(),"StartAddress", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setStrStartAddress(str);
        }
        else if(0==QString::compare(element.tagName(),"EndAddress", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setStrEndAddress(str);
        }
        else if(0==QString::compare(element.tagName(),"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(element.text().toInt());
        }
        element = element.nextSiblingElement();
    }
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }
}
void CMDDataBlock::deserialize(const QJsonObject &json)
{
    this->setName(json.value("Name").toString(""));
    this->setDescription(json.value("Description").toString(""));
    this->setEnabled(json.value("Enabled").toString("1").toInt());
    this->setStrRegisterType(this->registerStringToIndex(json.value("RegisterType").toString("-1")));
    this->setStrStartAddress(json.value("StartAddress").toString("-1"));
    this->setStrEndAddress(json.value("EndAddress").toString("-1"));
    this->setPollRate(json.value("PollRate").toString("1000").toInt());

    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
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
        else if(0==QString::compare(parameterName,"StartAddress", Qt::CaseInsensitive))
        {
            QString str = parameterValue;
            this->setStrStartAddress(str);
        }
        else if(0==QString::compare(parameterName,"EndAddress", Qt::CaseInsensitive))
        {
            QString str = parameterValue;
            this->setStrEndAddress(str);
        }
        else if(0==QString::compare(parameterName,"RegisterType", Qt::CaseInsensitive))
        {
            QString str = parameterValue;
            this->setStrRegisterType(str);
        }
        else if(0==QString::compare(parameterName,"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(parameterValue.toInt());
        }
    }
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }
}
void CMDDataBlock::setPackBlockWrite(uint allpack)
{
    m_packCount = allpack;
}
uint  CMDDataBlock::packBlockWrite()
{
    return m_packCount;
}
void CMDDataBlock::cpyTempDBWriteBuffer(unsigned char * pbuffer,uint cpyLength,int nwei)
{
    memcpy(m_DBWriteArray[nwei],pbuffer,cpyLength);
}
quint8 * CMDDataBlock::tempDBWtiteBuffer(int nwei)
{
    return m_DBWriteArray[nwei];
}
void CMDDataBlock::setCurrentPackBlockWrite(uint currentpack)
{
    m_currentPack = currentpack;
}
uint  CMDDataBlock::currentPackBlockWrite()
{
    return m_currentPack;
}
void CMDDataBlock::setCurrentPackLength(int length,uint currentpack)
{
    m_currentPackLength[currentpack] = length;
}
int    CMDDataBlock::currentPackLength(uint currentpack)
{
    return m_currentPackLength[currentpack];
}
int CMDDataBlock::verifyRegisterFormat(QString strRegister,qint32 &nRegister,int &addressFormat,qint16 &nLength)
{
    bool okRegister = false;

    qint16 tempRegister = strRegister.toInt(&okRegister,10);
    if((okRegister==false)||(tempRegister<0||tempRegister>9))
        return REGISTER_FORMAT_ERROR;//!寄存器格式校验错误
    else
    {
        nRegister = tempRegister;
        if(tempRegister == ID_MEM_X||
                tempRegister == ID_MEM_Y)
        {
            addressFormat = 8;
            nLength = 512;
        }
        else if(tempRegister == ID_MEM_M||
                    tempRegister == ID_MEM_S||
                        tempRegister == ID_MEM_CS||
                            tempRegister == ID_MEM_TS)
        {
            addressFormat = 10;
            nLength = 512;
        }
        else
        {
            addressFormat = 10;
            nLength = 64;
        }

    }
    return REGISTER_FORMAT_RIGHT;
}

int CMDDataBlock::verifyAddressFormat(QString strAddress,qint32 &nAddress,qint16 addressFormat)
{
    bool okAddress = false;
    qint16 tempAddress = 0;

    tempAddress = strAddress.toInt(&okAddress,addressFormat);

    if((okAddress == false)||(tempAddress<0))
        return ADDRESS_FORMAT_ERROR;//!地址格式校验错误
    else
        nAddress = tempAddress;
    return ADDRESS_FORMAT_RIGHT;
}
int CMDDataBlock::verifyLength(qint32 startAddress,qint32 endAddress,qint16 &nLength,qint32 registerType)
{
    if(registerType==ID_MEM_CN)
    {
        if(startAddress>199)
        {
            nLength = 32;
        }
    }
    if(startAddress > endAddress)
    {
        return LENGTH_NEGATIVENUMBER_ERROR;//!开始地址大于结束地址
    }
    else
    {
        qint16 nLengthTemp = endAddress - startAddress+1;
        if(nLengthTemp<=0||nLengthTemp>nLength)
        {
            return LENGTH_OVERRUN_ERROR;//!超过最大长度
        }
    }
    return LENGTH_RIGHT;
}
void CMDDataBlock::overallVerify()
{
    qint32 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = verifyRegisterFormat(this->strRegisterType(),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_RIGHT)
    {
        this->setRegisterType(nTempRegister);

        qint32 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(this->strStartAddress(),nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            this->setVerifyFlag(FLAG_STARTADDRESS_ERROR);//!开始地址错误
            return;
        }

        qint32 nTempEndAddress = 0;

        int tempEndAddressFlag = verifyAddressFormat(this->strEndAddress(),nTempEndAddress,nTempAddressFormat);
        if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            this->setVerifyFlag(FLAG_ENDADDRESS_ERROR);//!结束地址错误
            return;
        }

        int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength,nTempRegister);
        if(tempLengthFlag == LENGTH_NEGATIVENUMBER_ERROR)
        {
            this->setVerifyFlag(FLAG_LENGT_NEGATIVE_ERROR);//!长度负数错误
            return;
        }
        else if(tempLengthFlag == LENGTH_OVERRUN_ERROR)
        {
            this->setVerifyFlag(FLAG_LENGT_OVERRUN_ERROR);//!长度超限
            return;
        }
        this->setStartAddress(nTempStartAddress);
        this->setEndAddress(nTempEndAddress);
    }
    else
    {
        m_registerType_Pro = (MDRegister_Pro)10;
        this->setVerifyFlag(FLAG_REGISTER_ERROR);//!寄存器错误
        return;
    }
}
QString CMDDataBlock::registerIndexToString(QString index)
{
    QString strRegisterIndexToString = index;
    if(index == "0")
    {
        strRegisterIndexToString = "X";
    }
    else if(index == "1")
    {
        strRegisterIndexToString = "Y";
    }
    else if(index == "2")
    {
        strRegisterIndexToString = "M";
    }
    else if(index == "3")
    {
        strRegisterIndexToString = "S";
    }
    else if(index == "4")
    {
        strRegisterIndexToString = "TS";
    }
    else if(index == "5")
    {
        strRegisterIndexToString = "CS";
    }
    else if(index == "6")
    {
        strRegisterIndexToString = "TN";
    }
    else if(index == "7")
    {
        strRegisterIndexToString = "CN";
    }
    else if(index == "8")
    {
        strRegisterIndexToString = "D";
    }
    else if(index == "9")
    {
        strRegisterIndexToString = "R";
    }
    return strRegisterIndexToString;
}
QString CMDDataBlock::registerStringToIndex(QString string)
{
    QString strRegisterStringToIndex = string;
    if(0==QString::compare(string,"X",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "0";
    }
    else if(0==QString::compare(string,"Y",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "1";
    }
    else if(0==QString::compare(string,"M",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "2";
    }
    else if(0==QString::compare(string,"S",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "3";
    }
    else if(0==QString::compare(string,"TS",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "4";
    }
    else if(0==QString::compare(string,"CS",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "5";
    }
    else if(0==QString::compare(string,"TN",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "6";
    }
    else if(0==QString::compare(string,"CN",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "7";
    }
    else if(0==QString::compare(string,"D",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "8";
    }
    else if(0==QString::compare(string,"R",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "9";
    }
    return strRegisterStringToIndex;
}
//**********************************属性框翻译与提示***************************//
void CMDDataBlock::initTranslateNameAndPropertyType()
{
    m_translateName.insert(QString("GUID_DataBlock"),QString("数据块"));
    m_translateName.insert(QString("Register"),QString("寄存器"));
    m_propertyTip.insert(QString("Register"),QString("寄存器类型为NONE时,寄存器错误!"));
    m_translateName.insert(QString("StartAddress"),QString("开始地址"));
    m_propertyTip.insert(QString("StartAddress"),QString("寄存器正确:开始地址不能大于结束地址.寄存器错误:地址无意义.\
                                                         X,Y,M,S,TS,CS内存区的单元地址为布尔.TN、CN、D、R内存区的单元地址可以为字节.\
                                                         位软元件最大长度为512,字软元件最大长度为64(注：32位计数器C200--C255最大长度为32)"));
    m_translateName.insert(QString("EndAddress"),QString("结束地址"));
    m_propertyTip.insert(QString("EndAddress"),QString("寄存器正确:结束地址不能小于开始地址.寄存器错误:地址无意义.\
                                                       X,Y,M,S,TS,CS内存区的单元地址为布尔.TN、CN、D、R内存区的单元地址可以为字节.\
                                                       位软元件最大长度为512,字软元件最大长度为64(注：32位计数器C200--C255最大长度为32)"));
}
QMap<QString,QVariant> CMDDataBlock::translateName() const//!翻译函数
{
    return m_translateName;
}
QMap<QString, QVariant> CMDDataBlock::propertyTip() const//!提示框函数
{
    return m_propertyTip;
}
CMDDataBlock::MDRegister_Pro CMDDataBlock::register_Pro()
{
    if(m_bError == 0)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            connect(dev->getPropertyBrowse(),SIGNAL(propertyFinish()),this,SLOT(propertyLoadFinish_Pro()));
            m_bError = 1;
        }
    }
    return m_registerType_Pro;
}
void CMDDataBlock::setRegister_Pro(CMDDataBlock::MDRegister_Pro reg)
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        if(reg!=MDRegister_Pro::NONE)
        {
            qint32 nTempRegister = 0;
            qint16 nTempLength = 0;
            int nTempAddressFormat = 0;
            int tempRegisterFlag = verifyRegisterFormat(QString::number((int)reg,10),nTempRegister,nTempAddressFormat,nTempLength);
            if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::red);
                return;
            }
            dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::white);
            m_registerType_Pro = reg;
            m_strRegisterType = QString::number((int)reg,10);
            m_registerType = (int)reg;

            qint32 nTempStartAddress = 0;

            int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);


            qint32 nTempEndAddress = 0;

            int tempEndAddressFlag = verifyAddressFormat(m_endAddress_Pro,nTempEndAddress,nTempAddressFormat);
            if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR || tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
            {
                if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
                {
                    setVerifyFlag(FLAG_ENDADDRESS_ERROR);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                }
                else
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
                }
                if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
                {
                    setVerifyFlag(FLAG_STARTADDRESS_ERROR);
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                }
                else
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
                }
                return;
            }
            else
            {
                int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength,nTempRegister);
                if(tempLengthFlag != LENGTH_RIGHT)
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                }
                else
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
                    this->calculateDataBufferSize();
                    setVerifyFlag(FLAG_RIGHT);
                }
            }

        }
    }
}
QString CMDDataBlock::startAddress_Pro()
{
    return m_startAddress_Pro;
}
void CMDDataBlock::setStartAddress_Pro(QString address)
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        qint32 nTempRegister = 0;
        qint16 nTempLength = 0;
        int nTempAddressFormat = 0;
        int tempRegisterFlag = verifyRegisterFormat(QString::number((int)m_registerType_Pro,10),nTempRegister,nTempAddressFormat,nTempLength);
        if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
        {
            return;
        }
        qint32 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(address,nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            return;
        }

        qint32 nTempEndAddress = 0;

        int tempEndAddressFlag = verifyAddressFormat(m_endAddress_Pro,nTempEndAddress,nTempAddressFormat);
        if(tempEndAddressFlag != ADDRESS_FORMAT_ERROR)
        {
            int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength,nTempRegister);
            if(tempLengthFlag != LENGTH_RIGHT)
            {
                if(nTempStartAddress>nTempEndAddress)
                {
                    nTempStartAddress = nTempEndAddress;
                }
                else
                {
                    nTempStartAddress = m_endAddress_Pro.toInt()-119;
                }
            }
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
            m_startAddress_Pro = QString::number(nTempStartAddress,nTempAddressFormat);
            m_strStartAddress = m_startAddress_Pro;
            m_startAddress = nTempStartAddress;
            this->calculateDataBufferSize();
            setVerifyFlag(FLAG_RIGHT);
        }
        else
        {
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
            m_startAddress_Pro = QString::number(nTempStartAddress,nTempAddressFormat);
            m_strStartAddress = m_startAddress_Pro;
            m_startAddress = nTempStartAddress;
        }
    }
}

QString CMDDataBlock::endAddress_Pro()
{
    return m_endAddress_Pro;
}
void CMDDataBlock::setEndAddress_Pro(QString address)
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        qint32 nTempRegister = 0;
        qint16 nTempLength = 0;
        int nTempAddressFormat = 0;
        int tempRegisterFlag = verifyRegisterFormat(QString::number((int)m_registerType_Pro,10),nTempRegister,nTempAddressFormat,nTempLength);
        if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
        {
            return;
        }

        qint32 nTempEndAddress = 0;

        int tempEndAddressFlag = verifyAddressFormat(address,nTempEndAddress,nTempAddressFormat);
        if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            return;
        }

        qint32 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag != ADDRESS_FORMAT_ERROR)
        {
            int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength,nTempRegister);
            if(tempLengthFlag != LENGTH_RIGHT)
            {
                if(nTempStartAddress>nTempEndAddress)
                {
                    nTempEndAddress = nTempStartAddress;
                }
                else
                {
                    nTempEndAddress = m_startAddress_Pro.toInt()+119;
                }
            }
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
            m_endAddress_Pro = QString::number(nTempEndAddress,nTempAddressFormat);
            m_strEndAddress = m_endAddress_Pro;
            m_endAddress = nTempEndAddress;
            this->calculateDataBufferSize();
            setVerifyFlag(FLAG_RIGHT);
        }
        else
        {
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
            m_endAddress_Pro = QString::number(nTempEndAddress,nTempAddressFormat);
            m_strEndAddress = m_endAddress_Pro;
            m_endAddress = nTempEndAddress;
        }

    }
}
void CMDDataBlock::propertyLoadFinish_Pro()
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        if(this == qobject_cast<CMDDataBlock*>(dev->getPropertyBrowse()->object()))
        {
            qint32 nTempRegister = 0;
            qint16 nTempLength = 0;
            int nTempAddressFormat = 0;
            int tempRegisterFlag = verifyRegisterFormat(QString::number((int)m_registerType_Pro,10),nTempRegister,nTempAddressFormat,nTempLength);
            if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::red);
                return;
            }
            qint32 nTempStartAddress = 0;

            int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);
            if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
            }

            qint32 nTempEndAddress = 0;

            int tempEndAddressFlag = verifyAddressFormat(m_endAddress_Pro,nTempEndAddress,nTempAddressFormat);
            if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
            }
            if(tempEndAddressFlag == ADDRESS_FORMAT_RIGHT&&tempStartAddressFlag == ADDRESS_FORMAT_RIGHT)
            {
                int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength,nTempRegister);
                if(tempLengthFlag != LENGTH_RIGHT)
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                }
            }
        }
    }
}
//**************************************************************************//

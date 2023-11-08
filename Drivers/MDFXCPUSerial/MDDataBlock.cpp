#include "MDDataBlock.h"
#include <QStringList>
#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
CMDDataBlock::CMDDataBlock()
{
     m_szReadCMD = "0";
     m_szWriteCMD = "1";
     m_nOffsetAddr	= 0x0080;
     m_nBitOffset = 0;
     m_registerType=0;
     m_startAddress=0;
     m_endAddress=0;
     m_dataBufferSize=0;
     m_dataBuffer=NULL;
     setEnabled(true);
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
    CMDDevice *dev = (CMDDevice*)device();
    if(NULL != dev)
    {
        switch (dev->PLCType())
        {
         default:
        case PLC_FX:
        case PLC_FX0:
        case PLC_FX0N:
            m_szReadCMD	= "0";
            m_szWriteCMD	= "1";
            break;
        case PLC_FX2N:
            m_szReadCMD	= "E00";
            m_szWriteCMD	= "E10";
            break;
        }
        switch (m_registerType)
        {
        case ID_MEM_X:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
               m_nOffsetAddr = 0x0080;
               break;
           case PLC_FX2N:
               m_nOffsetAddr = 0x240;
               break;
            }
        }
            break;
        case ID_MEM_Y:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
               m_nOffsetAddr = 0x00A0;
               break;
           case PLC_FX2N:
               m_nOffsetAddr = 0x180;
               break;
            }
        }
            break;
        case ID_MEM_M:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
                if (m_startAddress >= 8000)
                {
                    m_nOffsetAddr	= 0x01E0;
                    m_nBitOffset	= 0x2E40;
                }
                else
                {
                    m_nOffsetAddr	= 0x0100;
                    m_nBitOffset	= 0x0800;
                }
               break;
           case PLC_FX2N:
                if (m_startAddress >= 8000)
                {
                    m_nOffsetAddr	= 0x01C0;
                    m_nBitOffset	= 0x2D40;
                }
                else
                {
                    m_nOffsetAddr	= 0x0;
                    m_nBitOffset	= 0;
                }
               break;
            }
        }
            break;
        case ID_MEM_S:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
                m_nOffsetAddr	= 0;
                m_nBitOffset	= 0;
               break;
           case PLC_FX2N:
                m_nOffsetAddr	= 0x280;
                m_nBitOffset	= 0x1400;
               break;
            }
        }
            break;
        case ID_MEM_CS:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
               m_nOffsetAddr = 0x01C0;
               break;
           case PLC_FX2N:
               m_nOffsetAddr = 0x1E0;
               break;
            }
        }
            break;
        case ID_MEM_TS:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
               m_nOffsetAddr = 0x00C0;
               break;
           case PLC_FX2N:
               m_nOffsetAddr = 0x200;
               break;
            }
        }
            break;
        case ID_MEM_D:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
               m_nOffsetAddr = 0x1000;
               break;
           case PLC_FX2N:
               m_nOffsetAddr = 0x4000;
               break;
            }
            if (m_startAddress>=8000)
            {
                m_nOffsetAddr = 0x0E00;
            }
        }
            break;
        case ID_MEM_C:
        {
            if (m_startAddress >= 200)
            {
                m_nOffsetAddr = 0xC00;
            }
            else
            {
                m_nOffsetAddr = 0xA00;
            }
        }
            break;
        case ID_MEM_T:
        {
            switch (dev->PLCType())
            {
            default:
           case PLC_FX:
           case PLC_FX0:
           case PLC_FX0N:
               m_nOffsetAddr = 0x0800;
               break;
           case PLC_FX2N:
               m_nOffsetAddr = 0x1000;
               break;
            }
        }
            break;
        default:
            break;
        }
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
    m_startAddress= ((CMDDataBlock*)otherDataBlock)->startAddress();
    m_endAddress = ((CMDDataBlock*)otherDataBlock)->endAddress ();
    m_registerType = ((CMDDataBlock*)otherDataBlock)->registerType();
    calculateDataBufferSize();
 }
void CMDDataBlock::setCmdRead(QString readCmd)
{
    m_szReadCMD = readCmd;
}
void CMDDataBlock::setCmdWrite(QString writeCmd)
{
    m_szWriteCMD = writeCmd;
}
QString CMDDataBlock::cmdRead()
{
    return m_szReadCMD;
}
QString CMDDataBlock::cmdWrite()
{
    return m_szWriteCMD;
}
void CMDDataBlock::setOffsetAddr(qint16 offsetAddr)
{
    m_nOffsetAddr = offsetAddr;
}
qint16 CMDDataBlock::offsetAddr()
{
    return m_nOffsetAddr;
}

void CMDDataBlock::setBitOffsetAddr(qint16 bitOffsetAddr)
{
    m_nBitOffset = bitOffsetAddr;
}
qint16 CMDDataBlock::bitOffsetAddr()
{
    return m_nBitOffset;
}
//开始地址
void CMDDataBlock::setStartAddress(qint16 address)
{
    m_startAddress = address;
    calculateDataBufferSize();
}

qint16 CMDDataBlock::startAddress()
{
    return m_startAddress;
}

//结束地址
void CMDDataBlock::setEndAddress(qint16 address)
{
    m_endAddress = address;
    calculateDataBufferSize();
}

qint16 CMDDataBlock::endAddress()
{
    return m_endAddress;
}
void CMDDataBlock::setRegisterType(qint16 registerType)
{
    m_registerType = registerType;
    calculateDataBufferSize();
}

qint16 CMDDataBlock::registerType()
{
    return m_registerType;
}
void CMDDataBlock::getVariableAddressList(QStringList &varAddressList)
{
    QString strRegisterType = "";
    switch (m_registerType)
    {
    case ID_MEM_X:
        strRegisterType = "X";
        break;
    case ID_MEM_Y:
        strRegisterType = "Y";
        break;
    case ID_MEM_M:
        strRegisterType = "M";
        break;
    case ID_MEM_S:
        strRegisterType = "S";
        break;
    case ID_MEM_CS:
        strRegisterType = "CS";
        break;
    case ID_MEM_TS:
        strRegisterType = "TS";
        break;
    case ID_MEM_D:
        strRegisterType = "D";
        break;
    case ID_MEM_C:
        strRegisterType = "C";
        break;
    case ID_MEM_T:
        strRegisterType = "T";
        break;
    default:
        strRegisterType = "?";
        break;
    }
    QString addressTemp = QString("%1.%2%3--%1.%2%4")
            .arg(name())
            .arg(strRegisterType)
            .arg(m_startAddress)
            .arg(m_endAddress);
    varAddressList.append(addressTemp);
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
        tempbuffer[0] = m_dataBuffer[internalAddress + 1];
        tempbuffer[1] =  m_dataBuffer[internalAddress];
        memcpy(&data,tempbuffer,2);
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

qint16 CMDDataBlock::getDataBufferSize()
{
    return m_dataBufferSize;
}

void CMDDataBlock::calculateDataBufferSize()
{
    switch (m_registerType)
    {
    case ID_MEM_X:
    case ID_MEM_Y:
    {

        qint16 m_nTemp1 = (qint16)(m_startAddress/100)* 8 + (qint16)(m_startAddress%100/10);
        qint16 nTemp = m_endAddress%100;
        if (nTemp >77)
        {
            nTemp = 77;
        }
        qint16 m_nTemp2 = (qint16)(m_endAddress/100)* 8 + (qint16)(nTemp/10);

        m_dataBufferSize = m_nTemp2 - m_nTemp1+1;

    }
        break;
    case ID_MEM_M:
    case ID_MEM_S:
    case ID_MEM_CS:
    case ID_MEM_TS:
    {
        qint16 m_nTemp1 = (qint16)(m_startAddress/8);
        qint16 m_nTemp2 = (qint16)(m_endAddress/8);

        m_dataBufferSize = m_nTemp2 - m_nTemp1 +1;
    }
        break;
    case ID_MEM_D:
    case ID_MEM_T:
    {
        qint16 nlength = m_endAddress-m_startAddress +1;
        m_dataBufferSize = nlength*2;
    }
        break;
    case ID_MEM_C:
    {
        if (m_startAddress >= 200)
        {
            qint16 nlength = m_endAddress-m_startAddress +1;
            m_dataBufferSize = nlength*4;
        }
        else
        {
            qint16 nlength = m_endAddress-m_startAddress +1;
            m_dataBufferSize = nlength*2;
        }
    }
        break;

    default:
        break;
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
        return ret;
    }

    QT_TRY
    {
        nsVar->setInternalAddress(-1);
        QStringList addressSplit = nsVar->address().split(".");
        if (addressSplit.count() < 2)
        {
            return ret;
        }
        QString strRegisterType = "";
        switch (m_registerType)
        {
        case ID_MEM_X:
            strRegisterType = "X";
            break;
        case ID_MEM_Y:
            strRegisterType = "Y";
            break;
        case ID_MEM_M:
            strRegisterType = "M";
            break;
        case ID_MEM_S:
            strRegisterType = "S";
            break;
        case ID_MEM_CS:
            strRegisterType = "CS";
            break;
        case ID_MEM_TS:
            strRegisterType = "TS";
            break;
        case ID_MEM_D:
            strRegisterType = "D";
            break;
        case ID_MEM_C:
            strRegisterType = "C";
            break;
        case ID_MEM_T:
            strRegisterType = "T";
            break;
        default:
            return ret;
        }
        QString strRegisterTypeVar=addressSplit[1];
        strRegisterTypeVar.truncate(strRegisterType.size());

        if (strRegisterType!=strRegisterTypeVar)
        {
            return ret;
        }
        QString strAddress = addressSplit[1];
        ushort address =  strAddress.remove(0,strRegisterType.size()).toUShort();

        if(address <m_startAddress||address>m_endAddress)
        {
            return ret;
        }
        switch (m_registerType)
        {
           case ID_MEM_X:
           case ID_MEM_Y:
               {
                   qint16 internalAddress = -1;
                   qint16 internalSubAddress = -1;
                   if (nsVar->originalDataType() == MDDataType::Bool)
                   {
                       if((address%10)<=7 &&(address%100/10)<=7&&(address/100)<=7)
                       {
                           qint16 m_nTemp1 = (qint16)(m_startAddress/100)* 8 + (qint16)(m_startAddress%100/10);
                           qint16 nTemp = address%100;
                           if (nTemp >77)
                           {
                               nTemp = 77;
                           }
                           qint16 m_nTemp2 = (qint16)(address/100)* 8 + (qint16)(nTemp/10);

                           internalAddress = m_nTemp2 - m_nTemp1;

                           internalSubAddress = address%10;
                           if(internalAddress>=m_dataBufferSize)
                           {
                               internalAddress = -1;
                           }
                           else
                           {
                               ret = true;
                           }
                           if (internalSubAddress > 7)
                           {
                               internalSubAddress =7;
                           }
                       }
                       else
                       {
                           return ret;
                       }
                       nsVar->setInternalAddress(internalAddress);
                       nsVar->setInternalSubAddress(internalSubAddress);
                   }
               }
               break;
           case ID_MEM_M:
           case ID_MEM_S:
           case ID_MEM_CS:
           case ID_MEM_TS:
              {
                   qint16 internalAddress = -1;
                   qint16 internalSubAddress = -1;
                   if (nsVar->originalDataType() == MDDataType::Bool)
                   {
                       qint16 m_nTemp1 = (qint16)(m_startAddress/8);
                       qint16 m_nTemp2 = (qint16)(address/8);
                       internalAddress = m_nTemp2 - m_nTemp1;
                       internalSubAddress = address%8;
                       if(internalAddress>=m_dataBufferSize)
                       {
                           internalAddress = -1;
                       }
                       else
                       {
                           ret = true;
                       }
                       if (internalSubAddress > 7)
                       {
                           internalSubAddress =7;
                       }
                    }
                    nsVar->setInternalAddress(internalAddress);
                    nsVar->setInternalSubAddress(internalSubAddress);
               }
               break;
           case ID_MEM_T:
               {
                   qint16 internalAddress = -1;
                   if((nsVar->originalDataType() == MDDataType::Int16) ||(nsVar->originalDataType() == MDDataType::UInt16))
                   {
                        internalAddress = (qint16)((address - m_startAddress)*2);
                        if (internalAddress+1 >= m_dataBufferSize)
                        {
                            internalAddress = -1;
                        }
                        else
                        {
                            ret = true;
                        }
                   }
                   nsVar->setInternalAddress(internalAddress);
                   nsVar->setInternalSubAddress(-1);
                }
                break;
        case ID_MEM_C:
            {
                qint16 internalAddress = -1;
                if((nsVar->originalDataType() == MDDataType::Int16) ||(nsVar->originalDataType() == MDDataType::UInt16))
                {
                     if(m_startAddress >= 200)
                     {
                         return ret;
                     }
                     internalAddress = (qint16)((address - m_startAddress)*2);
                     if (internalAddress+1 >= m_dataBufferSize)
                     {
                         internalAddress = -1;
                     }
                     else
                     {
                         ret = true;
                     }
                }
                else if((nsVar->originalDataType() == MDDataType::Int32) ||(nsVar->originalDataType() == MDDataType::UInt32))
                {
                    if(m_startAddress<200)
                    {
                        return ret;
                    }
                    internalAddress = (qint16)((address - m_startAddress)*4);
                    if (internalAddress+3 >= m_dataBufferSize)
                    {
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                }
                else
                {
                    return ret;
                }
                nsVar->setInternalAddress(internalAddress);
                nsVar->setInternalSubAddress(-1);
             }
             break;
           case ID_MEM_D:
               {
                   qint16 internalAddress = (qint16)((address - m_startAddress)*2);
                   switch (nsVar->originalDataType())
                   {
                       case MDDataType::Int16:
                       case MDDataType::UInt16:
                           {
                               if (internalAddress+1 >= m_dataBufferSize)
                               {
                                   internalAddress = -1;
                               }
                               else
                               {
                                   ret = true;
                               }
                           }
                           break;
                       case MDDataType::Int32:
                       case MDDataType::UInt32:
                       case MDDataType::Float:
                           {
                               if (internalAddress+3 >= m_dataBufferSize)
                               {
                                   internalAddress = -1;
                               }
                               else
                               {
                                   ret = true;
                               }
                           }
                           break;
                       default:
                           break;
                   }
                   nsVar->setInternalAddress(internalAddress);
                   nsVar->setInternalSubAddress(-1);
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
        bool bDataType = false;//!数据类型标志位
        unsigned char tempBuffer[4];
        switch (m_registerType)
        {
            case ID_MEM_X:
            case ID_MEM_Y:
            case ID_MEM_M:
            case ID_MEM_S:
            case ID_MEM_TS:
            case ID_MEM_CS:
               {
                   if (nsVar->originalDataType() == MDDataType::Bool)
                   {
                       qint32 dataValue = CBitFuntons::getBit((qint32)m_dataBuffer[nsVar->internalAddress()],nsVar->internalSubAddress());

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
                       bDataType = true;
                   }
               }
               break;
           case ID_MEM_T:
               {
                   tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                   tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 1];
                   switch (nsVar->originalDataType())
                   {
                   case MDDataType::Int16:
                       {
                           qint16 tempValue=0;
                           memcpy(&tempValue,tempBuffer,2);
                           varDataValue =tempValue;
                           bRightValue = true;
                       }
                       break;
                   case MDDataType::UInt16:
                       {
                           quint16 tempValue=0;
                           memcpy(&tempValue,tempBuffer,2);
                           varDataValue =tempValue;
                           bRightValue = true;
                       }
                       break;
                   default:
                       bDataType = true;
                       break;
                   }

                }
                break;
            case ID_MEM_C:
                {
                    tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                    tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 1];
                    tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 2];
                    tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 3];
                    switch (nsVar->originalDataType())
                    {
                    case MDDataType::Int16:
                        {
                            if(m_startAddress >=200)
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
                            if(m_startAddress >=200)
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
                    case MDDataType::Int32:
                        {
                            if(m_startAddress <200)
                            {
                                bDataType = true;
                            }
                            else
                            {
                                qint32 tempValue=0;
                                memcpy(&tempValue,tempBuffer,4);
                                varDataValue =tempValue;
                                bRightValue = true;
                            }
                        }
                        break;
                    case MDDataType::UInt32:
                        {
                            if(m_startAddress <200)
                            {
                                bDataType = true;
                            }
                            else
                            {
                                quint32 tempValue=0;
                                memcpy(&tempValue,tempBuffer,4);
                                varDataValue =tempValue;
                                bRightValue = true;
                            }
                        }
                        break;
                    default:
                        bDataType = true;
                        break;
                    }

                 }
                 break;
           case ID_MEM_D:
               {
                   tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                   tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 1];
                   tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 2];
                   tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 3];
                   switch (nsVar->originalDataType())
                   {
                       case MDDataType::Int16:
                           {
                               qint16 tempValue=0;
                               memcpy(&tempValue,tempBuffer,2);
                               varDataValue =tempValue;
                               bRightValue = true;
                           }
                           break;
                       case MDDataType::UInt16:
                           {
                               quint16 tempValue=0;
                               memcpy(&tempValue,tempBuffer,2);
                               varDataValue =tempValue;
                               bRightValue = true;
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
           }
        }
        else if(bDataType == true)
        {
            nsVar->refreshData(varDataValue, timeStamp(), MDVariableDataQuality::DataTypeError, false);

        }
    }
    if(changeVarCount>0 && lastChangeVar!=NULL)
    {
        lastChangeVar->notifyDeviceVariablesChange(false);
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
            nsVar->refreshData(NULL, timeStamp(), quality(), true);
        }
    }
    if(lastChangeVar!=NULL)
    {
        lastChangeVar->notifyDeviceVariablesChange(true);
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
    CMDProtocolCommand  *pProtocoCommand = (CMDProtocolCommand*)cmd;
    if(cmd->getCommandType() == ProtocolCommandType::Read)
    {
        if (cmd->getCommandResult() == ProtocolCommandResult::Right)
        {
           QMutexLocker locker(&m_dataBufferMutex);

           if (NULL == m_dataBuffer)
           {
               m_dataBuffer = new quint8[m_dataBufferSize];
           }
           char chTempDataBuffer[1024];
           memset(chTempDataBuffer,0,1024);
           if(pProtocoCommand->bagHeadFlag())
           {
               memcpy(chTempDataBuffer,cmd->getReceiveData()+2,m_dataBufferSize*2);
           }
           else
           {
               memcpy(chTempDataBuffer,cmd->getReceiveData()+1,m_dataBufferSize*2);
           }
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
           //memcpy(m_dataBuffer,cmd->getReceiveData()+3,m_dataBufferSize);
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

    element = doc.createElement("StartAddress");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->startAddress()));
    element.appendChild(elementText);

    element = doc.createElement("EndAddress");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->endAddress()));
    element.appendChild(elementText);

    element = doc.createElement("RegisterType");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->registerType()));
    element.appendChild(elementText);

    element = doc.createElement("PollRate");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->pollRate()));
    element.appendChild(elementText);

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
        else if(0==QString::compare(element.tagName(),"StartAddress", Qt::CaseInsensitive))
        {
            this->setStartAddress(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"EndAddress", Qt::CaseInsensitive))
        {
            this->setEndAddress(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"RegisterType", Qt::CaseInsensitive))
        {
            this->setRegisterType(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(element.text().toInt());
        }
        element = element.nextSiblingElement();
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
            this->setStartAddress(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"EndAddress", Qt::CaseInsensitive))
        {
            this->setEndAddress(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"RegisterType", Qt::CaseInsensitive))
        {
            this->setRegisterType(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(parameterValue.toInt());
        }
    }

}

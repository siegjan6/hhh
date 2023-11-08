#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"


CMDProtocolCommand::CMDProtocolCommand()
{
     m_readCmdDataLength = 0;
      setReceiveBufferSize(256);//PKX20151119
}
CMDProtocolCommand::~CMDProtocolCommand()
{
}
quint8* CMDProtocolCommand::getSendData()
{
    return m_sendData;
}
void CMDProtocolCommand::setupCommand()
{

    if (getCommandType() == ProtocolCommandType::Write)
    {
         setupWriteSingleVariableCommand();
    }
    else if (getCommandType() == ProtocolCommandType::BlockWrite)
    {
        setupWriteMultiVariableCommand();
    }
    else
    {
       setupReadCommand();
    }
}
bool CMDProtocolCommand::setupReadCommand()
{
    bool bRet = false;
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();
    if (NULL == dev || NULL == db)
    {
        return bRet;
    }
    setCommandLength(0);
    setReceiveDataLength(0);

    ushort usLength = (ushort)(db->endAddress() - db->startAddress() + 1);

    m_sendData[0] = dev->station(); //!站号

    switch (db->registerType())
    {
        case 0://!Coil Status
            {
                m_sendData[1] = 0x01;//!功能码
                m_readCmdDataLength = (ushort)(usLength / 8);
                if (usLength % 8 > 0)
                {
                    m_readCmdDataLength = (ushort)(m_readCmdDataLength + 1);
                }
            }
            break;
        case 1://!Input Status
            {
                m_sendData[1] = 0x02;//!功能码
                m_readCmdDataLength = (ushort)(usLength / 8);
                if (usLength % 8 > 0)
                {
                    m_readCmdDataLength = (ushort)(m_readCmdDataLength + 1);
                }
            }
            break;
        case 2://!holding Register
            {
                m_sendData[1] = 0x03;//!功能码
                m_readCmdDataLength = (ushort)(usLength*2);
            }
            break;
        case 3://!Input Registers
            {
                m_sendData[1] = 0x04;//!功能码
                m_readCmdDataLength = (ushort)(usLength*2);
            }
            break;
        default:
            return bRet;
    }
    qint16  startAddress = db->startAddress()-1;
    //!开始地址（高位在前，低位在后）
    m_sendData[2] = (unsigned char)((startAddress >> 8));
    m_sendData[3] = (unsigned char)((startAddress & 0xFF));

    //!寄存器数量（高位在前，低位在后）
    m_sendData[4] =(unsigned char)(usLength >> 8);
    m_sendData[5] =(unsigned char)(usLength & 0xFF);
    //!校验（高位在前，低位在后）
    ushort usCRC = CCyclicRedundancyCheck::CRC16((unsigned char*)m_sendData, 0, 5);
    m_sendData[6] =(unsigned char)(usCRC >> 8);
    m_sendData[7] =(unsigned char)(usCRC & 0xFF);
    setCommandLength(8);
    bRet = true;
    return bRet;
}


bool CMDProtocolCommand::setupWriteSingleVariableCommand()
{
    bool bRet = false;
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();
    IMDRTDBVariable *nsVar = getWriteVarAt(0);
    if (NULL == dev || NULL == db||NULL == nsVar)
    {
        return bRet;
    }
    if (db->registerType() == 1 ||    //!input Status
        db->registerType() == 3)     //!input Register
    {
        return bRet;
    }
    setCommandLength(0);


    m_sendData[0] = dev->station();//!站号

    ushort address = 0;
    if (db->registerType() == 0)//!Coil Status
    {
        address = (ushort)(db->startAddress()-1 + nsVar->internalAddress() * 8 + nsVar->internalSubAddress());
    }
    else if (db->registerType() == 2)//!holding Register
    {
        address = (ushort)(db->startAddress()-1 + nsVar->internalAddress() / 2);
    }
    switch (nsVar->originalDataType())
    {
        case MDDataType::Bool:
            {
                ushort dataValue = 0;
                if (db->registerType() == 0)//!Coil Status
                {

                    m_sendData[1] = 0x05;//!功能码
                    if(nsVar->originalValue().toBool()== true)
                    {
                        dataValue = (ushort)0xFF00;
                    }
                }
                else if (db->registerType() == 2)//!holding Register
                {

                    m_sendData[1] = 0x06;
                    if (db->getDataBufferUShortValue(nsVar->internalAddress(), dataValue))
                    {
                        if (nsVar->originalValue().toBool() == true)
                        {
                            dataValue = CBitFuntons::setBit(dataValue, nsVar->internalSubAddress());
                        }
                        else
                        {
                            dataValue = CBitFuntons::clearBit(dataValue, nsVar->internalSubAddress());
                        }
                    }
                }
                //!开始地址（高位在前，低位在后）
                m_sendData[2] = address >> 8;
                m_sendData[3] = address & 0xFF;
                m_sendData[4] = dataValue >> 8;
                m_sendData[5] = dataValue & 0xFF;
                setCommandLength(6);
            }
            break;
        case MDDataType::Int16:
            {
                short dataValue = (short)(nsVar->originalValue().toInt());
                if (db->registerType() == 0)//!Coil Status
                {
                     m_sendData[1] = 0x05;//!功能码
                }
                else if (db->registerType() == 2)//!holding Register
                {
                     m_sendData[1] = 0x06;//!功能码
                }
                //!开始地址（高位在前，低位在后）
                m_sendData[2] = address >> 8;
                m_sendData[3] = address & 0xFF;
                m_sendData[4] = dataValue >> 8;
                m_sendData[5] = dataValue & 0xFF;
                setCommandLength(6);
             }
            break;
        case MDDataType::UInt16:
            {

                ushort dataValue =(ushort)(nsVar->originalValue().toUInt());

                if (db->registerType() == 0)//!Coil Status
                {
                     m_sendData[1] = 0x05;//!功能码
                }
                else if (db->registerType() == 2)//!holding Register
                {
                     m_sendData[1] = 0x06;//!功能码
                }
                //!开始地址（高位在前，低位在后）
                m_sendData[2] = address >> 8;
                m_sendData[3] = address & 0xFF;
                m_sendData[4] = dataValue >> 8;
                m_sendData[5] = dataValue & 0xFF;
                setCommandLength(6);
            }
            break;
        case MDDataType::Float:
            {
                float dataValueTemp = nsVar->originalValue().toFloat();
                quint8 *dataValue  =(quint8*)&dataValueTemp;
                if (db->registerType() == 0)//!Coil Status
                {
                    m_sendData[1] = 0x0F;//!功能码
                }
                else if (db->registerType() == 2)//!holding Register
                {
                    m_sendData[1] = 0x10;
                }
                m_sendData[2] = address >> 8;
                m_sendData[3] = address & 0xFF;

                short length = 2;
                m_sendData[4] = length >> 8;
                m_sendData[5] = length & 0xFF;
                m_sendData[6] = 4;

                m_sendData[7] = *(dataValue+1);
                m_sendData[8] = *(dataValue);
                m_sendData[9] = *(dataValue+3);
                m_sendData[10] = *(dataValue+2);
                setCommandLength(11);

            }
            break;
        case MDDataType::Int32:
            {
                int dataValue = (nsVar->originalValue().toInt());
                if (db->registerType() == 0)//!Coil Status
                {
                    m_sendData[1] = 0x0F;//!功能码
                }
                else if (db->registerType() == 2)//!holding Register
                {
                    m_sendData[1] = 0x10;//!功能码
                }
                //!开始地址（高位在前，低位在后）
                m_sendData[2] = address >> 8;
                m_sendData[3] = address & 0xFF;

                short length = 2;
                m_sendData[4] = length >> 8;
                m_sendData[5] = length & 0xFF;
                m_sendData[6] = 4;

                m_sendData[7] = (dataValue>>8)&0xFF;
                m_sendData[8] = dataValue & 0xFF;
                m_sendData[9] = (dataValue>>24)&0xFF;
                m_sendData[10] = (dataValue>>16)&0xFF;
                setCommandLength(11);

            }
            break;
        case MDDataType::UInt32:
            {
                uint dataValue = (nsVar->originalValue().toUInt());

                if (db->registerType() == 0)//!Coil Status
                {
                    m_sendData[1] = 0x0F;//!功能码
                }
                else if (db->registerType() == 2)//!holding Register
                {
                    m_sendData[1] = 0x10;//!功能码
                }
                //!开始地址（高位在前，低位在后）
                m_sendData[2] = address >> 8;
                m_sendData[3] = address & 0xFF;

                short length = 2;
                m_sendData[4] = length >> 8;
                m_sendData[5] = length & 0xFF;
                m_sendData[6] = 4;

                m_sendData[7] = (dataValue>>8)&0xFF;
                m_sendData[8] = dataValue & 0xFF;
                m_sendData[9] = (dataValue>>24)&0xFF;
                m_sendData[10] = (dataValue>>16)&0xFF;
                setCommandLength(11);
            }
            break;
        case MDDataType::String:
              {
                  if(db->registerType() == 0)
                  {
                      return bRet;
                  }
                  QString tempstring= nsVar->originalValue().toString();
                  QByteArray tempstring1 = tempstring.toLatin1();
                  char* tempstring2 = tempstring1.data();
                  int  nstrWtiteSize = tempstring.size();

                  quint16 nRegulationSize = 0;
                  db->getDataBufferUShortValue(nsVar->internalAddress(), nRegulationSize);

                  if(nRegulationSize>=(db->endAddress()-address-1)*2)
                  {
                    nRegulationSize = (db->endAddress()-address-1)*2;
                  }
                  quint16 cmdLength = 1;

                  m_sendData[1] = 0x10;
                  cmdLength++;

                  m_sendData[2] = (address+1) >> 8;//!开始地址（高位在前，低位在后）
                  cmdLength++;
                  m_sendData[3] = (address+1)  & 0xFF;
                  cmdLength++;

                  bool bLenghtEqual = 0;
                  if(nstrWtiteSize>=nRegulationSize)
                  {
                      nstrWtiteSize = nRegulationSize;
                      bLenghtEqual =1;
                  }
                  bool bOddOrEven = 0;
                  if(nstrWtiteSize%2 != 0)
                  {
                      bOddOrEven = 1;
                      nstrWtiteSize = nstrWtiteSize+1;
                  }
                  else
                  {
                      if(bLenghtEqual != 1)
                        nstrWtiteSize = nstrWtiteSize+2;
                  }
                  m_sendData[4] = (quint8)(((nstrWtiteSize/2) >> 8)&0xFF);
                  cmdLength++;
                  m_sendData[5] = (quint8)((nstrWtiteSize/2) & 0xFF);
                  cmdLength++;
                  m_sendData[6] = (quint8)(nstrWtiteSize);
                  cmdLength++;
                  for(int i = 0;i<(nstrWtiteSize/2);i++)
                  {
                      if(i == (nstrWtiteSize/2 - 1))
                      {
                          if(bOddOrEven)
                          {
                              m_sendData[2*i + 7] = 0x00;
                              m_sendData[2*i + 8] = *(tempstring2+2*i);
                          }
                          else
                          {
                              if(bLenghtEqual == 1)
                              {
                                  m_sendData[2*i + 7] = *(tempstring2+2*i+1);
                                  m_sendData[2*i + 8] = *(tempstring2+2*i);
                              }
                              else
                              {
                                  m_sendData[2*i + 7] = 0x00;
                                  m_sendData[2*i + 8] = 0x00;
                              }
                          }
                      }
                      else
                      {
                          m_sendData[2*i + 7] = *(tempstring2+2*i+1);
                          m_sendData[2*i + 8] = *(tempstring2+2*i);
                      }
                      cmdLength = cmdLength + 2;
                  }
                  setCommandLength(cmdLength);
              }
              break;
        default:
            break;
    }
    if (getCommandLength() > 0)
    {

        ushort usCRC =CCyclicRedundancyCheck::CRC16((unsigned char*)m_sendData,0,getCommandLength()-1);
        m_sendData[getCommandLength()] = usCRC >> 8;//usCRC >> 8;0x08
        m_sendData[getCommandLength()+1] = usCRC & 0xFF;//usCRC & 0xFF;
        setCommandLength(getCommandLength()+2);
        bRet = true;
    }
    return bRet;
}

bool CMDProtocolCommand::setupWriteMultiVariableCommand()
{
    bool bRet = false;
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();
    if (NULL == dev || NULL == db||getWriteVarCount()<=0)
    {
        return bRet;
    }
    if (db->registerType() != 0 &&    //!Coil Status
        db->registerType() != 2)     //!holding Register
    {
        return bRet;
    }
    setCommandLength(0);

    unsigned char *dataDatabuffer =new unsigned char[db->getDataBufferSize()] ;
    db->getDataBuffer(dataDatabuffer, 0, db->getDataBufferSize());

    QString strAddr="";
    uint nMinAddress = 0;
    uint nMaxAddress = 0;
    uint nAddress = 0;
    uint nItemOff = 0;
    IMDRTDBVariable *nsVar=NULL;
    //!刷新数据
    for(int n = 0 ; n < getWriteVarCount() ; n++)
    {
        QT_TRY
        {
            nsVar=getWriteVarAt(n);
            if(NULL == nsVar)
            {
                continue;
            }
            if (nsVar->address().isNull()|| nsVar->address().isEmpty())
            {
                continue;
            }
            strAddr =nsVar->address();
            uint nSubAddress = 0;

            nAddress = (uint)nsVar->internalAddress();
            nSubAddress = nsVar->internalSubAddress();

            if (db->registerType() == 0)//!Coil Status
            {
                nAddress = (ushort)(db->startAddress() -1+ nAddress * 8 + nsVar->internalSubAddress());
            }
            else if (db->registerType() == 2)//!holding Register
            {
                nAddress = (ushort)(db->startAddress() -1+ nAddress / 2);
            }

            if(n ==0 )
            {
                nMinAddress = nAddress;
                nMaxAddress = nAddress;
            }
            nItemOff = (nAddress +1- db->startAddress())*2;
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
                {
                    if (db->registerType() == 0)  //!Coil Status
                    {
                        int offset = 0;
                        unsigned char subOffet = 0;
                        int diff = (int)(nAddress+1 - db->startAddress());
                        offset = diff / 8;
                        subOffet = (unsigned char)(diff % 8);

                        if (nsVar->originalValue().toBool() == true)
                        {
                            *(dataDatabuffer+offset) = (unsigned char)CBitFuntons::setBit(*(dataDatabuffer+offset), subOffet);
                        }
                        else
                        {
                            *(dataDatabuffer+offset) =(unsigned char)CBitFuntons::clearBit(*(dataDatabuffer+offset), subOffet);
                        }
                    }
                    else
                    {
                        ushort tempValue=0;
                        unsigned char tempBuffer[2];
                        tempBuffer[0] = dataDatabuffer[nItemOff+1];
                        tempBuffer[1] = dataDatabuffer[nItemOff];
                        memcpy(&tempValue,tempBuffer,2);
                        if (nsVar->originalValue().toBool() == true)
                        {
                            tempValue = CBitFuntons::setBit(tempValue, nSubAddress);
                        }
                        else
                        {
                            tempValue = CBitFuntons::clearBit(tempValue, nSubAddress);
                        }
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                        dataDatabuffer[nItemOff+1] =(unsigned char) (tempValue&0xFF);

                    }
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::Int16:
                {
                    if (db->registerType() == 0)  //!Coil Status
                    {
                        delete dataDatabuffer;
                        return bRet;
                    }
                    qint16 tempValue=(qint16)(nsVar->originalValue().toInt());
                    dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+1] =(unsigned char) (tempValue&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::UInt16:
                {
                    if (db->registerType() == 0)  //!Coil Status
                    {
                        delete dataDatabuffer;
                        return bRet;
                    }
                    quint16 tempValue=(quint16)(nsVar->originalValue().toUInt());
                    dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+1] =(unsigned char) (tempValue&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::Int32:
                {
                    if (db->registerType() == 0)  //!Coil Status
                    {
                        delete dataDatabuffer;
                        return bRet;
                    }
                    qint32 tempValue=(qint32)(nsVar->originalValue().toInt());
                    dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+1] =(unsigned char) (tempValue&0xFF);
                    dataDatabuffer[nItemOff+2] = (unsigned char)((tempValue>>24)&0xFF);
                    dataDatabuffer[nItemOff+3] = (unsigned char)((tempValue>>16)&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress+1)
                    {
                        nMaxAddress = nAddress+1;
                    }

                }
                break;
            case MDDataType::UInt32:
                {
                    if (db->registerType() == 0)  //!Coil Status
                    {
                        delete dataDatabuffer;
                        return bRet;
                    }
                    quint32 tempValue=(quint32)(nsVar->originalValue().toUInt());
                    dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+1] =(unsigned char) (tempValue&0xFF);
                    dataDatabuffer[nItemOff+2] = (unsigned char)((tempValue>>24)&0xFF);
                    dataDatabuffer[nItemOff+3] = (unsigned char)((tempValue>>16)&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress+1)
                    {
                        nMaxAddress = nAddress+1;
                    }
                }
                break;
            case MDDataType::Float:
                {
                    if (db->registerType() == 0)  //!Coil Status
                    {
                        delete dataDatabuffer;
                        return bRet;
                    }
                    float tempValue=(float)(nsVar->originalValue().toFloat());
                    unsigned char *pTempVale = (unsigned char *)&tempValue;
                    dataDatabuffer[nItemOff+1] = *pTempVale ;
                    dataDatabuffer[nItemOff] = *(pTempVale+1) ;
                    dataDatabuffer[nItemOff+3] = *(pTempVale+2);
                    dataDatabuffer[nItemOff+2] = *(pTempVale+3);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress+1)
                    {
                        nMaxAddress = nAddress+1;
                    }
                }
                break;
            default:
                break;
            }
        }
        QT_CATCH (...)
        {
            continue;
        }
    }
    uint nLength = nMaxAddress - nMinAddress+1;

    if((nMinAddress+1) < db->startAddress()|| (nMinAddress+1) > db->endAddress()||
       nLength<=0|| nLength > db->endAddress()-db->startAddress()+1)
    {
        if(NULL!=dataDatabuffer)
        {
            delete dataDatabuffer;
            dataDatabuffer = NULL;
        }
        return bRet;
    }

    uint nIndex = 6;

    m_sendData[0] = dev->station() ;//!站号

    if (db->registerType() == 0)//!Coil Status
    {

       m_sendData[1] = 0x0F;//!功能码

       //!开始地址(高位在前，低位在后)
       m_sendData[2] = (unsigned char)(nMinAddress>>8);
       m_sendData[3] = (unsigned char)(nMinAddress& 0xFF);

       //!寄存器个数(高位在前，低位在后)
       m_sendData[4] = (unsigned char)(nLength>>8);
       m_sendData[5] = (unsigned char)(nLength& 0xFF);

       //!数据字节数
       uint nByte = nLength/8;
       if(nLength%8 != 0)
       {
           nByte +=1;
       }
       m_sendData[nIndex]=(unsigned char)nByte;
       nIndex++;
       //!数据
       for (uint i = (nMinAddress+1 - db->startAddress()) / 8; i < nByte; i++)
       {
           m_sendData[nIndex] = dataDatabuffer[i];
           nIndex++;
       }
    }
    else if (db->registerType() == 2)//holding Register
    {

       m_sendData[1] = 0x10;//!功能码

       //!开始地址(高位在前，低位在后)
       m_sendData[2] = (unsigned char)(nMinAddress>>8);
       m_sendData[3] = (unsigned char)(nMinAddress& 0xFF);

       //!寄存器个数(高位在前，低位在后)
       m_sendData[4] = (unsigned char)(nLength>>8);
       m_sendData[5] = (unsigned char)(nLength& 0xFF);
       //!数据字节数
       m_sendData[nIndex]=(unsigned char)(nLength*2);
       nIndex++;
       //!数据
       for(uint i = 0 ;i<nLength;i++)
       {
           m_sendData[nIndex] = dataDatabuffer[(nMinAddress+1-db->startAddress()) *2+i*2];
           nIndex++;
           m_sendData[nIndex] = dataDatabuffer[(nMinAddress+1-db->startAddress()) *2+i*2+1];
           nIndex++;
       }

    }
    ushort usCRC =CCyclicRedundancyCheck::CRC16((unsigned char *)m_sendData,0,nIndex-1);
    m_sendData[nIndex] = usCRC >> 8;
    nIndex++;
    m_sendData[nIndex] = usCRC & 0xFF;
    nIndex++;
    setCommandLength(nIndex);
    if(NULL!=dataDatabuffer)
    {
        delete dataDatabuffer;
        dataDatabuffer = NULL;
    }
    bRet = true;
    return bRet;
}
quint8* CMDProtocolCommand::getReceiveData()
{
    return m_receiveData;
}

ProtocolCommandResult CMDProtocolCommand::parseCommand()
{
    setCommandResult(ProtocolCommandResult::Error);
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();
    if (NULL == dev || NULL == db)
    {
        return getCommandResult();
    }
    if(getReceiveDataLength()<2)
    {
        setCommandResult(ProtocolCommandResult::SplitData);
        return getCommandResult();
    }
    //!功能码判断
    if (m_sendData[1] != m_receiveData[1])//nFunc>0x80异常
    {
        setReceiveDataLength(0);
        return getCommandResult();
    }
    if (getCommandType() == ProtocolCommandType::Read)
    {
        if (getReceiveDataLength() > getReceiveBufferSize() ||
            getReceiveDataLength() > (m_readCmdDataLength + 5))
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        else if (getReceiveDataLength() < (m_readCmdDataLength + 5))
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
    }
    else
    {
        if (getReceiveDataLength() > 8)//!正确返回码长度应该为8
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        else if (getReceiveDataLength() < 8)
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
    }
    //!设备地址判断
    if (m_sendData[0] != m_receiveData[0])
    {
        setReceiveDataLength(0);
        return getCommandResult();
    }

    if (getCommandType() == ProtocolCommandType::Read)
    {
        //!数据区长度判断
        if (m_readCmdDataLength != m_receiveData[2])
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        //!CRC校验判断
        ushort usCRCLow = m_receiveData[getReceiveDataLength() - 1];
        ushort usCRCHigh = m_receiveData[getReceiveDataLength() - 2];

        ushort usCRC = CCyclicRedundancyCheck::CRC16((unsigned char *)m_receiveData, 0, getReceiveDataLength() - 3);

        if (usCRCLow != (usCRC & 0xff) || usCRCHigh != (usCRC >> 8))
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        else
        {
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::Right);
            return getCommandResult();
        }
    }
    else
    {

        for (int i = 0; i < getReceiveDataLength()-2; i++)
        {
            if (m_sendData[i] != m_receiveData[i])
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
        }
        setReceiveDataLength(0);
        setCommandResult(ProtocolCommandResult::Right);
        return getCommandResult();
    }
    return getCommandResult();
}

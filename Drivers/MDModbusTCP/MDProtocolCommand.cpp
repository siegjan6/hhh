#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "MDChannel.h"

CMDProtocolCommand::CMDProtocolCommand()
{
     m_readCmdDataLength = 0;
     m_transactionID = 1;
     setReceiveBufferSize(256);
}
CMDProtocolCommand::~CMDProtocolCommand()
{
}
void CMDProtocolCommand::setTransactionID(quint16 transactionID)
{
    m_transactionID = transactionID;
}

quint16 CMDProtocolCommand::getTransactionID()
{
    return m_transactionID;
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

    quint16 usLength = (quint16)(db->endAddress() - db->startAddress() + 1);

    m_transactionID = ((CMDChannel*)getChannel())->getTransactionID();

    m_sendData[0] = m_transactionID >> 8;//!传输标识符高位
    m_sendData[1] = m_transactionID & 0xFF;//!传输标识符低位

    m_sendData[2] = 0x00; //!协议标识符  modbus tcp/ip 为定值0
    m_sendData[3] = 0x00;

    m_sendData[4] = 0x00;//!长度（从设备地址开始的字节数）: 此处固定为 6，
    m_sendData[5] = 0x06;

    m_sendData[6] = dev->station();//!站号
    /*!
      *\section
      * 功能码
      */
    switch (db->registerType())
    {
        case 0://!Coil Status
            {
                m_sendData[7] = 0x01;
                m_readCmdDataLength = (quint16)(usLength / 8);
                if (usLength % 8 > 0)
                {
                    m_readCmdDataLength = (quint16)(m_readCmdDataLength + 1);
                }
            }
            break;
        case 1://!Input Status
            {
                m_sendData[7] = 0x02;
                m_readCmdDataLength = (quint16)(usLength / 8);
                if (usLength % 8 > 0)
                {
                    m_readCmdDataLength = (quint16)(m_readCmdDataLength + 1);
                }
            }
            break;
        case 2://!holding Register
            {
                m_sendData[7] = 0x03;
                m_readCmdDataLength = (quint16)(usLength*2);
            }
            break;
        case 3://!Input Registers
            {
                m_sendData[7] = 0x04;
                m_readCmdDataLength = (quint16)(usLength*2);
            }
            break;
        default:
            return bRet;
    }
    qint16  usStartAddress = db->startAddress()-1;

    m_sendData[8] = (unsigned char)((usStartAddress >> 8));//!开始地址（高位在前，低位在后）
    m_sendData[9] = (unsigned char)((usStartAddress & 0xFF));

    m_sendData[10] =(unsigned char)(usLength >> 8);//!寄存器数量（高位在前，低位在后）
    m_sendData[11] =(unsigned char)(usLength & 0xFF);

    setCommandLength(12);
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
    if (db->registerType() == 1 ||    //Input Status
        db->registerType() == 3)     //Input Register
    {
        return bRet;
    }
    setCommandLength(0);


    m_transactionID = ((CMDChannel*)getChannel())->getTransactionID();

    m_sendData[0] = m_transactionID >> 8;//!传输标识符(高位在前低位在后)
    m_sendData[1] = m_transactionID & 0xFF;


    m_sendData[2] = 0x00;//!协议标识符  modbus tcp/ip 为定值0
    m_sendData[3] = 0x00;


    quint16 cmdLength =0;


    m_sendData[6] = dev->station();//!站号
    cmdLength++;
    //功能码

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
                    m_sendData[7] = 0x05;//!命令码
                    cmdLength++;
                    if(nsVar->originalValue().toBool()== true)
                    {
                        dataValue = (ushort)0xFF00;
                    }
                }
                else if (db->registerType() == 2)//!holding Register
                {

                    m_sendData[7] = 0x06;//!命令码
                    cmdLength++;
                    if (db->getDataBufferUShortValue(nsVar->internalAddress(), dataValue))
                    {
                        if (nsVar->originalValue().toBool() == true)
                        {
                            dataValue = CBitFuntons::setBit(dataValue, nsVar->internalSubAddress());
                        }
                        else
                        {
                             dataValue =CBitFuntons::clearBit(dataValue, nsVar->internalSubAddress());
                        }
                    }
                }

                m_sendData[8] = address >> 8;//!开始地址（高位在前，低位在后）
                cmdLength++;
                m_sendData[9] = address & 0xFF;
                cmdLength++;
                m_sendData[10] = dataValue >> 8;
                cmdLength++;
                m_sendData[11] = dataValue & 0xFF;
                cmdLength++;
                m_sendData[4] = cmdLength >> 8;
                m_sendData[5] = cmdLength & 0xFF;
                setCommandLength(cmdLength+6);
            }
            break;
        case MDDataType::Int16:
        {
            short dataValue =(short)(nsVar->originalValue().toInt());

            if (db->registerType() == 0)//!Coil Status
            {
                 m_sendData[7] = 0x05; //!命令码
                 cmdLength++;
            }
            else if (db->registerType() == 2)//!holding Register
            {
                 m_sendData[7] = 0x06;
                 cmdLength++;
            }

            m_sendData[8] = address >> 8; //!开始地址（高位在前，低位在后）
            cmdLength++;
            m_sendData[9] = address & 0xFF;
            cmdLength++;
            m_sendData[10] = dataValue >> 8;
            cmdLength++;
            m_sendData[11] = dataValue & 0xFF;
            cmdLength++;
            m_sendData[4] = cmdLength >> 8;
            m_sendData[5] = cmdLength & 0xFF;
            setCommandLength(cmdLength+6);
        }
        break;
        case MDDataType::UInt16:
            {
                ushort dataValue =(ushort)(nsVar->originalValue().toUInt());

                if (db->registerType() == 0)//!Coil Status
                {
                     m_sendData[7] = 0x05;//!命令码
                     cmdLength++;
                }
                else if (db->registerType() == 2)//!holding Register
                {
                     m_sendData[7] = 0x06;
                     cmdLength++;
                }

                m_sendData[8] = address >> 8; //!开始地址（高位在前，低位在后）
                cmdLength++;
                m_sendData[9] = address & 0xFF;
                cmdLength++;
                m_sendData[10] = dataValue >> 8;
                cmdLength++;
                m_sendData[11] = dataValue & 0xFF;
                cmdLength++;
                m_sendData[4] = cmdLength >> 8;
                m_sendData[5] = cmdLength & 0xFF;
                setCommandLength(cmdLength+6);
            }
            break;
        case MDDataType::Float:
            {
                float dataValueTemp = nsVar->originalValue().toFloat();
                quint8 *dataValue  =(quint8*)&dataValueTemp;

                if (db->registerType() == 0)//!Coil Status
                {
                    m_sendData[7] = 0x0F;//!命令码
                    cmdLength++;
                }
                else if (db->registerType() == 2)//!holding Register
                {
                    m_sendData[7] = 0x10;
                    cmdLength++;
                }

                m_sendData[8] = address >> 8;//!开始地址（高位在前，低位在后）
                cmdLength++;
                m_sendData[9] = address & 0xFF;
                cmdLength++;

                quint16 dataLength= 2;
                m_sendData[10] = dataLength >> 8;
                cmdLength++;
                m_sendData[11] = dataLength & 0xFF;
                cmdLength++;

                m_sendData[12] = dataLength*2;
                 cmdLength++;

                m_sendData[13] = *(dataValue+1);
                cmdLength++;
                m_sendData[14] = *(dataValue);
                cmdLength++;
                m_sendData[15] = *(dataValue+3);
                cmdLength++;
                m_sendData[16] = *(dataValue+2);
                cmdLength++;
                m_sendData[4] = cmdLength >> 8;
                m_sendData[5] = cmdLength & 0xFF;
                setCommandLength(cmdLength+6);
            }
            break;
        case MDDataType::Int32:
        {
            int dataValue = (nsVar->originalValue().toInt());

            if (db->registerType() == 0)//!Coil Status
            {
                m_sendData[7] = 0x0F;//!命令码
                cmdLength++;
            }
            else if (db->registerType() == 2)//!holding Register
            {
                m_sendData[7] = 0x10;
                cmdLength++;
            }

            m_sendData[8] = address >> 8;//!开始地址（高位在前，低位在后）
            cmdLength++;
            m_sendData[9] = address & 0xFF;
            cmdLength++;

            quint16 dataLength= 2;
            m_sendData[10] = dataLength >> 8;
            cmdLength++;
            m_sendData[11] = dataLength & 0xFF;
            cmdLength++;

            m_sendData[12] = dataLength*2;
             cmdLength++;

            m_sendData[13] = (dataValue>>8)&0xFF;
            cmdLength++;
            m_sendData[14] = dataValue & 0xFF;
            cmdLength++;
            m_sendData[15] = (dataValue>>24)&0xFF;
            cmdLength++;
            m_sendData[16] = (dataValue>>16)&0xFF;
            cmdLength++;
            m_sendData[4] = cmdLength >> 8;
            m_sendData[5] = cmdLength & 0xFF;
            setCommandLength(cmdLength+6);
        }
        break;
        case MDDataType::UInt32:
            {
                uint dataValue = (nsVar->originalValue().toUInt());

                if (db->registerType() == 0)//!Coil Status
                {
                    m_sendData[7] = 0x0F;//!命令码
                    cmdLength++;
                }
                else if (db->registerType() == 2)//!holding Register
                {
                    m_sendData[7] = 0x10;
                    cmdLength++;
                }

                m_sendData[8] = address >> 8;//!开始地址（高位在前，低位在后）
                cmdLength++;
                m_sendData[9] = address & 0xFF;
                cmdLength++;

                quint16 dataLength= 2;
                m_sendData[10] = dataLength >> 8;
                cmdLength++;
                m_sendData[11] = dataLength & 0xFF;
                cmdLength++;

                m_sendData[12] = dataLength*2;
                cmdLength++;

                m_sendData[13] = (dataValue>>8)&0xFF;
                cmdLength++;
                m_sendData[14] = dataValue & 0xFF;
                cmdLength++;
                m_sendData[15] = (dataValue>>24)&0xFF;
                cmdLength++;
                m_sendData[16] = (dataValue>>16)&0xFF;
                cmdLength++;
                m_sendData[4] = cmdLength >> 8;
                m_sendData[5] = cmdLength & 0xFF;
                setCommandLength(cmdLength+6);
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

                  m_sendData[7] = 0x10;
                  cmdLength++;

                  m_sendData[8] = (address+1) >> 8;//!开始地址（高位在前，低位在后）
                  cmdLength++;
                  m_sendData[9] = (address+1)  & 0xFF;
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
                  m_sendData[10] = (quint8)(((nstrWtiteSize/2) >> 8)&0xFF);
                  cmdLength++;
                  m_sendData[11] = (quint8)((nstrWtiteSize/2) & 0xFF);
                  cmdLength++;
                  m_sendData[12] = (quint8)(nstrWtiteSize);
                  cmdLength++;

                  for(int i = 0;i<(nstrWtiteSize/2);i++)
                  {
                      if(i == (nstrWtiteSize/2 - 1))
                      {
                          if(bOddOrEven)
                          {
                              m_sendData[2*i + 13] = 0x00;
                              m_sendData[2*i + 14] = *(tempstring2+2*i);
                          }
                          else
                          {
                              if(bLenghtEqual == 1)
                              {
                                  m_sendData[2*i + 13] = *(tempstring2+2*i+1);
                                  m_sendData[2*i + 14] = *(tempstring2+2*i);
                              }
                              else
                              {
                                  m_sendData[2*i + 13] = 0x00;
                                  m_sendData[2*i + 14] = 0x00;
                              }
                          }
                      }
                      else
                      {
                          m_sendData[2*i + 13] = *(tempstring2+2*i+1);
                          m_sendData[2*i + 14] = *(tempstring2+2*i);
                      }
                      cmdLength = cmdLength + 2;
                  }
                  m_sendData[4] = cmdLength >> 8;
                  m_sendData[5] = cmdLength & 0xFF;
                  setCommandLength(cmdLength+6);
              }
              break;
        default:
            break;
    }
    if (getCommandLength() > 0)
    {
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
                nAddress = (ushort)(db->startAddress() - 1 + nAddress * 8 + nsVar->internalSubAddress());
            }
            else if (db->registerType() == 2)//!holding Register
            {
                nAddress = (ushort)(db->startAddress() - 1+ nAddress / 2);
            }

            if(n ==0 )
            {
                nMinAddress = nAddress;
                nMaxAddress = nAddress;
            }
            nItemOff = (nAddress +1 - db->startAddress())*2;
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
    m_transactionID = ((CMDChannel*)getChannel())->getTransactionID();

    m_sendData[0] = m_transactionID >> 8;//!传输标识符（高位在前低位在后）
    m_sendData[1] = m_transactionID & 0xFF;


    m_sendData[2] = 0x00;//!协议标识符  modbus tcp/ip 为定值0
    m_sendData[3] = 0x00;
    uint nIndex = 6;

    m_sendData[6] = dev->station() ; //!站地址
    nIndex ++;
    if (db->registerType() == 0)//!Coil Status
    {

       m_sendData[nIndex] = 0x0F;//!功能码
       nIndex++;

       m_sendData[nIndex] = (unsigned char)(nMinAddress>>8); //!开始地址高位
       nIndex++;
       m_sendData[nIndex] = (unsigned char)(nMinAddress& 0xFF);	//!开始地址低位
       nIndex++;

       m_sendData[nIndex] = (unsigned char)(nLength>>8);//!寄存器个数高位
       nIndex++;
       m_sendData[nIndex] = (unsigned char)(nLength& 0xFF); //!寄存器个数低位
       nIndex++;
       //!数据字节数
       uint nByte = nLength/8;
       if(nLength%8 != 0)
       {
           nByte +=1;
       }
       m_sendData[nIndex]=(unsigned char)nByte;
       nIndex++;
       //!数据
       for (uint i = (nMinAddress +1 - db->startAddress()) / 8; i < nByte; i++)
       {
           m_sendData[nIndex] = dataDatabuffer[i];
           nIndex++;
       }
       m_sendData[4] = (quint8)(((nIndex-6)>>8)&0xFF);
       m_sendData[5] = (quint8)((nIndex-6)&0xFF);
    }
    else if (db->registerType() == 2)//!holding Register
    {

       m_sendData[nIndex] = 0x10;//!功能码
       nIndex++;


       m_sendData[nIndex] = (unsigned char)(nMinAddress>>8);//!开始地址高位
       nIndex++;
       m_sendData[nIndex] = (unsigned char)(nMinAddress& 0xFF);	//!开始地址低位
       nIndex++;

       m_sendData[nIndex] = (unsigned char)(nLength>>8);//!寄存器个数高位
       nIndex++;
       m_sendData[nIndex] = (unsigned char)(nLength& 0xFF); //!寄存器个数低位
       nIndex++;
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
       m_sendData[4] = (quint8)(((nIndex-6)>>8)&0xFF);
       m_sendData[5] = (quint8)((nIndex-6)&0xFF);

    }
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
    if(getReceiveDataLength()<8)
    {
        setCommandResult(ProtocolCommandResult::SplitData);
        return getCommandResult();
    }
    //!功能码
    if(m_receiveData[7]!=m_sendData[7]) //异常  nFunc>0x80
    {
        setReceiveDataLength(0);
        return getCommandResult();
    }
    //!write返回码:[0][1]传输标识符+[2][3]协议标识符+[4][5]长度+[6]单元标识符+[7]功能码+[8....]
    //!read返回码:[0][1]传输标识符+[2][3]协议标识符+[4][5]长度+[6]单元标识符+[7]功能码+[8]数据单元数+[9....]数据
    if (getCommandType() == ProtocolCommandType::Read)
    {
        if (getReceiveDataLength() > getReceiveBufferSize() ||
            getReceiveDataLength() > (m_readCmdDataLength + 9))
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        else if (getReceiveDataLength() < (m_readCmdDataLength + 9))
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
    }
    else
    {
        if (getReceiveDataLength() > 12)//!正确返回码长度应该为12
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        else if (getReceiveDataLength() < 12)
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
    }
    //!传输标识符和协议标识符 判断
    for(int i=0;i<4;i++)
    {
        if(m_receiveData[i]!=m_sendData[i])
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
    }
    //!单元标识符
    if(m_receiveData[6]!=m_sendData[6])
    {
        setReceiveDataLength(0);
        return getCommandResult();
    }

    if (getCommandType() == ProtocolCommandType::Read)
    {
        //!数据区长度判断
        quint16 dataLength = (quint16)m_receiveData[8];
        if (m_readCmdDataLength != dataLength)
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        //!返回码长度不对
        quint16 receiveLength= (quint16)m_receiveData[4];
        receiveLength = receiveLength<<8;
        receiveLength= receiveLength+(quint16)m_receiveData[5];
        if(receiveLength+6 > (m_readCmdDataLength+9))
        {
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::Error);
            return getCommandResult();
        }
        else if(receiveLength+6 <(m_readCmdDataLength+9))
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
        setReceiveDataLength(0);
        setCommandResult(ProtocolCommandResult::Right);
        return getCommandResult();
    }
    else
    {

        if(0x05 == m_receiveData[7]||0x06==m_receiveData[7])
        {
            for(int i=0;i<getReceiveDataLength() ;i++)
            {
                if(m_sendData[i]!=m_receiveData[i])
                {
                    setReceiveDataLength(0);
                    setCommandResult(ProtocolCommandResult::Error);
                    return getCommandResult();
                }
            }
            setCommandResult(ProtocolCommandResult::Right);
            return getCommandResult();
        }
        else
        {
            //!开始地址
            if(m_receiveData[8]!=m_sendData[8]||m_receiveData[9]!=m_sendData[9])
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
            setCommandResult(ProtocolCommandResult::Right);
            return getCommandResult();

        }
    }
    return getCommandResult();
}

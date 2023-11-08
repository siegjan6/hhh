#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "MDChannel.h"

CMDProtocolCommand::CMDProtocolCommand()
{
     m_readCmdDataLength = 0;
     m_transactionID = 1;
     setReceiveBufferSize(1300);
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
    if(0==dev->protocalType())//!3E
    {
        unsigned char nTemp[21] =
        {
            0x50,0,                                                        //!副标题
            0,0,                                                              //!网络编号，PLC编号
            0xFF,0x03,0,                                               //!目标模块的IO编号（2字节），目标模块的站编号
            0x0C,0,                                                       //!请求数据长度（2字节）
            0,0,                                                             //!CPU监视定时器
            0x01,0x04,0,0,                                          //!命令，子命令
            0,0,0,                                                         //!起始软元件（3字节）
            0,                                                               //!软元件代码
            0,0                                                             //!点数
        };
        //!网络编号
        nTemp[2] = (unsigned char)dev->netNumber();

        //!PLC 编号
        nTemp[3] = (unsigned char)dev->station();

        //!起始软元件
        nTemp[15] = (unsigned char)(db->simStar()&0xFF);
        nTemp[16] = (unsigned char)((db->simStar()>>8)&0xFF);
        nTemp[17] = (unsigned char)((db->simStar()>>16)&0xFF);
        //!软元件代码
        nTemp[18] = (unsigned char)db->areaCode();

        qint32 nLength = db->revLen()/2;
        //!点数
        nTemp[19] = (unsigned char)(nLength&0xFF);
        nTemp[20] = (unsigned char)((nLength>>8)&0xFF);

        memcpy(m_sendData,nTemp,21);

        setCommandLength(21);
        bRet = true;
        return bRet;
    }
    else
    {
        unsigned char nTemp[12] =
        {
            0x01,                                                           //!子标题
            0xFF,                                                           //!PLC编号
            0xA,0x0,                                                     //!CPU监视定时器
            0x40,0x0,0x0,0x0,0x20,0x59,                  //!起始软元件
            0x02,                                                         //!软元件点数
            0                                                                //!
        };
        //!起始软元件
        nTemp[4] = (unsigned char)(db->simStar()&0xFF);
        nTemp[5] = (unsigned char)((db->simStar()>>8)&0xFF);
        nTemp[6] = (unsigned char)((db->simStar()>>16)&0xFF);
        nTemp[7] = (unsigned char)((db->simStar()>>24)&0xFF);

        //!软元件代码
        nTemp[8] = (unsigned char)(db->areaCode() & 0xFF);
        nTemp[9] = (unsigned char)((db->areaCode() >> 8) & 0xFF);
        nTemp[10] = (unsigned char)(db->revLen()/2);
        setCommandLength(12);
        bRet = true;
        return bRet;
    }


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
    setCommandLength(0);
    setReceiveDataLength(0);
    if(dev->protocalType() == 0)
    {
        unsigned char nTemp[21] =
        {
            0x50,0,                                                        //副标题
            0,0,                                                              //网络编号，PLC编号
            0xFF,0x03,0,                                               //目标模块的IO编号（2字节），目标模块的站编号
            0x0C,0,                                                       //请求数据长度（2字节）
            0,0,                                                             //CPU监视定时器
            0x01,0x14,0,0,                                           //命令，子命令				//11,12,13,14
            0,0,0,                                                          //起始软元件（3字节）		//15,16,17
            0,                                                                //软元件代码				//18
            0,0                                                              //点数						//19,20
        };
        //网络编号
        nTemp[2] = (unsigned char)dev->netNumber();

        //PLC 编号
        nTemp[3] = (unsigned char)dev->station();
        quint32 address = 0;
        if (db->registerType()  ==ID_MEM_D||
                db->registerType() ==ID_MEM_SD||
                    db->registerType() ==ID_MEM_W||
                        db->registerType() ==ID_MEM_SW||
                            db->registerType() ==ID_MEM_R||
                                db->registerType() ==ID_MEM_Z||
                                    db->registerType() ==ID_MEM_TN||
                                        db->registerType() ==ID_MEM_SN||
                                            db->registerType() ==ID_MEM_CN)
        {
            address = (quint32)(db->startAddress() + nsVar->internalAddress()/2);

            nTemp[15] = (unsigned char)(address & 0xFF);
            nTemp[16] = (unsigned char)((address>>8) & 0xFF);
            nTemp[17] = (unsigned char)((address>>16) & 0xFF);

            nTemp[18] = (unsigned char)db->areaCode();
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                ushort dataValue = 0;
                if (db->getDataBufferUShortValue(nsVar->internalAddress(), dataValue))
                {
                    if (nsVar->originalValue().toBool() != false)
                    {
                        dataValue = CBitFuntons::setBit(dataValue, nsVar->internalSubAddress());
                    }
                    else
                    {
                         dataValue =CBitFuntons::clearBit(dataValue, nsVar->internalSubAddress());
                    }
                }
                nTemp[19] = 0x01;
                nTemp[20] = 0x00;
                memcpy(m_sendData,nTemp,21);
                memcpy(&m_sendData[21],&dataValue,2);
                quint16 nLength = 21 +2 -9;
                memcpy(&m_sendData[7],&nLength,2);
                setCommandLength(23);
            }
                break;
            case MDDataType::Int16:
            {
                nTemp[19] = 0x01;
                nTemp[20] = 0x00;
                memcpy(m_sendData,nTemp,21);

                qint16 dataValue = (qint16)nsVar->originalValue().toInt();

                memcpy(&m_sendData[21],&dataValue,2);

                quint16 nLength = 21 +2 -9;
                memcpy(&m_sendData[7],&nLength,2);

                setCommandLength(23);
            }
                break;
            case MDDataType::UInt16:
            {
                nTemp[19] = 0x01;
                nTemp[20] = 0x00;
                memcpy(m_sendData,nTemp,21);

                quint16 dataValue = (quint16)nsVar->originalValue().toUInt();

                memcpy(&m_sendData[21],&dataValue,2);

                quint16 nLength = 21 +2 -9;
                memcpy(&m_sendData[7],&nLength,2);

                setCommandLength(23);
            }
                break;
            case MDDataType::Int32:
            {
                nTemp[19] = 0x02;
                nTemp[20] = 0x00;
                memcpy(m_sendData,nTemp,21);

                qint32 dataValue = (qint32)nsVar->originalValue().toInt();

                memcpy(&m_sendData[21],&dataValue,4);

                quint16 nLength = 21 +4 -9;
                memcpy(&m_sendData[7],&nLength,2);

                setCommandLength(25);
            }
                break;
            case MDDataType::UInt32:
            {
                nTemp[19] = 0x02;
                nTemp[20] = 0x00;
                memcpy(m_sendData,nTemp,21);

                quint32 dataValue = (quint32)nsVar->originalValue().toUInt();

                memcpy(&m_sendData[21],&dataValue,4);

                quint16 nLength = 21 +4 -9;
                memcpy(&m_sendData[7],&nLength,2);

                setCommandLength(25);
            }
                break;
            case MDDataType::Float:
            {
                nTemp[19] = 0x02;
                nTemp[20] = 0x00;
                memcpy(m_sendData,nTemp,21);

                float dataValue = (float)nsVar->originalValue().toFloat();

                memcpy(&m_sendData[21],&dataValue,4);

                quint16 nLength = 21 +4 -9;
                memcpy(&m_sendData[7],&nLength,2);

                setCommandLength(25);
            }
                break;
            case MDDataType::Double:
            {
                nTemp[19] = 0x04;
                nTemp[20] = 0x00;
                memcpy(m_sendData,nTemp,21);

                double dataValue = (double)nsVar->originalValue().toDouble();

                memcpy(&m_sendData[21],&dataValue,8);

                quint16 nLength = 21 +8 -9;
                memcpy(&m_sendData[7],&nLength,2);

                setCommandLength(29);
            }
                break;
            case MDDataType::String:
            {
                nTemp[15] = (unsigned char)((address +1 ) & 0xFF);
                nTemp[16] = (unsigned char)(((address + 1)>>8) & 0xFF);
                nTemp[17] = (unsigned char)(((address + 1)>>16) & 0xFF);


                ushort nRegulationSize = 0;
                if(db->getDataBufferUShortValue(nsVar->internalAddress(), nRegulationSize))
                {
                    QString strDataValue = nsVar->originalValue().toString();
                    QByteArray tempstring1 = strDataValue.toLatin1();
                    char* tempstring2 = tempstring1.data();
                    qint32 nStrLength = (qint32)strDataValue.size();
                    qint32 nLengthScope = (db->endAddress()-address)*2;

                    if(nRegulationSize>=nLengthScope)
                    {
                      nRegulationSize = nLengthScope;
                    }
                    bool bLenghtEqual = 0;
                    if(nStrLength>=nRegulationSize)
                    {
                        nStrLength = nRegulationSize;
                        bLenghtEqual =1;
                    }
                    bool bOddOrEven = 0;
                    if(nStrLength%2 != 0)
                    {
                        bOddOrEven = 1;
                        nStrLength = nStrLength+1;
                    }
                    else
                    {
                        if(bLenghtEqual != 1)
                          nStrLength = nStrLength+2;
                    }
                    nTemp[19] = (unsigned char)((nStrLength/2)&0xFF);
                    nTemp[20] = (unsigned char)(((nStrLength/2)>>8)&0xFF);
                    memcpy(m_sendData,nTemp,21);

                    if(bOddOrEven)
                    {
                        m_sendData[21+nStrLength-1] = 0x00;
                        memcpy(&m_sendData[21],tempstring2,nStrLength-1);
                    }
                    else
                    {
                        if(bLenghtEqual == 1)
                        {
                            memcpy(&m_sendData[21],tempstring2,nStrLength);

                        }
                        else
                        {
                            memcpy(&m_sendData[21],tempstring2,nStrLength-2);
                            m_sendData[21+nStrLength-2] = 0x00;
                            m_sendData[21+nStrLength-1] = 0x00;
                        }
                    }

                    quint16 nLength = 21 +nStrLength -9;
                    memcpy(&m_sendData[7],&nLength,2);

                    setCommandLength(nLength+9);
                }

            }
                break;
            default:
                return bRet;
                break;
            }

        }
        else
        {
            address = (quint32)((db->startAddress()/16 + nsVar->internalAddress() /2)*16 + nsVar->internalSubAddress());
            if(nsVar->originalDataType()== MDDataType::Bool)
            {
                nTemp[11] = 2;		//!随机写
                nTemp[13] = 1;
                nTemp[15] = 1;		//!点数

                nTemp[16] = (unsigned char)(address & 0xFF);
                nTemp[17] = (unsigned char)((address>>8) & 0xFF);
                nTemp[18] = (unsigned char)((address>>16) & 0xFF);

                nTemp[19] =(unsigned char) db->areaCode();
                if(nsVar->originalValue().toBool()!= false)
                {
                    nTemp[20] = 1;
                }
                else
                {
                    nTemp[20] = 0;
                }
                memcpy(m_sendData,nTemp,21);
                setCommandLength(21);
            }
            else
            {
                return bRet;
            }
        }

    }
    else
    {
        unsigned char nTemp[13] =
        {
            0x03,                                               //子标题 0
            0xFF,                                               //PLC编号        1
            0xA,0x0,                                         //CPU监视定时器  2 3
            0x01,                                              //软元件点数     4
            0,                                                    //               5
            0x0,0x0,0x0,0x0,0x20,0x44,        //指定软元件 6.7.8.9.10.11
            0x03,                                             //ON/OFF指定
        };
        quint32 address = 0;
        if (db->registerType()  ==ID_MEM_D||
                db->registerType() ==ID_MEM_SD||
                    db->registerType() ==ID_MEM_W||
                        db->registerType() ==ID_MEM_SW||
                            db->registerType() ==ID_MEM_R||
                                db->registerType() ==ID_MEM_Z||
                                    db->registerType() ==ID_MEM_TN||
                                        db->registerType() ==ID_MEM_SN||
                                            db->registerType() ==ID_MEM_CN)
        {
            address = (quint32)(db->startAddress() + nsVar->internalAddress()/2);
            if (db->registerType() == ID_MEM_CN  && db->startAddress() > 199)
            {
                address = (quint32)(db->startAddress() + nsVar->internalAddress()/4);
            }
            //起始软元件(4字节，低－>高)
            memcpy(&nTemp[4],&address,4);

            //!软元件代码
            nTemp[8] =(unsigned char)(db->areaCode() & 0xFF);
            nTemp[9] = (unsigned char)((db->areaCode() >> 8) & 0xFF);

            nTemp[10] = 1;		//!点数

            nTemp[11] = 0;
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                ushort dataValue = 0;
                if (db->getDataBufferUShortValue(nsVar->internalAddress(), dataValue))
                {
                    if (nsVar->originalValue().toBool() != false)
                    {
                        dataValue = CBitFuntons::setBit(dataValue, nsVar->internalSubAddress());
                    }
                    else
                    {
                         dataValue =CBitFuntons::clearBit(dataValue, nsVar->internalSubAddress());
                    }
                }
                //!点数
                nTemp[10] = 1;

                memcpy(m_sendData,nTemp,12);

                memcpy(&m_sendData[12],&dataValue,2);

                setCommandLength(14);
            }
                break;
            case MDDataType::Int16:
            {
                //!点数
                nTemp[10] = 1;
                memcpy(m_sendData,nTemp,12);

                qint16 dataValue = (qint16)nsVar->originalValue().toInt();

                memcpy(&m_sendData[12],&dataValue,2);

                setCommandLength(14);
            }
                break;
            case MDDataType::UInt16:
            {
                //!点数
                nTemp[10] = 1;
                memcpy(m_sendData,nTemp,12);

                quint16 dataValue = (quint16)nsVar->originalValue().toUInt();

                memcpy(&m_sendData[12],&dataValue,2);

                setCommandLength(14);
            }
                break;
            case MDDataType::Int32:
            {
                //!点数
                nTemp[10] = 2;
                memcpy(m_sendData,nTemp,12);

                qint32 dataValue = (qint32)nsVar->originalValue().toInt();

                memcpy(&m_sendData[12],&dataValue,4);

                setCommandLength(16);
            }
                break;
            case MDDataType::UInt32:
            {
                //!点数
                nTemp[10] = 2;
                memcpy(m_sendData,nTemp,12);

                quint32 dataValue = (quint32)nsVar->originalValue().toUInt();

                memcpy(&m_sendData[12],&dataValue,4);

                setCommandLength(16);
            }
                break;
            case MDDataType::Float:
            {
                //!点数
                nTemp[10] = 2;
                memcpy(m_sendData,nTemp,12);

                float dataValue = (float)nsVar->originalValue().toFloat();

                memcpy(&m_sendData[12],&dataValue,4);

                setCommandLength(16);
            }
                break;
            case MDDataType::String:
            {
                address = address+1;

                memcpy(&nTemp[4],&address,4);

                ushort nRegulationSize = 0;
                if(db->getDataBufferUShortValue(nsVar->internalAddress(), nRegulationSize))
                {
                    QString strDataValue = nsVar->originalValue().toString();
                    QByteArray tempstring1 = strDataValue.toLatin1();
                    char* tempstring2 = tempstring1.data();
                    qint32 nStrLength = (qint32)strDataValue.size();
                    qint32 nLengthScope = (db->endAddress()-address+1)*2;

                    if(nRegulationSize>=nLengthScope)
                    {
                      nRegulationSize = nLengthScope;
                    }
                    bool bLenghtEqual = 0;
                    if(nStrLength>=nRegulationSize)
                    {
                        nStrLength = nRegulationSize;
                        bLenghtEqual =1;
                    }
                    bool bOddOrEven = 0;
                    if(nStrLength%2 != 0)
                    {
                        bOddOrEven = 1;
                        nStrLength = nStrLength+1;
                    }
                    else
                    {
                        if(bLenghtEqual != 1)
                          nStrLength = nStrLength+2;
                    }
                    nTemp[10] = (unsigned char)((nStrLength/2)&0xFF);
                    nTemp[11] = (unsigned char)(((nStrLength/2)>>8)&0xFF);
                    memcpy(m_sendData,nTemp,12);

                    if(bOddOrEven)
                    {
                        m_sendData[12+nStrLength-1] = 0x00;
                        memcpy(&m_sendData[12],tempstring2,nStrLength-1);
                    }
                    else
                    {
                        memcpy(&m_sendData[12],tempstring2,nStrLength);
                        if(bLenghtEqual != 1)
                        {
                            m_sendData[12+nStrLength-2] = 0x00;
                            m_sendData[12+nStrLength-1] = 0x00;
                        }
                    }
                    setCommandLength(12+nStrLength);
                }

            }
                break;
            default:
                return bRet;
                break;
            }
        }
        else
        {
            address = (quint32)((db->startAddress()/16 + nsVar->internalAddress() /2 )*16 + nsVar->internalSubAddress());
            if(nsVar->originalDataType()== MDDataType::Bool)
            {
                nTemp[0] = 0x04;	//随机写
                nTemp[4] = 1;		//点数
                //!起始软元件(4字节，低－>高)
                memcpy(&nTemp[6],&address,4);

                //!软元件代码
                nTemp[10] = (unsigned char) (db->areaCode()& 0xFF);
                nTemp[11] = (unsigned char)((db->areaCode() >> 8) & 0xFF);

                if(nsVar->originalValue().toBool()!= false)
                {
                    nTemp[12] = 1;
                }
                else
                {
                    nTemp[12] = 0;
                }
                memcpy(m_sendData,nTemp,13);
                setCommandLength(13);
            }
            else
            {
                return bRet;
            }

        }
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

    setCommandLength(0);
    setReceiveDataLength(0);

    unsigned char *dataDatabuffer =new unsigned char[db->getDataBufferSize()] ;
    db->getDataBuffer(dataDatabuffer, 0, db->getDataBufferSize());

    uint nMinAddress = 0;
    uint nMaxAddress = 0;
    uint nAddress = 0;
    uint nItemOff = 0;
    uint nBitPos = 0;
    bool bC200 = false;
    IMDRTDBVariable *nsVar=NULL;

    if (db->registerType() == ID_MEM_CN && dev->protocalType() == 1 && db->startAddress() > 199)
        bC200 = true;
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

            if (db->registerType()  ==ID_MEM_D||
                    db->registerType() ==ID_MEM_SD||
                        db->registerType() ==ID_MEM_W||
                            db->registerType() ==ID_MEM_SW||
                                db->registerType() ==ID_MEM_R||
                                    db->registerType() ==ID_MEM_Z||
                                        db->registerType() ==ID_MEM_TN||
                                            db->registerType() ==ID_MEM_SN||
                                                db->registerType() ==ID_MEM_CN)
            {
                nAddress = (uint)(db->startAddress() + nsVar->internalAddress()/2);
             }
            else
            {
                nAddress = (uint)((db->startAddress()/16 + nsVar->internalAddress() /2)*16 + nsVar->internalSubAddress());
            }
            if(nAddress<db->startAddress() ||nAddress >db->endAddress()||
                    ((db->registerType()  !=ID_MEM_D&&
                     db->registerType() !=ID_MEM_SD&&
                         db->registerType() !=ID_MEM_W&&
                             db->registerType() !=ID_MEM_SW&&
                                 db->registerType() !=ID_MEM_R&&
                                     db->registerType() !=ID_MEM_Z&&
                                         db->registerType() !=ID_MEM_TN&&
                                             db->registerType() !=ID_MEM_SN&&
                                                 db->registerType() !=ID_MEM_CN)&&nsVar->originalDataType()!=MDDataType::Bool))
            {
                continue;
            }
            if(db->registerType()  ==ID_MEM_D||
                    db->registerType() ==ID_MEM_SD||
                        db->registerType() ==ID_MEM_W||
                            db->registerType() ==ID_MEM_SW||
                                db->registerType() ==ID_MEM_R||
                                    db->registerType() ==ID_MEM_Z||
                                        db->registerType() ==ID_MEM_TN||
                                            db->registerType() ==ID_MEM_SN||
                                                db->registerType() ==ID_MEM_CN)
            {
                nItemOff = (uint)(nAddress- db->simStar()) * 2;
                nBitPos = (uint)nsVar->internalSubAddress();
                if (bC200 != false)
                {
                    nItemOff = (nAddress- db->simStar()) * 4;
                    if (nsVar->originalDataType() != MDDataType::Int32 ||
                        nsVar->originalDataType() != MDDataType::UInt32 ||
                        nsVar->originalDataType() != MDDataType::Float)
                    {
                        continue;
                    }
                }

            }
            else
            {
                nItemOff = (uint)(nAddress- db->simStar())/8;
                nBitPos = (uint)(nAddress- db->simStar())%8;
            }
            if(nMinAddress > nAddress||n ==0 )
            {
                nMinAddress = nAddress;
            }

            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
                {
                    ushort tempValue=0;
                    memcpy(&tempValue,&dataDatabuffer[nItemOff],2);
                    if(nsVar->originalValue().toBool()==false)
                    {
                        tempValue= tempValue& (~(1 <<nBitPos));
                    }
                    else
                    {
                        tempValue= tempValue| (1 <<nBitPos);
                    }
                    memcpy(&dataDatabuffer[nItemOff],&tempValue,2);
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::Int16:
                {
                    qint16 tempValue=(qint16)nsVar->originalValue().toInt();
                    memcpy(&dataDatabuffer[nItemOff],&tempValue,2);
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::UInt16:
                {
                    quint16 tempValue=(quint16)nsVar->originalValue().toUInt();
                    memcpy(&dataDatabuffer[nItemOff],&tempValue,2);
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::Int32:
                {
                    qint32 tempValue=(qint32)nsVar->originalValue().toInt();
                    memcpy(&dataDatabuffer[nItemOff],&tempValue,4);

                    if (!bC200)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress +1;
                        }
                    }
                    else
                    {
                        nMaxAddress = nAddress;
                    }


                }
                break;
            case MDDataType::UInt32:
                {
                    quint32 tempValue=(quint32)nsVar->originalValue().toUInt();
                    memcpy(&dataDatabuffer[nItemOff],&tempValue,4);

                    if (!bC200)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress +1;
                        }
                    }
                    else
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::Float:
                {
                    float tempValue=(float)nsVar->originalValue().toFloat();

                    memcpy(&dataDatabuffer[nItemOff],&tempValue,4);

                    if (!bC200)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress +1;
                        }
                    }
                    else
                    {
                        nMaxAddress = nAddress;
                    }
                }
                break;
            case MDDataType::Double:
                {
                    double tempValue=(double)nsVar->originalValue().toDouble();

                    memcpy(&dataDatabuffer[nItemOff],&tempValue,8);

                    if (!bC200)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress +3;
                        }
                    }
                    else
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

    if(nMaxAddress < nMinAddress || nMaxAddress > db->endAddress()||nMinAddress<db->startAddress())
    {
        if(NULL!=dataDatabuffer)
        {
            delete dataDatabuffer;
            dataDatabuffer = NULL;
        }
        return bRet;
    }
    if(dev->protocalType() == 0)
    {
        unsigned char nTemp[21] =
        {
            0x50,0,                     //!副标题
            0,0,                           //!网络编号，PLC编号
            0xFF,0x03,0,            //!目标模块的IO编号（2字节），目标模块的站编号
            0x0C,0,				     //!请求数据长度（2字节）
            0,0,                           //!CPU监视定时器
            0x01,0x14,0,0,		 //!命令，子命令  12
            0,0,0,				         //!起始软元件（3字节）
            0,					             //!软元件代码
            0,0					         //!点数
        };
        //!网络编号
        nTemp[2] = (unsigned char)dev->netNumber();

        //!PLC 编号
        nTemp[3] = (unsigned char)dev->station();

        nTemp[15] = (unsigned char)(nMinAddress&0xFF);
        nTemp[16] = (unsigned char)((nMinAddress>>8)&0xFF);
        nTemp[17] = (unsigned char)((nMinAddress>>16)&0xFF);
        //!软元件代码
        nTemp[18] = (unsigned char)(db->areaCode()&0xFF);
        //!点数 & 数据
        quint16 TempVar = 0;
        if (db->registerType()  ==ID_MEM_D||
                db->registerType() ==ID_MEM_SD||
                    db->registerType() ==ID_MEM_W||
                        db->registerType() ==ID_MEM_SW||
                            db->registerType() ==ID_MEM_R||
                                db->registerType() ==ID_MEM_Z||
                                    db->registerType() ==ID_MEM_TN||
                                        db->registerType() ==ID_MEM_SN||
                                            db->registerType() ==ID_MEM_CN)
        {
            TempVar =(quint16) (nMaxAddress - nMinAddress +1);
            memcpy(&nTemp[19],&TempVar,2);
            memcpy(m_sendData,nTemp,21);

            //数据
            nItemOff = (nMinAddress-db->startAddress())*2;
            memcpy(&m_sendData[21],&dataDatabuffer[nItemOff],TempVar*2);

            quint16 nSendLength = 21+ TempVar*2;
            TempVar = nSendLength- 9;
            memcpy(&m_sendData[7],&TempVar,2);
            setCommandLength(nSendLength);
        }
        else
        {
            //点数
            TempVar = (quint16)(nMaxAddress - nMinAddress +1) ;
            memcpy(&nTemp[19],&TempVar,2);

            nTemp[13] = 1;  //位写
            memcpy(m_sendData,nTemp,21);
            TempVar = 21;
            unsigned char d1,d2;
            quint16 d3 = 0;
            for(uint i =nMinAddress;i<= nMaxAddress;i++)
            {
                nItemOff = (i- db->simStar())/8;
                nBitPos = (i- db->simStar())%8;
                d1 =  dataDatabuffer[nItemOff]>>nBitPos & 0x01;
                i++;
                if (i>nMaxAddress)
                {
                    d2 = 0;
                }
                else
                {
                    nItemOff = (i- db->simStar())/8;
                    nBitPos = (i- db->simStar())%8;
                    d2 = dataDatabuffer[nItemOff]>>nBitPos & 0x01;
                }
                m_sendData[TempVar++] = d1<<4 | d2;
                d3 ++;
            }
            quint16 nSendLength = 21 +d3;
            TempVar = nSendLength -9;
            memcpy(&m_sendData[7],&TempVar,2);
            setCommandLength(nSendLength);
        }
    }
    else
    {
        unsigned char nTemp[12] =
        {
            0x03,                                               //!子标题 0
            0xFF,                                               //!PLC编号        1
            0xA,0x0,                                         //!CPU监视定时器  2.3
            0x64,0x0,0x0,0x0,0x20,0x44,      //!起始软元件 4.5.6.7.8.9
            0x03,                                             //!始软元件点数
            0x0,
        };

        memcpy(&nTemp[4],&nMinAddress,4);
        //!软元件代码
        nTemp[8] = (unsigned char)(db->areaCode() & 0xFF);
        nTemp[9] = (unsigned char)((db->areaCode() >> 8) & 0xFF);
        //!点数 & 数据
        quint16 TempVar = 0;
        if (db->registerType()  ==ID_MEM_D||
                db->registerType() ==ID_MEM_SD||
                    db->registerType() ==ID_MEM_W||
                        db->registerType() ==ID_MEM_SW||
                            db->registerType() ==ID_MEM_R||
                                db->registerType() ==ID_MEM_Z||
                                    db->registerType() ==ID_MEM_TN||
                                        db->registerType() ==ID_MEM_SN||
                                            db->registerType() ==ID_MEM_CN)
        {
            //长度为256时，传输长度0，数据为256×2
            if (nMaxAddress - nMinAddress +1 >= 256)
            {
                TempVar = 256;
                nTemp[10] = 0;
            }
            else
            {
                TempVar = nMaxAddress - nMinAddress +1;
                nTemp[10] = (unsigned char)(TempVar&0xFF);
            }

            memcpy(m_sendData,nTemp,12);

            if(!bC200)
            {
                nItemOff = (nMinAddress-db->startAddress())*2;
                memcpy(&m_sendData[12],&dataDatabuffer[nItemOff],TempVar*2);
                quint16 nSendLength = 12 + TempVar*2;
                setCommandLength(nSendLength);
            }
            else
            {
                nItemOff = (nMinAddress-db->startAddress())*4;
                memcpy(&m_sendData[12],&dataDatabuffer[nItemOff],TempVar*4);
                quint16 nSendLength = 12 + TempVar*4;
                setCommandLength(nSendLength);
            }
        }
        else
        {
            nTemp[0] = 0x02;
            if ((nMaxAddress - nMinAddress +1) >= 160)
            {
                nTemp[10] = 160;
            }
            else
            {
                nTemp[10] = nMaxAddress - nMinAddress +1;
            }
            memcpy(m_sendData,nTemp,12);
            TempVar = 12;
            quint16 d3 = 0;
            unsigned char d1= 0,d2 = 0;
            uint nSimMax = nTemp[10] + nMinAddress;
            for(uint i =nMinAddress;i< nSimMax ;i++)
            {
                nItemOff = (i- db->simStar())/8;
                nBitPos = (i- db->simStar())%8;
                d1 =  dataDatabuffer[nItemOff]>>nBitPos & 0x01;
                i++;
                if (i>= nSimMax)
                {
                    d2 = 0;
                }
                else
                {
                    nItemOff = (i- db->simStar())/8;
                    nBitPos = (i- db->simStar())%8;
                    d2 = dataDatabuffer[nItemOff]>>nBitPos & 0x01;
                }
                m_sendData[TempVar++] = (d1<<4) | d2;
                d3 ++;
            }
            quint16 nSendLength = 12 +d3;
            setCommandLength(nSendLength);
        }

    }

    if(NULL!=dataDatabuffer)
    {
        delete dataDatabuffer;
        dataDatabuffer = NULL;
    }
    if(getCommandLength() > 0)
    {
        bRet = true;
    }
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
    if (getCommandType() == ProtocolCommandType::Read)
    {
        if(dev->protocalType() == 0)//!3E
        {
            if(getReceiveDataLength()<11)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            if (m_receiveData[9]!=0 || m_receiveData[10]!=0 || m_receiveData[0]!=0xd0)
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
            quint16 nLen = 0;

            memcpy(&nLen,&m_receiveData[7],2);
            if((nLen -2)!=db->revLen())
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }

            if (getReceiveDataLength() > (db->revLen()+11))
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
            else if(getReceiveDataLength() < (db->revLen()+11))
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            else
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Right);
                return getCommandResult();
            }

        }
        else//!1E
        {
            if(getReceiveDataLength()<2)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            if ( m_receiveData[0]!= (m_sendData[0]+0x80) || m_receiveData[1]!=0x0)
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
            if (getReceiveDataLength() > (db->revLen()+2))
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
            else if(getReceiveDataLength() < (db->revLen()+2))
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            else
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Right);
                return getCommandResult();
            }
        }
    }
    else
    {
        if(dev->protocalType() == 0)//!3E
        {
            if(getReceiveDataLength()<11)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            if (m_receiveData[9]!=0 || m_receiveData[10]!=0 || m_receiveData[0]!=0xd0)
            {
                if(getReceiveDataLength()<20)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
            }
        }
        else
        {
            if(getReceiveDataLength()<2)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            if(m_receiveData[1]!=0x00)
            {
                if(getReceiveDataLength()<4)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
            }
        }
        setReceiveDataLength(0);
        setCommandResult(ProtocolCommandResult::Right);
        return getCommandResult();
    }
    return getCommandResult();
}

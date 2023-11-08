#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "MDChannel.h"
//***************************PKX***************************//
S7RegInform  S71200RegInform[MAX_NUMBER_MEMAREA] =
                                                {{ID_MEM_V,"V",0x84,0x02},
                                                  {ID_MEM_I,"I",0x81,0x02},
                                                  {ID_MEM_Q,"Q",0x82,0x02},
                                                  {ID_MEM_AI,"AI",0x06,0x04},
                                                  {ID_MEM_AQ,"AQ",0x07,0x04},
                                                  {ID_MEM_T,"T",0x1F,0x1F},
                                                  {ID_MEM_C,"C",0x1E,0x1E},
                                                  {ID_MEM_HC,"HC",0x20,0x20},
                                                  {ID_MEM_M,"M",0x83,0x02},
                                                  {ID_MEM_SM,"SM",0x05,0x02},
                                                  {ID_MEM_S,"S",0x04,0x02},
                                                  {ID_MEM_PI,"PI",0x80,0x02},
                                                  {ID_MEM_PQ,"PQ",0x80,0x02},
                                                  {ID_MEM_DB,"DB",0x84,0x02} };
S7RegInform  S7300And400RegInform[MAX_NUMBER_MEMAREA] =
                                                {{ID_MEM_V,"V",0x84,0x02},
                                                  {ID_MEM_I,"I",0x81,0x02},
                                                  {ID_MEM_Q,"Q",0x82,0x02},
                                                  {ID_MEM_AI,"AI",0x06,0x04},
                                                  {ID_MEM_AQ,"AQ",0x07,0x04},
                                                  {ID_MEM_T,"T",0x1D,0x1D},
                                                  {ID_MEM_C,"C",0x1C,0x1C},
                                                  {ID_MEM_HC,"HC",0x20,0x20},
                                                  {ID_MEM_M,"M",0x83,0x02},
                                                  {ID_MEM_SM,"SM",0x05,0x02},
                                                  {ID_MEM_S,"S",0x04,0x02},
                                                  {ID_MEM_PI,"PI",0x80,0x02},
                                                  {ID_MEM_PQ,"PQ",0x80,0x02},
                                                  {ID_MEM_DB,"DB",0x84,0x02} };
S7RegInform* GetS7RegInform(qint16 nRegType,qint16 nPLCType)
{
    if((nPLCType == PLC_1200) ||(nPLCType == PLC_1500))
    {
        if((nRegType >= 0)&&(nRegType<MAX_NUMBER_MEMAREA))
        {
            return &S71200RegInform[nRegType];
        }
        else
        {
            return NULL;
        }
    }
    else if((nPLCType == PLC_300) || (nPLCType == PLC_400))
    {
        if((nRegType >= 0)&&(nRegType<MAX_NUMBER_MEMAREA))
        {
            return &S7300And400RegInform[nRegType];
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}
//************************************************************//
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

    quint16 usLength = (quint16)(db->getEndAddress() - db->getStartAddress() + 1);

    m_transactionID = ((CMDChannel*)getChannel())->getTransactionID();

    qint16  sCurrentRegisterType = db->getRegisterType();
    qint16  sCurrentStartAddress = db->getStartAddress();
    if((sCurrentRegisterType<0)||(sCurrentRegisterType >MAX_NUMBER_MEMAREA))
    {
        return bRet;
    }
    if(-1 == dev->m_nPLCComInitialStatus)
    {
        /*!
         * \section
         * 获取source TSAP与 destination TSAP（1200 1500）
         * 获取CPUrack  与  CPUslot （300 400 ）
         */
        quint8 RemoteTSAP1,RemoteTSAP2,LocalTSAP1,LocalTSAP2;
        if((dev->PLCType() == PLC_1200) || (dev->PLCType() == PLC_1500))
        {
            QString LocalTSAP = dev->LocalTSAP();
            int localindex = LocalTSAP.indexOf(".");
            if(localindex>0)
            {
                QString TempLocalTSAP1 =LocalTSAP.left(localindex);
                 LocalTSAP1 = (quint8)(TempLocalTSAP1.toUInt());
                QString TempLocalTSAP2 =LocalTSAP.right(localindex);
                 LocalTSAP2 = (quint8)(TempLocalTSAP2.toUInt());
            }
            QString RemoteTSAP = dev->RemoteTSAP();
            int remoteindex = RemoteTSAP.indexOf(".");
            if(remoteindex>0)
            {
                QString TempRemoteTSAP1 =RemoteTSAP.left(remoteindex);
                 RemoteTSAP1 = (quint8)(TempRemoteTSAP1.toUInt());
                QString TempRemoteTSAP2 =RemoteTSAP.right(remoteindex);
                 RemoteTSAP2 = (quint8)(TempRemoteTSAP2.toUInt());
            }
        }
        else if((dev->PLCType() == PLC_300) ||(dev->PLCType() == PLC_400))
        {
            LocalTSAP1 = 0x4B;
            LocalTSAP2 = 0x54;
            RemoteTSAP1 = 0x03;
            QString strCPURack = dev->LocalTSAP();
            quint8 uiCPURack  = (quint8)strCPURack.toUInt();
            QString strCPUSlot   = dev->RemoteTSAP();
            quint8   uiCPUSlot    = (quint8)strCPUSlot.toInt();
            if((uiCPURack>=0 && uiCPURack<=7)&&(uiCPUSlot>=2 && uiCPUSlot<=31))
            {
                RemoteTSAP2 = (quint8)(uiCPURack*32+uiCPUSlot);
            }
            else
            {
                return bRet;
            }

        }
        /*!
         * \brief BuildPLCServerMSG
         * 初始化第一步
         */
        quint16  ichLength = BuildPLCServerMSG(LocalTSAP1,LocalTSAP2,RemoteTSAP1,RemoteTSAP2);
        if(ichLength <= 0)
        {
            return bRet;
        }
        setCommandLength(ichLength);
        dev->m_nPLCComInitialStatus = 0;

    }
    else if(1 == dev->m_nPLCComInitialStatus)
    {
        /*!
         * \brief BuildPLCConnectMSG
         * 初始化第二步
         */
        quint16 ichLength = BuildPLCConnectMSG(m_transactionID,dev);
        if(ichLength <= 0)
        {
            return bRet;
        }
        setCommandLength(ichLength);
        dev->m_nPLCComInitialStatus = 2;

    }
    else if(dev->m_nPLCComInitialStatus >= 3)
    {
        S7RegInform*pS7RegInform = GetS7RegInform(sCurrentRegisterType,dev->PLCType());
        if((dev->PLCType() == PLC_1200) || (dev->PLCType() == PLC_1500))
        {
            if(
               (pS7RegInform->m_nRegType  != ID_MEM_Q) &&
               (pS7RegInform->m_nRegType  != ID_MEM_M) &&
               (pS7RegInform->m_nRegType  != ID_MEM_DB) &&
               (pS7RegInform->m_nRegType  != ID_MEM_I)
               )
            {
                return bRet;
            }
        }
        if(pS7RegInform == NULL)
        {
            return bRet;
        }
        quint16 ichLength = BuildRead(pS7RegInform,sCurrentStartAddress,usLength,m_transactionID,db,dev);
        if(ichLength <= 0)
        {
                return bRet;
        }
        setCommandLength(ichLength);
    }
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
 //***************************PKX***************************//
   if(dev->m_nPLCComInitialStatus < 3)
    {
        return bRet;
    }
    qint16  sCurrentRegisterType = db->getRegisterType();
    if((sCurrentRegisterType<0)||(sCurrentRegisterType >MAX_NUMBER_MEMAREA))
    {
        return bRet;
    }
    S7RegInform *pS7RegInform = GetS7RegInform(sCurrentRegisterType,dev->PLCType() );
    if(pS7RegInform == NULL)
    {
        return bRet;
    }
    if(dev->PLCType() == PLC_1200)
    {
        if(
           (pS7RegInform->m_nRegType  != ID_MEM_Q)&&
           (pS7RegInform->m_nRegType  != ID_MEM_M) &&
           (pS7RegInform->m_nRegType  != ID_MEM_DB)
           )
        {
            return bRet;
        }
    }
    else if(dev->PLCType() == PLC_1500)
    {
        if(
           (pS7RegInform->m_nRegType  != ID_MEM_Q)&&
           (pS7RegInform->m_nRegType  != ID_MEM_M) &&
           (pS7RegInform->m_nRegType  != ID_MEM_DB) &&
           (pS7RegInform->m_nRegType  != ID_MEM_I)
           )
        {
            return bRet;
        }
    }
    else if((dev->PLCType() == PLC_300)||(dev->PLCType() == PLC_400))
    {
        if(
           (pS7RegInform->m_nRegType  != ID_MEM_Q) &&
           (pS7RegInform->m_nRegType  != ID_MEM_M) &&
           (pS7RegInform->m_nRegType  != ID_MEM_DB) &&
           (pS7RegInform->m_nRegType  != ID_MEM_I) &&
           (pS7RegInform->m_nRegType  != ID_MEM_PQ) &&
           (pS7RegInform->m_nRegType  != ID_MEM_C)
           )
        {
            return bRet;
        }
    }

     int Elements = 0;
     if(nsVar->originalDataType() == MDDataType::String)
     {
         QString  strValueTemp =  nsVar->originalValue().toString();
         Elements = strValueTemp.size();
     }
     else
     {
         Elements = 1;
     }
     if(Elements<=0)
     {
         return bRet;
     }
     quint16 ElementsConvert =(quint16) (Elements & 0xFF);

     setCommandLength(0);
     m_transactionID = ((CMDChannel*)getChannel())->getTransactionID();
     quint16  uiLength = 0;
     if((dev->PLCType() == PLC_1500) || (dev->PLCType() == PLC_1200) )
     {
           uiLength = BuildWrite(pS7RegInform,nsVar,db,m_transactionID,ElementsConvert);

     }
     else if((dev->PLCType() == PLC_300)||(dev->PLCType() == PLC_400))
     {
            uiLength = BuildWrite300And400(pS7RegInform,nsVar,db,m_transactionID,ElementsConvert);
     }
     if(uiLength <= 0)
     {
        return bRet;
     }
     setCommandLength(uiLength);
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
    if(dev->m_nPLCComInitialStatus < 3)
     {
         return bRet;
     }
     qint16  sCurrentRegisterType = db->getRegisterType();
     if((sCurrentRegisterType<0)||(sCurrentRegisterType >MAX_NUMBER_MEMAREA))
     {
         return bRet;
     }
     S7RegInform *pS7RegInform = GetS7RegInform(sCurrentRegisterType,dev->PLCType() );
     if(pS7RegInform == NULL)
     {
         return bRet;
     }
     if(dev->PLCType() == PLC_1200)
     {
         if(
            (pS7RegInform->m_nRegType  != ID_MEM_Q)&&
            (pS7RegInform->m_nRegType  != ID_MEM_M) &&
            (pS7RegInform->m_nRegType  != ID_MEM_DB)
            )
         {
             return bRet;
         }
     }
     else if(dev->PLCType() == PLC_1500)
     {
         if(
            (pS7RegInform->m_nRegType  != ID_MEM_Q)&&
            (pS7RegInform->m_nRegType  != ID_MEM_M) &&
            (pS7RegInform->m_nRegType  != ID_MEM_DB) &&
            (pS7RegInform->m_nRegType  != ID_MEM_I)
            )
         {
             return bRet;
         }
     }
     else if((dev->PLCType() == PLC_300)||(dev->PLCType() == PLC_400))
     {
         if(
            (pS7RegInform->m_nRegType  != ID_MEM_Q) &&
            (pS7RegInform->m_nRegType  != ID_MEM_M) &&
            (pS7RegInform->m_nRegType  != ID_MEM_DB) &&
            (pS7RegInform->m_nRegType  != ID_MEM_I) &&
            (pS7RegInform->m_nRegType  != ID_MEM_PQ) &&
            (pS7RegInform->m_nRegType  != ID_MEM_C)
            )
         {
             return bRet;
         }
     }

    setCommandLength(0);

    unsigned char *dataDatabuffer =new unsigned char[db->getDataBufferSize()] ;
    db->getDataBuffer(dataDatabuffer, 0, db->getDataBufferSize());

    QString strAddr="";
    uint nMinAddress = 0;
    uint nMaxAddress = 0;
    uint nAddress = 0;
    uint nItemOff = 0;
    uint nSize = 1;
    IMDRTDBVariable *nsVar=NULL;
    //刷新数据
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

            if(dev->PLCType() == PLC_1200 || dev->PLCType() == PLC_1500)
            {
                switch (pS7RegInform->m_nRegType)
                {
                case ID_MEM_C:
                case ID_MEM_AQ:
                {
                    nSize = 2;
                    nAddress = (ushort)(db->getStartAddress() + nAddress /nSize);
                    nItemOff = (nAddress - db->getStartAddress())*nSize;
                }
                    break;
                case ID_MEM_T:
                case ID_MEM_HC:
                {
                    nSize = 4;
                    nAddress = (ushort)(db->getStartAddress() + nAddress / nSize);
                    nItemOff = (nAddress - db->getStartAddress())*nSize;
                }
                    break;
                case ID_MEM_V:
                case ID_MEM_S:
                case ID_MEM_SM:
                case ID_MEM_M:
                case ID_MEM_I:
                case ID_MEM_Q:
                case ID_MEM_DB:
                {
                    nSize = 1;
                    nAddress = (ushort)(db->getStartAddress() + nAddress/nSize);
                    nItemOff = (nAddress - db->getStartAddress())*nSize;
                }
                    break;
                default:
                    break;
                }
            }
            else if(dev->PLCType() == PLC_300 || dev->PLCType() == PLC_400)
            {
                switch (pS7RegInform->m_nRegType)
                {
                case ID_MEM_C:
                case ID_MEM_T:
                {
                    nSize = 2;
                    nAddress = (ushort)(db->getStartAddress() + nAddress / nSize);
                    nItemOff = (nAddress - db->getStartAddress())*nSize;
                }
                    break;
                case ID_MEM_PI:
                case ID_MEM_PQ:
                case ID_MEM_M:
                case ID_MEM_I:
                case ID_MEM_Q:
                case ID_MEM_DB:
                {
                    nSize = 1;
                    nAddress = (ushort)(db->getStartAddress() + nAddress/nSize);
                    nItemOff = (nAddress - db->getStartAddress())*nSize;
                }
                    break;
                default:
                    break;
                }

            }
            if(n ==0 )
            {
                nMinAddress = nAddress;
                nMaxAddress = nAddress;
            }

            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
                {
                    if(pS7RegInform->m_nRegType == ID_MEM_C||
                            pS7RegInform->m_nRegType == ID_MEM_T||
                                pS7RegInform->m_nRegType == ID_MEM_HC||
                                    pS7RegInform->m_nRegType == ID_MEM_AQ||
                                        pS7RegInform->m_nRegType == ID_MEM_PQ)
                    {
                       if(dataDatabuffer!=NULL)
                       {
                           delete dataDatabuffer;
                           dataDatabuffer = NULL;
                       }
                       return bRet;
                    }
                    if (nSize == 2)
                    {
                        unsigned char temp[2];
                        temp[0] =  dataDatabuffer[nItemOff+1];
                        temp[1] =  dataDatabuffer[nItemOff];
                        quint16 tempValue;
                        memcpy(&tempValue,temp,2);
                        if (nsVar->originalValue().toBool() == true)
                        {
                            tempValue = tempValue | (1<<nSubAddress);
                        }
                        else
                        {
                            tempValue = tempValue & (~(1<<nSubAddress));
                        }
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue >> 8)&0xFF);
                        dataDatabuffer[nItemOff+1] = (unsigned char)(tempValue &0xFF);
                    }
                    else if(nSize == 1)
                    {
                        if (nsVar->originalValue().toBool() == true)
                        {
                            dataDatabuffer[nItemOff] = dataDatabuffer[nItemOff] | (1<<nSubAddress);
                        }
                        else
                        {
                            dataDatabuffer[nItemOff] = dataDatabuffer[nItemOff] & (~(1<<nSubAddress));
                        }
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
            case MDDataType::SByte:
                {
                    if(nSize != 1)
                    {
                        if(dataDatabuffer!=NULL)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    qint8 tempValue = (qint8)nsVar->originalValue().toInt();
                    dataDatabuffer[nItemOff] = tempValue;
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
            case MDDataType::Byte:
                {
                    if(nSize != 1)
                    {
                        if(dataDatabuffer!=NULL)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    qint8 tempValue = (qint8)nsVar->originalValue().toInt();
                    dataDatabuffer[nItemOff] = tempValue;
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
                    if((dev->PLCType() == PLC_300 || dev->PLCType() == PLC_400)&&
                            (pS7RegInform->m_nRegType == ID_MEM_C))
                    {
                        QString strCounter = nsVar->originalValue().toString();
                        bool ok;
                        qint16   ustCounter  = (qint16)strCounter.toInt(&ok,16);
                        if(ustCounter>0x999)
                        {
                            ustCounter = 0x999;
                        }
                        dataDatabuffer[nItemOff] = (unsigned char)((ustCounter >> 8)&0xFF);
                        dataDatabuffer[nItemOff+1] = (unsigned char)(ustCounter & 0xFF);
                    }
                    else
                    {
                        qint16 tempValue = (qint16)nsVar->originalValue().toInt();
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue >> 8)&0xFF);
                        dataDatabuffer[nItemOff+1] = (unsigned char)(tempValue & 0xFF);
                    }
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nSize == 1)
                    {
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                    else if(nSize == 2)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress;
                        }
                    }
                 }
                break;
            case MDDataType::UInt16:
                {
                    if((dev->PLCType() == PLC_300 || dev->PLCType() == PLC_400)&&
                            (pS7RegInform->m_nRegType == ID_MEM_C))
                    {
                        QString strCounter = nsVar->originalValue().toString();
                        bool ok;
                        quint16   ustCounter  = (quint16)strCounter.toUInt(&ok,16);
                        if(ustCounter>0x999)
                        {
                            ustCounter = 0x999;
                        }
                        dataDatabuffer[nItemOff] = (unsigned char)((ustCounter >> 8)&0xFF);
                        dataDatabuffer[nItemOff+1] = (unsigned char)(ustCounter & 0xFF);
                    }
                    else
                    {
                        quint16 tempValue = (quint16)nsVar->originalValue().toUInt();
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue >> 8)&0xFF);
                        dataDatabuffer[nItemOff+1] = (unsigned char)(tempValue & 0xFF);
                    }
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nSize == 1)
                    {
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                    else if(nSize == 2)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress;
                        }
                    }
                }
                break;
            case MDDataType::Int32:
                {
                    qint32 tempValue = (qint32)nsVar->originalValue().toInt();
                    dataDatabuffer [nItemOff] = (unsigned char)((tempValue>>24)&0xFF);
                    dataDatabuffer [nItemOff+1] = (unsigned char)((tempValue>>16)&0xFF);
                    dataDatabuffer [nItemOff+2] = (unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer [nItemOff+3] = (unsigned char)(tempValue&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nSize == 1)
                    {
                        if(nMaxAddress<nAddress+3)
                        {
                            nMaxAddress = nAddress+3;
                        }
                    }
                    else if(nSize == 2)
                    {
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                    else if(nSize == 4)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress;
                        }
                    }
                }
                break;
            case MDDataType::UInt32:
                {
                    quint32 tempValue = (quint32)nsVar->originalValue().toUInt();
                    dataDatabuffer [nItemOff] = (unsigned char)((tempValue>>24)&0xFF);
                    dataDatabuffer [nItemOff+1] = (unsigned char)((tempValue>>16)&0xFF);
                    dataDatabuffer [nItemOff+2] = (unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer [nItemOff+3] = (unsigned char)(tempValue&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nSize == 1)
                    {
                        if(nMaxAddress<nAddress+3)
                        {
                            nMaxAddress = nAddress+3;
                        }
                    }
                    else if(nSize == 2)
                    {
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                    else if(nSize == 4)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress;
                        }
                    }
                }
                break;
            case MDDataType::Float:
                {
                    float tempValue=(float)(nsVar->originalValue().toFloat());
                    unsigned char * pTempValue = (unsigned char *)&tempValue;
                    dataDatabuffer[nItemOff] = *(pTempValue+3);
                    dataDatabuffer[nItemOff+1] = *(pTempValue+2);
                    dataDatabuffer[nItemOff+2] = *(pTempValue+1);
                    dataDatabuffer[nItemOff+3] = *pTempValue;
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nSize == 1)
                    {
                        if(nMaxAddress<nAddress+3)
                        {
                            nMaxAddress = nAddress+3;
                        }
                    }
                    else if(nSize == 2)
                    {
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                    else if(nSize == 4)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress;
                        }
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

    if(nMinAddress < db->getStartAddress()|| nMinAddress > db->getEndAddress()||
       nLength<=0|| nLength > db->getEndAddress()-db->getStartAddress()+1)
    {
        if(dataDatabuffer!=NULL)
        {
            delete dataDatabuffer;
            dataDatabuffer = NULL;
        }

        return bRet;
    }
    m_transactionID = ((CMDChannel*)getChannel())->getTransactionID();
     setCommandLength(0);
    quint16  uiLength = 0;
    if(dev->PLCType() == PLC_300 || dev->PLCType() == PLC_400)
    {
        uiLength = BuildDBWrite300And400(pS7RegInform,nMinAddress,db,m_transactionID,nLength,dataDatabuffer);
    }
    else if(dev->PLCType() == PLC_1200 || dev->PLCType() == PLC_1500)
    {
        uiLength = BuildDBWrite(pS7RegInform,nMinAddress,db,m_transactionID,nLength,dataDatabuffer);
    }
    if(dataDatabuffer!=NULL)
    {
        delete dataDatabuffer;
        dataDatabuffer = NULL;
    }
    if(uiLength <= 0)
    {
       return bRet;
    }
    if (getCommandLength() > 0)
    {
        bRet = true;
    }
    setCommandLength(uiLength);
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
//******************************PKX****************************//
    if(dev->m_nPLCComInitialStatus == 0)
    {
        if((getCommandLength() == getReceiveDataLength())&&(getReceiveDataLength() == 22))
        {
            for(quint64 n = 0; n < getReceiveDataLength() ; n++)
            {
                if(n == 5)//!PDU TYPE
                {
                    continue;
                }
                else if(n == 6)
                {
                    //!Source reference
                    if((m_receiveData[6]!=m_sendData[8]) || (m_receiveData[7]!=m_sendData[9]))
                    {
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::Error);
                        return getCommandResult();
                    }
                    n = 9;
                }
                else
                {

                }
            }
            dev->m_nPLCComInitialStatus = 1;
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::RightAndNextCmd);
            return getCommandResult();
        }
    }//!初始化第一步校验
    else if(dev->m_nPLCComInitialStatus == 2)
    {
        if((getCommandLength() == 25) && (getReceiveDataLength() == 27))
        {
            if(m_receiveData[7] == 0x32)
            {
                //!id校验
                if((m_receiveData[11] == m_sendData[11]) && (m_receiveData[12] == m_sendData[12]) )
                {
                    if(m_receiveData[8] == 0x03 && m_receiveData[9] == 0x00 && m_receiveData[10] == 0x00 &&
                            m_receiveData[13] == 0x00 && m_receiveData[14] == 0x08 && m_receiveData[15] == 0x00 &&
                            m_receiveData[16] == 0x00 && m_receiveData[17] == 0x00 && m_receiveData[18] == 0x00 && m_receiveData[19] == 0xF0)
                    {
                        dev->m_nPLCComInitialStatus = 3;
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                        return getCommandResult();
                    }
                }
            }
        }
    }//!初始化第二步校验
    else if(dev->m_nPLCComInitialStatus >=3)
    {
        if((getReceiveDataLength()<20) || (getReceiveDataLength() > getReceiveBufferSize()))
        {
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::Error);
            return getCommandResult();
        }

        for(qint32 n = 0;n < 7;n++)
        {
            if(n != 3)
            {
                if(m_receiveData[n] != m_sendData[n])
                {
                    setReceiveDataLength(0);
                    setCommandResult(ProtocolCommandResult::Error);
                    return getCommandResult();
                }
            }
            else
            {
                if(m_receiveData[n] != (getReceiveDataLength()))
                {
                    setReceiveDataLength(0);
                    setCommandResult(ProtocolCommandResult::Error);
                    return getCommandResult();
                }
            }
        }
        if(m_receiveData[7] == 0x32)
        {
            //!id校验
            if((m_receiveData[11] != m_sendData[11]) || (m_receiveData[12] != m_sendData[12]) )
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
             if(m_receiveData[8] == 0x03)
             {
                 if (getCommandType() == ProtocolCommandType::Read)
                 {
                     if(m_receiveData[21]  != 0xFF)//!错误码校验
                     {
                         setReceiveDataLength(0);
                         setCommandResult(ProtocolCommandResult::Error);
                         return getCommandResult();
                     }
                     if(m_receiveData[22]  != 0x00)
                     {
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::Right);
                        return getCommandResult();
                     }
                 }
                 else
                 {
                     setReceiveDataLength(0);
                     setCommandResult(ProtocolCommandResult::Right);
                     return getCommandResult();
                 }
             }
        }

    }
    setReceiveDataLength(0);
    setCommandResult(ProtocolCommandResult::Error);
    return getCommandResult();
}
//************************PKX**********************//
//!构建获取服务号命令
quint16 CMDProtocolCommand::BuildPLCServerMSG(quint8 local1,quint8 local2,quint8 remote1,quint8 remote2)
{
    //!version
    m_sendData[0] = 0x03 ;
   //!reserved
    m_sendData[1] = 0x00 ;
    //!length1(version---destination TSAP)
    m_sendData[2] =  0x00;
    m_sendData[3] =  0x16;
    //!length2(PDU TYPE---destination TSAP)
    m_sendData[4] =  0x11;
    //!PDU TYPE
    m_sendData[5] =  0xE0;
    //!Destination reference
    m_sendData[6] =  0x00;
    m_sendData[7] =  0x00;
    //!Source reference
    m_sendData[8] =  0x00;
    m_sendData[9] =  0x01;
    //!class and option
    m_sendData[10] = 0x00;
    //!parameter code
    m_sendData[11] = 0xC0;
    //!parameter length
    m_sendData[12] =  0x01;
    //!TPDU SIZE
    m_sendData[13] =  0x09;
    //!parameter code
    m_sendData[14] =  0xC1;
    //!parameter length
    m_sendData[15] =  0x02;
    //!source TSAP
    m_sendData[16] =  local1;
    m_sendData[17] =  local2;
    //!parameter code
    m_sendData[18] =  0xC2;
    //!parameter length
    m_sendData[19] =  0x02;
    //!destination TSAP
    m_sendData[20] = remote1;
    m_sendData[21] = remote2;
    return 22;

}

quint16 CMDProtocolCommand::BuildPLCConnectMSG(quint16 iRanMsgID,CMDDevice *dev)
{
    quint16 uiDataUnitLength;
    quint8 ucDataUnit[256];

    //!PROTO_ID
    ucDataUnit[0] = 0x32 ;
   //!ROSCTR
    ucDataUnit[1] = 0x01 ;
    //!RED_ID
    ucDataUnit[2] =  0x00;
    ucDataUnit[3] =  0x00;
    //!PDU_REF(传输id)
    ucDataUnit[4] =  (quint8)((iRanMsgID&0xFF00)>>8);
    ucDataUnit[5] =  (quint8)(iRanMsgID&0x00FF);
    //!PAR_LG
    ucDataUnit[6] =  0x00;
    ucDataUnit[7] =  0x08;
    //!DAT_LG
    ucDataUnit[8] =  0x00;
    ucDataUnit[9] =  0x00;
    //!SERVICE_ID
    ucDataUnit[10] = 0xF0;
    //!RESERVED
    ucDataUnit[11] = 0x00;
    //!P1(Max called suggestion)
    ucDataUnit[12] =  0x00;
    if(dev->PLCType() == PLC_1200 || dev->PLCType() == PLC_1500)
    {
        ucDataUnit[13] =  0x02;
    }
    else
    {
        ucDataUnit[13] =  0x01;
    }

    //!P2(Max called suggestion)
    ucDataUnit[14] =  0x00;
    if((dev->PLCType() == PLC_1200) || (dev->PLCType() == PLC_1500))
    {
        ucDataUnit[15] =  0x02;
    }
    else if((dev->PLCType() == PLC_300)||(dev->PLCType() == PLC_400))
    {
        ucDataUnit[15] =  0x01;
    }

    //P2(Max PDU Size of Caller)
    ucDataUnit[16] =  0x00;
    ucDataUnit[17] =  0xF0;

    uiDataUnitLength = 18;

    uiDataUnitLength = uiDataUnitLength + 7;
    m_sendData[0] =  0x03;
    m_sendData[1] =  0x00;
    //!数据包总长度（高位在前 低位在后）
    m_sendData[2] = (quint8)((uiDataUnitLength&0xFF00)>>8);
    m_sendData[3] = (quint8)(uiDataUnitLength&0x00FF);
    m_sendData[4] = 0x02;
    m_sendData[5] = 0xF0;
    m_sendData[6] = 0x80;
    for(int i = 1 ; i <= uiDataUnitLength - 7 ; i++ )
    {
        m_sendData[6+i] = ucDataUnit[i-1];
    }
    return uiDataUnitLength;


}

quint16 CMDProtocolCommand::BuildRead(S7RegInform*pSRegInform,qint16 iMemAdd,quint16 iElements,quint16 iRanMsgID,CMDDataBlock*pdatablock,CMDDevice *dev)
{
    if(pSRegInform == NULL)
    {
        return 0;
    }
    quint16 uiDataUnitLength,uiReadBytes,uiFillBytes;
    quint8 ucDataUnit[256];
    quint64 uloffset;
    if((dev->PLCType() == PLC_1200) || (dev->PLCType() == PLC_1500))
    {
        switch(pSRegInform->m_nRegType)
        {
        case ID_MEM_C:
        case ID_MEM_T:
        case ID_MEM_HC:
        {
                uloffset = iMemAdd;
                uiReadBytes = iElements*3;
        }
                break;
        case ID_MEM_AI:
        case ID_MEM_AQ:
        {
                uloffset = (iMemAdd*2)*8;
                uiReadBytes = iElements*5;
        }
                break;
        case ID_MEM_I:
        case ID_MEM_Q:
        case ID_MEM_S:
        case ID_MEM_M:
        case ID_MEM_V:
        case ID_MEM_SM:
        {
            uloffset = iMemAdd*8;
            uiReadBytes = iElements*2;
        }
            break;
       case ID_MEM_DB:
        {
            uloffset = iMemAdd*8;
            uiReadBytes = iElements*2;
        }
            break;
        default:
            return 0;
        }
    }
    else if((dev->PLCType() == PLC_300)||(dev->PLCType() == PLC_400))
    {
        switch(pSRegInform->m_nRegType)
        {
        case ID_MEM_C:
        case ID_MEM_T:
        {
            uloffset = iMemAdd;
            uiReadBytes = iElements*3;
        }
            break;
        case ID_MEM_I:
        case ID_MEM_Q:
        case ID_MEM_M:
        case ID_MEM_DB:
        case ID_MEM_PI:
        case ID_MEM_PQ:
        {
            uloffset = iMemAdd*8;
            uiReadBytes = iElements*2;
        }
            break;
        default:
            return 0;
        }

    }
    if( uiReadBytes % 2 == 1 )
    {
        uiFillBytes = uiReadBytes + 1;
    }
    else
    {
        uiFillBytes = uiReadBytes;
    }
    //!PROTO_ID
    ucDataUnit[0] = 0x32 ;
   //!ROSCTR
    ucDataUnit[1] = 0x01 ;
    //!RED_ID
    ucDataUnit[2] =  0x00;
    ucDataUnit[3] =  0x00;
    //!PDU_REF
    ucDataUnit[4] =  (quint8)((iRanMsgID&0xFF00)>>8);
    ucDataUnit[5] =  (quint8)(iRanMsgID&0x00FF);
    //!参数长度 = 参数块长度（4个字节）+Any Point 的长度(10个字节)
    ucDataUnit[6] =  0x00;
    ucDataUnit[7] =  0x0E;
    //!数据长度 = 数据块长度
    ucDataUnit[8] =  0x00;
    ucDataUnit[9] =  0x00;
    //!Requst Parameter Block固定长度 4个字节
    ucDataUnit[10] = 0x04;

    ucDataUnit[11] = 0x01;

    ucDataUnit[12] =  0x12;

    ucDataUnit[13] =  0x0A;
    //!Any Point固定长度 10个字节
    ucDataUnit[14] =  0x10;

    ucDataUnit[15] =  pSRegInform->m_ustrDataType;//!数据类型
    //!实际读取数据的长度（高位在前 低位在后）
    ucDataUnit[16] =  (quint8)((iElements&0xFF00)>>8);
    ucDataUnit[17] =  (quint8)(iElements&0x00FF);
    ucDataUnit[18] =  0x00;
    if(pSRegInform->m_nRegType == ID_MEM_V)
    {
        ucDataUnit[19] =  0x01;
    }
    else if(pSRegInform->m_nRegType == ID_MEM_DB)
    {
        //!db块号
        qint16 sTempDBNumber =  pdatablock->getDBRegisterNum();
        ucDataUnit[18] =  (quint8)((sTempDBNumber&0xFF00)>>8);
        ucDataUnit[19] =  (quint8)(sTempDBNumber&0x00FF);
    }
    else
    {
        ucDataUnit[19] = 0x00;
    }
    ucDataUnit[20] =  pSRegInform->m_ustrRegCode;
    //!实际的偏移地址
    ucDataUnit[21] =  (quint8)((uloffset&0xFF0000)>>16);
    ucDataUnit[22] =  (quint8)((uloffset&0x00FF00)>>8);
    ucDataUnit[23] =  (quint8)(uloffset&0x0000FF);

    uiDataUnitLength = 24;
    //************************************************
    uiDataUnitLength = uiDataUnitLength + 7 ;
    m_sendData[0] =  0x03;
    m_sendData[1] =  0x00;
    //!数据包的总长度
    m_sendData[2] = (quint8)((uiDataUnitLength&0xFF00)>>8);
    m_sendData[3] = (quint8)(uiDataUnitLength&0x00FF);
    m_sendData[4] = 0x02;
    m_sendData[5] = 0xF0;
    m_sendData[6] = 0x80;
    for(int i = 1 ; i <= uiDataUnitLength - 7 ; i++ )
    {
        m_sendData[6+i] = ucDataUnit[i-1];
    }

    return uiDataUnitLength;

}
quint16 CMDProtocolCommand::BuildWrite(S7RegInform*pSRegInform,IMDRTDBVariable *nsVar,CMDDataBlock*pdatablock,quint16 iRanMsgID,quint16 Elements)
{
    quint16 uiDataUnitLength,uiRealBytes,uiFillBytes;
    quint8 ucDataUnit[256];
    int uloffset;
    quint8 ucDataType;

    switch(pSRegInform->m_nRegType)
    {
    case ID_MEM_C:
    {
        if(nsVar->originalDataType()== MDDataType::Bool ||nsVar->originalDataType() == MDDataType::String )
        {
            return 0;
        }
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*3;
        uloffset = nsVar->internalAddress()+pdatablock->getStartAddress();
    }
        break;
   case ID_MEM_T:
    {
        if(nsVar->originalDataType() == MDDataType::Bool ||nsVar->originalDataType() == MDDataType::String )
        {
            return 0;
        }
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*5;
        uloffset = nsVar->internalAddress()+pdatablock->getStartAddress();
    }
        break;
   case ID_MEM_HC:
   {
            if(nsVar->originalDataType()== MDDataType::Bool ||nsVar->originalDataType() == MDDataType::String )
            {
                return 0;
            }
            ucDataType = pSRegInform->m_ustrDataType;
            uiRealBytes = Elements*5;
            uloffset = nsVar->internalAddress()+pdatablock->getStartAddress();
   }
        break;
   case ID_MEM_AQ:
    {
        if(nsVar->originalDataType() == MDDataType::Bool ||nsVar->originalDataType() == MDDataType::String )
        {
            return 0;
        }
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*2;
        uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*2)*8;
    }
        break;
   case ID_MEM_M:
   case ID_MEM_V:
   case ID_MEM_S:
   case ID_MEM_SM:
   case ID_MEM_I:
   case ID_MEM_Q:
   case ID_MEM_DB:
    {
        if(nsVar->originalDataType() == MDDataType::Bool )
        {
            ucDataType = S7_DT_Bool;
            uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8 + nsVar->internalSubAddress();
            uiRealBytes = Elements*1;
        }
        else
        {
            if(nsVar->originalDataType() == MDDataType::String)
            {
                ucDataType = S7_DT_Byte;
                uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                uiRealBytes = 210;
            }
            else
            {
                switch(nsVar->originalDataType())
                {
                case MDDataType::SByte :
                case MDDataType::Byte :
                {
                    ucDataType = S7_DT_Byte;
                    uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                    uiRealBytes = Elements*1;
                }
                    break;
                case MDDataType::Int16 :
                case MDDataType::UInt16 :
                {
                    ucDataType = S7_DT_Word;
                    uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                    uiRealBytes = Elements*2;
                }
                    break;
                case MDDataType::Int32 :
                case MDDataType::UInt32 :
                case MDDataType::Float :
                {
                    ucDataType = S7_DT_DWord;
                    uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                    uiRealBytes = Elements*4;
                }
                    break;
                default:
                    return 0;
                }
            }
        }


    }
        break;
    default:
        return 0;
    }
    if((uiRealBytes%2) == 1)
    {
        uiFillBytes = uiRealBytes +1;
    }
    else
    {
        uiFillBytes = uiRealBytes;
    }

    ucDataUnit[0] = 0x32;
    ucDataUnit[1] = 0x01;
    ucDataUnit[2] = 0x00;
    ucDataUnit[3] = 0x00;
    //!传输id
    ucDataUnit[4] = (quint8)((iRanMsgID&0xFF00)>>8);
    ucDataUnit[5] = (quint8)(iRanMsgID&0x00FF);
    //!参数长度 = 参数块长度 + Any Point 的长度(10个字节)
    ucDataUnit[6] = 0x00;
    ucDataUnit[7] = 0x0E;
    uiDataUnitLength = uiFillBytes + 4;
    //!数据长度 = 数据块长度
    ucDataUnit[8] = (quint8)((uiDataUnitLength&0xFF00)>>8);
    ucDataUnit[9] = (quint8)(uiDataUnitLength&0x00FF);
    //!Request Parameter Block     固定长度 4个字节   10-13
    ucDataUnit[10] = 0x05;
    ucDataUnit[11] = 0x01;
    ucDataUnit[12] = 0x12;
    ucDataUnit[13] = 0x0A;
    //Any Point                   固定长度 10个字节   14-23
    ucDataUnit[14] = 0x10;
    ucDataUnit[15] = ucDataType;
    if(nsVar->originalDataType() == MDDataType::Bool )
    {
        ucDataUnit[16] = 0x00;
        ucDataUnit[17] = 0x01;
    }
    else
    {
        if(nsVar->originalDataType() == MDDataType::String )
        {
            ucDataUnit[16] = (quint8)((uiRealBytes&0xFF00)>>8);
            ucDataUnit[17] = (quint8)(uiRealBytes&0x00FF);
        }
        else
        {
            ucDataUnit[16] = (quint8)((Elements&0xFF00)>>8);
            ucDataUnit[17] = (quint8)(Elements&0x00FF);
        }

    }
    ucDataUnit[18] = 0x00;
    if(pSRegInform->m_nRegType == ID_MEM_V)
    {
        ucDataUnit[19] =  0x01;
    }
    else if(pSRegInform->m_nRegType == ID_MEM_DB)
    {
        qint16 sTempDBNumber =  pdatablock->getDBRegisterNum();
        ucDataUnit[18] =  (quint8)((sTempDBNumber&0xFF00)>>8);
        ucDataUnit[19] =  (quint8)(sTempDBNumber&0x00FF);
    }
    else
    {
        ucDataUnit[19] = 0x00;
    }

    ucDataUnit[20] = pSRegInform->m_ustrRegCode;
    ucDataUnit[21] = (quint8)((uloffset & 0xFF0000)>>16);
    ucDataUnit[22] = (quint8)((uloffset & 0x00FF00)>>8);
    ucDataUnit[23] = (quint8)(uloffset & 0x0000FF);
    //!Data Block
    ucDataUnit[24] = 0x00;

    if(nsVar->originalDataType() == MDDataType::Bool )
    {
        ucDataUnit[25] = 0x03;//!数据类型
        ucDataUnit[26] = 0x00;//!数据长度
        ucDataUnit[27] = 0x01;//!数据长度
    }
    else
    {
        ucDataUnit[25] = 0x04;//!数据类型
        ucDataUnit[26] = (quint8)(((uiRealBytes*8) & 0xFF00)>>8);//!数据长度
        ucDataUnit[27] = (quint8)((uiRealBytes*8) & 0x00FF);//!数据长度
    }

    int nDatasieze = 0;
    int nIndex = 27;
    switch(nsVar->originalDataType())
    {
    case MDDataType::Bool:
    {
        nDatasieze = 1;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        if(nsVar->originalValue().toBool()!=false)
        {
            ucDataUnit[nIndex+1] = 0x01;
        }
        else
        {
            ucDataUnit[nIndex+1] = 0x00;
        }
    }
        break;
    case MDDataType::SByte:
    {
        nDatasieze = 1;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        qint8 temp_char= (qint8)(nsVar->originalValue().toInt());
        qint8 *temp_char1 = (qint8*)&temp_char;
        memcpy(&ucDataUnit[nIndex+1],temp_char1,1);
    }
        break;
    case MDDataType::Byte:
    {
        nDatasieze = 1;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }

        quint8 temp_byte= (quint8)(nsVar->originalValue().toUInt());
        quint8 *temp_byte1 = (quint8*)&temp_byte;
        memcpy(&ucDataUnit[nIndex+1],temp_byte1, 1);
    }
        break;
    case MDDataType::Int16:
    {
        nDatasieze = 2;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        short tempushort =(short)nsVar->originalValue().toInt();
        quint8 *tempushort1 = (quint8*)&tempushort;
        memcpy(&ucDataUnit[nIndex+1],(tempushort1+1),1);
        memcpy(&ucDataUnit[nIndex+2],(tempushort1),1);
    }
        break;
    case MDDataType::UInt16:
    {
        nDatasieze = 2;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        ushort tempushort =(ushort)nsVar->originalValue().toUInt();
        quint8 *tempushort1 = (quint8*)&tempushort;
        memcpy(&ucDataUnit[nIndex+1],(tempushort1+1),1);
        memcpy(&ucDataUnit[nIndex+2],(tempushort1),1);
    }
        break;
    case MDDataType::Int32:
    {
        nDatasieze = 4;

        for(int i= 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        int tempint= nsVar->originalValue().toInt();
        quint8 *tempint1 = (quint8*)&tempint;
        memcpy(&ucDataUnit[nIndex+1],(tempint1+3),1);
        memcpy(&ucDataUnit[nIndex+2],(tempint1+2),1);
        memcpy(&ucDataUnit[nIndex+3],(tempint1+1),1);
        memcpy(&ucDataUnit[nIndex+4],(tempint1),1);
    }
        break;
    case MDDataType::UInt32:
    {
        nDatasieze = 4;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        uint tempuint= (uint)(nsVar->originalValue().toUInt());
        quint8 *tempuint1 = (quint8*)&tempuint;
        memcpy(&ucDataUnit[nIndex+1],(tempuint1+3),1);
        memcpy(&ucDataUnit[nIndex+2],(tempuint1+2),1);
        memcpy(&ucDataUnit[nIndex+3],(tempuint1+1),1);
        memcpy(&ucDataUnit[nIndex+4],(tempuint1),1);
    }
        break;
    case MDDataType::Float:
    {
        nDatasieze = 4;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        float tempfloat= (float)(nsVar->originalValue().toFloat());
        quint8 *tempfloat1 = (quint8*)&tempfloat;
        memcpy(&ucDataUnit[nIndex+1],(tempfloat1+3),1);
        memcpy(&ucDataUnit[nIndex+2],(tempfloat1+2),1);
        memcpy(&ucDataUnit[nIndex+3],(tempfloat1+1),1);
        memcpy(&ucDataUnit[nIndex+4],(tempfloat1),1);
    }
        break;
    case MDDataType::String:
    {
        ucDataUnit[nIndex+1] =  uiRealBytes-2;
        ucDataUnit[nIndex+2] =  Elements;

        QString tempstring= nsVar->originalValue().toString();
        QByteArray tempstring1 = tempstring.toLatin1();
        char* tempstring2 = tempstring1.data();

         memcpy(&ucDataUnit[nIndex+3],tempstring2,Elements);

         for(int j = 0;j<(210 -Elements);j++)
         {
            ucDataUnit[nIndex+3+Elements+j] = 0x00;
         }
    }
        break;
    default:
        return 0;
    }
    if(uiRealBytes!=uiFillBytes)
    {
        ucDataUnit[28+uiRealBytes] = 0x00;
        uiDataUnitLength = 10+4+10+4+uiRealBytes+1;
    }
    else
    {
        uiDataUnitLength = 10+4+10+4+uiRealBytes;
    }

    uiDataUnitLength = uiDataUnitLength +7;

    m_sendData[0] =  0x03;
    m_sendData[1] =  0x00;
    //!整包数据长度
    m_sendData[2] = (quint8)((uiDataUnitLength&0xFF00)>>8);
    m_sendData[3] = (quint8)(uiDataUnitLength&0x00FF);
    m_sendData[4] = 0x02;
    m_sendData[5] = 0xF0;
    m_sendData[6] = 0x80;
    for(int i = 1 ; i <= uiDataUnitLength - 7 ; i++ )
    {
        m_sendData[6+i] = ucDataUnit[i-1];
    }

    return uiDataUnitLength;

}
quint16 CMDProtocolCommand::BuildWrite300And400(S7RegInform*pSRegInform,IMDRTDBVariable *nsVar,CMDDataBlock*pdatablock ,quint16 iRanMsgID,quint16 Elements)
{
    quint16 uiDataUnitLength,uiRealBytes,uiFillBytes;
    quint8 ucDataUnit[256];
    int uloffset;
    quint8 ucDataType;

    switch(pSRegInform->m_nRegType)
    {
    case ID_MEM_C:
    {
        if(nsVar->originalDataType() == MDDataType::Bool ||nsVar->originalDataType() == MDDataType::String )
        {
            return 0;
        }
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*2;
        uloffset = nsVar->internalAddress()/2+pdatablock->getStartAddress();
    }
        break;
   case ID_MEM_T:
    {
        if(nsVar->originalDataType() == MDDataType::Bool ||nsVar->originalDataType() == MDDataType::String )
        {
            return 0;
        }
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*2;
        uloffset = nsVar->internalAddress()/2+pdatablock->getStartAddress();
    }
        break;
   case ID_MEM_M:
   case ID_MEM_DB:
   case ID_MEM_PI:
   case ID_MEM_PQ:
   case ID_MEM_I:
   case ID_MEM_Q:
    {
        if(nsVar->originalDataType() == MDDataType::Bool )
        {
            ucDataType = S7_DT_Bool;
            uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8 + nsVar->internalSubAddress();
            uiRealBytes = Elements*1;
        }
        else
        {
            if(nsVar->originalDataType() == MDDataType::String)
            {
                ucDataType = S7_DT_Byte;
                uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                uiRealBytes = 210;
            }
            else
            {
                switch(nsVar->originalDataType())
                {
                case MDDataType::SByte :
                case MDDataType::Byte :
                {
                    ucDataType = S7_DT_Byte;
                    uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                    uiRealBytes = Elements*1;
                }
                    break;
                case MDDataType::Int16 :
                case MDDataType::UInt16 :
                {
                    ucDataType = S7_DT_Word;
                    uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                    uiRealBytes = Elements*2;
                }
                    break;
                case MDDataType::Int32 :
                case MDDataType::UInt32 :
                case MDDataType::Float :
                {
                    ucDataType = S7_DT_DWord;
                    uloffset = ((nsVar->internalAddress()+pdatablock->getStartAddress())*1)*8;
                    uiRealBytes = Elements*4;
                }
                    break;
                default:
                    return 0;
                }
            }
        }


    }
        break;
    default:
        return 0;
    }
    if((uiRealBytes%2) == 1)
    {
        uiFillBytes = uiRealBytes +1;
    }
    else
    {
        uiFillBytes = uiRealBytes;
    }

    ucDataUnit[0] = 0x32;
    ucDataUnit[1] = 0x01;
    ucDataUnit[2] = 0x00;
    ucDataUnit[3] = 0x00;
    ucDataUnit[4] = (quint8)((iRanMsgID&0xFF00)>>8);
    ucDataUnit[5] = (quint8)(iRanMsgID&0x00FF);
    ucDataUnit[6] = 0x00;
    ucDataUnit[7] = 0x0E;
    uiDataUnitLength = uiRealBytes + 4;
    ucDataUnit[8] = (quint8)((uiDataUnitLength&0xFF00)>>8);
    ucDataUnit[9] = (quint8)(uiDataUnitLength&0x00FF);
    ucDataUnit[10] = 0x05;
    ucDataUnit[11] = 0x01;
    ucDataUnit[12] = 0x12;
    ucDataUnit[13] = 0x0A;
    ucDataUnit[14] = 0x10;
    ucDataUnit[15] = ucDataType;
    if(nsVar->originalDataType() == MDDataType::Bool )
    {
        ucDataUnit[16] = 0x00;
        ucDataUnit[17] = 0x01;
    }
    else
    {
        if(nsVar->originalDataType() == MDDataType::String )
        {
            ucDataUnit[16] = (quint8)((uiRealBytes&0xFF00)>>8);
            ucDataUnit[17] = (quint8)(uiRealBytes&0x00FF);
        }
        else
        {
            ucDataUnit[16] = (quint8)((Elements&0xFF00)>>8);
            ucDataUnit[17] = (quint8)(Elements&0x00FF);
        }

    }
    ucDataUnit[18] = 0x00;
    if(pSRegInform->m_nRegType == ID_MEM_V)
    {
        ucDataUnit[19] =  0x01;
    }
    else if(pSRegInform->m_nRegType == ID_MEM_DB)
    {
        qint16 sTempDBNumber =  pdatablock->getDBRegisterNum();
        ucDataUnit[18] =  (quint8)((sTempDBNumber&0xFF00)>>8);
        ucDataUnit[19] =  (quint8)(sTempDBNumber&0x00FF);
    }
    else
    {
        ucDataUnit[19] = 0x00;
    }

    ucDataUnit[20] = pSRegInform->m_ustrRegCode;
    ucDataUnit[21] = (quint8)((uloffset & 0xFF0000)>>16);
    ucDataUnit[22] = (quint8)((uloffset & 0x00FF00)>>8);
    ucDataUnit[23] = (quint8)(uloffset & 0x0000FF);

    ucDataUnit[24] = 0x00;

    if((pSRegInform->m_nRegType ==  ID_MEM_C) || (pSRegInform->m_nRegType == ID_MEM_T))
    {
        if(nsVar->originalDataType() == MDDataType::Bool)
        {
            ucDataUnit[25] = 0x03;
            ucDataUnit[26] = 0x00;
            ucDataUnit[27] = 0x01;
        }
        else
        {
            ucDataUnit[25] = 0x09;
            ucDataUnit[26] = (quint8)(((uiRealBytes) & 0xFF00)>>8);
            ucDataUnit[27] = (quint8)((uiRealBytes) & 0x00FF);
        }
    }
    else
    {
        if(nsVar->originalDataType() == MDDataType::Bool)
        {
            ucDataUnit[25] = 0x03;
            ucDataUnit[26] = 0x00;
            ucDataUnit[27] = 0x01;
        }
        else
        {
            ucDataUnit[25] = 0x04;
            ucDataUnit[26] = (quint8)(((uiRealBytes*8) & 0xFF00)>>8);
            ucDataUnit[27] = (quint8)((uiRealBytes*8) & 0x00FF);
        }
    }

    int nDatasieze = 0;
    int nIndex = 27;
    switch(nsVar->originalDataType())
    {
    case MDDataType::Bool:
    {
        nDatasieze = 1;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        if(nsVar->originalValue().toBool()!=false)
        {
            ucDataUnit[nIndex+1] = 0x01;
        }
        else
        {
            ucDataUnit[nIndex+1] = 0x00;
        }
    }
        break;

    case MDDataType::SByte:
    {
        nDatasieze = 1;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        qint8 temp_char= (qint8)(nsVar->originalValue().toInt());
        qint8 *temp_char1 = (qint8*)&temp_char;
        memcpy(&ucDataUnit[nIndex+1],temp_char1,1);
    }
        break;

    case MDDataType::Byte:
    {
        nDatasieze = 1;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }

        quint8 temp_byte= (quint8)(nsVar->originalValue().toUInt());
        quint8 *temp_byte1 = (quint8*)&temp_byte;
        memcpy(&ucDataUnit[nIndex+1],temp_byte1, 1);
    }
        break;
    case MDDataType::Int16:
    {
        nDatasieze = 2;

        if((pSRegInform->m_nRegType ==  ID_MEM_C))
        {
            for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
            {
                nIndex++;
                ucDataUnit[nIndex] = 0x00;
            }
            QString strCounter = nsVar->originalValue().toString();
            bool ok;
            ushort   ustCounter  = (ushort)strCounter.toInt(&ok,16);
            if(ustCounter>0x999)
            {
                ustCounter = 0x999;
            }
            quint8 *ui8Counter = (quint8*)&ustCounter;
            memcpy(&ucDataUnit[nIndex+1],(ui8Counter+1),1);
            memcpy(&ucDataUnit[nIndex+2],(ui8Counter),1);
        }
        else
        {
            for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
            {
                nIndex++;
                ucDataUnit[nIndex] = 0x00;
            }
            short tempushort =(short)nsVar->originalValue().toInt();
            quint8 *tempushort1 = (quint8*)&tempushort;
            memcpy(&ucDataUnit[nIndex+1],(tempushort1+1),1);
            memcpy(&ucDataUnit[nIndex+2],(tempushort1),1);
        }
    }
        break;

    case MDDataType::UInt16:
    {
        nDatasieze = 2;

        if((pSRegInform->m_nRegType ==  ID_MEM_C))
        {
            for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
            {
                nIndex++;
                ucDataUnit[nIndex] = 0x00;
            }
            QString strCounter = nsVar->originalValue().toString();
            bool ok;
            ushort   ustCounter  = (ushort)strCounter.toInt(&ok,16);
            if(ustCounter>0x999)
            {
                ustCounter = 0x999;
            }
            quint8 *ui8Counter = (quint8*)&ustCounter;
            memcpy(&ucDataUnit[nIndex+1],(ui8Counter+1),1);
            memcpy(&ucDataUnit[nIndex+2],(ui8Counter),1);
        }
        else
        {
            for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
            {
                nIndex++;
                ucDataUnit[nIndex] = 0x00;
            }
            ushort tempushort =(ushort)nsVar->originalValue().toUInt();
            quint8 *tempushort1 = (quint8*)&tempushort;
            memcpy(&ucDataUnit[nIndex+1],(tempushort1+1),1);
            memcpy(&ucDataUnit[nIndex+2],(tempushort1),1);
        }
    }
        break;
    case MDDataType::Int32:
    {
        nDatasieze = 4;

        for(int i= 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        int tempint= nsVar->originalValue().toInt();
        quint8 *tempint1 = (quint8*)&tempint;
        memcpy(&ucDataUnit[nIndex+1],(tempint1+3),1);
        memcpy(&ucDataUnit[nIndex+2],(tempint1+2),1);
        memcpy(&ucDataUnit[nIndex+3],(tempint1+1),1);
        memcpy(&ucDataUnit[nIndex+4],(tempint1),1);
    }
        break;
    case MDDataType::UInt32:
    {
        nDatasieze = 4;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        uint tempuint= (uint)(nsVar->originalValue().toUInt());
        quint8 *tempuint1 = (quint8*)&tempuint;
        memcpy(&ucDataUnit[nIndex+1],(tempuint1+3),1);
        memcpy(&ucDataUnit[nIndex+2],(tempuint1+2),1);
        memcpy(&ucDataUnit[nIndex+3],(tempuint1+1),1);
        memcpy(&ucDataUnit[nIndex+4],(tempuint1),1);
    }
        break;
    case MDDataType::Float:
    {
        nDatasieze = 4;

        for(int i = 1;i<=(uiRealBytes-nDatasieze);i++)
        {
            nIndex++;
            ucDataUnit[nIndex] = 0x00;
        }
        float tempfloat= (float)(nsVar->originalValue().toFloat());
        quint8 *tempfloat1 = (quint8*)&tempfloat;
        memcpy(&ucDataUnit[nIndex+1],(tempfloat1+3),1);
        memcpy(&ucDataUnit[nIndex+2],(tempfloat1+2),1);
        memcpy(&ucDataUnit[nIndex+3],(tempfloat1+1),1);
        memcpy(&ucDataUnit[nIndex+4],(tempfloat1),1);
    }
        break;
    case MDDataType::String:
    {
        ucDataUnit[nIndex+1] =  uiRealBytes-2;
        ucDataUnit[nIndex+2] =  Elements;

        QString tempstring= nsVar->originalValue().toString();
        QByteArray tempstring1 = tempstring.toLatin1();
        char* tempstring2 = tempstring1.data();

         memcpy(&ucDataUnit[nIndex+3],tempstring2,Elements);

         for(int j = 0;j<(210 -Elements);j++)
         {
            ucDataUnit[nIndex+3+Elements+j] = 0x00;
         }
    }
        break;
    default:
        return 0;
    }

    uiDataUnitLength = 10+4+10+4+uiRealBytes;


    uiDataUnitLength = uiDataUnitLength +7;

    m_sendData[0] =  0x03;
    m_sendData[1] =  0x00;
    m_sendData[2] = (quint8)((uiDataUnitLength&0xFF00)>>8);
    m_sendData[3] = (quint8)(uiDataUnitLength&0x00FF);
    m_sendData[4] = 0x02;
    m_sendData[5] = 0xF0;
    m_sendData[6] = 0x80;
    for(int i = 1 ; i <= uiDataUnitLength - 7 ; i++ )
    {
        m_sendData[6+i] = ucDataUnit[i-1];
    }

    return uiDataUnitLength;
}
quint16 CMDProtocolCommand::BuildDBWrite(S7RegInform*pSRegInform,uint iMemAdd,CMDDataBlock*pdatablock,quint16 iRanMsgID,uint Elements,unsigned char *dataBuffer)
{
    quint16 uiDataUnitLength,uiRealBytes,uiFillBytes;
    quint8 ucDataUnit[256];
    int uloffset;
    int nSize = 1;
    quint8 ucDataType;

    switch (pSRegInform->m_nRegType)
    {
    case ID_MEM_C:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*3;
        uloffset = iMemAdd;
        nSize = 2;
    }
        break;
    case ID_MEM_T:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*5;
        uloffset = iMemAdd;
        nSize = 4;
    }
        break;
    case ID_MEM_HC:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*5;
        uloffset = iMemAdd;
        nSize = 4;
    }
        break;
    case ID_MEM_AQ:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*2;
        uloffset = (iMemAdd*2)*8;
        nSize = 2;
    }
        break;
    case ID_MEM_I:
    case ID_MEM_M:
    case ID_MEM_V:
    case ID_MEM_S:
    case ID_MEM_SM:
    case ID_MEM_Q:
    case ID_MEM_DB:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*1;
        uloffset = iMemAdd*8;
        nSize = 1;
    }
        break;
    default:
        return 0;
    }
    if((uiRealBytes%2) == 1)
    {
        uiFillBytes = uiRealBytes +1;
    }
    else
    {
        uiFillBytes = uiRealBytes;
    }
    ucDataUnit[0] = 0x32;
    ucDataUnit[1] = 0x01;
    ucDataUnit[2] = 0x00;
    ucDataUnit[3] = 0x00;
    ucDataUnit[4] = (quint8)((iRanMsgID>>8)&0xFF);
    ucDataUnit[5] = (quint8)(iRanMsgID&0xFF);
    ucDataUnit[6] = 0x00;
    ucDataUnit[7] = 0x0E;
    uiDataUnitLength = uiFillBytes + 4;
    ucDataUnit[8] = (quint8)((uiDataUnitLength>>8)&0xFF);
    ucDataUnit[9] = (quint8)(uiDataUnitLength&0xFF);
    ucDataUnit[10] = 0x05;
    ucDataUnit[11] = 0x01;
    ucDataUnit[12] = 0x12;
    ucDataUnit[13] = 0x0A;
    ucDataUnit[14] = 0x10;
    ucDataUnit[15] = ucDataType;
    ucDataUnit[16] = (quint8)((Elements>>8)&0xFF);
    ucDataUnit[17] = (quint8)(Elements&0xFF);
    ucDataUnit[18] = 0x00;
    if(pSRegInform->m_nRegType == ID_MEM_V)
    {
        ucDataUnit[19] = 0x01;
    }
    else if(pSRegInform->m_nRegType == ID_MEM_DB)
    {
        qint16 sTempDBNumber =  pdatablock->getDBRegisterNum();
        ucDataUnit[18] =  (quint8)((sTempDBNumber&0xFF00)>>8);
        ucDataUnit[19] =  (quint8)(sTempDBNumber&0x00FF);
    }
    else
    {
        ucDataUnit[19] = 0x00;
    }
    ucDataUnit[20] = pSRegInform->m_ustrRegCode;
    ucDataUnit[21] = (quint8)((uloffset&0xFF0000)>>16);
    ucDataUnit[22] = (quint8)((uloffset&0xFF00)>>8);
    ucDataUnit[23] = (quint8)(uloffset&0xFF);
    ucDataUnit[24] = 0x00;
    ucDataUnit[25] = 0x04;
    ucDataUnit[26] = (quint8)(((uiRealBytes*8)&0xFF00)>>8);
    ucDataUnit[27] = (quint8)((uiRealBytes*8)&0xFF);
    int nIndex1 = (iMemAdd - pdatablock->getStartAddress())*nSize;
    int nIndex2 = 27;
    for(int i = 1;i<=Elements;i++)
    {
        if(ucDataType == S7_DT_Counter ||
                ucDataType == S7_DT_Counter ||
                    ucDataType == S7_DT_Counter )
        {
            nIndex2++;
            ucDataUnit[nIndex2] = 0x00;
        }
        for(int j = 0;j<nSize;j++)
        {
            nIndex2 ++;
            ucDataUnit[nIndex2] = dataBuffer[nIndex1];
            nIndex1++;
        }
    }

    if(uiFillBytes != uiRealBytes)
    {
        ucDataUnit[28+uiRealBytes] = 0x00;
        uiDataUnitLength = 10 + 4 +10 +4 +uiRealBytes +1;
    }
    else
    {
        uiDataUnitLength = 10 + 4 +10 +4 +uiRealBytes;
    }
    uiDataUnitLength = uiDataUnitLength + 7;

    m_sendData[0] = 0x03;
    m_sendData[1] = 0x00;
    m_sendData[2] = (quint8)((uiDataUnitLength>>8)&0xFF);
    m_sendData[3] = (quint8)(uiDataUnitLength&0xFF);
    m_sendData[4] = 0x02;
    m_sendData[5] = 0xF0;
    m_sendData[6] = 0x80;
    for(int i = 1;i<=uiDataUnitLength-7;i++)
    {
        m_sendData[6+i] = ucDataUnit[i-1];
    }

    return uiDataUnitLength;
}
quint16 CMDProtocolCommand::BuildDBWrite300And400(S7RegInform*pSRegInform,uint iMemAdd,CMDDataBlock*pdatablock,quint16 iRanMsgID,uint Elements,unsigned char *dataBuffer)
{
    quint16 uiDataUnitLength,uiRealBytes;//,uiFillBytes
    quint8 ucDataUnit[256];
    int uloffset;
    int nSize = 1;
    quint8 ucDataType;

    switch (pSRegInform->m_nRegType)
    {
    case ID_MEM_C:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*2;
        uloffset = iMemAdd;
        nSize = 2;
    }
        break;
    case ID_MEM_T:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*2;
        uloffset = iMemAdd;
        nSize = 2;
    }
        break;
    case ID_MEM_I:
    case ID_MEM_M:
    case ID_MEM_PQ:
    case ID_MEM_Q:
    case ID_MEM_DB:
    {
        ucDataType = pSRegInform->m_ustrDataType;
        uiRealBytes = Elements*1;
        uloffset = iMemAdd*8;
        nSize = 1;
    }
        break;
    default:
        return 0;
    }

    ucDataUnit[0] = 0x32;
    ucDataUnit[1] = 0x01;
    ucDataUnit[2] = 0x00;
    ucDataUnit[3] = 0x00;
    ucDataUnit[4] = (quint8)((iRanMsgID>>8)&0xFF);
    ucDataUnit[5] = (quint8)(iRanMsgID&0xFF);
    ucDataUnit[6] = 0x00;
    ucDataUnit[7] = 0x0E;
    uiDataUnitLength = uiRealBytes + 4;
    ucDataUnit[8] = (quint8)((uiDataUnitLength>>8)&0xFF);
    ucDataUnit[9] = (quint8)(uiDataUnitLength&0xFF);
    ucDataUnit[10] = 0x05;
    ucDataUnit[11] = 0x01;
    ucDataUnit[12] = 0x12;
    ucDataUnit[13] = 0x0A;
    ucDataUnit[14] = 0x10;
    ucDataUnit[15] = ucDataType;
    ucDataUnit[16] = (quint8)((Elements>>8)&0xFF);
    ucDataUnit[17] = (quint8)(Elements&0xFF);
    ucDataUnit[18] = 0x00;
    if(pSRegInform->m_nRegType == ID_MEM_V)
    {
        ucDataUnit[19] = 0x01;
    }
    else if(pSRegInform->m_nRegType == ID_MEM_DB)
    {
        qint16 sTempDBNumber =  pdatablock->getDBRegisterNum();
        ucDataUnit[18] =  (quint8)((sTempDBNumber&0xFF00)>>8);
        ucDataUnit[19] =  (quint8)(sTempDBNumber&0x00FF);
    }
    else
    {
        ucDataUnit[19] = 0x00;
    }
    ucDataUnit[20] = pSRegInform->m_ustrRegCode;
    ucDataUnit[21] = (quint8)((uloffset&0xFF0000)>>16);
    ucDataUnit[22] = (quint8)((uloffset&0xFF00)>>8);
    ucDataUnit[23] = (quint8)(uloffset&0xFF);
    ucDataUnit[24] = 0x00;
    if(pSRegInform->m_nRegType == ID_MEM_C ||pSRegInform->m_nRegType == ID_MEM_T)
    {
        ucDataUnit[25] = 0x09;
        ucDataUnit[26] = (quint8)((uiRealBytes&0xFF00)>>8);
        ucDataUnit[27] = (quint8)(uiRealBytes&0xFF);
    }
    else
    {
        ucDataUnit[25] = 0x04;
        ucDataUnit[26] = (quint8)(((uiRealBytes*8)&0xFF00)>>8);
        ucDataUnit[27] = (quint8)((uiRealBytes*8)&0xFF);
    }
    int nIndex1 = (iMemAdd - pdatablock->getStartAddress())*nSize;
    int nIndex2 = 27;
    for(int i = 1;i<=Elements;i++)
    {
        for(int j = 0;j<nSize;j++)
        {
            nIndex2 ++;
            ucDataUnit[nIndex2] = dataBuffer[nIndex1];
            nIndex1++;
        }
    }
    uiDataUnitLength = 10 + 4 +10 +4 +uiRealBytes;

    uiDataUnitLength = uiDataUnitLength + 7;

    m_sendData[0] = 0x03;
    m_sendData[1] = 0x00;
    m_sendData[2] = (quint8)((uiDataUnitLength>>8)&0xFF);
    m_sendData[3] = (quint8)(uiDataUnitLength&0xFF);
    m_sendData[4] = 0x02;
    m_sendData[5] = 0xF0;
    m_sendData[6] = 0x80;
    for(int i = 1;i<=uiDataUnitLength-7;i++)
    {
        m_sendData[6+i] = ucDataUnit[i-1];
    }

    return uiDataUnitLength;
}
//*************************************************//

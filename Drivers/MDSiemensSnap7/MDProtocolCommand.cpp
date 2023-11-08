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
    setCommandLength(0);
    setReceiveDataLength(0);
    bool bRet = false;
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();

    if (NULL == dev || NULL == db)
    {
        return bRet;
    }
    TS7Client *pTS7Clent = dev->clientPoint();
    if(NULL==pTS7Clent)
    {
        return bRet;
    }

    QString strIPAddress = dev->commParameter();
    QByteArray arrIPAddress = strIPAddress.toLatin1();
    char *chIPAddress = arrIPAddress.data();

    if(dev->m_nPLCComInitialStatus==-1)
    {
        if(dev->PLCType()==RACK_SLOT)
        {
            bool ok = false;
            int rack = dev->LocalTSAP().toInt(&ok,10);

            bool ok1 = false;
            int slot = dev->RemoteTSAP().toInt(&ok1,10);

            if(ok!=false&&ok1!=false)
            {
                if(pTS7Clent->Disconnect()==0)
                {
                    if(pTS7Clent->ConnectTo(chIPAddress,rack,slot)==0)
                    {
                        dev->m_nPLCComInitialStatus = 1;
                    }
                }
            }
        }
        else if(dev->PLCType()==TSAP)
        {
            bool ok,ok1,ok2,ok3;
            quint8 LocalTSAP1,LocalTSAP2,RemoteTSAP1,RemoteTSAP2;
            QString LocalTSAP = dev->LocalTSAP();
            int localindex = LocalTSAP.indexOf(".");
            if(localindex>0)
            {

                QString TempLocalTSAP1 =LocalTSAP.left(localindex);
                LocalTSAP1 = (quint8)(TempLocalTSAP1.toUInt(&ok,10));
                QString TempLocalTSAP2 =LocalTSAP.right(localindex);
                LocalTSAP2 = (quint8)(TempLocalTSAP2.toUInt(&ok1,10));
            }
            QString RemoteTSAP = dev->RemoteTSAP();
            int remoteindex = RemoteTSAP.indexOf(".");
            if(remoteindex>0)
            {
                QString TempRemoteTSAP1 =RemoteTSAP.left(remoteindex);
                RemoteTSAP1 = (quint8)(TempRemoteTSAP1.toUInt(&ok2,10));
                QString TempRemoteTSAP2 =RemoteTSAP.right(remoteindex);
                RemoteTSAP2 = (quint8)(TempRemoteTSAP2.toUInt(&ok3,10));
            }
            if(ok==true&&ok1==true&&ok2==true&&ok3==true)
            {
                word nLocalTSAP = (word)((LocalTSAP1<<8)|LocalTSAP2);
                word nRemoteTSAP = (word)((RemoteTSAP1<<8)|RemoteTSAP2);
                if(pTS7Clent->SetConnectionParams(chIPAddress,nLocalTSAP,nRemoteTSAP)==0)
                {
                    if(pTS7Clent->Disconnect()==0)
                    {
                        if(pTS7Clent->Connect()==0)
                        {
                            dev->m_nPLCComInitialStatus = 1;
                        }
                    }
                }
            }
        }
    }

    if(dev->m_nPLCComInitialStatus == 1)
    {
        int retResult = 0;
        int nAmount = (int)(db->getEndAddress()-db->getStartAddress()+1);
        int startAddress = (int)db->getStartAddress();
        switch (db->getRegisterType())
        {
        case ID_MEM_I:
        {
            retResult = pTS7Clent->EBRead(startAddress,nAmount,db->dataBuffer());
            if(retResult == 0)
            {
                setCommandResult(ProtocolCommandResult::Right);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else if(retResult == errCliAddressOutOfRange)
            {
                setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else
            {
                dev->m_nPLCComInitialStatus = -1;
                setCommandResult(ProtocolCommandResult::TimeOut);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
        }
            break;
        case ID_MEM_Q:
        {
            retResult = pTS7Clent->ABRead(startAddress,nAmount,db->dataBuffer());
            if(retResult == 0)
            {
                setCommandResult(ProtocolCommandResult::Right);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else if(retResult == errCliAddressOutOfRange)
            {
                setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else
            {
                dev->m_nPLCComInitialStatus = -1;
                setCommandResult(ProtocolCommandResult::TimeOut);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
        }
            break;
        case ID_MEM_M:
        {
            retResult = pTS7Clent->MBRead(startAddress,nAmount,db->dataBuffer());
            if(retResult == 0)
            {
                setCommandResult(ProtocolCommandResult::Right);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else if(retResult == errCliAddressOutOfRange)
            {
                setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else
            {
                dev->m_nPLCComInitialStatus = -1;
                setCommandResult(ProtocolCommandResult::TimeOut);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
        }
            break;
        case ID_MEM_DB:
        {
            retResult = pTS7Clent->DBRead((int)db->getDBRegisterNum(),startAddress,nAmount,db->dataBuffer());
            if(retResult == 0)
            {
                setCommandResult(ProtocolCommandResult::Right);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else if(retResult == errCliAddressOutOfRange||
                        retResult == errCliItemNotAvailable)
            {
                setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else
            {
                dev->m_nPLCComInitialStatus = -1;
                setCommandResult(ProtocolCommandResult::TimeOut);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
        }
            break;
        case ID_MEM_C:
        {
            word tempBuffer[106];
            retResult = pTS7Clent->CTRead(startAddress,nAmount,tempBuffer);
            if(retResult == 0)
            {

                for(int i=0;i<nAmount;i++)
                {
                    memcpy(db->dataBuffer()+i*2,&tempBuffer[i],2);
                }
                setCommandResult(ProtocolCommandResult::Right);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else if(retResult == errCliAddressOutOfRange)
            {
                setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else
            {
                dev->m_nPLCComInitialStatus = -1;
                setCommandResult(ProtocolCommandResult::TimeOut);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
        }
            break;
        case ID_MEM_T:
        {
            word tempBuffer[106];
            retResult = pTS7Clent->TMRead(startAddress,nAmount,tempBuffer);
            if(retResult == 0)
            {
                for(int i=0;i<nAmount;i++)
                {
                    memcpy(db->dataBuffer()+i*2,&tempBuffer[i],2);
                }
                setCommandResult(ProtocolCommandResult::Right);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else if(retResult == errCliAddressOutOfRange)
            {
                setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
            else
            {
                dev->m_nPLCComInitialStatus = -1;
                setCommandResult(ProtocolCommandResult::TimeOut);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }
        }
            break;
        default:
            break;
        }
    }
    else if(dev->m_nPLCComInitialStatus == -1)
    {
        setCommandResult(ProtocolCommandResult::FailedToSend);
        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
        db->parseCommand(pCmd);
    }
    bRet = false;
    return bRet;
}


bool CMDProtocolCommand::setupWriteSingleVariableCommand()
{
    setCommandLength(0);
    setReceiveDataLength(0);
    bool bRet = false;
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();
    IMDRTDBVariable *nsVar = getWriteVarAt(0);
    if (NULL == dev || NULL == db||NULL == nsVar)
    {
        return bRet;
    }
    if(dev->m_nPLCComInitialStatus !=1)
    {
        return bRet;
    }
    TS7Client *pTS7Clent = dev->clientPoint();
    if(NULL==pTS7Clent)
    {
        return bRet;
    }
    qint16 startAddress = -1;
    switch (db->getRegisterType())
    {
    case ID_MEM_C:
    case ID_MEM_T:
    {
        startAddress = db->getStartAddress()+nsVar->internalAddress()/2;
    }
        break;
    case ID_MEM_I:
    case ID_MEM_Q:
    case ID_MEM_M:
    case ID_MEM_DB:
    {
        startAddress = db->getStartAddress()+nsVar->internalAddress();
    }
        break;
    default:
        break;
    }
    int nAmount = 1;
    quint8 tempBuffer[256];
    memset(tempBuffer,0,256);
    bool bNoWriteFlag = 0;
    switch (db->getRegisterType())
    {
    case ID_MEM_C:
    {
        switch (nsVar->originalDataType())
        {
        case MDDataType::Int16:
        {
            QString strValue = nsVar->originalValue().toString();
            bool ok;
            word   wordValue  = (word)strValue.toInt(&ok,16);

            if(wordValue>0x999)
            {
                wordValue = 0x999;
            }
            tempBuffer[0]=(quint8)((wordValue>>8)&0xFF);
            tempBuffer[1]=(quint8)(wordValue&0xFF);
        }
            break;
        case MDDataType::UInt16:
        {
            QString strValue = nsVar->originalValue().toString();
            bool ok;
            word   wordValue  = (word)strValue.toUInt(&ok,16);

            if(wordValue>0x999)
            {
                wordValue = 0x999;
            }
            tempBuffer[0]=(quint8)((wordValue>>8)&0xFF);
            tempBuffer[1]=(quint8)(wordValue&0xFF);
        }
            break;
        default:
            break;
        }

    }
        break;
    case ID_MEM_T:
    {
        switch (nsVar->originalDataType())
        {
        case MDDataType::Int16:
        {
            QString strValue = nsVar->originalValue().toString();
            bool ok;
            word   wordValue  = (word)strValue.toInt(&ok,16);

            tempBuffer[0]=(quint8)((wordValue>>8)&0xFF);
            tempBuffer[1]=(quint8)(wordValue&0xFF);
        }
            break;
        case MDDataType::UInt16:
        {
            QString strValue = nsVar->originalValue().toString();
            bool ok;
            word   wordValue  = (word)strValue.toUInt(&ok,16);

            tempBuffer[0]=(quint8)((wordValue>>8)&0xFF);
            tempBuffer[1]=(quint8)(wordValue&0xFF);
        }
            break;
        case MDDataType::Int32:
        case MDDataType::UInt32:
        {
            bNoWriteFlag = 1;
        }
            break;
        default:
            break;
        }
    }
        break;
    case ID_MEM_I:
    case ID_MEM_Q:
    case ID_MEM_M:
    case ID_MEM_DB:
    {
        switch (nsVar->originalDataType())
        {
        case MDDataType::Bool:
        {
            quint8 tempValue = 0;
            if(db->getDataBufferByteValue(nsVar->internalAddress(),tempValue))
            {
                if(nsVar->originalValue().toBool() !=0)
                {
                    tempValue = tempValue | (1<<nsVar->internalSubAddress());
                }
                else
                {
                    tempValue = tempValue & ~(1<<nsVar->internalSubAddress());
                }
            }
            tempBuffer[0] = tempValue;
        }
            break;
        case MDDataType::SByte:
        {
            qint8 tempValue = (qint8)nsVar->originalValue().toInt();
            tempBuffer[0] = (quint8)(tempValue&0xFF);
        }
            break;
        case MDDataType::Byte:
        {
            quint8 tempValue = (quint8)nsVar->originalValue().toUInt();
            tempBuffer[0] = (quint8)(tempValue&0xFF);
        }
            break;
        case MDDataType::Int16:
        {
            nAmount = 2;
            qint16 tempValue = (qint16)nsVar->originalValue().toInt();
            tempBuffer[0]=(quint8)((tempValue>>8)&0xFF);
            tempBuffer[1]=(quint8)(tempValue&0xFF);
        }
            break;
        case MDDataType::UInt16:
        {
            nAmount = 2;
            quint16 tempValue = (quint16)nsVar->originalValue().toUInt();
            tempBuffer[0]=(quint8)((tempValue>>8)&0xFF);
            tempBuffer[1]=(quint8)(tempValue&0xFF);
        }
            break;
        case MDDataType::Int32:
        {
            nAmount = 4;
            qint32 tempValue = (qint32)nsVar->originalValue().toInt();
            tempBuffer[0]=(quint8)((tempValue>>24)&0xFF);
            tempBuffer[1]=(quint8)((tempValue>>16)&0xFF);
            tempBuffer[2]=(quint8)((tempValue>>8)&0xFF);
            tempBuffer[3]=(quint8)(tempValue&0xFF);
        }
            break;
        case MDDataType::UInt32:
        {
            nAmount = 4;
            quint32 tempValue = (quint32)nsVar->originalValue().toUInt();
            tempBuffer[0]=(quint8)((tempValue>>24)&0xFF);
            tempBuffer[1]=(quint8)((tempValue>>16)&0xFF);
            tempBuffer[2]=(quint8)((tempValue>>8)&0xFF);
            tempBuffer[3]=(quint8)(tempValue&0xFF);
        }
            break;
        case MDDataType::Float:
        {
            nAmount = 4;
            float tempValue = nsVar->originalValue().toFloat();
            quint8 *dataValue  =(quint8*)&tempValue;
            tempBuffer[0]=*(dataValue+3);
            tempBuffer[1]=*(dataValue+2);
            tempBuffer[2]=*(dataValue+1);
            tempBuffer[3]=*(dataValue);
        }
            break;
        case MDDataType::String:
        {
            QString tempstring= nsVar->originalValue().toString();
            QByteArray tempstring1 = tempstring.toLatin1();
            char* tempstring2 = tempstring1.data();
            int  nstrWtiteSize = tempstring.size();

            quint8 nRegulationSize = 0;
            db->getDataBufferByteValue(nsVar->internalAddress(),nRegulationSize);
            bool bEqual = 0;
            if(nRegulationSize>=(db->getEndAddress()-startAddress-1))
            {
                nRegulationSize = db->getEndAddress()-startAddress-1;
            }
            if(nstrWtiteSize>=nRegulationSize)
            {
                bEqual=1;
                nstrWtiteSize = nRegulationSize;
            }
            nAmount = nstrWtiteSize;
            memcpy(tempBuffer,tempstring2,nstrWtiteSize);
            if(bEqual!=1)
            {
                tempBuffer[nAmount] = 0x00;
                nAmount = nAmount + 1;
            }
            startAddress = startAddress + 2;
        }
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
    if(bNoWriteFlag==0)
    {
        switch (db->getRegisterType())
        {
        case ID_MEM_C:
        {
            pTS7Clent->CTWrite(startAddress,nAmount,tempBuffer);
        }
            break;
        case ID_MEM_T:
        {
            pTS7Clent->TMWrite(startAddress,nAmount,tempBuffer);
        }
            break;
        case ID_MEM_I:
        {
            pTS7Clent->EBWrite(startAddress,nAmount,tempBuffer);
        }
            break;
        case ID_MEM_Q:
        {
            pTS7Clent->ABWrite(startAddress,nAmount,tempBuffer);
        }
            break;
        case ID_MEM_M:
        {
            pTS7Clent->MBWrite(startAddress,nAmount,tempBuffer);
        }
            break;
        case ID_MEM_DB:
        {
            pTS7Clent->DBWrite((int)db->getDBRegisterNum(),startAddress,nAmount,tempBuffer);
        }
            break;
        default:
            break;
        }
    }
    setCommandResult(ProtocolCommandResult::Right);
    CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
    db->parseCommand(pCmd);
    bRet = false;
    return bRet;
}

bool CMDProtocolCommand::setupWriteMultiVariableCommand()
{
    setCommandLength(0);
    setReceiveDataLength(0);
    bool bRet = false;
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();
    if (NULL == dev || NULL == db||getWriteVarCount()<=0)
    {
        return bRet;
    }
    TS7Client *pTS7Clent = dev->clientPoint();
    if(NULL==pTS7Clent)
    {
        return bRet;
    }
    unsigned char *dataDatabuffer =new unsigned char[db->getDataBufferSize()] ;
    db->getDataBuffer(dataDatabuffer, 0, db->getDataBufferSize());

    uint nMinAddress = 0;
    uint nMaxAddress = 0;
    uint nAddress = 0;
    IMDRTDBVariable *nsVar=NULL;
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

            switch (db->getRegisterType())
            {
            case ID_MEM_C:
            case ID_MEM_T:
            {
                nAddress = db->getStartAddress()+nsVar->internalAddress()/2;
            }
                break;
            case ID_MEM_I:
            case ID_MEM_Q:
            case ID_MEM_M:
            case ID_MEM_DB:
            {
                nAddress = db->getStartAddress()+nsVar->internalAddress();
            }
                break;
            default:
                break;
            }

            if(n ==0 )
            {
                nMinAddress = nAddress;
                nMaxAddress = nAddress;
            }
            uint nItemOff = nsVar->internalAddress();
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
                {
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        if(dataDatabuffer)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    if(nsVar->originalValue().toBool()!=0)
                    {
                        dataDatabuffer[nItemOff] = dataDatabuffer[nItemOff] | (1<<nsVar->internalSubAddress());
                    }
                    else
                    {
                        dataDatabuffer[nItemOff] = dataDatabuffer[nItemOff] & ~(1<<nsVar->internalSubAddress());
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
            case MDDataType::Byte:
                {
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        if(dataDatabuffer)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    quint8 tempValue = (quint8)nsVar->originalValue().toUInt();
                    dataDatabuffer[nItemOff] = (unsigned char)(tempValue&0xFF);
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
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        if(dataDatabuffer)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    qint8 tempValue = (qint8)nsVar->originalValue().toInt();
                    dataDatabuffer[nItemOff] = (unsigned char)(tempValue&0xFF);
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
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        QString strValue = nsVar->originalValue().toString();
                        bool ok;
                        word   wordValue  = (word)strValue.toInt(&ok,16);

                        dataDatabuffer[nItemOff]=(unsigned char)((wordValue>>8)&0xFF);
                        dataDatabuffer[nItemOff+1]=(unsigned char)(wordValue&0xFF);
                    }
                    else
                    {
                        qint16 tempValue = (qint16)nsVar->originalValue().toInt();
                        dataDatabuffer[nItemOff]=(unsigned char)((tempValue>>8)&0xFF);
                        dataDatabuffer[nItemOff+1]=(unsigned char)(tempValue&0xFF);
                    }
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        if(nMaxAddress<nAddress)
                        {
                            nMaxAddress = nAddress;
                        }
                    }
                    else
                    {
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                }
                break;
            case MDDataType::UInt16:
                {
                if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                {
                    QString strValue = nsVar->originalValue().toString();
                    bool ok;
                    word   wordValue  = (word)strValue.toUInt(&ok,16);

                    dataDatabuffer[nItemOff]=(unsigned char)((wordValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+1]=(unsigned char)(wordValue&0xFF);
                }
                else
                {
                    quint16 tempValue = (quint16)nsVar->originalValue().toUInt();
                    dataDatabuffer[nItemOff]=(unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+1]=(unsigned char)(tempValue&0xFF);
                }
                if(nMinAddress>nAddress)
                {
                    nMinAddress = nAddress;
                }
                if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                {
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress;
                    }
                }
                else
                {
                    if(nMaxAddress<nAddress+1)
                    {
                        nMaxAddress = nAddress+1;
                    }
                }
                }
                break;
            case MDDataType::Int32:
                {
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        if(dataDatabuffer)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    qint32 tempValue = (qint32)nsVar->originalValue().toInt();
                    dataDatabuffer[nItemOff]=(unsigned char)((tempValue>>24)&0xFF);
                    dataDatabuffer[nItemOff+1]=(unsigned char)((tempValue>>16)&0xFF);
                    dataDatabuffer[nItemOff+2]=(unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+3]=(unsigned char)(tempValue&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress+3)
                    {
                        nMaxAddress = nAddress+3;
                    }

                }
                break;
            case MDDataType::UInt32:
                {
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        if(dataDatabuffer)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    quint32 tempValue = (quint32)nsVar->originalValue().toUInt();
                    dataDatabuffer[nItemOff]=(unsigned char)((tempValue>>24)&0xFF);
                    dataDatabuffer[nItemOff+1]=(unsigned char)((tempValue>>16)&0xFF);
                    dataDatabuffer[nItemOff+2]=(unsigned char)((tempValue>>8)&0xFF);
                    dataDatabuffer[nItemOff+3]=(unsigned char)(tempValue&0xFF);
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress+3)
                    {
                        nMaxAddress = nAddress+3;
                    }
                }
                break;
            case MDDataType::Float:
                {
                    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
                    {
                        if(dataDatabuffer)
                        {
                            delete dataDatabuffer;
                            dataDatabuffer = NULL;
                        }
                        return bRet;
                    }
                    float tempValue = nsVar->originalValue().toFloat();
                    unsigned char *dataValue  =(unsigned char*)&tempValue;
                    dataDatabuffer[nItemOff]=*(dataValue+3);
                    dataDatabuffer[nItemOff+1]=*(dataValue+2);
                    dataDatabuffer[nItemOff+2]=*(dataValue+1);
                    dataDatabuffer[nItemOff+3]=*(dataValue);;
                    if(nMinAddress>nAddress)
                    {
                        nMinAddress = nAddress;
                    }
                    if(nMaxAddress<nAddress+3)
                    {
                        nMaxAddress = nAddress+3;
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
        if(dataDatabuffer)
        {
            delete dataDatabuffer;
            dataDatabuffer = NULL;
        }
        return bRet;
    }
    int nAmount = (int)nLength;
    int firstItemOffset = nMinAddress-db->getStartAddress();
    if(db->getRegisterType()==ID_MEM_C||db->getRegisterType()==ID_MEM_T)
    {
        firstItemOffset = firstItemOffset*2;
    }
    switch (db->getRegisterType())
    {
    case ID_MEM_C:
    {
        pTS7Clent->CTWrite((int)nMinAddress,nAmount,&dataDatabuffer[firstItemOffset]);
    }
        break;
    case ID_MEM_T:
    {
        pTS7Clent->TMWrite((int)nMinAddress,nAmount,&dataDatabuffer[firstItemOffset]);
    }
        break;
    case ID_MEM_I:
    {
        pTS7Clent->EBWrite((int)nMinAddress,nAmount,&dataDatabuffer[firstItemOffset]);
    }
        break;
    case ID_MEM_Q:
    {
        pTS7Clent->ABWrite((int)nMinAddress,nAmount,&dataDatabuffer[firstItemOffset]);
    }
        break;
    case ID_MEM_M:
    {
        pTS7Clent->MBWrite((int)nMinAddress,nAmount,&dataDatabuffer[firstItemOffset]);
    }
        break;
    case ID_MEM_DB:
    {
        pTS7Clent->DBWrite((int)db->getDBRegisterNum(),(int)nMinAddress,nAmount,&dataDatabuffer[firstItemOffset]);
    }
        break;
    default:
        break;
    }

    setCommandResult(ProtocolCommandResult::Right);
    CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
    db->parseCommand(pCmd);

    if(dataDatabuffer)
    {
        delete dataDatabuffer;
        dataDatabuffer = NULL;
    }

    bRet = false;
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
    setReceiveDataLength(0);
    setCommandResult(ProtocolCommandResult::Error);
    return getCommandResult();
}

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
    CMDChannel *chl =(CMDChannel *) getChannel();
    if (NULL == dev || NULL == db ||NULL==chl)
    {
        return bRet;
    }
    if(chl->loadSuccessFlag()==1)
    {

        if(dev->activeNO()==-1)
        {
            bool bReConnectFlag = 1;
            for(int i = 0;i<chl->getDeviceCount();i++)
            {
                CMDDevice *device = (CMDDevice*)chl->getDevice(i);
                if(device->activeNO()!=-1)
                {
                    bReConnectFlag  = 0;
                    break;
                }
            }
            if(bReConnectFlag==1)
            {
                unsigned short deviceCount = (unsigned short)chl->getDeviceCount();
                for(int i = 0;i<deviceCount;i++)
                {
                    CMDDevice *device = (CMDDevice*)chl->getDevice(i);
                    CON_TABLE_TYPE* pCurrentConnect = chl->currentConnect(i);

                    QString strMPIStationNo = device->MPIStation();
                    QString strRackNo = device->LocalTSAP();
                    QString strSlotNo = device->RemoteTSAP();
                    bool okMPI=0,okRack=0,okSlot=0;
                    int nMPIStationNo = strMPIStationNo.toInt(&okMPI,10);
                    int nRackNo = strRackNo.toInt(&okRack,10);
                    int nSlotNo = strSlotNo.toInt(&okSlot,10);
                    if(okMPI==1&&okRack==1&&okSlot==1&&nMPIStationNo>=0&&nRackNo>=0&&nSlotNo>=0)
                    {
                        pCurrentConnect->AdrType = 1;
                        pCurrentConnect->Adr.Mpi = (unsigned char)nMPIStationNo;
                        pCurrentConnect->RackNr = (unsigned char)nRackNo;
                        pCurrentConnect->SlotNr = (unsigned char)nSlotNo;
                        device->setActiveNO(i);
                    }
                }
                int returnValue = 0;

                pfn_LoadConnection_ex6 connect=(pfn_LoadConnection_ex6)chl->libraryLoad()->resolve("LoadConnection_ex6");
                returnValue = connect((deviceCount-1),(char*)"S7ONLINE",576,chl->currentConnect(0));
                if(returnValue !=0)
                {
                    for(int i = 0;i<deviceCount;i++)
                    {
                        CMDDevice *device = (CMDDevice*)chl->getDevice(i);
                        device->setActiveNO(-1);
                    }
                }

            }

        }

        if(dev->activeNO()!=-1)
        {
            int retResult = 0;
            unsigned short nAmount = (unsigned short)(db->getEndAddress()-db->getStartAddress()+1);
            unsigned short startAddress = (unsigned short)db->getStartAddress();
            unsigned short dbNumber = (unsigned short)db->getDBRegisterNum();
            unsigned long dataLen = 0;

            pfn_SetActiveConnection_ex6 activeNR = (pfn_SetActiveConnection_ex6)chl->libraryLoad()->resolve("SetActiveConnection_ex6");

            retResult = activeNR((unsigned short)dev->activeNO());
            if(retResult==0)
            {
                pfn_field_read_ex6 fieldRead=(pfn_field_read_ex6)chl->libraryLoad()->resolve("field_read_ex6");

                switch (db->getRegisterType())
                {
                case ID_MEM_I:
                {
                    char fieldType = 'E';
                    retResult = fieldRead(fieldType,dbNumber,startAddress,nAmount,(unsigned long)db->getDataBufferSize(),db->dataBuffer(),&dataLen);
                    if(retResult == 0)
                    {
                        setCommandResult(ProtocolCommandResult::Right);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else if(retResult == 28741)
                    {
                        setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else
                    {
                        dev->setActiveNO(-1);
                        setCommandResult(ProtocolCommandResult::TimeOut);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                }
                    break;
                case ID_MEM_Q:
                {
                    char fieldType = 'A';
                    retResult = fieldRead(fieldType,dbNumber,startAddress,nAmount,(unsigned long)db->getDataBufferSize(),db->dataBuffer(),&dataLen);
                    if(retResult == 0)
                    {
                        setCommandResult(ProtocolCommandResult::Right);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else if(retResult == 28741)
                    {
                        setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else
                    {
                        dev->setActiveNO(-1);
                        setCommandResult(ProtocolCommandResult::TimeOut);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                }
                    break;
                case ID_MEM_M:
                {
                    char fieldType = 'M';
                    retResult = fieldRead(fieldType,dbNumber,startAddress,nAmount,(unsigned long)db->getDataBufferSize(),db->dataBuffer(),&dataLen);
                    if(retResult == 0)
                    {
                        setCommandResult(ProtocolCommandResult::Right);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else if(retResult == 28741)
                    {
                        setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else
                    {
                        dev->setActiveNO(-1);
                        setCommandResult(ProtocolCommandResult::TimeOut);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                }
                    break;
                case ID_MEM_DB:
                {
                    char fieldType = 'D';
                    retResult = fieldRead(fieldType,dbNumber,startAddress,nAmount,(unsigned long)db->getDataBufferSize(),db->dataBuffer(),&dataLen);
                    if(retResult == 0)
                    {
                        setCommandResult(ProtocolCommandResult::Right);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else if(retResult == 28741)
                    {
                        setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else
                    {
                        dev->setActiveNO(-1);
                        setCommandResult(ProtocolCommandResult::TimeOut);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                }
                    break;
                case ID_MEM_C:
                {
                    char fieldType = 'Z';
                    retResult = fieldRead(fieldType,dbNumber,startAddress,nAmount,(unsigned long)db->getDataBufferSize(),db->dataBuffer(),&dataLen);
                    if(retResult == 0)
                    {
                        setCommandResult(ProtocolCommandResult::Right);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else if(retResult == 28741)
                    {
                        setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else
                    {
                        dev->setActiveNO(-1);
                        setCommandResult(ProtocolCommandResult::TimeOut);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                }
                    break;
                case ID_MEM_T:
                {
                    char fieldType = 'T';
                    retResult = fieldRead(fieldType,dbNumber,startAddress,nAmount,(unsigned long)db->getDataBufferSize(),db->dataBuffer(),&dataLen);
                    if(retResult == 0)
                    {
                        setCommandResult(ProtocolCommandResult::Right);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else if(retResult == 28741)
                    {
                        setCommandResult(ProtocolCommandResult::DataBlockBConfigErr);
                        CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                        db->parseCommand(pCmd);
                    }
                    else
                    {
                        dev->setActiveNO(-1);
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
            else
            {
                dev->setActiveNO(-1);
                setCommandResult(ProtocolCommandResult::FailedToSend);
                CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
                db->parseCommand(pCmd);
            }

        }
        else
        {
            setCommandResult(ProtocolCommandResult::FailedToSend);
            CBaseProtocolCommand *pCmd = (CBaseProtocolCommand *)this;
            db->parseCommand(pCmd);
        }
    }
    else
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
    CMDChannel *chl =(CMDChannel *) getChannel();
    IMDRTDBVariable *nsVar = getWriteVarAt(0);
    if (NULL == dev || NULL == db || NULL == chl||NULL == nsVar)
    {
        return bRet;
    }
    if(chl->loadSuccessFlag()==0)
    {
        return bRet;
    }
    if(dev->activeNO()==-1)
    {
        return bRet;
    }
    pfn_SetActiveConnection_ex6 activeNR = (pfn_SetActiveConnection_ex6)chl->libraryLoad()->resolve("SetActiveConnection_ex6");

    int retResult = activeNR((unsigned short)dev->activeNO());
    if(retResult!=0)
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
            qint16   wordValue  = (qint16)strValue.toInt(&ok,16);

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
            qint16   wordValue  = (qint16)strValue.toUInt(&ok,16);

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
            qint16   wordValue  = (qint16)strValue.toInt(&ok,16);

            tempBuffer[0]=(quint8)((wordValue>>8)&0xFF);
            tempBuffer[1]=(quint8)(wordValue&0xFF);
        }
            break;
        case MDDataType::UInt16:
        {
            QString strValue = nsVar->originalValue().toString();
            bool ok;
            qint16   wordValue  = (qint16)strValue.toUInt(&ok,16);

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
    pfn_field_write_ex6 fieldWrite = (pfn_field_write_ex6)chl->libraryLoad()->resolve("field_write_ex6");
    if(bNoWriteFlag==0)
    {
        switch (db->getRegisterType())
        {
        case ID_MEM_C:
        {
            char fieldType = 'Z';
            fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)startAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);

        }
            break;
        case ID_MEM_T:
        {
            char fieldType = 'T';
            fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)startAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
        }
            break;
        case ID_MEM_I:
        {
            char fieldType = 'E';
            fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)startAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
        }
            break;
        case ID_MEM_Q:
        {
            char fieldType = 'A';
            fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)startAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
        }
            break;
        case ID_MEM_M:
        {
            char fieldType = 'M';
            fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)startAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
        }
            break;
        case ID_MEM_DB:
        {
            char fieldType = 'D';
            fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)startAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
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
    CMDChannel *chl =(CMDChannel *) getChannel();
    if (NULL == dev || NULL == db||NULL == chl||getWriteVarCount()<=0)
    {
        return bRet;
    }
    if(chl->loadSuccessFlag()==0)
    {
        return bRet;
    }
    if(dev->activeNO()==-1)
    {
        return bRet;
    }
    pfn_SetActiveConnection_ex6 activeNR = (pfn_SetActiveConnection_ex6)chl->libraryLoad()->resolve("SetActiveConnection_ex6");

    int retResult = activeNR((unsigned short)dev->activeNO());
    if(retResult!=0)
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
                        qint16   wordValue  = (qint16)strValue.toInt(&ok,16);

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
                    qint16   wordValue  = (qint16)strValue.toUInt(&ok,16);

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
    pfn_field_write_ex6 fieldWrite = (pfn_field_write_ex6)chl->libraryLoad()->resolve("field_write_ex6");
    switch (db->getRegisterType())
    {
    case ID_MEM_C:
    {
        char fieldType = 'Z';
        quint8 tempBuffer[256];
        memset(tempBuffer,0,256);
        memcpy(tempBuffer,&dataDatabuffer[firstItemOffset],nAmount);
        fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)nMinAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
    }
        break;
    case ID_MEM_T:
    {
        char fieldType = 'T';
        quint8 tempBuffer[256];
        memset(tempBuffer,0,256);
        memcpy(tempBuffer,&dataDatabuffer[firstItemOffset],nAmount);
        fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)nMinAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
    }
        break;
    case ID_MEM_I:
    {
        char fieldType = 'E';
        quint8 tempBuffer[256];
        memset(tempBuffer,0,256);
        memcpy(tempBuffer,&dataDatabuffer[firstItemOffset],nAmount);
        fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)nMinAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
    }
        break;
    case ID_MEM_Q:
    {
        char fieldType = 'A';
        quint8 tempBuffer[256];
        memset(tempBuffer,0,256);
        memcpy(tempBuffer,&dataDatabuffer[firstItemOffset],nAmount);
        fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)nMinAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
    }
        break;
    case ID_MEM_M:
    {
        char fieldType = 'M';
        quint8 tempBuffer[256];
        memset(tempBuffer,0,256);
        memcpy(tempBuffer,&dataDatabuffer[firstItemOffset],nAmount);
        fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)nMinAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
    }
        break;
    case ID_MEM_DB:
    {
        char fieldType = 'D';
        quint8 tempBuffer[256];
        memset(tempBuffer,0,256);
        memcpy(tempBuffer,&dataDatabuffer[firstItemOffset],nAmount);
        fieldWrite(fieldType,(unsigned short)db->getDBRegisterNum(),(unsigned short)nMinAddress,(unsigned long)nAmount,(unsigned long)sizeof(tempBuffer),(unsigned char *)tempBuffer);
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

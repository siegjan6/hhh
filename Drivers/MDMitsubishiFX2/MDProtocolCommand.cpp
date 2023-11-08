#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"


CMDProtocolCommand::CMDProtocolCommand()
{
     m_readCmdDataLength = 0;
     m_bRemovBagHeadFlag = 0;//!去除包头标志位
      setReceiveBufferSize(600);//PKX20151119
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

    m_sendData[0] = 0x02;

    qint16 m_nAddress,m_nLen,m_nTemp1,m_nTemp2,nTemp;
    switch (db->registerType())
    {
    case ID_MEM_X:
    case ID_MEM_Y:
    {
        m_nTemp1 = (qint16)(db->startAddress()/100)* 8 + (qint16)(db->startAddress()%100/10);
        nTemp = db->endAddress()%100;
        if (nTemp >77)
        {
            nTemp = 77;
        }
        m_nTemp2 = (qint16)(db->endAddress()/100)* 8 + (qint16)(nTemp/10);

        m_nAddress = m_nTemp1 + db->offsetAddr();
        m_nLen = m_nTemp2 - m_nTemp1 +1;

    }
        break;
    case ID_MEM_M:
    {
        m_nTemp1 = (qint16)(db->startAddress()/8);
        m_nTemp2 = (qint16)(db->endAddress()/8);
        m_nLen = m_nTemp2 - m_nTemp1 +1;

        if (db->startAddress() >= 8000)
        {
            m_nTemp1 = (db->startAddress()-8000)/8;
        }
        m_nAddress = m_nTemp1 + db->offsetAddr();
    }
        break;
    case ID_MEM_S:
    case ID_MEM_CS:
    case ID_MEM_TS:
    {
        m_nTemp1 = (qint16)(db->startAddress()/8);
        m_nTemp2 = (qint16)(db->endAddress()/8);
        m_nAddress = m_nTemp1 + db->offsetAddr();
        m_nLen = m_nTemp2 - m_nTemp1 +1;
    }
        break;
    case ID_MEM_D:
    case ID_MEM_T:
    {
        if (db->startAddress()  <8000)
        {
            m_nAddress = db->startAddress() * 2 + db->offsetAddr();
        }
        else
        {
            m_nAddress = (db->startAddress()-8000) * 2 + db->offsetAddr();
        }
        qint16 nlength = db->endAddress()-db->startAddress() +1;
        m_nLen = nlength*2;
    }
        break;
    case ID_MEM_C:
    {
        if (db->startAddress() >=200)
        {
            m_nAddress = (db->startAddress()-200) * 4 + 0xC00;
            qint16 nlength = db->endAddress()-db->startAddress() +1;
            m_nLen = nlength*4;
        }
        else
        {
            m_nAddress = db->startAddress() * 2 + db->offsetAddr();
            qint16 nlength = db->endAddress()-db->startAddress() +1;
            m_nLen = nlength*2;
        }
    }
        break;

    default:
        return bRet;
    }

    m_readCmdDataLength = m_nLen;
    QString strCmd = "";
    QString strTemp = "";
    strTemp.sprintf("%.4X",m_nAddress);
    strCmd = db->cmdRead() + strTemp;
    strTemp.sprintf("%.2X",m_nLen);
    strCmd = strCmd+strTemp;

    int nBCCDataLength = strCmd.size();


    QByteArray byteArrayCmdRead = strCmd.toLatin1();
    char * chCmdRead = byteArrayCmdRead.data();

    memcpy(&m_sendData[1],chCmdRead,nBCCDataLength);

    m_sendData[nBCCDataLength+1] = 0x03;

    nBCCDataLength = nBCCDataLength + 1;

    unsigned char uchBCC = BCC(nBCCDataLength,m_sendData);

    strTemp.sprintf("%.2X",uchBCC);

    QByteArray byteArrayBCC = strTemp.toLatin1();

    char * chBCC = byteArrayBCC.data();

    memcpy(&m_sendData[nBCCDataLength+1],chBCC,2);
    nBCCDataLength = nBCCDataLength + 3;

    setCommandLength(nBCCDataLength);
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
    if (db->registerType() == ID_MEM_X||
            db->registerType() == ID_MEM_TS ||
                db->registerType() == ID_MEM_CS )
    {
        return bRet;
    }
    setCommandLength(0);

    QString str,strtemp;

    qint16 m_nAddress,m_nTemp1,m_nTemp2;

    m_sendData[0] = 0x02;

    ushort lItemOffset = 0;
    ushort nSubItemOffset = 0;
    if(db->registerType() == ID_MEM_Y)
    {
        QStringList addressSplit = nsVar->address().split(".");
        if (addressSplit.count() < 2)
        {
            return bRet;
        }
        QString strRegisterTypeVar=addressSplit[1];
        strRegisterTypeVar.truncate(1);

        if ("Y"!=strRegisterTypeVar)
        {
            return bRet;
        }
        QString strAddress = addressSplit[1];
        nSubItemOffset =  strAddress.remove(0,1).toUShort();
    }
    else if(db->registerType()==ID_MEM_M ||db->registerType()==ID_MEM_S)
    {
       nSubItemOffset= (ushort)((nsVar->internalAddress()+db->startAddress()/8)*8 + nsVar->internalSubAddress());
    }

    if(nsVar->originalDataType() == MDDataType::Bool)
    {
        if(db->registerType() != ID_MEM_D &&db->registerType() != ID_MEM_M &&db->registerType()!= ID_MEM_Y &&db->registerType()!= ID_MEM_S)
        {
            return bRet;
        }
        if(db->registerType() != ID_MEM_D)
        {
            if(nsVar->originalValue().toBool()== true)
            {
                switch(dev->PLCType())
                {
                default:
                case PLC_FX:
                case PLC_FX0:
                case PLC_FX0N:
                    str =  "7";
                    break;
                case PLC_FX2N:
                    str =  "E7";
                    break;
                }
            }
            else
            {
                switch(dev->PLCType())
                {
                default:
                case PLC_FX:
                case PLC_FX0:
                case PLC_FX0N:
                    str =  "8";
                    break;
                case PLC_FX2N:
                    str =  "E8";
                    break;
                }
            }
            switch (db->registerType())
            {
            case ID_MEM_Y:
            {
                m_nAddress = nSubItemOffset/100 * 64 + nSubItemOffset%100/10 * 8 + nSubItemOffset%10;
                strtemp.sprintf("%.2X",m_nAddress);
                str = str + strtemp;
                switch(dev->PLCType())
                {
                default:
                case PLC_FX:
                case PLC_FX0:
                case PLC_FX0N:
                    str +=  "05";
                    break;
                case PLC_FX2N:
                    str +=  "0C";
                    break;
                }
            }
                break;
            case ID_MEM_M:
            {
                if (lItemOffset <8000)
                {

                    m_nTemp1 = (nSubItemOffset + db->bitOffsetAddr())&0xFF;
                    strtemp.sprintf("%.2X",m_nTemp1);
                    str += strtemp;

                    m_nTemp1 = ((nSubItemOffset + db->bitOffsetAddr())>>8)&0xFF;
                    strtemp.sprintf("%.2X",m_nTemp1);
                    str += strtemp;

                }
                else
                {
                    m_nTemp1 = nSubItemOffset-8000 + db->bitOffsetAddr();

                    m_nTemp2 = m_nTemp1&0xFF;
                    strtemp.sprintf("%.2X",m_nTemp2);
                    str += strtemp;

                    m_nTemp2 = (m_nTemp1>>8)&0xFF;
                    strtemp.sprintf("%.2X",m_nTemp2);
                    str += strtemp;
                }
            }
                break;
            case ID_MEM_S:
            {
                m_nTemp1 = nSubItemOffset + db->bitOffsetAddr();
                m_nTemp2 = m_nTemp1&0xFF;
                strtemp.sprintf("%.2X",m_nTemp2);
                str += strtemp;

                m_nTemp2 = (m_nTemp1>>8)&0xFF;
                strtemp.sprintf("%.2X",m_nTemp2);
                str += strtemp;
            }
                break;
            default:
                return bRet;
            }
        }
        else
        {
            str = db->cmdWrite();
            lItemOffset= (ushort) (nsVar->internalAddress()/2 + db->startAddress());
            if (db->startAddress() < 8000)
            {
                m_nAddress = lItemOffset * 2 + db->offsetAddr();
            }
            else
            {
                m_nAddress = (lItemOffset-8000) * 2 + db->offsetAddr();
            }
            strtemp.sprintf("%.4X",m_nAddress);
            str += strtemp;

            str += "02";		//len

            ushort datavalue = 0;
            if(db->getDataBufferUShortValue(nsVar->internalAddress(),datavalue))
            {
                if(nsVar->originalValue() !=false)
                {
                    datavalue = (datavalue | (1<<nsVar->internalSubAddress()));
                }
                else
                {
                    datavalue = (datavalue & ~(1<<nsVar->internalSubAddress()));
                }
            }
            quint8 temp[3];
            memcpy(temp,&datavalue,2);
            for(int i = 0 ;i<2;i++)
            {
                strtemp.sprintf("%.2X",temp[i]);
                str += strtemp;
            }
        }
    }
    else
    {
        str = db->cmdWrite();
        switch(db->registerType())
        {
        case ID_MEM_C:
            if (db->startAddress() >=200)
            {
                lItemOffset= (ushort) (nsVar->internalAddress()/4 + db->startAddress());
                m_nAddress = (lItemOffset-200) * 4 + db->offsetAddr();
            }
            else
            {
                lItemOffset= (ushort) (nsVar->internalAddress()/2 + db->startAddress());
                m_nAddress = lItemOffset * 2 + db->offsetAddr();
            }
            break;
        case ID_MEM_D:
        case ID_MEM_T:
            lItemOffset= (ushort) (nsVar->internalAddress()/2 + db->startAddress());
            if (db->startAddress() < 8000)
            {
                m_nAddress = lItemOffset * 2 + db->offsetAddr();
            }
            else
            {
                m_nAddress = (lItemOffset-8000) * 2 + db->offsetAddr();
            }

            break;
        default:
            return bRet;
        }
        if(nsVar->originalDataType()==MDDataType::String)
        {
            qint16 tempAddress = m_nAddress+2;
            strtemp.sprintf("%.4X",tempAddress);
            str += strtemp;
        }
        else
        {
            strtemp.sprintf("%.4X",m_nAddress);
            str += strtemp;
        }


        switch(nsVar->originalDataType())
        {
            case MDDataType::Int16:
                {
                    str += "02";		//len
                    qint16 tempValue = (qint16)nsVar->originalValue().toInt();

                    quint8 temp[3];
                    memcpy(temp,&tempValue,2);
                    for(int i = 0 ;i<2;i++)
                    {
                        strtemp.sprintf("%.2X",temp[i]);
                        str += strtemp;
                    }
                }
                break;
            case MDDataType::UInt16:
                {
                    str += "02";
                    quint16 tempValue = (quint16)nsVar->originalValue().toUInt();
                    quint8 temp[3];
                    memcpy(temp,&tempValue,2);
                    for(int i = 0 ;i<2;i++)
                    {
                        strtemp.sprintf("%.2X",temp[i]);
                        str += strtemp;
                    }
                }
                break;

            case MDDataType::Int32:
                {
                    str += "04";
                    qint32 tempValue = (qint32)nsVar->originalValue().toInt();
                    quint8 temp[5];
                    memcpy(temp,&tempValue,4);
                    for(int i = 0 ;i<4;i++)
                    {
                        strtemp.sprintf("%.2X",temp[i]);
                        str += strtemp;
                    }
                }
                break;
            case MDDataType::UInt32:
                {
                    str += "04";
                    quint32 tempValue = (quint32)nsVar->originalValue().toUInt();
                    quint8 temp[5];
                    memcpy(temp,&tempValue,4);
                    for(int i = 0 ;i<4;i++)
                    {
                        strtemp.sprintf("%.2X",temp[i]);
                        str += strtemp;
                    }
                }
                break;
            case MDDataType::Float:
                {
                    str += "04";
                    float tempValue = (float)nsVar->originalValue().toFloat();
                    quint8 temp[5];
                    memcpy(temp,&tempValue,4);
                    for(int i = 0 ;i<4;i++)
                    {
                        strtemp.sprintf("%.2X",temp[i]);
                        str += strtemp;
                    }
                }
                break;
            case MDDataType::String:
                {
                    if(db->registerType()!=ID_MEM_D)
                    {
                        return bRet;
                    }
                    QString tempstring= nsVar->originalValue().toString();
                    QByteArray tempstring1 = tempstring.toLatin1();
                    char* tempstring2 = tempstring1.data();
                    quint8 temp[1024];
                    memset(temp,0,1024);

                    int  nstrWtiteSize = tempstring.size();
                    memcpy(temp,tempstring2,nstrWtiteSize);
                    quint16 nRegulationSize = 0;
                    db->getDataBufferUShortValue(nsVar->internalAddress(), nRegulationSize);

                    if(nRegulationSize>=(db->endAddress()-nsVar->internalAddress()/2-db->startAddress())*2)
                    {
                      nRegulationSize = (db->endAddress()-nsVar->internalAddress()/2-db->startAddress())*2;
                    }
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
                    strtemp.sprintf("%.2X",nstrWtiteSize);
                    str += strtemp;
                    for(int i = 0 ;i<nstrWtiteSize;i++)
                    {
                        if(i == (nstrWtiteSize - 2))
                        {
                            if(bOddOrEven)
                            {
                                strtemp.sprintf("%.2X",temp[i]);
                                str += strtemp;
                            }
                            else
                            {
                                if(bLenghtEqual == 1)
                                {
                                    strtemp.sprintf("%.2X",temp[i]);
                                    str += strtemp;
                                }
                                else
                                {
                                    str += "00";

                                }
                            }
                        }
                        else if(i == (nstrWtiteSize - 1))
                        {
                            if(bOddOrEven)
                            {
                                str += "00";

                            }
                            else
                            {
                                if(bLenghtEqual == 1)
                                {
                                    strtemp.sprintf("%.2X",temp[i]);
                                    str += strtemp;
                                }
                                else
                                {
                                    str += "00";
                                }
                            }
                        }
                        else
                        {
                            strtemp.sprintf("%.2X",temp[i]);
                            str += strtemp;
                        }
                    }
                }
                break;
            default:
                return bRet;
        }
    }
    int nBCCDataLength = str.size();

    QByteArray byteArrayCmdRead = str.toLatin1();
    char * chCmdRead = byteArrayCmdRead.data();

    memcpy(&m_sendData[1],chCmdRead,nBCCDataLength);

    m_sendData[nBCCDataLength+1] = 0x03;

    nBCCDataLength = nBCCDataLength + 1;

    unsigned char uchBCC = BCC(nBCCDataLength,m_sendData);

    strtemp.sprintf("%.2X",uchBCC);

    QByteArray byteArrayBCC = strtemp.toLatin1();

    char * chBCC = byteArrayBCC.data();

    memcpy(&m_sendData[nBCCDataLength+1],chBCC,2);
    nBCCDataLength = nBCCDataLength + 3;

    setCommandLength(nBCCDataLength);

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
    if (db->registerType() == ID_MEM_X||
            db->registerType() == ID_MEM_TS ||
                db->registerType() == ID_MEM_CS )
    {
        return bRet;
    }
    setCommandLength(0);

    unsigned char *dataDatabuffer =new unsigned char[db->getDataBufferSize()] ;
    db->getDataBuffer(dataDatabuffer, 0, db->getDataBufferSize());

    QString strtemp;
    uint nMinAddress = 0;
    uint nMaxAddress = 0;
    uint m_nTemp1,m_nTemp2;
    uint nAddress;
    uint nTypeLength = 2;

    uint nBitPos;
    uint nItemOff;

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
            switch (db->registerType())
            {
            case ID_MEM_Y:
            {
                QStringList addressSplit = nsVar->address().split(".");
                if (addressSplit.count() < 2)
                {
                    return bRet;
                }
                QString strRegisterTypeVar=addressSplit[1];
                strRegisterTypeVar.truncate(1);

                if ("Y"!=strRegisterTypeVar)
                {
                    if(dataDatabuffer != NULL)
                    {
                        delete dataDatabuffer;
                        dataDatabuffer = NULL;
                    }
                    return bRet;
                }
                QString strAddress = addressSplit[1];
                nAddress =  strAddress.remove(0,1).toUInt();
            }
                break;
            case ID_MEM_M:
            case ID_MEM_S:
                nAddress =(uint) ((nsVar->internalAddress() + db->startAddress()/8)*8 + nsVar->internalSubAddress());
                break;
            case ID_MEM_C:
                if (db->startAddress() >=200)
                {
                    nAddress= (uint) (nsVar->internalAddress()/4 + db->startAddress());
                }
                else
                {
                    nAddress= (uint) (nsVar->internalAddress()/2 + db->startAddress());
                }
                break;
            case ID_MEM_D:
            case ID_MEM_T:
                nAddress= (uint) (nsVar->internalAddress()/2 + db->startAddress());
                break;
            default:
                if(dataDatabuffer != NULL)
                {
                    delete dataDatabuffer;
                    dataDatabuffer = NULL;
                }
                return bRet;
            }

            if (nMinAddress > nAddress || n==0)
            {
                nMinAddress = nAddress;
            }

            nItemOff = (nAddress- db->startAddress()) * nTypeLength;//默认为D/T

            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                if(db->registerType() != ID_MEM_Y &&
                        db->registerType() != ID_MEM_M&&
                            db->registerType() != ID_MEM_S)
                {
                     if(dataDatabuffer != NULL)
                     {
                         delete dataDatabuffer;
                         dataDatabuffer = NULL;
                     }
                    return bRet;
                }
                switch(db->registerType())
                {
                case ID_MEM_Y:
                    m_nTemp1 = (nAddress/100)* 8 + nAddress%100/10;
                    m_nTemp2 = (db->startAddress()/100)* 8 + db->startAddress()%100/10;
                    nItemOff = m_nTemp1- m_nTemp2;
                    nBitPos = nAddress%100%10;
                    break;
                case ID_MEM_M:
                case ID_MEM_S:
                    nItemOff = nAddress/8-db->startAddress()/8;
                    nBitPos = nAddress%8;
                    break;
                default:
                     if(dataDatabuffer != NULL)
                     {
                         delete dataDatabuffer;
                         dataDatabuffer = NULL;
                     }
                    return bRet;
                }

                if(nsVar->originalValue().toBool()==false)
                {
                    dataDatabuffer[nItemOff]   = dataDatabuffer[nItemOff]& (~(1 <<nBitPos));
                }
                else
                {
                    dataDatabuffer[nItemOff]= dataDatabuffer[nItemOff]| (1 <<nBitPos);
                }

                if(nMaxAddress<nAddress)
                {
                    nMaxAddress = nAddress;
                }
            }
                break;
            case MDDataType::Int16:
            {
                if(db->registerType() == ID_MEM_Y ||
                        db->registerType() == ID_MEM_M||
                            db->registerType() == ID_MEM_S||
                                (db->registerType() == ID_MEM_C && db->startAddress() >=200))
                {
                     if(dataDatabuffer != NULL)
                     {
                         delete dataDatabuffer;
                         dataDatabuffer = NULL;
                     }

                    return bRet;
                }
                qint16 tempValue = (qint16)nsVar->originalValue().toInt();
                dataDatabuffer[nItemOff] = (unsigned char) (tempValue&0xFF);
                dataDatabuffer[nItemOff+1] =(unsigned char)((tempValue>>8)&0xFF);

                if(nMaxAddress<nAddress)
                {
                    nMaxAddress = nAddress;
                }
            }
                break;
            case MDDataType::UInt16:
            {
                if(db->registerType() == ID_MEM_Y ||
                        db->registerType() == ID_MEM_M||
                            db->registerType() == ID_MEM_S||
                                (db->registerType() == ID_MEM_C && db->startAddress() >=200))
                {
                     if(dataDatabuffer != NULL)
                     {
                         delete dataDatabuffer;
                         dataDatabuffer = NULL;
                     }
                    return bRet;
                }
                quint16 tempValue = (quint16)nsVar->originalValue().toUInt();
                dataDatabuffer[nItemOff] = (unsigned char) (tempValue&0xFF);
                dataDatabuffer[nItemOff+1] =(unsigned char)((tempValue>>8)&0xFF);

                if(nMaxAddress<nAddress)
                {
                    nMaxAddress = nAddress;
                }
            }
                break;
            case MDDataType::Int32:
            {
                if(db->registerType() == ID_MEM_Y ||
                        db->registerType() == ID_MEM_M||
                            db->registerType() == ID_MEM_S||
                                db->registerType() == ID_MEM_T||
                                    (db->registerType() == ID_MEM_C && db->startAddress() <200))
                {
                     if(dataDatabuffer != NULL)
                     {
                         delete dataDatabuffer;
                         dataDatabuffer = NULL;
                     }
                    return bRet;
                }

                if (db->registerType() ==ID_MEM_C) //C200~ 4byte
                {
                    nItemOff = (nAddress - db->startAddress())*4;
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress ;
                    }
                }
                else
                {
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress +1;
                    }
                }
                qint32 tempValue = (qint32) nsVar->originalValue().toInt();
                dataDatabuffer[nItemOff] = (unsigned char) (tempValue&0xFF);
                dataDatabuffer[nItemOff+1] =(unsigned char)((tempValue>>8)&0xFF);
                dataDatabuffer[nItemOff+2] = (unsigned char) ((tempValue>>16)&0xFF);
                dataDatabuffer[nItemOff+3] =(unsigned char)((tempValue>>24)&0xFF);
            }
                break;
            case MDDataType::UInt32:
            {
                if(db->registerType() == ID_MEM_Y ||
                        db->registerType() == ID_MEM_M||
                            db->registerType() == ID_MEM_S||
                                db->registerType() == ID_MEM_T||
                                    (db->registerType() == ID_MEM_C && db->startAddress() <200))
                {
                     if(dataDatabuffer != NULL)
                     {
                         delete dataDatabuffer;
                         dataDatabuffer = NULL;
                     }
                    return bRet;
                }

                if (db->registerType() ==ID_MEM_C) //C200~ 4byte
                {
                    nItemOff = (nAddress - db->startAddress())*4;
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress ;
                    }
                }
                else
                {
                    if(nMaxAddress<nAddress)
                    {
                        nMaxAddress = nAddress +1;
                    }
                }
                quint32 tempValue = (quint32) nsVar->originalValue().toUInt();
                dataDatabuffer[nItemOff] = (unsigned char) (tempValue&0xFF);
                dataDatabuffer[nItemOff+1] =(unsigned char)((tempValue>>8)&0xFF);
                dataDatabuffer[nItemOff+2] = (unsigned char) ((tempValue>>16)&0xFF);
                dataDatabuffer[nItemOff+3] =(unsigned char)((tempValue>>24)&0xFF);
            }
                break;
            case MDDataType::Float:
            {
                if(db->registerType() != ID_MEM_D)
                {
                     if(dataDatabuffer != NULL)
                     {
                         delete dataDatabuffer;
                         dataDatabuffer = NULL;
                     }
                    return bRet;
                }
                if(nMaxAddress<nAddress)
                {
                    nMaxAddress = nAddress +1;
                }

                float tempValue = (float) nsVar->originalValue().toFloat();
                unsigned char *ptempValue = (unsigned char*)&tempValue;
                dataDatabuffer[nItemOff] = *ptempValue;
                dataDatabuffer[nItemOff+1] =*(ptempValue+1);
                dataDatabuffer[nItemOff+2] = *(ptempValue+2);
                dataDatabuffer[nItemOff+3] =*(ptempValue+3);
            }
                break;
            default:
                 if(dataDatabuffer != NULL)
                 {
                     delete dataDatabuffer;
                     dataDatabuffer = NULL;
                 }
                return bRet;
            }
        }
        QT_CATCH (...)
        {
            continue;
        }
    }
    if (nMaxAddress < nMinAddress)
    {
         if(dataDatabuffer != NULL)
         {
             delete dataDatabuffer;
             dataDatabuffer = NULL;
         }
        return bRet;
    }
    QString str;
    uint m_nLen,nItemOff1;
    m_sendData[0] = 0x02;

    str = db->cmdWrite();

    switch(db->registerType())
    {
    case ID_MEM_Y:
        m_nTemp1 = (nMinAddress/100)* 8 + nMinAddress%100/10;
        m_nTemp2 = (nMaxAddress/100)* 8 + nMaxAddress%100/10;
        nAddress = m_nTemp1 + db->offsetAddr();

        m_nLen  = m_nTemp2 - m_nTemp1 +1;

        nItemOff1 = (db->startAddress()/100)* 8 + db->startAddress()%100/10;
        nItemOff = m_nTemp1- nItemOff1;
        break;

    case ID_MEM_M:
        m_nTemp1 = nMinAddress/8;
        m_nTemp2 = nMaxAddress/8;
        m_nLen = m_nTemp2 - m_nTemp1 +1;

        nItemOff1 = db->startAddress()/8;
        nItemOff = m_nTemp1- nItemOff1;
        if (nMinAddress <8000)
        {
            m_nTemp1 = nMinAddress/8;
        }
        else
        {
            m_nTemp1 = (nMinAddress-8000)/8;
        }
        nAddress = m_nTemp1 + db->offsetAddr();
        break;
    case ID_MEM_S:
        nItemOff1 = db->startAddress()/ 8;

        m_nTemp1 = nMinAddress/8;
        m_nTemp2 = nMaxAddress/8;
        m_nLen = m_nTemp2 - m_nTemp1 +1;

        nAddress = m_nTemp1 + db->offsetAddr();
        nItemOff = m_nTemp1- nItemOff1;
        break;
    case ID_MEM_C:
        if (db->startAddress() >=200)
        {
            nAddress = (nMinAddress-200) * 4 + db->offsetAddr();
            m_nLen = (nMaxAddress- nMinAddress+1) *4;
            nItemOff = (nMinAddress- db->startAddress())*4;
        }
        else
        {
            nAddress = nMinAddress * 2 + db->offsetAddr();
            m_nLen = (nMaxAddress- nMinAddress+1) *2;
            nItemOff = (nMinAddress- db->startAddress())*2;
        }
        break;
    case ID_MEM_T:
    case ID_MEM_D:
        if (db->startAddress() >= 8000)
        {
            nAddress = (nMinAddress-8000) * 2 + db->offsetAddr();
        }
        else
        {
            nAddress = nMinAddress * 2 + db->offsetAddr();
        }
        m_nLen = (nMaxAddress- nMinAddress+1) *2;
        nItemOff = (nMinAddress- db->startAddress())*2;
        break;

    default:
         if(dataDatabuffer != NULL)
         {
             delete dataDatabuffer;
             dataDatabuffer = NULL;
         }

        return bRet;
    }

    strtemp.sprintf("%.4X",nAddress);
    str += strtemp;

    strtemp.sprintf("%.2X",m_nLen);
    str += strtemp;

    for (int i = 0;i<m_nLen ;i++)
    {
        strtemp.sprintf("%.2X",dataDatabuffer[nItemOff + i]);
        str += strtemp;
    }
    int nBCCDataLength = str.size();

    QByteArray byteArrayCmdRead = str.toLatin1();
    char * chCmdRead = byteArrayCmdRead.data();

    memcpy(&m_sendData[1],chCmdRead,nBCCDataLength);

    m_sendData[nBCCDataLength+1] = 0x03;

    nBCCDataLength = nBCCDataLength + 1;

    unsigned char uchBCC = BCC(nBCCDataLength,m_sendData);

    strtemp.sprintf("%.2X",uchBCC);

    QByteArray byteArrayBCC = strtemp.toLatin1();

    char * chBCC = byteArrayBCC.data();

    memcpy(&m_sendData[nBCCDataLength+1],chBCC,2);
    nBCCDataLength = nBCCDataLength + 3;

    setCommandLength(nBCCDataLength);
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
    if (getCommandType() == ProtocolCommandType::Read)
    {
        if(getReceiveDataLength()<4)
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
        if(m_receiveData[getReceiveDataLength()-3] != 0x03)
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
        else
        {
            if(m_receiveData[0] !=0x02)
            {
                if(m_receiveData[1] == 0x02)
                {
                    m_bRemovBagHeadFlag = 1;
                    unsigned char uchBCC = BCC(getReceiveDataLength()-4,&m_receiveData[1]);

                    char chTempData[3] ;
                    memset(chTempData,0,3);
                    memcpy(chTempData,&m_receiveData[getReceiveDataLength()-2],2);
                    QString strTempData(chTempData);
                    bool ok;

                    unsigned char uchBCCR = (unsigned char)strTempData.toUInt(&ok,16);

                    if((uchBCCR!=uchBCC) || (m_readCmdDataLength!=(getReceiveDataLength()-5)/2))
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
                    setReceiveDataLength(0);
                    return getCommandResult();
                }
            }
            else
            {
                unsigned char uchBCC = BCC(getReceiveDataLength()-3,&m_receiveData[0]);

                char chTempData[3] ;
                memset(chTempData,0,3);
                memcpy(chTempData,&m_receiveData[getReceiveDataLength()-2],2);
                QString strTempData(chTempData);
                bool ok;

                unsigned char uchBCCR = (unsigned char)strTempData.toUInt(&ok,16);

                if((uchBCCR!=uchBCC) || (m_readCmdDataLength!=(getReceiveDataLength()-4)/2))
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
        }
    }
    else
    {
        if(getReceiveDataLength()<1)
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
        setReceiveDataLength(0);
        setCommandResult(ProtocolCommandResult::Right);
        return getCommandResult();
    }
    return getCommandResult();
}
unsigned char CMDProtocolCommand::BCC(int nLength,quint8 *pBCC)
{
    unsigned char BCC;
    BCC = *(pBCC+1);
    for(int i=2;i<nLength+1;i++)
    {
        BCC += *(pBCC+i)	;
    }
    return BCC;
}
bool CMDProtocolCommand::bagHeadFlag()
{
    return m_bRemovBagHeadFlag;
}
void CMDProtocolCommand::setbagHeadFlag(bool baghead)
{
    m_bRemovBagHeadFlag = baghead;
}

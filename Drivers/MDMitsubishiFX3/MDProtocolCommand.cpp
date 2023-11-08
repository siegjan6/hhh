#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
struRegisterArea StruRegisterArea[MAX_NUMBER_REGISTER] = { {0,"X","BR","BW"},
                                            {1,"Y","BR","BW"},
                                            {2,"M","BR","BW"},
                                            {3,"S","BR","BW"},
                                            {4,"TS","BR","BW"},
                                            {5,"CS","BR","BW"},
                                            {6,"TN","WR","WW"},
                                            {7,"CN","WR","WW"},
                                            {8,"D","WR","WW"},
                                            {9,"R","QR","QW"}
                                            };
struRegisterArea*GetRegisterStruct(int iIndex)
{
    if((iIndex >=0)&&(iIndex < MAX_NUMBER_REGISTER))
        return &StruRegisterArea[iIndex];
    else
        return NULL;
}

CMDProtocolCommand::CMDProtocolCommand()
{
     m_readCmdDataLength = 0;
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
    QString strTemp = "";
    QString strReadCom = "";

    m_sendData[0] = 0x05;
    strTemp.sprintf("%.2d",dev->station());
    strReadCom = strReadCom +strTemp;

    strReadCom = strReadCom + "FF";

    if(db->registerType()>MAX_NUMBER_REGISTER
            ||db->registerType()<0)
    {
        return bRet;
    }

    strTemp=GetRegisterStruct(db->registerType())->m_strRead;
    strReadCom = strReadCom +strTemp;

    strTemp = QString::number(dev->comOverTimer(),16);
    strReadCom = strReadCom +strTemp;

    strTemp=GetRegisterStruct(db->registerType())->m_strName;
    strReadCom = strReadCom +strTemp;
    quint16 nLength = 0;
    if(db->registerType()==ID_MEM_R)
    {
        nLength = 7 - strTemp.size();
    }
    else
    {
        nLength = 5 - strTemp.size();
    }
    if(db->registerType() == ID_MEM_X
            ||db->registerType() == ID_MEM_Y)
    {
        strTemp.sprintf("%.*o",nLength,db->startAddress());
    }
    else
    {
        strTemp.sprintf("%.*d",nLength,db->startAddress());
    }

    strReadCom = strReadCom +strTemp;

    m_readCmdDataLength = 6;
    nLength=db->endAddress()-db->startAddress()+1;
    if (db->registerType()<=5)
    {
        m_readCmdDataLength+=nLength*1;
    }
    else if (db->registerType()>5)
    {
        if (db->registerType()==ID_MEM_CN && db->startAddress()>=200)
        {
            m_readCmdDataLength+=nLength*8;
        }
        else
        {
            m_readCmdDataLength+=nLength*4;
        }
    }
    if (nLength==256)
    {
        nLength=0;
    }
    strTemp.sprintf("%.2X",nLength);
    strReadCom = strReadCom +strTemp;

    if(db->registerType()==ID_MEM_R)
    {
        nLength=17;
    }
    else
    {
        nLength=15;
    }
    QByteArray byteArrayCmdRead = strReadCom.toLatin1();
    char * chCmdRead = byteArrayCmdRead.data();
    memcpy(&m_sendData[1],chCmdRead,nLength-1);
    quint32 chBCC = 0;
    if(dev->checkSum()==1)
    {
        for (int nIndex=1;nIndex<nLength;nIndex++)
        {
            chBCC+=m_sendData[nIndex];
        }

        chBCC&=0xFF;
        strTemp.sprintf("%.2X",chBCC);
        QByteArray byteArrayCheckSum = strTemp.toLatin1();
        char * chCheckSum = byteArrayCheckSum.data();
        memcpy(&m_sendData[nLength],chCheckSum,2);
        m_readCmdDataLength+=2;
        nLength +=2;
    }

    if (1==dev->protocalFormat())
    {
        m_sendData[nLength] = 0x0D;
        m_sendData[nLength+1] = 0x0A;
        m_readCmdDataLength+=2;
        nLength +=2;
    }

    setCommandLength(nLength);
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

    setCommandLength(0);

    QString strWriteCom = "";
    QString strTemp = "";
    m_sendData[0] = 0x05;
    strTemp.sprintf("%.2d",dev->station());
    strWriteCom = strWriteCom +strTemp;

    strWriteCom = strWriteCom + "FF";

    if(db->registerType()>MAX_NUMBER_REGISTER
            ||db->registerType()<0)
    {
        return bRet;
    }

    strTemp=GetRegisterStruct(db->registerType())->m_strWrite;
    strWriteCom = strWriteCom +strTemp;

    strTemp = QString::number(dev->comOverTimer(),16);
    strWriteCom = strWriteCom +strTemp;

    strTemp=GetRegisterStruct(db->registerType())->m_strName;
    strWriteCom = strWriteCom +strTemp;

    quint16 nLength = 0;
    if(db->registerType()==ID_MEM_R)
    {
        nLength = 7 - strTemp.size();
    }
    else
    {
        nLength = 5 - strTemp.size();
    }
    ushort address = 0;
    if (db->registerType() <= 5)
    {
        address = (ushort)(db->startAddress() + nsVar->internalAddress());
    }
    else
    {
        if(db->registerType()==7 && db->startAddress()>=200)
        {
            address = (ushort)(db->startAddress()+ nsVar->internalAddress() / 4);
        }
        else
        {
            address = (ushort)(db->startAddress()+ nsVar->internalAddress() / 2);
        }
    }
    if(db->registerType() == ID_MEM_X
            ||db->registerType() == ID_MEM_Y)
    {
        strTemp.sprintf("%.*o",nLength,address);
    }
    else
    {
        if(nsVar->originalDataType() == MDDataType::String)
        {
            ushort tempAddress = address + 1;
            strTemp.sprintf("%.*d",nLength,tempAddress);
        }
        else
        {
            strTemp.sprintf("%.*d",nLength,address);
        }

    }

    strWriteCom = strWriteCom +strTemp;

    quint16 valuesCount=0;                             //数据字节总数
    if(db->registerType()==9)
    {
        valuesCount=17;
    }
    else
    {
        valuesCount=15;
    }

    switch (nsVar->originalDataType())
    {
    case MDDataType::Bool:
    {
        strTemp.sprintf("%.2X",1);
        strWriteCom = strWriteCom +strTemp;
        if(db->registerType()<=5)
        {
            bool datavalue = (bool)nsVar->originalValue().toBool();
            if(datavalue != false)
            {
                strWriteCom = strWriteCom +"1";
            }
            else
            {
                strWriteCom = strWriteCom +"0";
            }
            valuesCount = valuesCount + 1;
        }
        else
        {
            if(db->registerType() == ID_MEM_CN
                    &&db->startAddress()>=200)
            {
                ulong datavalue = 0;
                if(db->getDataBufferULongValue(nsVar->internalAddress(),datavalue))
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
                strTemp.sprintf("%.8X",datavalue);
                strWriteCom = strWriteCom + strTemp.right(8);
                valuesCount = valuesCount + 8;
            }
            else
            {
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
                strTemp.sprintf("%.4X",datavalue);
                strWriteCom = strWriteCom + strTemp.right(4);
                valuesCount = valuesCount + 4;
            }
        }
    }
        break;
    case MDDataType::Int16:
    {
        if((db->registerType()<=5)||(db->registerType() == ID_MEM_CN&&db->startAddress()>=200))
        {
            return bRet;
        }
        strTemp.sprintf("%.2X",1);
        strWriteCom = strWriteCom +strTemp;
        qint16 datavalue = (qint16)nsVar->originalValue().toInt();

        strTemp.sprintf("%.4X",datavalue);
        strWriteCom = strWriteCom +strTemp.right(4);

        valuesCount = valuesCount + 4;
    }
        break;
    case MDDataType::UInt16:
    {
        if((db->registerType()<=5)||(db->registerType() == ID_MEM_CN&&db->startAddress()>=200))
        {
            return bRet;
        }
        strTemp.sprintf("%.2X",1);
        strWriteCom = strWriteCom +strTemp;
        quint16 datavalue = (quint16)nsVar->originalValue().toUInt();

        strTemp.sprintf("%.4X",datavalue);
        strWriteCom = strWriteCom +strTemp.right(4);

        valuesCount = valuesCount + 4;
    }
        break;
    case MDDataType::Float:
    {
        if(db->registerType()<=5)
        {
            return bRet;
        }
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
            strTemp.sprintf("%.2X",1);
        }
        else
        {
            strTemp.sprintf("%.2X",2);
        }
        strWriteCom = strWriteCom +strTemp;
        float datavalue = (float)nsVar->originalValue().toFloat();
        quint8 tempBuffer[4];
        memcpy(tempBuffer,&datavalue,4);
        quint32 datavalue1 = 0;
        memcpy(&datavalue1,tempBuffer,4);
        strTemp.sprintf("%.8X",datavalue1);
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
            strWriteCom = strWriteCom +strTemp;
        }
        else
        {
            strWriteCom = strWriteCom +strTemp.mid(4,4);
            strWriteCom = strWriteCom +strTemp;
            strWriteCom = strWriteCom +strTemp.mid(0,4);
            strWriteCom = strWriteCom +strTemp;
        }
        valuesCount = valuesCount + 8;
    }
        break;
    case MDDataType::Int32:
    {
        if(db->registerType()<=5)
        {
            return bRet;
        }
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
            strTemp.sprintf("%.2X",1);
        }
        else
        {
            strTemp.sprintf("%.2X",2);
        }
        strWriteCom = strWriteCom +strTemp;
        qint32 datavalue = (qint32)nsVar->originalValue().toInt();

        strTemp.sprintf("%.8X",datavalue);
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
            strWriteCom = strWriteCom +strTemp.right(8);
        }
        else
        {
            strWriteCom = strWriteCom +strTemp.mid(4,4);
            strWriteCom = strWriteCom +strTemp.mid(0,4);
        }
        valuesCount = valuesCount + 8;
    }
        break;
    case MDDataType::UInt32:
    {
        if(db->registerType()<=5)
        {
            return bRet;
        }
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
            strTemp.sprintf("%.2X",1);
        }
        else
        {
            strTemp.sprintf("%.2X",2);
        }
        strWriteCom = strWriteCom +strTemp;
        quint32 datavalue = (quint32)nsVar->originalValue().toUInt();

        strTemp.sprintf("%.8X",datavalue);
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
            strWriteCom = strWriteCom +strTemp.right(8);
        }
        else
        {
            strWriteCom = strWriteCom +strTemp.mid(4,4);
            strWriteCom = strWriteCom +strTemp.mid(0,4);
        }
        valuesCount = valuesCount + 8;
    }
        break;
    case MDDataType::Double:
    {
        if(db->registerType()<=5)
        {
            return bRet;
        }
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
            strTemp.sprintf("%.2X",2);
        }
        else
        {
            strTemp.sprintf("%.2X",4);
        }
        strWriteCom = strWriteCom +strTemp;
        double datavalue = (double)nsVar->originalValue().toDouble();
        quint8 tempBuffer[8];
        memcpy(tempBuffer,&datavalue,8);
        quint32 datavalue1 = 0;
        quint32 datavalue2 = 0;
        memcpy(&datavalue1,tempBuffer,4);
        memcpy(&datavalue2,&tempBuffer[4],4);
        QString strTemp1="";
        strTemp.sprintf("%.8X",datavalue1);
        strTemp1.sprintf("%.8X",datavalue2);
        strTemp = strTemp +strTemp1;
        if(db->registerType() == ID_MEM_CN&&db->startAddress()>=200)
        {
             strWriteCom = strWriteCom +strTemp.mid(0,8);
            strWriteCom = strWriteCom +strTemp.mid(8,8);
       }
        else
        {
            strWriteCom = strWriteCom +strTemp.mid(4,4);
            strWriteCom = strWriteCom +strTemp.mid(0,4);
            strWriteCom = strWriteCom +strTemp.mid(12,4);
            strWriteCom = strWriteCom +strTemp.mid(8,4);
        }
        valuesCount = valuesCount + 16;

    }
        break;
    case MDDataType::String:
    {
        if(db->registerType()!=ID_MEM_D&&db->registerType()!=ID_MEM_R)
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
        strTemp.sprintf("%.2X",nstrWtiteSize/2);
        strWriteCom = strWriteCom +strTemp;
        for(int i = 0 ;i<nstrWtiteSize;i++)
        {
            if(i == (nstrWtiteSize - 2))
            {
                if(bOddOrEven)
                {
                    strWriteCom += "0";
                    strWriteCom += "0";
                }
                else
                {
                    if(bLenghtEqual == 1)
                    {
                        strTemp.sprintf("%.2X",temp[i+1]);
                        strWriteCom += strTemp;
                    }
                    else
                    {
                        strWriteCom += "0";
                        strWriteCom += "0";
                    }
                }
            }
            else if(i == (nstrWtiteSize - 1))
            {
                if(bOddOrEven)
                {
                    strTemp.sprintf("%.2X",temp[i-1]);
                    strWriteCom += strTemp;
                }
                else
                {
                    if(bLenghtEqual == 1)
                    {
                        strTemp.sprintf("%.2X",temp[i-1]);
                        strWriteCom += strTemp;
                    }
                    else
                    {
                        strWriteCom += "0";
                        strWriteCom += "0";
                    }
                }
            }
            else
            {
                if(i%2 == 0)
                {
                    strTemp.sprintf("%.2X",temp[i+1]);
                }
                else
                {
                    strTemp.sprintf("%.2X",temp[i-1]);
                }

                strWriteCom += strTemp;
            }
        }

        valuesCount = valuesCount + nstrWtiteSize*2;
    }
        break;
    default:
        break;
    }
    QByteArray byteArrayCmdWrite = strWriteCom.toLatin1();
    char * chCmdWrite = byteArrayCmdWrite.data();
    memcpy(&m_sendData[1],chCmdWrite,valuesCount-1);
    quint32 chBCC = 0;
    if(dev->checkSum()==1)
    {
        for (int nIndex=1;nIndex<valuesCount;nIndex++)
        {
            chBCC+=m_sendData[nIndex];
        }

        chBCC&=0xFF;
        strTemp.sprintf("%.2X",chBCC);
        QByteArray byteArrayCheckSum = strTemp.toLatin1();
        char * chCheckSum = byteArrayCheckSum.data();
        memcpy(&m_sendData[valuesCount],chCheckSum,2);
        valuesCount +=2;
    }

    if (1==dev->protocalFormat())
    {
        m_sendData[valuesCount] = 0x0D;
        m_sendData[valuesCount+1] = 0x0A;
        valuesCount +=2;
    }
    setCommandLength(valuesCount);

    bRet = true;

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
    if(db->currentPackBlockWrite()==0)
    {
        unsigned char *dataDatabuffer =new unsigned char[db->getDataBufferSize()] ;
        db->getDataBuffer(dataDatabuffer, 0, db->getDataBufferSize());

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

                if (db->registerType() <=5)
                {
                    nAddress = db->startAddress()+ nsVar->internalAddress();
                }
                else
                {
                    nAddress = db->startAddress()+ nsVar->internalAddress() / 2;
                    if(db->registerType() == ID_MEM_CN
                            &&db->startAddress()>=200)
                    {
                        nAddress = db->startAddress()+ nsVar->internalAddress() / 4;
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
                        if (db->registerType() <= 5)  //!Coil Status
                        {
                            nItemOff = nAddress-db->startAddress();
                            if(nsVar->originalValue().toBool() !=false)
                            {
                                dataDatabuffer[nItemOff] = 0x31;
                            }
                            else
                            {
                                dataDatabuffer[nItemOff] = 0x30;
                            }
                        }
                        else
                        {
                            quint32 tempValue=0;
                            unsigned char tempBuffer[4];
                            if(db->registerType() == ID_MEM_CN
                                    &&db->startAddress()>=200)
                            {
                                nItemOff = (nAddress-db->startAddress())*4;
                                tempBuffer[0] = dataDatabuffer[nItemOff+3];
                                tempBuffer[1] = dataDatabuffer[nItemOff+2];
                                tempBuffer[2] = dataDatabuffer[nItemOff+1];
                                tempBuffer[3] = dataDatabuffer[nItemOff];
                                memcpy(&tempValue,tempBuffer,4);
                            }
                            else
                            {
                                nItemOff = (nAddress-db->startAddress())*2;
                                tempBuffer[0] = dataDatabuffer[nItemOff+1];
                                tempBuffer[1] = dataDatabuffer[nItemOff+0];
                                memcpy(&tempValue,tempBuffer,2);
                            }
                            if (nsVar->originalValue().toBool() == true)
                            {
                                tempValue = CBitFuntons::setBit(tempValue, nsVar->internalSubAddress());
                            }
                            else
                            {
                                tempValue = CBitFuntons::clearBit(tempValue, nsVar->internalSubAddress());
                            }
                            if(db->registerType() == ID_MEM_CN
                                    &&db->startAddress()>=200)
                            {
                                dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>24)&0xFF);
                                dataDatabuffer[nItemOff+1] =(unsigned char) ((tempValue>>16)&0xFF);
                                dataDatabuffer[nItemOff+2] =(unsigned char) ((tempValue>>8)&0xFF);
                                dataDatabuffer[nItemOff+3] =(unsigned char) (tempValue&0xFF);
                            }
                            else
                            {
                                dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                                dataDatabuffer[nItemOff+1] =(unsigned char) (tempValue&0xFF);
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
                case MDDataType::Int16:
                    {
                        if (db->registerType() <=5
                                ||(db->registerType() == ID_MEM_CN
                                                     &&db->startAddress()>=200))
                        {
                            if(NULL!=dataDatabuffer)
                            {
                                delete dataDatabuffer;
                            }
                            return bRet;
                        }
                        nItemOff = (nAddress-db->startAddress())*2;
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
                        if (db->registerType() <=5
                                ||(db->registerType() == ID_MEM_CN
                                   &&db->startAddress()>=200))
                        {
                            if(NULL!=dataDatabuffer)
                            {
                                delete dataDatabuffer;
                            }
                            return bRet;
                        }
                        nItemOff = (nAddress-db->startAddress())*2;
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
                        if (db->registerType() <=5)
                        {
                            if(NULL!=dataDatabuffer)
                            {
                                delete dataDatabuffer;
                            }
                            return bRet;
                        }
                        qint32 tempValue=(qint32)(nsVar->originalValue().toInt());
                        if(db->registerType() == ID_MEM_CN
                                &&db->startAddress()>=200)
                        {
                            nItemOff = (nAddress-db->startAddress())*4;
                            dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>24)&0xFF);
                            dataDatabuffer[nItemOff+1] =(unsigned char) ((tempValue>>16)&0xFF);
                            dataDatabuffer[nItemOff+2] = (unsigned char)((tempValue>>8)&0xFF);
                            dataDatabuffer[nItemOff+3] = (unsigned char)(tempValue&0xFF);
                            if(nMinAddress>nAddress)
                            {
                                nMinAddress = nAddress;
                            }
                            if(nMaxAddress<nAddress)
                            {
                                nMaxAddress = nAddress;
                            }
                        }
                        else
                        {
                            nItemOff = (nAddress-db->startAddress())*2;
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
                    }
                    break;
                case MDDataType::UInt32:
                    {
                        if (db->registerType() <=5)
                        {
                            if(NULL!=dataDatabuffer)
                            {
                                delete dataDatabuffer;
                            }
                            return bRet;
                        }
                        quint32 tempValue=(quint32)(nsVar->originalValue().toUInt());
                        if(db->registerType() == ID_MEM_CN
                                &&db->startAddress()>=200)
                        {
                            nItemOff = (nAddress-db->startAddress())*4;
                            dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>24)&0xFF);
                            dataDatabuffer[nItemOff+1] =(unsigned char) ((tempValue>>16)&0xFF);
                            dataDatabuffer[nItemOff+2] = (unsigned char)((tempValue>>8)&0xFF);
                            dataDatabuffer[nItemOff+3] = (unsigned char)(tempValue&0xFF);
                            if(nMinAddress>nAddress)
                            {
                                nMinAddress = nAddress;
                            }
                            if(nMaxAddress<nAddress)
                            {
                                nMaxAddress = nAddress;
                            }
                        }
                        else
                        {
                            nItemOff = (nAddress-db->startAddress())*2;
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
                    }
                    break;
                case MDDataType::Float:
                    {
                        if (db->registerType() <=5)
                        {
                            if(NULL!=dataDatabuffer)
                            {
                                delete dataDatabuffer;
                            }
                            return bRet;
                        }
                        float tempValue=(float)(nsVar->originalValue().toFloat());
                        unsigned char *pTempVale = (unsigned char *)&tempValue;
                        if(db->registerType() == ID_MEM_CN
                                &&db->startAddress()>=200)
                        {
                            nItemOff = (nAddress-db->startAddress())*4;
                            dataDatabuffer[nItemOff] = (unsigned char)(*(pTempVale+3));
                            dataDatabuffer[nItemOff+1] =(unsigned char) (*(pTempVale+2));
                            dataDatabuffer[nItemOff+2] = (unsigned char)(*(pTempVale+1));
                            dataDatabuffer[nItemOff+3] = (unsigned char)(*(pTempVale));
                            if(nMinAddress>nAddress)
                            {
                                nMinAddress = nAddress;
                            }
                            if(nMaxAddress<nAddress)
                            {
                                nMaxAddress = nAddress;
                            }
                        }
                        else
                        {
                            nItemOff = (nAddress-db->startAddress())*2;
                            dataDatabuffer[nItemOff] = (unsigned char)(*(pTempVale+1));
                            dataDatabuffer[nItemOff+1] =(unsigned char) (*pTempVale);
                            dataDatabuffer[nItemOff+2] = (unsigned char)(*(pTempVale+3));
                            dataDatabuffer[nItemOff+3] = (unsigned char)(*(pTempVale+2));
                            if(nMinAddress>nAddress)
                            {
                                nMinAddress = nAddress;
                            }
                            if(nMaxAddress<nAddress+1)
                            {
                                nMaxAddress = nAddress+1;
                            }
                        }
                    }
                    break;
                case MDDataType::Double:
                    {
                        if (db->registerType() <=5)
                        {
                            if(NULL!=dataDatabuffer)
                            {
                                delete dataDatabuffer;
                            }
                            return bRet;
                        }
                        double tempValue=(double)(nsVar->originalValue().toDouble());
                        unsigned char *pTempVale = (unsigned char *)&tempValue;
                        if(db->registerType() == ID_MEM_CN
                                &&db->startAddress()>=200)
                        {
                            nItemOff = (nAddress-db->startAddress())*4;
                            dataDatabuffer[nItemOff] = (unsigned char)(*(pTempVale+3));
                            dataDatabuffer[nItemOff+1] =(unsigned char) (*(pTempVale+2));
                            dataDatabuffer[nItemOff+2] = (unsigned char)(*(pTempVale+1));
                            dataDatabuffer[nItemOff+3] = (unsigned char)(*(pTempVale));
                            dataDatabuffer[nItemOff+4] = (unsigned char)(*(pTempVale+7));
                            dataDatabuffer[nItemOff+5] =(unsigned char) (*(pTempVale+6));
                            dataDatabuffer[nItemOff+6] = (unsigned char)(*(pTempVale+5));
                            dataDatabuffer[nItemOff+7] = (unsigned char)(*(pTempVale+4));
                            if(nMinAddress>nAddress)
                            {
                                nMinAddress = nAddress;
                            }
                            if(nMaxAddress<nAddress+1)
                            {
                                nMaxAddress = nAddress+1;
                            }
                        }
                        else
                        {
                            nItemOff = (nAddress-db->startAddress())*2;
                            dataDatabuffer[nItemOff] = (unsigned char)(*(pTempVale+1));
                            dataDatabuffer[nItemOff+1] =(unsigned char) (*pTempVale);
                            dataDatabuffer[nItemOff+2] = (unsigned char)(*(pTempVale+3));
                            dataDatabuffer[nItemOff+3] = (unsigned char)(*(pTempVale+2));
                            dataDatabuffer[nItemOff+4] = (unsigned char)(*(pTempVale+5));
                            dataDatabuffer[nItemOff+5] = (unsigned char)(*(pTempVale+4));
                            dataDatabuffer[nItemOff+6] = (unsigned char)(*(pTempVale+7));
                            dataDatabuffer[nItemOff+7] = (unsigned char)(*(pTempVale+6));
                            if(nMinAddress>nAddress)
                            {
                                nMinAddress = nAddress;
                            }
                            if(nMaxAddress<nAddress+3)
                            {
                                nMaxAddress = nAddress+3;
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
        int nLength = nMaxAddress - nMinAddress+1;

        if(nMinAddress< db->startAddress()|| nMinAddress> db->endAddress()||
           nLength<=0|| nLength > db->endAddress()-db->startAddress()+1)
        {
            if(NULL!=dataDatabuffer)
            {
                delete dataDatabuffer;
                dataDatabuffer = NULL;
            }
            return bRet;
        }
        int mMaxDotCount=0;
        if (db->registerType()<=5)
        {
            mMaxDotCount=160;
        }
        else if(db->registerType()==ID_MEM_CN && db->startAddress()>=200)
        {
            mMaxDotCount=32;
        }
        else
        {
            mMaxDotCount=64;
        }

        int nByteCount=0;
        int mCircleTimes=0;
        int curOffset=0;
        int nSendLength=0;
        mCircleTimes=(nLength+mMaxDotCount-1)/mMaxDotCount;

        db->setPackBlockWrite(mCircleTimes);

        if (db->registerType()<=5)
        {
            curOffset=nMinAddress-db->startAddress();
            nSendLength=nMaxAddress - db->startAddress()+1;
        }
        else if (db->registerType()==7 && db->startAddress()>=200)
        {
            curOffset=(nMinAddress-db->startAddress())*4;
            nSendLength=(nMaxAddress - db->startAddress()+1)*4;
        }
        else
        {
            curOffset=(nMinAddress-db->startAddress())*2;
            nSendLength=(nMaxAddress - db->startAddress()+1)*2;
        }
        for (int times=0;times<mCircleTimes;times++)
        {
            QString strWriteCom = "";
            QString strTemp = "";
            unsigned char m_sendDataTemp[600];
            memset(m_sendDataTemp,0,600);
            m_sendDataTemp[0] = 0x05;

            strTemp.sprintf("%.2d",dev->station());
            strWriteCom = strWriteCom +strTemp;

            strWriteCom = strWriteCom + "FF";

            if(db->registerType()>MAX_NUMBER_REGISTER
                    ||db->registerType()<0)
            {
                return bRet;
            }

            strTemp=GetRegisterStruct(db->registerType())->m_strWrite;
            strWriteCom = strWriteCom +strTemp;

            strTemp = QString::number(dev->comOverTimer(),16);
            strWriteCom = strWriteCom +strTemp;

            strTemp=GetRegisterStruct(db->registerType())->m_strName;
            strWriteCom = strWriteCom +strTemp;

            quint16 nLengthSoft = 0;
            if(db->registerType()==ID_MEM_R)
            {
                nLengthSoft = 7 - strTemp.size();
            }
            else
            {
                nLengthSoft = 5 - strTemp.size();
            }

            if(db->registerType() == ID_MEM_X
                    ||db->registerType() == ID_MEM_Y)
            {
                strTemp.sprintf("%.*o",nLengthSoft,nMinAddress);
            }
            else
            {
                strTemp.sprintf("%.*d",nLengthSoft,nMinAddress);
            }

            strWriteCom = strWriteCom +strTemp;

            if (times==(mCircleTimes-1))
            {
               strTemp.sprintf("%.2X",nLength);
            }
            else
            {
                strTemp.sprintf("%.2X",mMaxDotCount);
                nMinAddress+=mMaxDotCount;
                nLength-=mMaxDotCount;
            }
            strWriteCom = strWriteCom +strTemp;
            if(db->registerType()==ID_MEM_R)
            {
               nByteCount=17;
               QByteArray byteArrayWriteCom = strWriteCom.toLatin1();
               char * chWriteCom = byteArrayWriteCom.data();
               memcpy(&m_sendDataTemp[1],chWriteCom,16);
            }
            else
            {
                nByteCount=15;
                QByteArray byteArrayWriteCom = strWriteCom.toLatin1();
                char * chWriteCom = byteArrayWriteCom.data();
                memcpy(&m_sendDataTemp[1],chWriteCom,14);
            }
            int n = 0;
            //int tempCurOffset = curOffset;
            while(curOffset<nSendLength)
            {
                if (db->registerType()<=5)
                {
                    memcpy(&m_sendDataTemp[nByteCount],&dataDatabuffer[curOffset],1);
                    nByteCount++;
                    curOffset++;
                }
                else
                {
                    if (db->registerType()==7 && db->startAddress()>=200)
                    {
                        quint32 tempValue = 0;
                        unsigned char tempArray[4];
                        tempArray[3] = dataDatabuffer[curOffset];
                        tempArray[2] = dataDatabuffer[curOffset+1];
                        tempArray[1] = dataDatabuffer[curOffset+2];
                        tempArray[0] = dataDatabuffer[curOffset+3];
                        memcpy(&tempValue,tempArray,4);
                        strTemp.sprintf("%.8X",tempValue);
                        QByteArray byteArrayTempValue = strTemp.toLatin1();
                        char * chTempValue = byteArrayTempValue.data();
                        memcpy(&m_sendDataTemp[nByteCount],chTempValue,8);
                        nByteCount+=8;
                        curOffset+=4;
                    }
                    else
                    {
                        quint16 tempValue = 0;
                        unsigned char tempArray[2];
                        tempArray[1] = dataDatabuffer[curOffset];
                        tempArray[0] = dataDatabuffer[curOffset+1];
                        memcpy(&tempValue,tempArray,4);
                        strTemp.sprintf("%.4X",tempValue);
                        QByteArray byteArrayTempValue = strTemp.toLatin1();
                        char * chTempValue = byteArrayTempValue.data();
                        memcpy(&m_sendDataTemp[nByteCount],chTempValue,4);
                        nByteCount+=4;
                        curOffset+=2;
                    }
                }

                n++;
                if (n%mMaxDotCount==0)
                {
                    break;
                }
            }
            quint32 chBCC = 0;
            if(dev->checkSum()==1)
            {
                for (int nIndex=1;nIndex<nByteCount;nIndex++)
                {
                    chBCC+=m_sendDataTemp[nIndex];
                }

                chBCC&=0xFF;
                strTemp.sprintf("%.2X",chBCC);
                QByteArray byteArrayCheckSum = strTemp.toLatin1();
                char * chCheckSum = byteArrayCheckSum.data();
                memcpy(&m_sendDataTemp[nByteCount],chCheckSum,2);
                nByteCount +=2;
            }

            if (1==dev->protocalFormat())
            {
                m_sendDataTemp[nByteCount] = 0x0D;
                m_sendDataTemp[nByteCount+1] = 0x0A;
                nByteCount +=2;
            }
            db->setCurrentPackLength(nByteCount,times);
            db->cpyTempDBWriteBuffer(m_sendDataTemp,nByteCount,times);
        }
        db->setCurrentPackBlockWrite(1);
        if(NULL!=dataDatabuffer)
        {
            delete dataDatabuffer;
            dataDatabuffer = NULL;
        }
    }
    int nLengthCurrentPack = db->currentPackLength(db->currentPackBlockWrite()-1);
    quint8 *pSendData = db->tempDBWtiteBuffer(db->currentPackBlockWrite()-1);
    memcpy(m_sendData,pSendData,nLengthCurrentPack);
    setCommandLength(nLengthCurrentPack);
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
        if(getReceiveDataLength()<1)
        {
            setCommandResult(ProtocolCommandResult::SplitData);
            return getCommandResult();
        }
        if(m_receiveData[0]==0x02)
        {
            if (getReceiveDataLength() > getReceiveBufferSize() ||
                getReceiveDataLength() > m_readCmdDataLength)
            {
                setReceiveDataLength(0);
                return getCommandResult();
            }
            else if (getReceiveDataLength() < m_readCmdDataLength)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            else
            {
                if(dev->protocalFormat()==0)
                {
                    m_sendData[0] = 0x06;
                    QString strTemp = "";
                    strTemp.sprintf("%.2d",dev->station());
                    QByteArray byteArrayStation = strTemp.toLatin1();
                    char * chStation = byteArrayStation.data();
                    memcpy(&m_sendData[1],chStation,2);
                    m_sendData[3] = 0x46;
                    m_sendData[4] = 0x46;

                    setCommandLength(5);
                    if(dev->checkSum() == 1)
                    {
                        if(m_receiveData[getReceiveDataLength()-3] != 0x03)
                        {//wrong
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);
                            return getCommandResult();
                        }
                        else
                        {
                            QString strTempCompare = "";
                            quint32 chBCC = 0;
                            for (int nIndex=1;nIndex<=(getReceiveDataLength()-3);nIndex++)
                            {
                                chBCC+=m_receiveData[nIndex];
                            }
                            chBCC&=0xFF;
                            strTempCompare.sprintf("%.2X",chBCC);

                            char cTempCompare[2];
                            memcpy(cTempCompare,&m_receiveData[getReceiveDataLength()-2],2);
                            QString strTempCompare1(cTempCompare);
                            QString strTempCompare2 = strTempCompare1.mid(0,2);
                            if(strTempCompare != strTempCompare2)
                            {//wrong
                                setReceiveDataLength(0);
                                setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);
                                return getCommandResult();
                            }
                            else
                            {//right
                                setReceiveDataLength(0);
                                setCommandResult(ProtocolCommandResult::RightAndWithoutRet);
                                return getCommandResult();
                            }
                        }
                    }
                    else
                    {
                        if(m_receiveData[getReceiveDataLength()-1] != 0x03)
                        {//wrong
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);

                            return getCommandResult();
                        }
                        else
                        {//right
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::RightAndWithoutRet);
                            return getCommandResult();
                        }
                    }
                }
                else
                {

                    m_sendData[0] = 0x06;
                    QString strTemp = "";
                    strTemp.sprintf("%.2d",dev->station());
                    QByteArray byteArrayStation = strTemp.toLatin1();
                    char * chStation = byteArrayStation.data();
                    memcpy(&m_sendData[1],chStation,2);
                    m_sendData[3] = 0x46;
                    m_sendData[4] = 0x46;
                    m_sendData[5] = 0x0D;
                    m_sendData[6] = 0x0A;

                    setCommandLength(7);
                    if(dev->checkSum() == 1)
                    {
                        if(m_receiveData[getReceiveDataLength()-5] != 0x03
                                ||m_receiveData[getReceiveDataLength()-2] != 0x0D
                                    ||m_receiveData[getReceiveDataLength()-1] != 0x0A)
                        {//wrong
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);
                            return getCommandResult();
                        }
                        else
                        {
                            QString strTempCompare = "";
                            quint32 chBCC = 0;
                            for (int nIndex=1;nIndex<=(getReceiveDataLength()-5);nIndex++)
                            {
                                chBCC+=m_receiveData[nIndex];
                            }
                            chBCC&=0xFF;
                            strTempCompare.sprintf("%.2X",chBCC);

                            char cTempCompare[2] ;
                            memcpy(cTempCompare,&m_receiveData[getReceiveDataLength()-4],2);

                            QString strTempCompare1(cTempCompare);
                            QString strTempCompare2 = strTempCompare1.mid(0,2);
                            if(strTempCompare != strTempCompare2)
                            {//wrong
                                setReceiveDataLength(0);
                                setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);
                                return getCommandResult();
                            }
                            else
                            {//right
                                setReceiveDataLength(0);
                                setCommandResult(ProtocolCommandResult::RightAndWithoutRet);
                                return getCommandResult();
                            }
                        }
                    }
                    else
                    {
                        if(m_receiveData[getReceiveDataLength()-5] != 0x03
                                ||m_receiveData[getReceiveDataLength()-2] != 0x0D
                                    ||m_receiveData[getReceiveDataLength()-1] != 0x0A)
                        {//wrong
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);
                            return getCommandResult();
                        }
                        else
                        {//right
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::RightAndWithoutRet);
                            return getCommandResult();
                        }
                    }
                }
            }
        }
        else
        {
            if(dev->protocalFormat()==0)
            {
                if(getReceiveDataLength()<7)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
                else
                {
                    m_sendData[0] = 0x15;
                    QString strTemp = "";
                    strTemp.sprintf("%.2d",dev->station());
                    QByteArray byteArrayStation = strTemp.toLatin1();
                    char * chStation = byteArrayStation.data();
                    memcpy(&m_sendData[1],chStation,2);
                    m_sendData[3] = 0x46;
                    m_sendData[4] = 0x46;

                    setCommandLength(5);
                    //wrong
                    setReceiveDataLength(0);
                    setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);
                    return getCommandResult();
                }
            }
            else
            {
                if(getReceiveDataLength()<9)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
                else
                {
                    m_sendData[0] = 0x15;
                    QString strTemp = "";
                    strTemp.sprintf("%.2d",dev->station());
                    QByteArray byteArrayStation = strTemp.toLatin1();
                    char * chStation = byteArrayStation.data();
                    memcpy(&m_sendData[1],chStation,2);
                    m_sendData[3] = 0x46;
                    m_sendData[4] = 0x46;
                    m_sendData[5] = 0x0D;
                    m_sendData[6] = 0x0A;

                    setCommandLength(7);
                    //wrong
                    setReceiveDataLength(0);
                    setCommandResult(ProtocolCommandResult::ErrorAndWithoutRet);
                    return getCommandResult();
                }
            }
        }
    }
    else
    {
        if(db->currentPackBlockWrite()==0)
        {
            if(getReceiveDataLength()<1)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            if(m_receiveData[0] == 0x06)
            {
                if(dev->protocalFormat() == 0)
                {
                    if(getReceiveDataLength()<5)
                    {
                        setCommandResult(ProtocolCommandResult::SplitData);
                        return getCommandResult();
                    }
                }
                else
                {
                    if(getReceiveDataLength()<7)
                    {
                        setCommandResult(ProtocolCommandResult::SplitData);
                        return getCommandResult();
                    }
                }
            }
            else
            {
                if(dev->protocalFormat() == 0)
                {
                    if(getReceiveDataLength()<7)
                    {
                        setCommandResult(ProtocolCommandResult::SplitData);
                        return getCommandResult();
                    }
                }
                else
                {
                    if(getReceiveDataLength()<9)
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
        else
        {
            if(getReceiveDataLength()<1)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            if(m_receiveData[0] == 0x06)
            {
                if(dev->protocalFormat() == 0)
                {
                    if(getReceiveDataLength()<5)
                    {
                        setCommandResult(ProtocolCommandResult::SplitData);
                        return getCommandResult();
                    }
                }
                else
                {
                    if(getReceiveDataLength()<7)
                    {
                        setCommandResult(ProtocolCommandResult::SplitData);
                        return getCommandResult();
                    }
                }
            }
            else
            {
                if(dev->protocalFormat() == 0)
                {
                    if(getReceiveDataLength()<7)
                    {
                        setCommandResult(ProtocolCommandResult::SplitData);
                        return getCommandResult();
                    }
                }
                else
                {
                    if(getReceiveDataLength()<9)
                    {
                        setCommandResult(ProtocolCommandResult::SplitData);
                        return getCommandResult();
                    }
                }
            }
        }
        if(db->currentPackBlockWrite() == db->packBlockWrite())
        {
            db->setCurrentPackBlockWrite(0);
            db->setPackBlockWrite(0);
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::Right);
            return getCommandResult();
        }
        else
        {
            uint currentPack = db->currentPackBlockWrite() + 1;
            db->setCurrentPackBlockWrite(currentPack);
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::RightAndNextCmd);
            return getCommandResult();
        }
    }
    return getCommandResult();
}

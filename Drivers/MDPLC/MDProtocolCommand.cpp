#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "MDChannel.h"

#define HEAD_H 0xBD
#define HEAD_L 0xAE

CMDProtocolCommand::CMDProtocolCommand()
{
     m_saveOffset = 0;
     m_readCmdDataLength = 0;
     m_transactionID = 1;
     setReceiveBufferSize(1200);
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
    if(dev->communicationProcessFlag() == PROCESS_GETCONFIG)
    {
        //!帧同步头
        m_sendData[0] = HEAD_H; //0xEB;
        m_sendData[1] = HEAD_L; //0x91;
        //!报文长度低位在前高位在后
        m_sendData[2] = 0x04;
        m_sendData[3] = 0x00;
        //!报文种类低位在前高位在后
        m_sendData[4] = 0xFF;
        m_sendData[5] = 0x00;
        //!访问组低位在前高位在后
        m_sendData[6] = 0x00;
        m_sendData[7] = 0x00;
        //!校验TCP通信无校验
        m_sendData[8] = 0x00;
        m_sendData[9] = 0x00;
        //!报文结束标志
        m_sendData[10] = 0xAA;
        m_sendData[11] = 0x55;
        setCommandLength(12);
        bRet = true;
        return bRet;
    }
    else if(dev->communicationProcessFlag() == PROCESS_INITIAL)
    {
        listNoOrderRegistersConfig listTempInitialRegisters;
        listTempInitialRegisters.clear();
        db->InitialMapPackage(listTempInitialRegisters,db->currentPackage());

        qint16 nLength = 0;
        //!帧同步头
        m_sendData[0] = HEAD_H; //0xEB;
        m_sendData[1] = HEAD_L; //0x91;
        nLength = 2;
        //!软元件个数
        qint16 softComponentsCount = (qint16)listTempInitialRegisters.count();
        memcpy(&m_sendData[2],&softComponentsCount,2);
        nLength = nLength + 2;
        //!报文种类
        if(db->currentPackage()<db->overallPackage()-1)
        {
            if(db->currentPackage() == 0)
            {
                m_sendData[4] = 0x01;
                m_sendData[5] = 0x0;
                nLength = nLength + 2;
            }
            else
            {
                m_sendData[4] = 0x02;
                m_sendData[5] = 0x0;
                nLength = nLength + 2;
            }
        }
        else if(db->currentPackage()==db->overallPackage()-1)
        {
            m_sendData[4] = 0x03;
            m_sendData[5] = 0x0;
            nLength = nLength + 2;
        }
        //!访问组
        m_sendData[6] = 0x0;
        m_sendData[7] = 0x0;
        nLength = nLength + 2;
        //!构造帧序号
        int structuralFrameNo = db->currentPackage();
        memcpy(&m_sendData[8],&structuralFrameNo,4);
        nLength = nLength + 4;
        for(int i=0;i<listTempInitialRegisters.count();i++)
        {
            memcpy(&m_sendData[nLength],&listTempInitialRegisters.at(i)->m_nRegister,2);
            nLength = nLength + 2;
            memcpy(&m_sendData[nLength],&listTempInitialRegisters.at(i)->m_nArrayLength,2);
            nLength = nLength + 2;
            memcpy(&m_sendData[nLength],&listTempInitialRegisters.at(i)->m_nArraySubScript,4);
            nLength = nLength + 4;
        }

        m_sendData[nLength] = 0x0;
        nLength = nLength +1;

        m_sendData[nLength] = 0x0;
        nLength = nLength +1;

        m_sendData[nLength] = 0xAA;
        nLength = nLength +1;

        m_sendData[nLength] = 0x55;
        nLength = nLength +1;

        setCommandLength(nLength);
        bRet = true;
        return bRet;
    }
    else if(dev->communicationProcessFlag() == PROCESS_GETDATA)
    {
        //!帧同步头
        m_sendData[0] = HEAD_H; //0xEB;
        m_sendData[1] = HEAD_L; //0x91;
        //!报文长度低位在前高位在后
        m_sendData[2] = 0x08;
        m_sendData[3] = 0x00;
        //!报文种类低位在前高位在后
        m_sendData[4] = 0x11;
        m_sendData[5] = 0x00;
        //!访问组低位在前高位在后
        m_sendData[6] = 0x00;
        m_sendData[7] = 0x00;
        //!构造帧序号
        int structuralFrameNo = db->currentPackage();
        memcpy(&m_sendData[8],&structuralFrameNo,4);
        //!校验TCP通信无校验
        m_sendData[12] = 0x00;
        m_sendData[13] = 0x00;
        //!报文结束标志
        m_sendData[14] = 0xAA;
        m_sendData[15] = 0x55;
        setCommandLength(16);
        bRet = true;
        return bRet;
    }
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
    setReceiveDataLength(0);

    QStringList addressSplit = nsVar->address().split(".");

    QString strRegisterType=addressSplit[1];
    QStringList registerAndAddress = strRegisterType.split("[");
    if(registerAndAddress[1].right(1) == "]")
        registerAndAddress[1] = registerAndAddress[1].left(registerAndAddress[1].length()-1);

    int nAddressFormat = 0;
    db->verifyRegisterFormat(registerAndAddress[0],nAddressFormat);

    qint32 varAddress = 0;
    db->verifyAddressFormat(registerAndAddress[1],varAddress,nAddressFormat);
    if(registerAndAddress[0] == "X"
            ||registerAndAddress[0] == "T"
                ||registerAndAddress[0] == "C16"
                    ||registerAndAddress[0] == "C32"
                        ||registerAndAddress[0] == "TS"
                            ||registerAndAddress[0] == "CS16"
                                ||registerAndAddress[0] == "CS32")
    {
        return bRet;
    }
    //!帧同步头
    m_sendData[0] = HEAD_H; //0xEB;
    m_sendData[1] = HEAD_L; //0x91;
//    //!报文长度低位在前高位在后
//    m_sendData[2] = 0x08;
//    m_sendData[3] = 0x00;
    //!报文种类低位在前高位在后
    if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
    {
        m_sendData[4] = 0x34;
        m_sendData[5] = 0x00;
    }
    else
    {
        //!报文长度低位在前高位在后
        m_sendData[2] = 0x01;
        m_sendData[3] = 0x00;
        m_sendData[4] = 0x23;
        m_sendData[5] = 0x00;
    }

    //!访问组低位在前高位在后
    m_sendData[6] = 0x00;
    m_sendData[7] = 0x00;
    qint16 nLength = 12;
    if(registerAndAddress[0] == "Y")
    {
        m_sendData[8] = 0x02;
        m_sendData[9] = 0x00;
    }
    else if(registerAndAddress[0] == "S")
    {
        m_sendData[8] = 0x03;
        m_sendData[9] = 0x00;
    }
    else if(registerAndAddress[0] == "M")
    {
        m_sendData[8] = 0x05;
        m_sendData[9] = 0x00;
    }
    else if(registerAndAddress[0] == "SPM")
    {
        m_sendData[8] = 0x06;
        m_sendData[9] = 0x00;
    }
    else if(registerAndAddress[0] == "D")
    {
        m_sendData[8] = 0x0B;
        m_sendData[9] = 0x00;
        m_sendData[10] = 0x00;
        m_sendData[11] = 0x00;
    }
    else if(registerAndAddress[0] == "SPD")
    {
        m_sendData[8] = 0x0C;
        m_sendData[9] = 0x00;
        m_sendData[10] = 0x00;
        m_sendData[11] = 0x00;
    }
    switch (nsVar->originalDataType())
    {
    case MDDataType::Bool:
    {
        if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
        {
            m_sendData[2] = 0x0E;
            m_sendData[3] = 0x00;
            memcpy(&m_sendData[12],&varAddress,4);
            m_sendData[16] = 0x01;
            m_sendData[17] = 0x00;
            m_sendData[18] = 0x00;
            m_sendData[19] = 0x00;
            quint16 tempData = 0;
            if(db->getDataBufferUShortValue((qint16)nsVar->internalAddress(),tempData))
            {
                if(nsVar->originalValue().toBool()!=0)
                {
                    tempData = tempData |(1<<nsVar->internalSubAddress());
                }
                else
                {
                    tempData = tempData &(~(1<<nsVar->internalSubAddress()));
                }
            }
            memcpy(&m_sendData[20],&tempData,2);
            m_sendData[22] = 0x00;
            m_sendData[23] = 0x00;
            m_sendData[24] = 0xAA;
            m_sendData[25] = 0x55;
            nLength = nLength+14;
        }
        else
        {
            nLength = nLength +12;
            if(nsVar->originalValue().toBool()!=0)
            {
                m_sendData[10] = 0x01;
            }
            else
            {
                m_sendData[10] = 0x00;
            }
            m_sendData[11] = (quint8)nsVar->internalSubAddress();
            quint32 arrayScipt = (quint32)(varAddress/32);
            memcpy(&m_sendData[12],&arrayScipt,4);
            m_sendData[16] = 0x00;
            m_sendData[17] = 0x00;
            m_sendData[18] = 0x00;
            m_sendData[19] = 0x00;
            m_sendData[20] = 0x00;
            m_sendData[21] = 0x00;
            m_sendData[22] = 0xAA;
            m_sendData[23] = 0x55;
        }
    }
        break;
    case MDDataType::Int16:
    {
        if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
        {
            m_sendData[2] = 0x0E;
            m_sendData[3] = 0x00;
            memcpy(&m_sendData[12],&varAddress,4);
            m_sendData[16] = 0x01;
            m_sendData[17] = 0x00;
            m_sendData[18] = 0x00;
            m_sendData[19] = 0x00;
            qint16 tempData =(qint16) nsVar->originalValue().toInt();

            memcpy(&m_sendData[20],&tempData,2);
            m_sendData[22] = 0x00;
            m_sendData[23] = 0x00;
            m_sendData[24] = 0xAA;
            m_sendData[25] = 0x55;
            nLength = nLength+14;
        }
        else
        {
            return bRet;
        }

    }
        break;
    case MDDataType::UInt16:
    {
        if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
        {
            m_sendData[2] = 0x0E;
            m_sendData[3] = 0x00;
            memcpy(&m_sendData[12],&varAddress,4);
            m_sendData[16] = 0x01;
            m_sendData[17] = 0x00;
            m_sendData[18] = 0x00;
            m_sendData[19] = 0x00;
            quint16 tempData =(quint16) nsVar->originalValue().toUInt();

            memcpy(&m_sendData[20],&tempData,2);
            m_sendData[22] = 0x00;
            m_sendData[23] = 0x00;
            m_sendData[24] = 0xAA;
            m_sendData[25] = 0x55;
            nLength = nLength+14;
        }
        else
        {
            return bRet;
        }
    }
        break;
    case MDDataType::Int32:
    {
        if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
        {
            m_sendData[2] = 0x10;
            m_sendData[3] = 0x00;
            memcpy(&m_sendData[12],&varAddress,4);
            m_sendData[16] = 0x02;
            m_sendData[17] = 0x00;
            m_sendData[18] = 0x00;
            m_sendData[19] = 0x00;
            qint32 tempData =(qint32) nsVar->originalValue().toInt();

            memcpy(&m_sendData[20],&tempData,4);
            m_sendData[24] = 0x00;
            m_sendData[25] = 0x00;
            m_sendData[26] = 0xAA;
            m_sendData[27] = 0x55;
            nLength = nLength+16;
        }
        else
        {
            return bRet;
        }
    }
        break;
    case MDDataType::Float:
    {
        if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
        {
            m_sendData[2] = 0x10;
            m_sendData[3] = 0x00;
            memcpy(&m_sendData[12],&varAddress,4);
            m_sendData[16] = 0x02;
            m_sendData[17] = 0x00;
            m_sendData[18] = 0x00;
            m_sendData[19] = 0x00;
            float tempData = nsVar->originalValue().toFloat();

            memcpy(&m_sendData[20],&tempData,4);
            m_sendData[24] = 0x00;
            m_sendData[25] = 0x00;
            m_sendData[26] = 0xAA;
            m_sendData[27] = 0x55;
            nLength = nLength+16;
        }
        else
        {
            return bRet;
        }
    }
        break;
    case MDDataType::UInt32:
    {
        if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
        {
            m_sendData[2] = 0x10;
            m_sendData[3] = 0x00;
            memcpy(&m_sendData[12],&varAddress,4);
            m_sendData[16] = 0x02;
            m_sendData[17] = 0x00;
            m_sendData[18] = 0x00;
            m_sendData[19] = 0x00;
            quint32 tempData =(quint32) nsVar->originalValue().toUInt();

            memcpy(&m_sendData[20],&tempData,4);
            m_sendData[24] = 0x00;
            m_sendData[25] = 0x00;
            m_sendData[26] = 0xAA;
            m_sendData[27] = 0x55;
            nLength = nLength+16;
        }
        else
        {
            return bRet;
        }
    }
        break;
    case MDDataType::String:
    {
        if(registerAndAddress[0] != "D"&&registerAndAddress[0] != "SPD")
        {
            return bRet;
        }
        quint16 nRegulationSize = 0;
        if(db->getDataBufferUShortValue(nsVar->internalAddress(), nRegulationSize))
        {
            QString strDataValue = nsVar->originalValue().toString();
            QByteArray tempstring1 = strDataValue.toLatin1();
            char* tempstring2 = tempstring1.data();

            qint32 tempEndAddress = db->endAddressFromVarForStringMap(nsVar->name());
            qint32 nStrLength = (qint32)strDataValue.size();
            qint32 nLengthScope = (tempEndAddress-varAddress)*2;

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
            varAddress = varAddress+1;
            memcpy(&m_sendData[12],&varAddress,4);

            qint32 contiousAddressLength = nStrLength/2;
            memcpy(&m_sendData[16],&contiousAddressLength,4);
            if(bOddOrEven)
            {
                m_sendData[20+nStrLength-1] = 0x00;
                memcpy(&m_sendData[20],tempstring2,nStrLength-1);
            }
            else
            {

                if(bLenghtEqual == 1)
                {
                    memcpy(&m_sendData[20],tempstring2,nStrLength);
                }
                else
                {
                    memcpy(&m_sendData[20],tempstring2,nStrLength-2);
                    m_sendData[20+nStrLength-2] = 0x00;
                    m_sendData[20+nStrLength-1] = 0x00;
                }
            }
            m_sendData[20+nStrLength] = 0x00;
            m_sendData[20+nStrLength+1] = 0x00;
            m_sendData[20+nStrLength+2] = 0xAA;
            m_sendData[20+nStrLength+3] = 0x55;
            qint16 bytesAmont = 12 + nStrLength;
            memcpy(&m_sendData[2],&bytesAmont,2);

            nLength = nStrLength+20+4;
        }
    }
        break;
    default:
        break;
    }
    setCommandLength(nLength);
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

    IMDRTDBVariable *nsVar=NULL;

    quint8 tempBuffer[8200];
    memset(&tempBuffer,0,8200);
    uint nLength = 0;
    quint16 ntotalNumber = 0;
    //!刷新数据
    if(db->currentPackBlockWrite() == 0)
    {
        QMap<qint32,bool> registerY;
        QMap<qint32,bool> registerS;
        QMap<qint32,bool> registerM;
        QMap<qint32,bool> registerSPM;
        QMap<qint32,quint16> registerD;
        QMap<qint32,quint16> registerSPD;

        registerY.clear();
        registerS.clear();
        registerM.clear();
        registerSPM.clear();
        registerD.clear();
        registerSPD.clear();
        quint16 tempDataForDBool = 0;
        quint16 tempDataForSPDBool = 0;
        qint32 lastAddressForD = -1;
        qint32 lastAddressForSPD = -1;
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
                QStringList addressSplit = nsVar->address().split(".");

                QString strRegisterType=addressSplit[1];
                QStringList registerAndAddress = strRegisterType.split("[");
                if(registerAndAddress[1].right(1) == "]")
                    registerAndAddress[1] = registerAndAddress[1].left(registerAndAddress[1].length()-1);
                int nAddressFormat = 0;
                db->verifyRegisterFormat(registerAndAddress[0],nAddressFormat);

                qint32 varAddress = 0;
                db->verifyAddressFormat(registerAndAddress[1],varAddress,nAddressFormat);
                if(registerAndAddress[0] == "X"
                        ||registerAndAddress[0] == "T"
                        ||registerAndAddress[0] == "C16"
                        ||registerAndAddress[0] == "C32"
                        ||registerAndAddress[0] == "TS"
                        ||registerAndAddress[0] == "CS16"
                        ||registerAndAddress[0] == "CS32")
                {
                    continue;
                }
                if(registerAndAddress[0] == "D"||registerAndAddress[0] == "SPD")
                {
                    if(registerAndAddress[0] == "D")
                    {
                        if(nsVar->originalDataType()==MDDataType::Bool&&lastAddressForD!=varAddress)
                        {
                            if(db->getDataBufferUShortValue((qint16)nsVar->internalAddress(),tempDataForDBool))
                            {
                                lastAddressForD = varAddress;
                            }
                        }
                    }
                    else
                    {
                        if(nsVar->originalDataType()==MDDataType::Bool&&lastAddressForSPD!=varAddress)
                        {
                            if(db->getDataBufferUShortValue((qint16)nsVar->internalAddress(),tempDataForSPDBool))
                            {
                                lastAddressForSPD = varAddress;
                            }
                        }
                    }

                    bool typeErrorFlag = 0;
                    switch (nsVar->originalDataType())
                    {
                    case MDDataType::Bool:
                    {
                        if(registerAndAddress[0] == "D")
                        {
                            if(nsVar->originalValue().toBool()!=0)
                            {
                                tempDataForDBool = tempDataForDBool |(1<<nsVar->internalSubAddress());
                            }
                            else
                            {
                                tempDataForDBool = tempDataForDBool &(~(1<<nsVar->internalSubAddress()));
                            }
                            registerD.insert(varAddress,tempDataForDBool);
                        }
                        else if(registerAndAddress[0] == "SPD")
                        {
                            if(nsVar->originalValue().toBool()!=0)
                            {
                                tempDataForSPDBool = tempDataForSPDBool |(1<<nsVar->internalSubAddress());
                            }
                            else
                            {
                                tempDataForSPDBool = tempDataForSPDBool &(~(1<<nsVar->internalSubAddress()));
                            }
                            registerSPD.insert(varAddress,tempDataForSPDBool);
                        }
                    }
                        break;
                    case MDDataType::Int16:
                    {

                        qint16 tempData = (qint16)nsVar->originalValue().toInt();
                        if(registerAndAddress[0] == "D")
                        {
                            registerD.insert(varAddress,(quint16)tempData);
                        }
                        else if(registerAndAddress[0] == "SPD")
                        {
                            registerSPD.insert(varAddress,(quint16)tempData);
                        }

                    }
                        break;
                    case MDDataType::UInt16:
                    {
                        quint16 tempData = (quint16)nsVar->originalValue().toUInt();
                        if(registerAndAddress[0] == "D")
                        {
                            registerD.insert(varAddress,tempData);
                        }
                        else if(registerAndAddress[0] == "SPD")
                        {
                            registerSPD.insert(varAddress,tempData);
                        }
                    }
                        break;
                    case MDDataType::Float:
                    {
                        float tempData = nsVar->originalValue().toFloat();
                        quint16 *dataValue  =(quint16*)&tempData;
                        if(registerAndAddress[0] == "D")
                        {
//                            registerD.insert(varAddress,*(dataValue+1));
//                            registerD.insert(varAddress+1,*(dataValue));
//                            registerD.insert(varAddress+2,*(dataValue+3));
//                            registerD.insert(varAddress+3,*(dataValue+2));
                            quint16 l = *(dataValue+1) + (*(dataValue)>>8)&0xFF;
                            quint16 h = *(dataValue+3) + (*(dataValue+2)>>8)&0xFF;
                            registerD.insert(varAddress,l);
                            registerD.insert(varAddress+1,h);
                        }
                        else if(registerAndAddress[0] == "SPD")
                        {
//                            registerD.insert(varAddress,*(dataValue+1));
//                            registerD.insert(varAddress+1,*(dataValue));
//                            registerD.insert(varAddress+2,*(dataValue+3));
//                            registerD.insert(varAddress+3,*(dataValue+2));
                            quint16 l = *(dataValue+1) + (*(dataValue)>>8)&0xFF;
                            quint16 h = *(dataValue+3) + (*(dataValue+2)>>8)&0xFF;
                            registerSPD.insert(varAddress,l);
                            registerSPD.insert(varAddress+1,h);
                        }
                    }
                        break;
                    case MDDataType::Int32:
                    {
                        qint32 tempData = (qint32)nsVar->originalValue().toInt();
                        if(registerAndAddress[0] == "D")
                        {
                            registerD.insert(varAddress,(quint16)(tempData&0xFFFF));
                            registerD.insert(varAddress+1,(quint16)((tempData>>16)&0xFFFF));
                        }
                        else if(registerAndAddress[0] == "SPD")
                        {
                            registerSPD.insert(varAddress,(quint16)(tempData&0xFFFF));
                            registerSPD.insert(varAddress+1,(quint16)((tempData>>16)&0xFFFF));
                        }
                    }
                        break;
                    case MDDataType::UInt32:
                    {
                        quint32 tempData = (quint32)nsVar->originalValue().toUInt();
                        if(registerAndAddress[0] == "D")
                        {
                            registerD.insert(varAddress,(quint16)(tempData&0xFFFF));
                            registerD.insert(varAddress+1,(quint16)((tempData>>16)&0xFFFF));
                        }
                        else if(registerAndAddress[0] == "SPD")
                        {
                            registerSPD.insert(varAddress,(quint16)(tempData&0xFFFF));
                            registerSPD.insert(varAddress+1,(quint16)((tempData>>16)&0xFFFF));
                        }
                    }
                        break;
                    default:
                        typeErrorFlag = 1;
                        break;
                    }
                    if(typeErrorFlag == 1)
                    {
                        continue;
                    }
                }
                else
                {
                    if(nsVar->originalDataType() != MDDataType::Bool)
                    {
                        continue;
                    }
                    if(registerAndAddress[0] == "Y")
                    {
                        if(nsVar->originalValue().toBool() != 0)
                        {
                            registerY.insert(varAddress,1);
                        }
                        else
                        {
                            registerY.insert(varAddress,0);
                        }

                    }
                    else if(registerAndAddress[0] == "S")
                    {
                        if(nsVar->originalValue().toBool() != 0)
                        {
                            registerS.insert(varAddress,1);
                        }
                        else
                        {
                            registerS.insert(varAddress,0);
                        }
                    }
                    else if(registerAndAddress[0] == "M")
                    {
                        if(nsVar->originalValue().toBool() != 0)
                        {
                            registerM.insert(varAddress,1);
                        }
                        else
                        {
                            registerM.insert(varAddress,0);
                        }
                    }
                    else if(registerAndAddress[0] == "SPM")
                    {
                        if(nsVar->originalValue().toBool() != 0)
                        {
                            registerSPM.insert(varAddress,1);
                        }
                        else
                        {
                            registerSPM.insert(varAddress,0);
                        }
                    }
                }

            }
            QT_CATCH (...)
            {
                continue;
            }
        }
        if(!registerY.isEmpty()||!registerS.isEmpty()||!registerM.isEmpty()||!registerSPM.isEmpty())
        {
            tempBuffer[4]= HEAD_H; //0xEB;
            tempBuffer[5]= HEAD_L; //0x91;
            tempBuffer[8]= 0x31;
            tempBuffer[9]= 0x00;
            tempBuffer[10]= 0x00;
            tempBuffer[11]= 0x00;
            nLength = 12;
            if(!registerY.isEmpty())
            {
                qint32 registerYMinAddress = registerY.firstKey();
                qint32 nArrayIndexY = registerYMinAddress/32;
                //qint32 registerYMaxAddress = registerY.lastKey();
                //quint32 nArrayIndexYMax = registerYMaxAddress/32;
                quint32 nMask = 0;
                quint32 nValue = 0;
                //ntotalNumber = 1;
                for(QMap<qint32,bool>::iterator i =registerY.begin();i!=registerY.end();++i )
                {
                    if(i.key()/32==nArrayIndexY)
                    {
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                    }
                    else
                    {
                        tempBuffer[nLength] = 0x02;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        memcpy(&tempBuffer[nLength],&nArrayIndexY,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nValue,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nMask,4);
                        nLength = nLength + 4;
                        nValue = 0;
                        nMask = 0;
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                        ntotalNumber = ntotalNumber +1;
                        nArrayIndexY = i.key()/32;
                    }
                    if(i ==registerY.end()-1)
                    {
                        if(i.key()/32 == nArrayIndexY)
                        {
                            tempBuffer[nLength] = 0x02;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            memcpy(&tempBuffer[nLength],&nArrayIndexY,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nValue,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nMask,4);
                            nLength = nLength + 4;
                            nValue = 0;
                            nMask = 0;
                            ntotalNumber = ntotalNumber +1;
                        }
                    }
                }
            }

            if(!registerS.isEmpty())
            {
                qint32 registerSMinAddress = registerS.firstKey();
                qint32 nArrayIndexS = registerSMinAddress/32;
                quint32 nMask = 0;
                quint32 nValue = 0;
                //ntotalNumber = 1;
                for(QMap<qint32,bool>::iterator i =registerS.begin();i!=registerS.end();++i )
                {
                    if(i.key()/32==nArrayIndexS)
                    {
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                    }
                    else
                    {
                        tempBuffer[nLength] = 0x03;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        memcpy(&tempBuffer[nLength],&nArrayIndexS,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nValue,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nMask,4);
                        nLength = nLength + 4;
                        nValue = 0;
                        nMask = 0;
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                        ntotalNumber = ntotalNumber +1;
                        nArrayIndexS = i.key()/32;
                    }
                    if(i ==registerS.end()-1)
                    {
                        if(i.key()/32 == nArrayIndexS)
                        {
                            tempBuffer[nLength] = 0x03;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            memcpy(&tempBuffer[nLength],&nArrayIndexS,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nValue,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nMask,4);
                            nLength = nLength + 4;
                            nValue = 0;
                            nMask = 0;
                            ntotalNumber = ntotalNumber +1;
                        }
                    }
                }
            }

            if(!registerM.isEmpty())
            {
                qint32 registerMMinAddress = registerM.firstKey();
                qint32 nArrayIndexM = registerMMinAddress/32;
                quint32 nMask = 0;
                quint32 nValue = 0;
                for(QMap<qint32,bool>::iterator i =registerM.begin();i!=registerM.end();++i )
                {
                    if(i.key()/32==nArrayIndexM)
                    {
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                    }
                    else
                    {
                        tempBuffer[nLength] = 0x05;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        memcpy(&tempBuffer[nLength],&nArrayIndexM,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nValue,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nMask,4);
                        nLength = nLength + 4;
                        nValue = 0;
                        nMask = 0;
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                        ntotalNumber = ntotalNumber +1;
                        nArrayIndexM = i.key()/32;
                    }
                    if(i ==registerM.end()-1)
                    {
                        if(i.key()/32 == nArrayIndexM)
                        {
                            tempBuffer[nLength] = 0x05;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            memcpy(&tempBuffer[nLength],&nArrayIndexM,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nValue,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nMask,4);
                            nLength = nLength + 4;
                            nValue = 0;
                            nMask = 0;
                            ntotalNumber = ntotalNumber +1;
                        }
                    }
                }
            }

            if(!registerSPM.isEmpty())
            {
                qint32 registerSPMMinAddress = registerSPM.firstKey();
                qint32 nArrayIndexSPM = registerSPMMinAddress/32;
                quint32 nMask = 0;
                quint32 nValue = 0;
                for(QMap<qint32,bool>::iterator i =registerSPM.begin();i!=registerSPM.end();++i )
                {
                    if(i.key()/32==nArrayIndexSPM)
                    {
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                    }
                    else
                    {
                        tempBuffer[nLength] = 0x06;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        tempBuffer[nLength] = 0x00;
                        nLength = nLength + 1;
                        memcpy(&tempBuffer[nLength],&nArrayIndexSPM,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nValue,4);
                        nLength = nLength + 4;
                        memcpy(&tempBuffer[nLength],&nMask,4);
                        nLength = nLength + 4;
                        nValue = 0;
                        nMask = 0;
                        if(i.value() == 1)
                        {
                            nValue  = nValue | (1<<(i.key()%32));
                        }
                        nMask = nMask | (1<<(i.key()%32));
                        ntotalNumber = ntotalNumber +1;
                        nArrayIndexSPM = i.key()/32;
                    }
                    if(i ==registerSPM.end()-1)
                    {
                        if(i.key()/32 == nArrayIndexSPM)
                        {
                            tempBuffer[nLength] = 0x06;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            tempBuffer[nLength] = 0x00;
                            nLength = nLength + 1;
                            memcpy(&tempBuffer[nLength],&nArrayIndexSPM,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nValue,4);
                            nLength = nLength + 4;
                            memcpy(&tempBuffer[nLength],&nMask,4);
                            nLength = nLength + 4;
                            nValue = 0;
                            nMask = 0;
                            ntotalNumber = ntotalNumber +1;
                        }
                    }
                }
            }

            memcpy(&tempBuffer[6],&ntotalNumber,2);

            tempBuffer[nLength] = 0x00;
            nLength = nLength +1;
            tempBuffer[nLength] = 0x00;
            nLength = nLength +1;
            tempBuffer[nLength] = 0xAA;
            nLength = nLength +1;
            tempBuffer[nLength] = 0x55;
            nLength = nLength +1;
            memcpy(tempBuffer,&nLength,4);
            db->setPackBlockWrite(1);
            db->cpyTempDBWriteBuffer(tempBuffer,nLength,db->packBlockWrite()-1);
            db->setCurrentPackBlockWrite(1);
        }
        memset(&tempBuffer,0,8200);
        if(!registerD.isEmpty()||!registerSPD.isEmpty())
        {
            tempBuffer[4]= HEAD_H; //0xEB;
            tempBuffer[5]= HEAD_L; //0x91;
            tempBuffer[8]= 0x32;
            tempBuffer[9]= 0x00;
            tempBuffer[10]= 0x00;
            tempBuffer[11]= 0x00;
            nLength = 12;
            if(!registerD.isEmpty())
            {
                for(QMap<qint32,quint16>::iterator i = registerD.begin();i!=registerD.end();++i)
                {
                    tempBuffer[nLength] = 0x0B;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    memcpy(&tempBuffer[nLength],&i.key(),4);
                    nLength = nLength +4;
                    memcpy(&tempBuffer[nLength],&i.value(),2);
                    nLength = nLength +2;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;

                }
            }
            if(!registerSPD.isEmpty())
            {
                for(QMap<qint32,quint16>::iterator i = registerSPD.begin();i!=registerSPD.end();++i)
                {
                    tempBuffer[nLength] = 0x0C;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    memcpy(&tempBuffer[nLength],&i.key(),4);
                    nLength = nLength +4;
                    memcpy(&tempBuffer[nLength],&i.value(),2);
                    nLength = nLength +2;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;
                    tempBuffer[nLength] = 0x00;
                    nLength = nLength +1;

                }
            }
            ntotalNumber = registerD.count() + registerSPD.count();
            memcpy(&tempBuffer[6],&ntotalNumber,2);
            tempBuffer[nLength] = 0x00;
            nLength = nLength +1;
            tempBuffer[nLength] = 0x00;
            nLength = nLength +1;
            tempBuffer[nLength] = 0xAA;
            nLength = nLength +1;
            tempBuffer[nLength] = 0x55;
            nLength = nLength +1;
            memcpy(tempBuffer,&nLength,4);
            db->setPackBlockWrite(db->packBlockWrite()+1);
            db->cpyTempDBWriteBuffer(tempBuffer,nLength,db->packBlockWrite()-1);
            db->setCurrentPackBlockWrite(1);
        }

    }

    quint8 *ptempBuffer = db->tempDBWtiteBuffer(db->currentPackBlockWrite()-1);

    qint32 sendLength= 0;
    memcpy(&sendLength,ptempBuffer,4);
    sendLength = sendLength -4;
    if(sendLength>0)
    {
        memcpy(&m_sendData,ptempBuffer+4,sendLength);
        setCommandLength((qint16)sendLength);
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
        if(dev->communicationProcessFlag() == PROCESS_GETCONFIG)
        {
            if(getReceiveDataLength() == 88)
            {
                if(m_receiveData[0] != HEAD_H //0xEB
                        ||m_receiveData[1] != HEAD_L //0x91
                            ||m_receiveData[getReceiveDataLength()-2]!=0xAA
                                ||m_receiveData[getReceiveDataLength()-1]!=0x55)
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
            else if(getReceiveDataLength() < 88)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            else if(getReceiveDataLength() > 88)
            {
                setReceiveDataLength(0);
                return getCommandResult();
            }
        }
        else if(dev->communicationProcessFlag() == PROCESS_INITIAL)
        {
            if(getReceiveDataLength() == 20)
            {
                if(m_receiveData[0] != HEAD_H //0xEB
                        ||m_receiveData[1] != HEAD_L //0x91
                            ||m_receiveData[getReceiveDataLength()-2]!=0xAA
                                ||m_receiveData[getReceiveDataLength()-1]!=0x55)
                {
                    db->setCurrentPackage(0);
                    setReceiveDataLength(0);
                    return getCommandResult();
                }
                else
                {
                    int tempStructuralResult = 0;
                    memcpy(&tempStructuralResult,&m_receiveData[12],4);
                    if(tempStructuralResult!=0||
                            (m_receiveData[8] != m_sendData[8]||
                                m_receiveData[9] != m_sendData[9]||
                                    m_receiveData[10] != m_sendData[10]||
                                         m_receiveData[11] != m_sendData[11]))
                    {
                        db->setCurrentPackage(0);
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }
                    else
                    {
                        if(m_receiveData[4] == 0x03)
                        {
                            db->setCurrentPackage(0);
                            dev->setCommunicationProcessFlag(PROCESS_GETDATA);
                        }
                        else if(m_receiveData[4] == 0x01||m_receiveData[4] == 0x02)
                        {
                            db->setCurrentPackage(db->currentPackage()+1);
                        }
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                        return getCommandResult();
                    }
                }

            }
            else if(getReceiveDataLength() < 20)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            else if(getReceiveDataLength() > 20)
            {
                db->setCurrentPackage(0);
                setReceiveDataLength(0);
                return getCommandResult();
            }
        }
        else if(dev->communicationProcessFlag() == PROCESS_GETDATA)
        {
            if(getReceiveDataLength() < 12)
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
            if(m_receiveData[0] != HEAD_H //0xEB
                    ||m_receiveData[1] != HEAD_L //0x91
                        ||m_receiveData[4]==0x13
                            ||(m_receiveData[8] != m_sendData[8]
                               ||m_receiveData[9] != m_sendData[9]
                                    || m_receiveData[10] != m_sendData[10]
                                        ||m_receiveData[11] != m_sendData[11]))
            {
                db->setCurrentPackage(0);
                setReceiveDataLength(0);
                return getCommandResult();
            }
            else
            {
                qint16 nDataLength = 0;
                memcpy(&nDataLength,&m_receiveData[2],2);
                if(getReceiveDataLength()<nDataLength+16)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
                else if(getReceiveDataLength()>nDataLength+16)
                {
                    db->setCurrentPackage(0);
                    setReceiveDataLength(0);
                    return getCommandResult();
                }
                else if(getReceiveDataLength()==nDataLength+16)
                {
                    if(m_receiveData[getReceiveDataLength()-2]!=0xAA
                            ||m_receiveData[getReceiveDataLength()-1]!=0x55)
                    {
                        db->setCurrentPackage(0);
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }
                    else
                    {
                        qint32 overallPackage = db->getDataBufferSize()/1024;
                        if((db->getDataBufferSize()%1024)!=0)
                        {
                            overallPackage = overallPackage+1;
                        }
                        if(db->currentPackage()==overallPackage-1)
                        {
                            db->setCurrentPackage(0);
                            memcpy(db->pointerOfDataBuffer()+m_saveOffset,&m_receiveData[12],nDataLength);
                            m_saveOffset = 0;
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::Right);
                            return getCommandResult();
                        }
                        else if(db->currentPackage()<overallPackage-1)
                        {
                            db->setCurrentPackage(db->currentPackage()+1);
                            memcpy(db->pointerOfDataBuffer()+m_saveOffset,&m_receiveData[12],nDataLength);
                            m_saveOffset = m_saveOffset+nDataLength;
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                            return getCommandResult();
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(db->currentPackBlockWrite()==0)
        {
            if(getReceiveDataLength()<16)
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
            if(db->currentPackBlockWrite()<db->packBlockWrite())
            {
                db->setCurrentPackBlockWrite(db->currentPackBlockWrite()+1);
                if(getReceiveDataLength()<16)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }


                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                return getCommandResult();
            }
            else
            {
                db->setCurrentPackBlockWrite(0);
                db->setPackBlockWrite(0);
                if(getReceiveDataLength()<16)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }


                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Right);
                return getCommandResult();
            }
        }

    }
    return getCommandResult();
}


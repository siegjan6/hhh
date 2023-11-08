#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "MDChannel.h"

CMDProtocolCommand::CMDProtocolCommand()
{
     m_readCmdDataLength = 0;
     m_transactionID = 1;
     m_bFirstPack = 0;
     setReceiveBufferSize(4096);
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
    qint16 ver;
    qint32 cmdType;
    if(dev->m_runFlag == 0)
    {
        ver = SECP_PROTOCOL_VER;//!版本号（固定的）
        cmdType = SECP_CMD_RD_TAG;//!命令类型（读位号表命令）
        memcpy(m_sendData, &ver, 2);
        memset(m_sendData+2, 0, 32);//!密码（固定的）
        memcpy(m_sendData+34, &cmdType, 4);
        memcpy(m_sendData+38, &dev->m_tabCurrentNum, 2);//!当前位号表序号
    }
    else
    {
        ver = SECP_PROTOCOL_VER;//!版本号（固定的）
        cmdType = SECP_CMD_RD_DATA;//!命令类型（读数据表命令）
        memcpy(m_sendData, &ver, 2);
        memset(m_sendData+2, 0, 32);//!密码（固定的）
        memcpy(m_sendData+34, &cmdType, 4);
        qint16 currentDataTabNO = db->dataTabNumber();
        memcpy(m_sendData+38, &currentDataTabNO, 2);//!当前数据表序号
    }
    setCommandLength(40);
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
    if(dev->m_runFlag == 0)
    {
        return bRet;
    }
    setCommandLength(0);
    QStringList addressSplit = nsVar->address().split(".");
    if(addressSplit.count()!=4)
    {
        return bRet;
    }
    QString tempName = addressSplit[2];
    tempName.remove(" ");
    qint16 ver;
    qint32 cmdType;
    qint32 sendDataLength = 0;
    if(dev->nameTabMapRun.contains(tempName))
    {
        NameAndPro::iterator varPrope = dev->nameTabMapRun.find(tempName);
        MDVarPrope *pVar = varPrope.value();
        if(db->m_dataBufferVariantMap.contains(pVar->m_tagID))
        {
            ver = SECP_PROTOCOL_VER;//!版本号（固定的）
            cmdType = SECP_CMD_WR_DATA;//!命令类型（写变量命令）
            memcpy(m_sendData,&ver,2);
            memset(m_sendData+2, 0, 32);
            memcpy(m_sendData+34,&cmdType,4);
            memcpy(m_sendData+38,db->m_tag,8);//!标签

            m_sendData[50] = 0x01;
            memcpy(m_sendData+51,&pVar->m_tagID,2);//!变量id
            memcpy(m_sendData+53,&pVar->m_tagTyp,2);//!变量类型
            memcpy(m_sendData+55,&pVar->m_tagLength,2);//!变量长度

            sendDataLength = 57 + pVar->m_tagLength;
            bool bNoTypeFlag = false;
            switch (pVar->m_tagTyp)
            {
            case 1:
            {
                if(nsVar->originalValue().toBool() != false)
                {
                    m_sendData[57] = 0x01;
                }
                else
                {
                    m_sendData[57] = 0x00;
                }
            }
                break;
            case 2:
            case 5:
            {
                quint8 tempByte = (quint8)(nsVar->originalValue().toUInt());
                quint8 *pTempByte = (quint8*)&tempByte;
                memcpy(m_sendData+57,pTempByte,1);
            }
                break;
            case 3:
            case 6:
            {
                ushort tempUshort = (ushort)(nsVar->originalValue().toUInt());
                quint8 *pTempUshort = (quint8*)&tempUshort;
                memcpy(m_sendData+57,pTempUshort,2);
            }
                break;
            case 4:
            case 7:
            {
                uint tempUint = (uint)(nsVar->originalValue().toUInt());
                quint8 *pTempUint = (quint8*)&tempUint;
                memcpy(m_sendData+57,pTempUint,4);
            }
            case 8:
            {
                qint8 tempChar = (qint8)(nsVar->originalValue().toInt());
                quint8 *pTempChar = (quint8*)&tempChar;
                memcpy(m_sendData+57,pTempChar,1);
            }
                break;
            case 9:
            {
                short tempShort = (short)(nsVar->originalValue().toInt());
                quint8 *pTempShort = (quint8*)&tempShort;
                memcpy(m_sendData+57,pTempShort,2);
            }
                break;
            case 10:
            {
                int tempInt = (int)(nsVar->originalValue().toInt());
                quint8 *pTempInt = (quint8*)&tempInt;
                memcpy(m_sendData+57,pTempInt,4);
            }
                break;
            case 11:
            {
                float tempFloat = (float)(nsVar->originalValue().toFloat());
                quint8 *pTempFloat = (quint8*)&tempFloat;
                memcpy(m_sendData+57,pTempFloat,4);
            }
                break;
            case 20:
            {
                QString tempstring= nsVar->originalValue().toString();
                QByteArray tempstring1 = tempstring.toLatin1().toUpper();
                char* tempstring2 = tempstring1.data();
                int stringLength = tempstring.size();
                memcpy(m_sendData+57,tempstring2,stringLength);
                memset(m_sendData+57+stringLength,0, 92-57-stringLength);

                qint16 stringLength1 = (qint16)(stringLength + 2);
                memcpy(m_sendData+55,&stringLength1,2);

                sendDataLength = 92;
            }
                break;
            default:
                bNoTypeFlag = true;
                break;
            }
            if(bNoTypeFlag)
            {
                return bRet;
            }
            memcpy(m_sendData+46,&sendDataLength,4);
            if (sendDataLength > 0)
            {
                bRet = true;
                setCommandLength(sendDataLength);
            }
            return bRet;
        }
        else
        {
            return bRet;
        }
    }
    else
    {
        return bRet;
    }

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
    if(dev->m_runFlag == 0)
    {
        return bRet;
    }
    setCommandLength(0);

    qint16 ver;
    qint32 cmdType;
    qint32 sendDataLength = 0;

    ver = SECP_PROTOCOL_VER;
    cmdType = SECP_CMD_WR_DATA;
    memcpy(m_sendData,&ver,2);
    memset(m_sendData+2, 0, 32);
    memcpy(m_sendData+34,&cmdType,4);
    memcpy(m_sendData+38,db->m_tag,8);

    //m_sendData[50] = 0x01;
    //memcpy(m_sendData+51,&pVar->m_tagID,2);
    //memcpy(m_sendData+53,&pVar->m_tagTyp,2);
    //memcpy(m_sendData+55,&pVar->m_tagLength,2);

    quint8 varCount = getWriteVarCount();
    quint32 dataIndex = 51;
    for(int n = 0 ; n < getWriteVarCount() ; n++)
    {
        IMDRTDBVariable *nsVar = getWriteVarAt(n);
        if(NULL == nsVar)
        {
            varCount --;
            continue;
        }
        QStringList addressSplit = nsVar->address().split(".");
        if(addressSplit.count()!=4)
        {
            varCount --;
            continue;
        }
        QString tempName = addressSplit[2];
        tempName.remove(" ");
        if(dev->nameTabMapRun.contains(tempName))
        {
            NameAndPro::iterator varPrope = dev->nameTabMapRun.find(tempName);
            MDVarPrope *pVar = varPrope.value();
             if(db->m_dataBufferVariantMap.contains(pVar->m_tagID))
             {
                 memcpy(m_sendData+dataIndex,&pVar->m_tagID,2);
                 dataIndex = dataIndex +2;
                 memcpy(m_sendData+dataIndex,&pVar->m_tagTyp,2);
                 dataIndex = dataIndex +2;
                 memcpy(m_sendData+dataIndex,&pVar->m_tagLength,2);
                 dataIndex = dataIndex +2;
                 bool bNoTypeFlag = false;
                 switch (pVar->m_tagTyp)
                 {
                 case 1:
                 {
                     if(nsVar->originalValue().toBool() != false)
                     {
                         m_sendData[dataIndex] = 0x01;
                     }
                     else
                     {
                         m_sendData[dataIndex] = 0x00;
                     }
                     dataIndex = dataIndex +1;
                 }
                     break;
                 case 2:
                 case 5:
                 {
                     quint8 tempByte = (quint8)(nsVar->originalValue().toUInt());
                     quint8 *pTempByte = (quint8*)&tempByte;
                     memcpy(m_sendData+dataIndex,pTempByte,1);
                     dataIndex = dataIndex +1;
                 }
                     break;
                 case 3:
                 case 6:
                 {
                     ushort tempUshort = (ushort)(nsVar->originalValue().toUInt());
                     quint8 *pTempUshort = (quint8*)&tempUshort;
                     memcpy(m_sendData+dataIndex,pTempUshort,2);
                     dataIndex = dataIndex +2;
                 }
                     break;
                 case 4:
                 case 7:
                 {
                     uint tempUint = (uint)(nsVar->originalValue().toUInt());
                     quint8 *pTempUint = (quint8*)&tempUint;
                     memcpy(m_sendData+dataIndex,pTempUint,4);
                     dataIndex = dataIndex +4;
                 }
                 case 8:
                 {
                     qint8 tempChar = (qint8)(nsVar->originalValue().toInt());
                     quint8 *pTempChar = (quint8*)&tempChar;
                     memcpy(m_sendData+dataIndex,pTempChar,1);
                     dataIndex = dataIndex +1;
                 }
                     break;
                 case 9:
                 {
                     short tempShort = (short)(nsVar->originalValue().toInt());
                     quint8 *pTempShort = (quint8*)&tempShort;
                     memcpy(m_sendData+dataIndex,pTempShort,2);
                     dataIndex = dataIndex +2;
                 }
                     break;
                 case 10:
                 {
                     int tempInt = (int)(nsVar->originalValue().toInt());
                     quint8 *pTempInt = (quint8*)&tempInt;
                     memcpy(m_sendData+dataIndex,pTempInt,4);
                     dataIndex = dataIndex +4;
                 }
                     break;
                 case 11:
                 {
                     float tempFloat = (float)(nsVar->originalValue().toFloat());
                     quint8 *pTempFloat = (quint8*)&tempFloat;
                     memcpy(m_sendData+dataIndex,pTempFloat,4);
                     dataIndex = dataIndex +4;
                 }
                     break;
                 case 20:
                 {
                     QString tempstring= nsVar->originalValue().toString();
                     QByteArray tempstring1 = tempstring.toLatin1().toUpper();
                     char* tempstring2 = tempstring1.data();
                     int stringLength = tempstring.size();
                     memcpy(m_sendData+dataIndex,tempstring2,stringLength);
                     memset(m_sendData+dataIndex+stringLength,0, 92-57-stringLength);

                     qint16 stringLength1 = (qint16)(stringLength + 2);
                     memcpy(m_sendData+dataIndex-2,&stringLength1,2);

                     dataIndex = dataIndex + 35 ;
                 }
                     break;
                 default:
                     bNoTypeFlag = true;
                     break;
                 }
                 if(bNoTypeFlag)
                 {
                    dataIndex = dataIndex - 6 ;
                    varCount --;
                    continue;
                 }

             }
             else
             {
                 varCount --;
                 continue;
             }
        }
        else
        {
            varCount --;
            continue;
        }
    }
    m_sendData[50] = varCount;
    memcpy(m_sendData+46,&dataIndex,4);
    if(varCount==0 || dataIndex <=51)
    {
        return bRet;
    }
    setCommandLength(dataIndex);
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
    qint32 currentPackSize = 0;
    if (getCommandType() == ProtocolCommandType::Read)
    {
        if(dev->m_runFlag == 0)
        {
            if(getReceiveDataLength()>16||m_bFirstPack )
            {
                memcpy(&currentPackSize,m_receiveData+12,4);
                if(getReceiveDataLength() <currentPackSize)
                {
                    m_bFirstPack = 1;
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
                else if(getReceiveDataLength() >currentPackSize)
                {
                    m_bFirstPack = 0;
                    setReceiveDataLength(0);
                    setCommandResult(ProtocolCommandResult::Error);
                    return getCommandResult();
                }//!分包处理
                else
                {
                    if(m_receiveData[3] == 0x80)//!发送数据错误返回
                    {
                        m_bFirstPack = 0;
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::Error);
                        return getCommandResult();
                    }
                    else
                    {
                        qint16 ver = (m_receiveData[1]<<8) + m_receiveData[0];
                        if(ver != SECP_PROTOCOL_VER)//!版本不一致
                        {
                            m_bFirstPack = 0;
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::Error);
                            return getCommandResult();
                        }
                        qint16  packTyp = (m_receiveData[3]<<8) + m_receiveData[2];
                        if(packTyp!=SECP_CMD_RD_TAG)//!命令类型判断
                        {
                            m_bFirstPack = 0;
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::Error);
                            return getCommandResult();
                        }
                        dev->m_tabNum = m_receiveData[16];//!位号表总数
                        memcpy(dev->m_tabTag,m_receiveData+4,8);//!保存标签
                        qint16 pos = 24;
                        /*!
                          *\section:
                           * 获取变量属性
                          */
                        while(pos< currentPackSize)//!获取当前位号表变量属性
                        {
                            qint16 tagID = (m_receiveData[pos+1]<<8) + m_receiveData[pos];//!变量id
                            qint16 tagTyp = (m_receiveData[pos+3]<<8) + m_receiveData[pos+2];//!变量类型
                            qint16 dataLen = (m_receiveData[pos+5]<<8) + m_receiveData[pos+4];//!变量长度
                            qint16 nameLen = (m_receiveData[pos+7]<<8) + m_receiveData[pos+6];//!变量名长度
                            if(nameLen > 32 || tagID >= MAX_TAG_NUM)//!判断名字与id的上线值
                            {
                                m_bFirstPack = 0;
                                setReceiveDataLength(0);
                                setCommandResult(ProtocolCommandResult::Error);
                                return getCommandResult();
                            }
                            char  tagName[33];
                            memset(tagName,0,33);
                            memcpy(tagName,m_receiveData+8+pos,nameLen);//!变量名获取
                            QString strTagName(tagName);
                            MDVarPrope *pVarPrope = new MDVarPrope();//!变量属性数据结构
                            pVarPrope->m_tagID = tagID;
                            pVarPrope->m_tagName = strTagName;
                            pVarPrope->m_tagTyp = tagTyp;
                            pVarPrope->m_tagLength = dataLen;
                            dev->idTabMapRun.insert(pVarPrope->m_tagID,pVarPrope);//!插入id--属性map
                            dev->nameTabMapRun.insert(pVarPrope->m_tagName,pVarPrope);//!插入名字--属性map
                            pos = pos+nameLen + 8;
                        }
                    }
                    dev->m_tabCurrentNum++;//!下个位号表号
                    if(dev->m_tabCurrentNum-1 <dev->m_tabNum )//!判断是否位号表扫描结束
                    {
                        m_bFirstPack = 0;
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                        return getCommandResult();
                    }
                    else//!位号表扫描结束
                    {
                        m_bFirstPack = 0;
                        dev->m_runFlag = 1;
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                        return getCommandResult();
                    }
                }
            }
            else
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }

        }
        else
        {
            if(getReceiveDataLength()>16||m_bFirstPack )
            {
                memcpy(&currentPackSize,m_receiveData+12,4);
                if(getReceiveDataLength() <currentPackSize)
                {
                    m_bFirstPack = 1;
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
                else if(getReceiveDataLength() >currentPackSize)
                {
                    m_bFirstPack = 0;
                    setReceiveDataLength(0);
                    setCommandResult(ProtocolCommandResult::Error);
                    return getCommandResult();
                }
                else
                {
                    if(m_receiveData[3] == 0x80)
                    {
                        m_bFirstPack = 0;
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::Error);
                        return getCommandResult();
                    }
                    else
                    {
                        qint16 ver = (m_receiveData[1]<<8) + m_receiveData[0];
                        if(ver != SECP_PROTOCOL_VER)
                        {
                            m_bFirstPack = 0;
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::Error);
                            return getCommandResult();
                        }
                        qint16  packTyp = (m_receiveData[3]<<8) + m_receiveData[2];
                        if(packTyp!=SECP_CMD_RD_DATA)
                        {
                            m_bFirstPack = 0;
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::Error);
                            return getCommandResult();
                        }
                        memcpy(db->m_tag,m_receiveData+4,8);
                        if(0!=memcmp(db->m_tag,dev->m_tabTag,8))//!当驱动运行时plc信息发生变化时  重新进行初始化
                        {
                            dev->m_runFlag = 0;
                            m_bFirstPack = 0;
                            for( IDAndPro ::iterator i=dev->idTabMapRun.begin();i != dev->idTabMapRun.end();++i)
                            {
                               MDVarPrope*pVarPrope = i.value();
                               if(NULL!=pVarPrope)
                               {
                                   delete pVarPrope;
                                   pVarPrope = NULL;
                                }
                            }//!清空map表内存释放
                            tempIDAndVariantMap.clear();
                            db->m_dataBufferVariantMap.clear();
                            dev->idTabMapRun.clear();
                            dev->nameTabMapRun.clear();
                            dev->m_tabCurrentNum = 1;
                            setReceiveDataLength(0);
                            setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                            return getCommandResult();
                        }
                        //tempIDAndVariantMap.clear();
                        qint16 pos = 20;
                        while(pos < currentPackSize)//!获取对应的变量值
                        {
                            qint16 tagID = (m_receiveData[pos+1]<<8) + m_receiveData[pos];
                            if(tagID >= MAX_TAG_NUM)
                            {
                                m_bFirstPack = 0;
                                setReceiveDataLength(0);
                                setCommandResult(ProtocolCommandResult::Error);
                                return getCommandResult();
                             }
                            MDVarPrope *pVarPrope = NULL;
                            if(dev->idTabMapRun.contains(tagID))
                            {
                                IDAndPro::iterator findID = dev->idTabMapRun.find(tagID);
                                pVarPrope = findID.value();
                                qint16 strlength  = 0;
                                if(pVarPrope->m_tagTyp == 20)
                                 {
                                     strlength = (m_receiveData[pos+3]<<8) + m_receiveData[pos+2];
                                 }
                                QVariant tempVariant = 0;
                                switch (pVarPrope->m_tagTyp)
                                {
                                case 1:
                                {
                                    if(m_receiveData[pos+3]>0)
                                    {
                                        tempVariant = 1;
                                    }
                                    else
                                    {
                                        tempVariant = 0;
                                    }
                                }
                                    break;
                                case 8:
                                {
                                    qint8 tempValue;
                                    memcpy(&tempValue,m_receiveData+pos + 3,pVarPrope->m_tagLength);
                                    tempVariant = tempValue;
                                }
                                    break;
                                case 2:
                                case 5:
                                {
                                    quint8 tempValue;
                                    memcpy(&tempValue,m_receiveData+pos + 3,pVarPrope->m_tagLength);
                                    tempVariant = tempValue;
                                }
                                    break;
                                case 9:
                                {
                                    qint16 tempValue;
                                    memcpy(&tempValue,m_receiveData+pos + 3,pVarPrope->m_tagLength);
                                    tempVariant = tempValue;
                                }
                                    break;
                                case 3:
                                case 6:
                                {
                                    quint16 tempValue;
                                    memcpy(&tempValue,m_receiveData+pos + 3,pVarPrope->m_tagLength);
                                    tempVariant = tempValue;
                                }
                                    break;
                                case 10:
                                {
                                    qint32 tempValue;
                                    memcpy(&tempValue,m_receiveData+pos + 3,pVarPrope->m_tagLength);
                                    tempVariant = tempValue;
                                }
                                    break;
                                case 4:
                                case 7:
                                {
                                    quint32 tempValue;
                                    memcpy(&tempValue,m_receiveData+pos + 3,pVarPrope->m_tagLength);
                                    tempVariant = tempValue;
                                }
                                    break;
                                case 11:
                                {
                                    float tempValue;
                                    memcpy(&tempValue,m_receiveData+pos + 3,pVarPrope->m_tagLength);
                                    tempVariant = tempValue;
                                }
                                    break;
                                case 20:
                                {
                                    char *tempValue = new char[strlength];
                                    memcpy(tempValue,m_receiveData+pos + 5,strlength);
                                    QString strTempValue(tempValue);
                                    tempVariant = strTempValue;
                                    delete tempValue;
                                    tempValue = NULL;
                                }
                                    break;
                                default:

                                    break;
                                }
                                tempIDAndVariantMap.insert(tagID,tempVariant);
                                pos = pos + 3 +pVarPrope->m_tagLength;

                            }
                        }
                        m_bFirstPack = 0;
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::Right);
                        return getCommandResult();
                    }
                }
            }
            else
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
        }
    }
    else
    {
        if(m_receiveData[3] == 0x80)
        {
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::Error);
            return getCommandResult();
        }
        else
        {
            qint16 ver = (m_receiveData[1]<<8) + m_receiveData[0];
            if(ver != SECP_PROTOCOL_VER)
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
            qint16  packTyp = (m_receiveData[3]<<8) + m_receiveData[2];
            if(packTyp!=SECP_CMD_WR_DATA)
            {
                setReceiveDataLength(0);
                setCommandResult(ProtocolCommandResult::Error);
                return getCommandResult();
            }
            setReceiveDataLength(0);
            setCommandResult(ProtocolCommandResult::Right);
            return getCommandResult();
        }
    }


    return getCommandResult();
}
quint8* CMDProtocolCommand::tempReceiveBuffer()
{
    return m_tempReceiveBuffer;
}

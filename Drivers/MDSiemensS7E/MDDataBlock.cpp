#include "MDDataBlock.h"
#include <QStringList>
#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "PropertyBrowser.h"

#ifdef Q_OS_WIN
    //#pragma execution_character_set("utf-8")   //解决 VS编译器下中文乱码
#endif

CMDDataBlock::CMDDataBlock()
{
     m_registerType=0;
     m_startAddress=0;
     m_endAddress=0;
     m_dataBufferSize=0;
     m_DBRegisterNum = 0;//PKX
     m_strRegisterType = "0";
     m_strStartAddress = "0";
     m_strEndAddress = "0";
     m_strDBRegisterNum = "1";
     m_dataBuffer=NULL;
     setEnabled(true);
     //*********************属性框变量*********************//
     this->initTranslateNameAndPropertyType();
     m_registerType_Pro = (MDRegister_Pro)0;
     m_startAddress_Pro = "0";
     m_endAddress_Pro = "0";
     m_dbRegisterNum_Pro  = "1";
     //!初始报错标志位
     m_bError = 0;
     //**************************************************//
}
CMDDataBlock::~CMDDataBlock()
{
    if(NULL !=m_dataBuffer)
    {
        delete []m_dataBuffer;
    }
}

bool CMDDataBlock::start()
{
    if(verifyFlag() != FLAG_RIGHT)
    {
        setQuality(MDVariableDataQuality::DataBlockConfigError);
        refreshVariablesQuality();
    }
    return CBaseDataBlock::start();
}

bool CMDDataBlock::stop()
{
    return CBaseDataBlock::stop();
}

void CMDDataBlock::copy(IDataBlock *otherDataBlock)
{
    if(NULL== otherDataBlock)
    {
        return;
    }
    CBaseDataBlock::copy(otherDataBlock);
    this->setStrRegisterType(((CMDDataBlock*)otherDataBlock)->strRegisterType());
    this->setstrDBNumber(((CMDDataBlock*)otherDataBlock)->strDBNumber());
    this->setStrStartAddress(((CMDDataBlock*)otherDataBlock)->strStartAddress());
    this->setStrEndAddress(((CMDDataBlock*)otherDataBlock)->strEndAddress ());
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }
 }

//开始地址
void CMDDataBlock::setStartAddress(qint16 address)
{
    m_startAddress = address;
    if(m_bError == 1)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
        }
    }

}

qint16 CMDDataBlock::getStartAddress()
{
    return m_startAddress;
}

//结束地址
void CMDDataBlock::setEndAddress(qint16 address)
{
    m_endAddress = address;
    if(m_bError == 1)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
        }
    }
}

qint16 CMDDataBlock::getEndAddress()
{
    return m_endAddress;
}
void CMDDataBlock::setRegisterType(qint16 registerType)
{
    m_registerType = registerType;
    m_registerType_Pro = (MDRegister_Pro)m_registerType;//!属性框
    if(m_bError == 1)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::white);
        }
    }
}

qint16 CMDDataBlock::getRegisterType()
{
    return m_registerType;
}
//********************PKX********************//
void CMDDataBlock::setDBRegisterNum(qint16 DBregisterNum)
{
    m_DBRegisterNum = DBregisterNum;
}
qint16 CMDDataBlock::getDBRegisterNum()
{
    return m_DBRegisterNum;
}
void CMDDataBlock::variablesAddressReflect(QStringList &varAddressList)
{
    if(verifyFlag()==FLAG_RIGHT)
    {
        CMDDevice *dev = (CMDDevice*)device();
        S7RegInform* pS7RegInform = GetS7RegInform(m_registerType,dev->PLCType());
        QString str_Register_Name(pS7RegInform->m_strName);
        if(str_Register_Name != "DB")
        {
            varAddressList.append
                    (QString("%1.%2%3---%1.%2%4(*hint:StartAddress---EndAddress)")
                     .arg(name())
                     .arg(str_Register_Name)
                     .arg(m_startAddress)
                     .arg(m_endAddress));
        }
        else
        {
            varAddressList.append
                    (QString("%1.%2[%3]%4---%1.%2[%3]%5(*hint:StartAddress---EndAddress)")
                     .arg(name())
                     .arg(str_Register_Name)
                     .arg(m_DBRegisterNum)
                     .arg(m_startAddress)
                     .arg(m_endAddress));
        }
    }
    else
    {
        QString addressTemp = QString("数据块%1配置错误!").arg(name());
        varAddressList.append(addressTemp);
    }

}
//********************************************//
bool CMDDataBlock::getDataBufferUShortValue(qint16 internalAddress,  quint16 &data)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);

    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }

    if (internalAddress < m_dataBufferSize-1 && internalAddress>0)
    {
        QByteArray tempbuffer;
        tempbuffer[0] = m_dataBuffer[internalAddress + 1];
        tempbuffer[1] = m_dataBuffer[internalAddress];
        data = tempbuffer.toUShort();
        bRet = true;
    }
    return bRet;
}

//获取数据缓冲区
bool CMDDataBlock::getDataBuffer(quint8* dataBuf, qint16 offset, qint16 length)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);
    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }
    memcpy(dataBuf,m_dataBuffer+offset,length);

    bRet = true;
    return bRet;
}

qint16 CMDDataBlock::getDataBufferSize()
{
    return m_dataBufferSize;
}
void CMDDataBlock::calculateDataBufferSize()
{
    qint16 tempDataBufferSize = 0;
    CMDDevice *dev = (CMDDevice*)device();
    if(dev == NULL)
    {
        return;
    }
    if((dev->PLCType() == PLC_1500) || (dev->PLCType() == PLC_1200))
    {
        switch(m_registerType)
        {
        case ID_MEM_C:
        {
            tempDataBufferSize = (m_endAddress -m_startAddress+1)*2;
        }
            break;
        case ID_MEM_T:
        case ID_MEM_HC:
        {
            tempDataBufferSize = (m_endAddress -m_startAddress+1)*4;
        }
            break;

        case ID_MEM_AI:
        case ID_MEM_AQ:
        {
            tempDataBufferSize = (m_endAddress -m_startAddress+1)*2;
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
            tempDataBufferSize = m_endAddress -m_startAddress+1;
        }
            break;
        default:
            break;

        }
    }
    else if((dev->PLCType() == PLC_300) || (dev->PLCType() == PLC_400) )
    {
        switch(m_registerType)
        {
        case ID_MEM_C:
        case ID_MEM_T:
        {
            tempDataBufferSize = (m_endAddress -m_startAddress+1)*2;
        }
            break;

        case ID_MEM_PI:
        case ID_MEM_PQ:
        case ID_MEM_M:
        case ID_MEM_I:
        case ID_MEM_Q:
        case ID_MEM_DB:
        {
            tempDataBufferSize = m_endAddress -m_startAddress+1;
        }
            break;
        default:
            break;

        }
    }
    if(tempDataBufferSize!=m_dataBufferSize)
    {
        if(m_dataBuffer==NULL)
        {
            m_dataBufferSize = tempDataBufferSize;
        }
        else
        {
            delete m_dataBuffer;
            m_dataBuffer = NULL;
            m_dataBufferSize = tempDataBufferSize;
        }
    }
}

bool CMDDataBlock::checkVarAddress(IMDRTDBVariable* nsVar,QString &error)
{
    bool ret = false;
     CMDDevice *dev = (CMDDevice*)device();
    if (NULL == nsVar || NULL ==dev )
    {
        return ret;
    }
    if (nsVar->address().isNull() || nsVar->address().isEmpty())
    {
        error = tr("变量地址不能为空!");
        return ret;
    }
    if(verifyFlag()!=FLAG_RIGHT)
    {
        error = tr("数据块配置错误!");
        return ret;
    }
    QT_TRY
    {
        nsVar->setInternalAddress(-1);
        QStringList addressSplit = nsVar->address().split(".");
        if (addressSplit.count() < 2)
        {
            error = tr("变量地址格式错误!");
            return ret;
        }
        //**************************PKX************************//
        S7RegInform* pS7RegInform = GetS7RegInform(m_registerType,dev->PLCType());
        QString cRegisterName(pS7RegInform->m_strName);

        QString strRegisterType=addressSplit[1];
        strRegisterType.truncate(cRegisterName.size());

        QString strAddress=addressSplit[1];
        strAddress.remove(0,cRegisterName.size());
        QString str_Temp_Address = strAddress;
        if(cRegisterName == "DB")
        {
            int i_Check_Element_Index = str_Temp_Address.indexOf("]");
            if(i_Check_Element_Index == -1)
            {
                error = tr("变量地址中寄存器错误!");
                return ret;
            }
            else
            {
                strAddress =  str_Temp_Address.remove(0,i_Check_Element_Index+1);
            }
        }
        if(cRegisterName != strRegisterType)
        {
            error = tr("变量地址中寄存器错误!");
            return ret;
        }
        else
        {
            bool chackflag = false;
            for(int i = 0;i<strAddress.size() ; i++)
            {
                 QString strCompare = strAddress.mid(i,1);
                 QString strCompared = "1234567890";
                 if(strCompared.indexOf(strCompare) == -1)
                 {
                     chackflag = true;
                     break;
                 }
            }
            if(chackflag)
            {
                error = tr("变量地址格式错误!");
                return ret;
            }
        }
        qint16 address =  strAddress.toUShort();
        if(address <m_startAddress||address>m_endAddress)
        {
            error = tr("变量地址超限");
            return ret;
        }
        switch(m_registerType)
        {
        case ID_MEM_C:
        {
            qint16 internalAddress = (qint16)(address - m_startAddress)*2;
            qint16 internalSubAddress = -1;
            if (addressSplit.count() > 2)
            {
                error = tr("变量地址格式错误!");
                return ret;
            }
            switch (nsVar->originalDataType())
            {
            case MDDataType::Int16:
            case MDDataType::UInt16:
            {
                if ((internalAddress+4) > m_dataBufferSize)
                {
                    error = tr("变量地址超限");
                    internalAddress = -1;
                }
                else
                {
                    ret = true;
                }
            }
                break;
            default:
                error = tr("变量类型错误!");
                break;
            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);
        }
            break;
        case ID_MEM_T:
        {
            if((dev->PLCType()==PLC_300)|| (dev->PLCType() == PLC_400))
            {
                qint16 internalAddress = (qint16)(address - m_startAddress)*2;
                qint16 internalSubAddress = -1;
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                switch (nsVar->originalDataType())
                {
                case MDDataType::Int16:
                case MDDataType::UInt16:
                case MDDataType::Int32:
                case MDDataType::UInt32:
                {
                    if ((internalAddress + 4) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                }
                    break;
                default:
                    error = tr("变量类型错误!");
                    break;
                }
                nsVar->setInternalAddress(internalAddress);
                nsVar->setInternalSubAddress(internalSubAddress);
            }
            else
            {
                qint16 internalAddress = (qint16)(address - m_startAddress)*2;
                qint16 internalSubAddress = -1;
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                switch (nsVar->originalDataType())
                {
                case MDDataType::Int32:
                case MDDataType::UInt32:
                {
                    if ((internalAddress+4) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                }
                    break;
                default:
                    error = tr("变量类型错误!");
                    break;
                }
                nsVar->setInternalAddress(internalAddress);
                nsVar->setInternalSubAddress(internalSubAddress);
            }

        }
            break;
        case ID_MEM_HC:
        {
            qint16 internalAddress = (qint16)(address - m_startAddress);
            qint16 internalSubAddress = -1;
            if (addressSplit.count() > 2)
            {
                error = tr("变量地址格式错误!");
                return ret;
            }
            switch (nsVar->originalDataType())
            {
            case MDDataType::Int32:
            case MDDataType::UInt32:
            {
                if ((internalAddress+4) > m_dataBufferSize)
                {
                    error = tr("变量地址超限");
                    internalAddress = -1;
                }
                else
                {
                    ret = true;
                }
            }
                break;
            default:
                error = tr("变量类型错误!");
                break;

            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);
        }
            break;
        case ID_MEM_AI:
        case ID_MEM_AQ:
        {
            qint16 internalAddress = (qint16)(address - m_startAddress);
            qint16 internalSubAddress = -1;
            if (addressSplit.count() > 2)
            {
                internalSubAddress=(qint16)addressSplit[2].toUInt();
            }
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            case MDDataType::Int16:
            case MDDataType::UInt16:
            {
                    if ((internalAddress*2+2) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                    if (internalSubAddress > 15)
                    {
                        internalSubAddress = 15;
                    }
            }
                break;
            default:
                error = tr("变量类型错误!");
                break;

            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);
         }
            break;
        case ID_MEM_I:
        case ID_MEM_Q:
        case ID_MEM_M:
        case ID_MEM_S:
        case ID_MEM_SM:
        case ID_MEM_DB:
        case ID_MEM_PI:
        case ID_MEM_PQ:
        {
            qint16 internalAddress = (qint16)(address - m_startAddress);
            qint16 internalSubAddress = -1;

            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            case MDDataType::Byte:
            case MDDataType::SByte:
            {
                if (addressSplit.count() > 2)
                {
                    internalSubAddress=(qint16)addressSplit[2].toUInt();
                }

                if ((internalAddress+1) > m_dataBufferSize)
                {
                    error = tr("变量地址超限");
                    internalAddress = -1;
                }
                else
                {
                    ret = true;
                }
                if (internalSubAddress > 7)
                {
                    internalSubAddress = 7;
                }

            }
                break;
            case MDDataType::Int16:
            case MDDataType::UInt16:
            {
                if ((internalAddress+2) > m_dataBufferSize)
                {
                    error = tr("变量地址超限");
                    internalAddress = -1;
                }
                else
                {
                    ret = true;
                }

            }
                break;
            case MDDataType::Int32:
            case MDDataType::UInt32:
            case MDDataType::Float:
            {
                if ((internalAddress+4) > m_dataBufferSize)
                {
                    error = tr("变量地址超限");
                    internalAddress = -1;
                }
                else
                {
                    ret = true;
                }

            }
                break;
            case MDDataType::String:
            {

                if(m_registerType == ID_MEM_DB)
                {
                    if((internalAddress+4) > m_dataBufferSize)
                    {
                        error = tr("变量地址超限");
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                }
            }
                break;

            default:
                error = tr("变量类型错误!");
                break;
            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);

        }
            break;
        default:
            break;

        }
        //*********************************************************//
   }
   QT_CATCH (...)
   {

   }
   return ret;
}

void CMDDataBlock::initialVariables()
{
    QString error;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        checkVarAddress(getVariableAt(varIndex),error) ;
    }
}

void CMDDataBlock::refreshVariables()
{
    if (m_dataBufferSize<=0)
    {
        return;
    }
    //******************PKX*****************//
    CMDDevice *dev = (CMDDevice*)device();
    if(dev == NULL)
    {
        return;
    }
    //**************************************//
    int changeVarCount =0;
    IMDRTDBVariable *lastChangeVar = NULL;
    //!变化变量列表
    QList<IMDRTDBVariable*> changevarList;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        IMDRTDBVariable *nsVar = getVariableAt(varIndex);
        if (NULL == nsVar)
        {
            continue;
        }
        if(nsVar->internalAddress()< 0||nsVar->internalAddress()>=m_dataBufferSize)
        {
            continue;
        }
        QVariant varDataValue;
        bool bRightValue = false;
        bool bDataType = false;//!数据类型标志位
        unsigned char tempBuffer[4];
//********************************PKX*****************************//
        if((dev->PLCType() == PLC_1200)||(dev->PLCType() == PLC_1500))
        {
            switch (m_registerType)
            {
            case ID_MEM_DB:
            case ID_MEM_M:
            case ID_MEM_I:
            case ID_MEM_Q:
            case ID_MEM_S:
            case ID_MEM_SM:
            {
                switch(nsVar->originalDataType())
                {
                case MDDataType::Bool:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,1);
                        quint32 dataValue=CBitFuntons::getBit(tempValue,nsVar->internalSubAddress());
                        if (dataValue > 0)
                        {
                            varDataValue =true;
                        }
                        else
                        {
                            varDataValue = false;
                        }
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Byte:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint8 tempValue=0;
                        memcpy(&tempValue,tempBuffer,1);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::SByte:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                        qint8 tempValue=0;
                        memcpy(&tempValue,tempBuffer,1);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Int16:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                        qint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::UInt16:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Int32:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                        tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                        qint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;

                    }
                    break;
                case MDDataType::UInt32:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                        tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Float:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                        tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                        float tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::String:
                {
                    if(m_registerType == ID_MEM_DB)
                    {
                        char strTempBuffer[1024];
                        for(int i = 0;i<(m_endAddress  - nsVar->internalAddress() -m_startAddress -1);i++)
                        {
                             strTempBuffer[i] = m_dataBuffer[nsVar->internalAddress() + 2 + i];

                        }
                        QString string(strTempBuffer);
                        varDataValue =string;
                        bRightValue = true;
                    }
                }
                    break;
                default:
                    bDataType = true;
                    break;
                }
            }
                break;
            case ID_MEM_AI:
            case ID_MEM_AQ:
            {
                tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                switch(nsVar->originalDataType())
                {
                case MDDataType::Bool:
                    {

                        quint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        quint32 dataValue=CBitFuntons::getBit(tempValue,nsVar->internalSubAddress());
                        if (dataValue > 0)
                        {
                            varDataValue =true;
                        }
                        else
                        {
                            varDataValue = false;
                        }
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Int16:
                    {
                        qint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::UInt16:
                    {
                        quint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                default:
                    bDataType = true;
                    break;
                }
            }
                break;
            case ID_MEM_T:
            case ID_MEM_HC:
            {
                tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                switch (nsVar->originalDataType())
                {
                case MDDataType::Int32:
                    {
                        qint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;

                    }
                    break;
                case MDDataType::UInt32:
                    {
                        quint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                default:
                    bDataType = true;
                    break;
                }
            }
                break;
            case ID_MEM_C:
            {
                tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                switch (nsVar->originalDataType())
                {
                case MDDataType::Int16:
                    {
                        qint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::UInt16:
                    {
                        quint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                default:
                    bDataType = true;
                    break;
                }
            }
                break;
            default:
                break;
            }

        }
        else if((dev->PLCType() == PLC_300)|| (dev->PLCType() == PLC_400))
        {
            switch (m_registerType)
            {
            case ID_MEM_DB:
            case ID_MEM_M:
            case ID_MEM_I:
            case ID_MEM_Q:
            case ID_MEM_PI:
            case ID_MEM_PQ:
            {
                switch(nsVar->originalDataType())
                {
                case MDDataType::Bool:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,1);
                        quint32 dataValue=CBitFuntons::getBit(tempValue,nsVar->internalSubAddress());
                        if (dataValue > 0)
                        {
                            varDataValue =true;
                        }
                        else
                        {
                            varDataValue = false;
                        }
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Byte:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint8 tempValue=0;
                        memcpy(&tempValue,tempBuffer,1);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::SByte:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
                        qint8 tempValue=0;
                        memcpy(&tempValue,tempBuffer,1);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Int16:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                        qint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::UInt16:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Int32:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                        tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                        qint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;

                    }
                    break;
                case MDDataType::UInt32:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                        tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                        quint32 tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::Float:
                    {
                        tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 3];
                        tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 2];
                        tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 1];
                        tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 0];
                        float tempValue=0;
                        memcpy(&tempValue,tempBuffer,4);
                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::String:
                {
                    if(m_registerType == ID_MEM_DB)
                    {
                        char strTempBuffer[1024];
                        for(int i = 0;i<(m_endAddress  - nsVar->internalAddress() -m_startAddress -1);i++)
                        {
                             strTempBuffer[i] = m_dataBuffer[nsVar->internalAddress() + 2 + i];

                        }
                        QString string(strTempBuffer);
                        varDataValue =string;
                        bRightValue = true;
                    }
                }
                    break;
                default:
                    bDataType = true;
                    break;
            }
                break;
            }
            case ID_MEM_T:
            {
                tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                switch (nsVar->originalDataType())
                {

                case MDDataType::Int16:
                {
                    qint16 tempValue=0;
                    memcpy(&tempValue,tempBuffer,2);

                    uint tempValue1 = (uint)tempValue;
                    QString tempValue2;
                    tempValue2 = QString::number(tempValue1,16);
                    tempValue = (qint16)tempValue2.toInt();

                    varDataValue =tempValue;
                    bRightValue = true;
                }
                    break;
                case MDDataType::UInt16:
                {
                    quint16 tempValue=0;
                    memcpy(&tempValue,tempBuffer,2);

                    uint tempValue1 = (uint)tempValue;
                    QString tempValue2;
                    tempValue2 = QString::number(tempValue1,16);
                    tempValue = (quint16)tempValue2.toInt();

                    varDataValue =tempValue;
                    bRightValue = true;
                }
                    break;
                case MDDataType::UInt32:
                {
                    quint32 tempValue=0;
                    int nE = (tempBuffer[1]&0x30)>>4;
                    ulong tempValue1 = (tempBuffer[1]&0x0F)*(0xFF+1)+tempBuffer[0];
                    QString tempValue2;
                    tempValue2 = QString::number(tempValue1,16);
                    tempValue = (quint32)tempValue2.toInt();
                    if(nE == 0)
                    {
                        tempValue = tempValue*10;
                    }
                    else if(nE == 1)
                    {
                        tempValue = tempValue*100;
                    }
                    else if(nE == 2)
                    {
                        tempValue = tempValue*1000;
                    }
                    else if(nE == 3)
                    {
                        tempValue = tempValue*10000;
                    }

                    varDataValue =tempValue;
                    bRightValue = true;
                }
                    break;
                case MDDataType::Int32:
                {

                    qint32 tempValue=0;

                    int nE = (tempBuffer[1]&0x30)>>4;
                    ulong tempValue1 = (tempBuffer[1]&0x0F)*(0xFF+1)+tempBuffer[0];
                    QString tempValue2;
                    tempValue2 = QString::number(tempValue1,16);
                    tempValue = (qint32)tempValue2.toInt();
                    if(nE == 0)
                    {
                        tempValue = tempValue*10;
                    }
                    else if(nE == 1)
                    {
                        tempValue = tempValue*100;
                    }
                    else if(nE == 2)
                    {
                        tempValue = tempValue*1000;
                    }
                    else if(nE == 3)
                    {
                        tempValue = tempValue*10000;
                    }
                    varDataValue =tempValue;
                    bRightValue = true;
                }
                    break;
                default:
                    bDataType = true;
                    break;
                }
            }
                break;
            case ID_MEM_C:
            {
                tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 1];
                tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 0];
                switch (nsVar->originalDataType())
                {
                case MDDataType::Int16:
                    {
                        qint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);

                        uint tempValue1 = (uint)tempValue;
                        QString tempValue2;
                        tempValue2 = QString::number(tempValue1,16);

                        tempValue = (qint16)tempValue2.toInt();

                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                case MDDataType::UInt16:
                    {
                        quint16 tempValue=0;
                        memcpy(&tempValue,tempBuffer,2);

                        uint tempValue1 = (uint)tempValue;
                        QString tempValue2;
                        tempValue2 = QString::number(tempValue1,16);
                        tempValue = (quint16)tempValue2.toInt();

                        varDataValue =tempValue;
                        bRightValue = true;
                    }
                    break;
                default:
                    bDataType = true;
                    break;
                }
            }
                break;
            default:
                break;
            }

        }
        if (true == bRightValue)
        {
           bool bChangeFlag = nsVar->refreshData(varDataValue, timeStamp(),quality(), false);

           if(bChangeFlag)
           {
               lastChangeVar = nsVar;
               changeVarCount++;
               changevarList.append(nsVar);
           }
        }
        else if(bDataType == true)
        {
            nsVar->refreshData(varDataValue, timeStamp(), MDVariableDataQuality::DataTypeError, false);
        }
    }
    if(changeVarCount>0 && lastChangeVar!=NULL)
    {
        lastChangeVar->notifyDeviceVariablesChange(changevarList);
        changevarList.clear();
    }
}
void CMDDataBlock::refreshVariablesQuality()
{
    IMDRTDBVariable *lastChangeVar = NULL;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        IMDRTDBVariable *nsVar = getVariableAt(varIndex);
        if (NULL != nsVar)
        {
            lastChangeVar =nsVar;
            nsVar->refreshData(QVariant(), timeStamp(), quality(), true);
        }
    }
    if(lastChangeVar!=NULL)
    {
        QList<IMDRTDBVariable*> changevarList;
        lastChangeVar->notifyDeviceVariablesChange(changevarList,true);
        changevarList.clear();
    }
}


CBaseProtocolCommand *CMDDataBlock::createCommand(ProtocolCommandType cmdType)
{
    CMDProtocolCommand *cmd = new CMDProtocolCommand();
    cmd->setChannel (getChannel());
    cmd->setDataBlock (this);
    cmd->setDevice(device());
    cmd->setCommandType(cmdType);
    cmd->setCommandResult(ProtocolCommandResult::None);
    return cmd;
}
//分析命令，将数据拷贝到数据区，并更新变量
void CMDDataBlock::parseCommand(CBaseProtocolCommand *cmd)
{
    if(NULL == cmd || m_dataBufferSize<=0)
    {
        return;
    }
    CMDDevice *dev = (CMDDevice*)device();
    if(dev == NULL)
    {
        return;
    }
    if(cmd->getCommandType() == ProtocolCommandType::Read)
    {
        if (cmd->getCommandResult() == ProtocolCommandResult::Right)
        {
           QMutexLocker locker(&m_dataBufferMutex);
           if(m_dataBufferSize>0)
           {
               if (NULL == m_dataBuffer)
               {
                   m_dataBuffer = new quint8[m_dataBufferSize];
               }
           }
           //*************************PKX**********************//
           quint8 ucTempData[1460];
           if((dev->PLCType() == PLC_1200) || (dev->PLCType() == PLC_1500))
           {
               switch(m_registerType)
               {
               case ID_MEM_C:
               {

                   for(int i = 0;i<m_endAddress -m_startAddress+1;i++)
                   {
                       ucTempData[i*2] = *(cmd->getReceiveData()+25+3*(i+1) - 2);
                       ucTempData[i*2+1] = *(cmd->getReceiveData()+25+3*(i+1) - 2+1);
                   }
                   memcpy(m_dataBuffer,ucTempData,m_dataBufferSize);
               }
                   break;
               case ID_MEM_T:
               case ID_MEM_HC:
               {
                   for(int i = 0;i<m_endAddress -m_startAddress+1;i++)
                   {
                       ucTempData[i*4] = *(cmd->getReceiveData()+25+5*(i+1) - 4);
                       ucTempData[i*4+1] = *(cmd->getReceiveData()+25+5*(i+1) - 4+1);
                       ucTempData[i*4+2] = *(cmd->getReceiveData()+25+5*(i+1) - 4+2);
                       ucTempData[i*4+3] = *(cmd->getReceiveData()+25+5*(i+1) - 4+3);
                   }
                   memcpy(m_dataBuffer,ucTempData,m_dataBufferSize);
               }
                   break;
               case ID_MEM_AI:
               case ID_MEM_AQ:
               {
                   memcpy(m_dataBuffer,cmd->getReceiveData()+25,m_dataBufferSize);
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
                   memcpy(m_dataBuffer,cmd->getReceiveData()+25,m_dataBufferSize);
               }
                   break;
               default:
                   break;

               }
           }
           else if((dev->PLCType() == PLC_300)|| (dev->PLCType() == PLC_400))
           {
               switch(m_registerType)
               {
               case ID_MEM_C:
               case ID_MEM_T:
               {
                   memcpy(m_dataBuffer,cmd->getReceiveData()+25,m_dataBufferSize);
               }
                   break;
               case ID_MEM_M:
               case ID_MEM_PI:
               case ID_MEM_PQ:
               case ID_MEM_I:
               case ID_MEM_Q:
               case ID_MEM_DB:
               {
                   memcpy(m_dataBuffer,cmd->getReceiveData()+25,m_dataBufferSize);
               }

                   break;
               default:
                   break;

               }

           }
           this->onReceiveCommand(true, cmd->getCommandResult());
           refreshVariables();
        }
        else
        {
           MDVariableDataQuality oldQuality = quality();
           this->onReceiveCommand(true, cmd->getCommandResult());
           if (oldQuality != quality())
           {
               refreshVariablesQuality();
           }
        }

    }
    else
    {
        /*if (cmd->getCommandResult() == ProtocolCommandResult::Right)
        {
           this->onReceiveCommand(false, true);
        }
        else
        {
           this->onReceiveCommand(false, false);
        }*/
        this->onReceiveCommand(false, cmd->getCommandResult());
    }

}

void CMDDataBlock::saveConfiguration(QDomDocument &doc,QDomElement &parentElement)
{
    if(doc.isNull()||parentElement.isNull())
    {
        return;
    }
    QDomElement variableElement = doc.createElement("DataBlock");
    parentElement.appendChild(variableElement);

    QDomElement element = doc.createElement("Name");
    variableElement.appendChild(element);
    QDomText elementText = doc.createTextNode(this->name());
    element.appendChild(elementText);

    element = doc.createElement("Description");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->description());
    element.appendChild(elementText);

    element = doc.createElement("Enabled");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->isEnabled()?"1":"0");
    element.appendChild(elementText);

    element = doc.createElement("RegisterType");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->strRegisterType());
    element.appendChild(elementText);

    element = doc.createElement("StartAddress");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->strStartAddress());
    element.appendChild(elementText);

    element = doc.createElement("EndAddress");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->strEndAddress());
    element.appendChild(elementText);

    //**************************PKX*************************//
    element = doc.createElement("DBNumber");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->strDBNumber());
    element.appendChild(elementText);

    //******************************************************//

    element = doc.createElement("PollRate");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->pollRate()));
    element.appendChild(elementText);

}
void CMDDataBlock::serialize(CSortJsonObject &json)
{
    json.insertSimpleValue("Name",this->name());
    json.insertSimpleValue("Description",this->description());
    json.insertSimpleValue("Enabled",this->isEnabled()?QString("1"):QString("0"));
    json.insertSimpleValue("RegisterType",this->registerIndexToString(this->strRegisterType()));
    json.insertSimpleValue("DBNumber",this->strDBNumber());
    json.insertSimpleValue("StartAddress",this->strStartAddress());
    json.insertSimpleValue("EndAddress",this->strEndAddress());
    json.insertSimpleValue("PollRate",QString("%1").arg(this->pollRate()));
}

void CMDDataBlock::readConfiguration(QDomElement &domElement)
{
    if(domElement.isNull())
    {
        return;
    }
    QDomElement element = domElement.firstChildElement();
    while ( !element.isNull() )
    {
        if(0==QString::compare(element.tagName(),"Name", Qt::CaseInsensitive))
        {
            this->setName(element.text());
        }
        else if(0==QString::compare(element.tagName(),"Description", Qt::CaseInsensitive))
        {
            this->setDescription(element.text());
        }
        else if(0==QString::compare(element.tagName(),"Enabled", Qt::CaseInsensitive))
        {
            if(element.text().toInt()!=0)
            {
                this->setEnabled(true);
            }
            else
            {
                this->setEnabled(false);
            }
        }
        else if(0==QString::compare(element.tagName(),"StartAddress", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setStrStartAddress(str);
        }
        else if(0==QString::compare(element.tagName(),"EndAddress", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setStrEndAddress(str);
        }
        else if(0==QString::compare(element.tagName(),"RegisterType", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setStrRegisterType(str);
        }
        else if(0==QString::compare(element.tagName(),"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(element.text().toInt());
        }
        //**************************PKX*******************************//
        else if(0==QString::compare(element.tagName(),"DBNumber", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setstrDBNumber(str);
        }
        //*************************************************************//
        element = element.nextSiblingElement();
    }
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }
}
void CMDDataBlock::deserialize(const QJsonObject &json)
{
    this->setName(json.value("Name").toString(""));
    this->setDescription(json.value("Description").toString(""));
    this->setEnabled(json.value("Enabled").toString("1").toInt());
    this->setStrRegisterType(this->registerStringToIndex(json.value("RegisterType").toString("-1")));
    this->setstrDBNumber(json.value("DBNumber").toString("-1"));
    this->setStrStartAddress(json.value("StartAddress").toString("-1"));
    this->setStrEndAddress(json.value("EndAddress").toString("-1"));
    this->setPollRate(json.value("PollRate").toString("1000").toInt());

    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }

}
void CMDDataBlock::dynamicSetParameter(const QVariantMap &dataBlockMap)
{
    QString dataBlockID =dataBlockMap["@DataBlockID"].toString();
    dataBlockID.trimmed();
    if(dataBlockID.isNull()||dataBlockID.isEmpty())
    {
        return;
    }
    this->setName(dataBlockID);

    QVariantList dataBlockParameterList =  dataBlockMap["MDDevicesCommunicateDataBlockParameter"].toList();
    for(int i=0;i<dataBlockParameterList.count();i++)
    {
        QVariantMap commParameterMap = dataBlockParameterList[i].toMap();
        QString parameterName =commParameterMap["@ParameterName"].toString();
        QString parameterValue =commParameterMap["@ParameterValue"].toString();
        parameterName.trimmed();
        parameterValue.trimmed();
        if(0==QString::compare(parameterName,"Description", Qt::CaseInsensitive))
        {
            this->setDescription(parameterValue);
        }
        else if(0==QString::compare(parameterName,"Enabled", Qt::CaseInsensitive))
        {
            if(parameterValue.toInt()!=0)
            {
                this->setEnabled(true);
            }
            else
            {
                if(0==QString::compare(parameterValue,"true", Qt::CaseInsensitive))
                {
                    this->setEnabled(true);
                }
                else
                {
                    this->setEnabled(false);
                }
            }
        }
        else if(0==QString::compare(parameterName,"StartAddress", Qt::CaseInsensitive))
        {
            QString str = parameterName;
            this->setStrStartAddress(str);
        }
        else if(0==QString::compare(parameterName,"EndAddress", Qt::CaseInsensitive))
        {
            QString str = parameterName;
            this->setStrEndAddress(str);
        }
        else if(0==QString::compare(parameterName,"RegisterType", Qt::CaseInsensitive))
        {
            QString str = parameterName;
            this->setStrRegisterType(str);
        }
        else if(0==QString::compare(parameterName,"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(parameterValue.toInt());
        }
        else if(0==QString::compare(parameterName,"DBNumber", Qt::CaseInsensitive))
        {
            QString str = parameterName;
            this->setstrDBNumber(str);
        }
    }
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }

}
int CMDDataBlock::verifyRegisterFormat(QString strRegister,qint16 &nRegister,int &addressFormat,qint16 &nLength)
{
    CMDDevice *dev = (CMDDevice*)device();
    if(dev == NULL)
    {
        return REGISTER_FORMAT_ERROR;
    }
    bool okRegister = false;
    qint16 tempRegister = strRegister.toInt(&okRegister,10);
    if((okRegister==false)||(tempRegister<0||tempRegister>13))
        return REGISTER_FORMAT_ERROR;//!寄存器格式校验错误
    if(dev->PLCType() == PLC_300 || dev->PLCType() == PLC_400)
    {
        if(tempRegister == ID_MEM_PI ||tempRegister == ID_MEM_PQ ||tempRegister == ID_MEM_I || tempRegister == ID_MEM_Q || tempRegister == ID_MEM_M|| tempRegister == ID_MEM_DB)
        {
            nRegister = tempRegister;
            addressFormat = 10;
            nLength = 210;
        }
        else if(tempRegister == ID_MEM_C ||tempRegister == ID_MEM_T)
        {
            nRegister = tempRegister;
            addressFormat = 10;
            nLength = 105;
        }
        else
        {
            return REGISTER_FORMAT_ERROR;
        }
    }
    else if(dev->PLCType() == PLC_1200 || dev->PLCType() == PLC_1500)
    {
        if(tempRegister == ID_MEM_I || tempRegister == ID_MEM_Q || tempRegister == ID_MEM_M|| tempRegister == ID_MEM_DB)
        {
            nRegister = tempRegister;
            addressFormat = 10;
            nLength = 210;
        }
        else
        {
            return REGISTER_FORMAT_ERROR;
        }
    }
    return REGISTER_FORMAT_RIGHT;
}

int CMDDataBlock::verifyAddressFormat(QString strAddress,qint16 &nAddress,qint16 addressFormat)
{
    bool okAddress = false;
    qint16 tempAddress = 0;

    tempAddress = strAddress.toInt(&okAddress,addressFormat);

    if((okAddress == false)||(tempAddress<0))
        return ADDRESS_FORMAT_ERROR;//!地址格式校验错误
    else
        nAddress = tempAddress;
    return ADDRESS_FORMAT_RIGHT;
}
int CMDDataBlock::verifyLength(qint16 startAddress,qint16 endAddress,qint16 nLength)
{
    if(startAddress > endAddress)
    {
        return LENGTH_NEGATIVENUMBER_ERROR;//!开始地址大于结束地址
    }
    else
    {
        qint16 nLengthTemp = endAddress - startAddress+1;
        if(nLengthTemp<=0||nLengthTemp>nLength)
        {
            return LENGTH_OVERRUN_ERROR;//!超过最大长度
        }
    }
    return LENGTH_RIGHT;
}
int CMDDataBlock::verifyDBNumber(QString strNumber,qint16 &nNumber)
{
    bool okDBNumber = false;

    qint16 tempDBNumber = strNumber.toInt(&okDBNumber,10);
    if((okDBNumber==false)||(tempDBNumber<1))
        return DB_NUMBER_ERROR;//!DB块号错误
    else
        nNumber = tempDBNumber;
    return DB_NUMBER_RIGHT;
}
void CMDDataBlock::overallVerify()
{
    qint16 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = verifyRegisterFormat(this->strRegisterType(),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_RIGHT)
    {
        this->setRegisterType(nTempRegister);
        if(this->getRegisterType()==ID_MEM_DB)
        {
            qint16 nDBNumber = 0;
            int dbFlag = this->verifyDBNumber(strDBNumber(),nDBNumber);
            if(dbFlag == DB_NUMBER_ERROR)
            {
                this->setVerifyFlag(FLAG_DB_NUMBER_ERROR);//!DB块号错误
                return;
            }
            else
            {
                this->setDBRegisterNum(nDBNumber);
            }
        }
        qint16 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(this->strStartAddress(),nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            this->setVerifyFlag(FLAG_STARTADDRESS_ERROR);//!开始地址错误
            return;
        }

        qint16 nTempEndAddress = 0;

        int tempEndAddressFlag = verifyAddressFormat(this->strEndAddress(),nTempEndAddress,nTempAddressFormat);
        if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            this->setVerifyFlag(FLAG_ENDADDRESS_ERROR);//!结束地址错误
            return;
        }

        int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
        if(tempLengthFlag == LENGTH_NEGATIVENUMBER_ERROR)
        {
            this->setVerifyFlag(FLAG_LENGT_NEGATIVE_ERROR);//!长度负数错误
            return;
        }
        else if(tempLengthFlag == LENGTH_OVERRUN_ERROR)
        {
            this->setVerifyFlag(FLAG_LENGT_OVERRUN_ERROR);//!长度超限
            return;
        }
        this->setStartAddress(nTempStartAddress);
        this->setEndAddress(nTempEndAddress);
    }
    else
    {
        m_registerType_Pro = (MDRegister_Pro)14;
        this->setVerifyFlag(FLAG_REGISTER_ERROR);//!寄存器错误
        return;
    }
}
void CMDDataBlock::setStrStartAddress(QString address)
{

    m_strStartAddress = address;
    m_startAddress_Pro = m_strStartAddress;

}
QString CMDDataBlock::strStartAddress()
{
    return m_strStartAddress;
}
void CMDDataBlock::setStrEndAddress(QString address)
{
    m_strEndAddress = address;
    m_endAddress_Pro = m_strEndAddress;
}
QString CMDDataBlock::strEndAddress()
{
    return m_strEndAddress;
}
void CMDDataBlock::setStrRegisterType(QString registerType)
{
    m_strRegisterType = registerType;
}
QString CMDDataBlock::strRegisterType()
{
    return m_strRegisterType;
}
void CMDDataBlock::setstrDBNumber(QString dbnumber)
{
    m_strDBRegisterNum = dbnumber;
    m_dbRegisterNum_Pro = m_strDBRegisterNum;
}
QString CMDDataBlock::strDBNumber()
{
    return m_strDBRegisterNum;
}
QString CMDDataBlock::registerIndexToString(QString index)
{
    QString strRegisterIndexToString = index;
    if(index == "0")
    {
        strRegisterIndexToString = "V";
    }
    else if(index == "1")
    {
        strRegisterIndexToString = "I";
    }
    else if(index == "2")
    {
        strRegisterIndexToString = "Q";
    }
    else if(index == "3")
    {
        strRegisterIndexToString = "AI";
    }
    else if(index == "4")
    {
        strRegisterIndexToString = "AQ";
    }
    else if(index == "5")
    {
        strRegisterIndexToString = "T";
    }
    else if(index == "6")
    {
        strRegisterIndexToString = "C";
    }
    else if(index == "7")
    {
        strRegisterIndexToString = "HC";
    }
    else if(index == "8")
    {
        strRegisterIndexToString = "M";
    }
    else if(index == "9")
    {
        strRegisterIndexToString = "SM";
    }
    else if(index == "10")
    {
        strRegisterIndexToString = "S";
    }
    else if(index == "11")
    {
        strRegisterIndexToString = "PI";
    }
    else if(index == "12")
    {
        strRegisterIndexToString = "PQ";
    }
    else if(index == "13")
    {
        strRegisterIndexToString = "DB";
    }
    return strRegisterIndexToString;
}
QString CMDDataBlock::registerStringToIndex(QString string)
{
    QString strRegisterStringToIndex = string;
    if(0==QString::compare(string,"V",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "0";
    }
    else if(0==QString::compare(string,"I",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "1";
    }
    else if(0==QString::compare(string,"Q",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "2";
    }
    else if(0==QString::compare(string,"AI",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "3";
    }
    else if(0==QString::compare(string,"AQ",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "4";
    }
    else if(0==QString::compare(string,"T",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "5";
    }
    else if(0==QString::compare(string,"C",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "6";
    }
    else if(0==QString::compare(string,"HC",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "7";
    }
    else if(0==QString::compare(string,"M",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "8";
    }
    else if(0==QString::compare(string,"SM",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "9";
    }
    else if(0==QString::compare(string,"S",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "10";
    }
    else if(0==QString::compare(string,"PI",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "11";
    }
    else if(0==QString::compare(string,"PQ",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "12";
    }
    else if(0==QString::compare(string,"DB",Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "13";
    }
    return strRegisterStringToIndex;
}
//**********************************属性框翻译与提示***************************//
void CMDDataBlock::initTranslateNameAndPropertyType()
{
    m_translateName.insert(QString("GUID_DataBlock"),QString("数据块"));
    m_translateName.insert(QString("Register"),QString("寄存器"));
    m_propertyTip.insert(QString("Register"),QString("寄存器类型为NONE时,寄存器错误!\
                                                     S7_1200,S7_1500只支持I,Q,M,DB寄存器.\
                                                     S7_300,S7_400只支持I,Q,M,DB,C,T,PI,PQ寄存器."));
    m_translateName.insert(QString("DBRegisterNum"),QString("DB寄存器编号"));
    m_propertyTip.insert(QString("DBRegisterNum"),QString("DB寄存器编号"));
    m_translateName.insert(QString("StartAddress"),QString("开始地址"));
    m_propertyTip.insert(QString("StartAddress"),QString("寄存器正确:开始地址不能大于结束地址.寄存器错误或者DB寄存器编号错误:地址无意义.\
                                                         本驱动所有地址格式均为10进制.\
                                                         I,Q,M,DB寄存器地址最大长度210.\
                                                         C,T寄存器地址最大长度105."));
    m_translateName.insert(QString("EndAddress"),QString("结束地址"));
    m_propertyTip.insert(QString("EndAddress"),QString("寄存器正确:结束地址不能小于开始地址.寄存器错误或者DB寄存器编号错误:地址无意义.\
                                                       本驱动所有地址格式均为10进制.\
                                                       I,Q,M,DB寄存器地址最大长度210.\
                                                       C,T寄存器地址最大长度105."));
}
QMap<QString,QVariant> CMDDataBlock::translateName() const//!翻译函数
{
    return m_translateName;
}
QMap<QString, QVariant> CMDDataBlock::propertyTip() const//!提示框函数
{
    return m_propertyTip;
}
CMDDataBlock::MDRegister_Pro CMDDataBlock::register_Pro()
{
    if(m_bError == 0)
    {
        CMDDevice*dev =(CMDDevice*)device();
        if(dev)
        {
            connect(dev->getPropertyBrowse(),SIGNAL(propertyFinish()),this,SLOT(propertyLoadFinish_Pro()));
            m_bError = 1;
        }
    }
    return m_registerType_Pro;
}
void CMDDataBlock::setRegister_Pro(CMDDataBlock::MDRegister_Pro reg)
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        if(reg!=MDRegister_Pro::NONE)
        {
            qint16 nTempRegister = 0;
            qint16 nTempLength = 0;
            int nTempAddressFormat = 0;
            int tempRegisterFlag = verifyRegisterFormat(QString::number((int)reg,10),nTempRegister,nTempAddressFormat,nTempLength);
            if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::red);
                return;
            }
            dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::white);
            m_registerType_Pro = reg;
            m_strRegisterType = QString::number((int)reg,10);
            m_registerType = (int)reg;
            if(nTempRegister == ID_MEM_DB)
            {
                dev->getPropertyBrowse()->setSinglePropertyHide("DBRegisterNum",false);
                qint16 nDBNumber = 0;
                int dbFlag = this->verifyDBNumber(m_dbRegisterNum_Pro,nDBNumber);
                if(dbFlag == DB_NUMBER_ERROR)
                {
                    setVerifyFlag(FLAG_DB_NUMBER_ERROR);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
                    dev->getPropertyBrowse()->setPropertyBackColor("DBRegisterNum",Qt::red);
                    return;
                }
                else
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("DBRegisterNum",Qt::white);
                }
            }
            else
            {
                dev->getPropertyBrowse()->setSinglePropertyHide("DBRegisterNum",true);
            }
            qint16 nTempStartAddress = 0;

            int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);


            qint16 nTempEndAddress = 0;

            int tempEndAddressFlag = verifyAddressFormat(m_endAddress_Pro,nTempEndAddress,nTempAddressFormat);
            if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR || tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
            {
                if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
                {
                    setVerifyFlag(FLAG_ENDADDRESS_ERROR);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                }
                else
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
                }
                if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
                {
                    setVerifyFlag(FLAG_STARTADDRESS_ERROR);
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                }
                else
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
                }
                return;
            }
            else
            {
                int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
                if(tempLengthFlag != LENGTH_RIGHT)
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                }
                else
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
                    this->calculateDataBufferSize();
                    setVerifyFlag(FLAG_RIGHT);
                }
            }

        }
    }
}
QString CMDDataBlock::dbRegisterNum_Pro()
{
    return m_dbRegisterNum_Pro;
}
void CMDDataBlock::setDBRegisterNum_Pro(QString num)
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        qint16 nTempRegister = 0;
        qint16 nTempLength = 0;
        int nTempAddressFormat = 0;
        int tempRegisterFlag = verifyRegisterFormat(QString::number((int)m_registerType_Pro,10),nTempRegister,nTempAddressFormat,nTempLength);
        if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
        {
            return;
        }
        if(nTempRegister == ID_MEM_DB)
        {
            qint16 nDBNumber = 0;
            int dbFlag = this->verifyDBNumber(num,nDBNumber);
            if(dbFlag == DB_NUMBER_ERROR)
            {
                return;
            }
            else
            {
                dev->getPropertyBrowse()->setPropertyBackColor("DBRegisterNum",Qt::white);
                m_dbRegisterNum_Pro = num;
                m_strDBRegisterNum = m_dbRegisterNum_Pro;
                m_DBRegisterNum = m_dbRegisterNum_Pro.toInt();
                qint16 nTempStartAddress = 0;
                int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);
                qint16 nTempEndAddress = 0;
                int tempEndAddressFlag = verifyAddressFormat(m_endAddress_Pro,nTempEndAddress,nTempAddressFormat);
                if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR || tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
                {
                    if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
                    {
                        setVerifyFlag(FLAG_ENDADDRESS_ERROR);
                        dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                    }
                    else
                    {
                        dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
                    }
                    if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
                    {
                        setVerifyFlag(FLAG_STARTADDRESS_ERROR);
                        dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                    }
                    else
                    {
                        dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
                    }
                    return;
                }
                else
                {
                    int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
                    if(tempLengthFlag != LENGTH_RIGHT)
                    {
                        dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                        dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                    }
                    else
                    {
                        dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
                        dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
                        this->calculateDataBufferSize();
                        setVerifyFlag(FLAG_RIGHT);
                    }
                }
            }
        }
    }
}
QString CMDDataBlock::startAddress_Pro()
{
    return m_startAddress_Pro;
}
void CMDDataBlock::setStartAddress_Pro(QString address)
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        qint16 nTempRegister = 0;
        qint16 nTempLength = 0;
        int nTempAddressFormat = 0;
        int tempRegisterFlag = verifyRegisterFormat(QString::number((int)m_registerType_Pro,10),nTempRegister,nTempAddressFormat,nTempLength);
        if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
        {
            return;
        }
        qint16 nDBNumber = 0;
        int dbFlag = this->verifyDBNumber(m_dbRegisterNum_Pro,nDBNumber);
        if(dbFlag == DB_NUMBER_ERROR)
        {
            return;
        }
        qint16 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(address,nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            return;
        }

        qint16 nTempEndAddress = 0;

        int tempEndAddressFlag = verifyAddressFormat(m_endAddress_Pro,nTempEndAddress,nTempAddressFormat);
        if(tempEndAddressFlag != ADDRESS_FORMAT_ERROR)
        {
            int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
            if(tempLengthFlag != LENGTH_RIGHT)
            {
                if(nTempStartAddress>nTempEndAddress)
                {
                    nTempStartAddress = nTempEndAddress;
                }
                else
                {
                    nTempStartAddress = m_endAddress_Pro.toInt()-119;
                }
            }
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
            m_startAddress_Pro = QString::number(nTempStartAddress,nTempAddressFormat);
            m_strStartAddress = m_startAddress_Pro;
            m_startAddress = nTempStartAddress;
            this->calculateDataBufferSize();
            setVerifyFlag(FLAG_RIGHT);
        }
        else
        {
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
            m_startAddress_Pro = QString::number(nTempStartAddress,nTempAddressFormat);
            m_strStartAddress = m_startAddress_Pro;
            m_startAddress = nTempStartAddress;
        }
    }
}

QString CMDDataBlock::endAddress_Pro()
{
    return m_endAddress_Pro;
}
void CMDDataBlock::setEndAddress_Pro(QString address)
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        qint16 nTempRegister = 0;
        qint16 nTempLength = 0;
        int nTempAddressFormat = 0;
        int tempRegisterFlag = verifyRegisterFormat(QString::number((int)m_registerType_Pro,10),nTempRegister,nTempAddressFormat,nTempLength);
        if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
        {
            return;
        }
        qint16 nDBNumber = 0;
        int dbFlag = this->verifyDBNumber(m_dbRegisterNum_Pro,nDBNumber);
        if(dbFlag == DB_NUMBER_ERROR)
        {
            return;
        }
        qint16 nTempEndAddress = 0;

        int tempEndAddressFlag = verifyAddressFormat(address,nTempEndAddress,nTempAddressFormat);
        if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            return;
        }

        qint16 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag != ADDRESS_FORMAT_ERROR)
        {
            int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
            if(tempLengthFlag != LENGTH_RIGHT)
            {
                if(nTempStartAddress>nTempEndAddress)
                {
                    nTempEndAddress = nTempStartAddress;
                }
                else
                {
                    nTempEndAddress = m_startAddress_Pro.toInt()+119;
                }
            }
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
            dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::white);
            m_endAddress_Pro = QString::number(nTempEndAddress,nTempAddressFormat);
            m_strEndAddress = m_endAddress_Pro;
            m_endAddress = nTempEndAddress;
            this->calculateDataBufferSize();
            setVerifyFlag(FLAG_RIGHT);
        }
        else
        {
            dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::white);
            m_endAddress_Pro = QString::number(nTempEndAddress,nTempAddressFormat);
            m_strEndAddress = m_endAddress_Pro;
            m_endAddress = nTempEndAddress;
        }

    }
}
void CMDDataBlock::propertyLoadFinish_Pro()
{
    CMDDevice*dev =(CMDDevice*)device();
    if(dev)
    {
        if(this == qobject_cast<CMDDataBlock*>(dev->getPropertyBrowse()->object()))
        {
            qint16 nTempRegister = 0;
            qint16 nTempLength = 0;
            int nTempAddressFormat = 0;
            int tempRegisterFlag = verifyRegisterFormat(QString::number((int)m_registerType_Pro,10),nTempRegister,nTempAddressFormat,nTempLength);
            if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("Register",Qt::red);
                return;
            }
            if(nTempRegister == ID_MEM_DB)
            {
                qint16 nDBNumber = 0;
                int dbFlag = this->verifyDBNumber(m_dbRegisterNum_Pro,nDBNumber);
                if(dbFlag == DB_NUMBER_ERROR)
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("DBRegisterNum",Qt::red);
                    return;
                }
            }
            else
            {
                dev->getPropertyBrowse()->setSinglePropertyHide("DBRegisterNum",true);
            }
            qint16 nTempStartAddress = 0;

            int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);
            if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
            }

            qint16 nTempEndAddress = 0;

            int tempEndAddressFlag = verifyAddressFormat(m_endAddress_Pro,nTempEndAddress,nTempAddressFormat);
            if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
            }
            if(tempEndAddressFlag == ADDRESS_FORMAT_RIGHT&&tempStartAddressFlag == ADDRESS_FORMAT_RIGHT)
            {
                int tempLengthFlag = verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
                if(tempLengthFlag != LENGTH_RIGHT)
                {
                    dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
                    dev->getPropertyBrowse()->setPropertyBackColor("EndAddress",Qt::red);
                }
            }
        }
    }
}
//**************************************************************************//

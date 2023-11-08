#include "MDDataBlock.h"
#include <QStringList>
#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "PropertyBrowser.h"
CMDDataBlock::CMDDataBlock()
{
     m_registerType=0;
     m_startAddress=0;
     m_endAddress=0;
     m_strRegisterType = "0";
     m_strStartAddress = "0";
     m_strEndAddress = "0";
     m_dataBufferSize=0;
     m_dataBuffer=NULL;
     setEnabled(true);
     m_nAreaCode= 0x9C;
     m_nRevLen = 0;
     m_mdimStart = 0;
     //*********************属性框变量*********************//
     this->initTranslateNameAndPropertyType();
     m_registerType_Pro = (MDRegister_Pro)0;
     m_startAddress_Pro = "0";
     m_endAddress_Pro = "0";
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
        return CBaseDataBlock::start();
    }
    CMDDevice *device = (CMDDevice*)CBaseDataBlock::device();
    if(device!=NULL)
    {
        if(device->protocalType()==0)//!3E
        {
            switch(m_registerType)
            {
            default:
            case ID_MEM_X:
                m_nAreaCode = 0x9C;
                break;
            case ID_MEM_Y:
                m_nAreaCode = 0x9D;
                break;
            case ID_MEM_DX:
                m_nAreaCode = 0xA2;
                break;
            case ID_MEM_DY:
                m_nAreaCode = 0xA3;
                break;
            case ID_MEM_B:
                m_nAreaCode = 0xA0;
                break;
            case ID_MEM_SB:
                m_nAreaCode = 0xA1;
                break;
            case ID_MEM_M:
                m_nAreaCode = 0x90;
                break;
            case ID_MEM_SM:
                m_nAreaCode = 0x91;
                break;
            case ID_MEM_L:
                m_nAreaCode = 0x92;
                break;
            case ID_MEM_F:
                m_nAreaCode = 0x93;
                break;
            case ID_MEM_V:
                m_nAreaCode = 0x94;
                break;
            case ID_MEM_S:
                m_nAreaCode = 0x98;
                break;
            case ID_MEM_TS:
                m_nAreaCode = 0xC1;
                break;
            case ID_MEM_TC:
                m_nAreaCode = 0xC0;
                break;
            case ID_MEM_SS:
                m_nAreaCode = 0xC7;
                break;
            case ID_MEM_SC:
                m_nAreaCode = 0xC6;
                break;
            case ID_MEM_CS:
                m_nAreaCode = 0xC4;
                break;
            case ID_MEM_CC:
                m_nAreaCode = 0xC3;
                break;
            case ID_MEM_D:
                m_nAreaCode = 0xA8;
                break;
            case ID_MEM_SD:
                m_nAreaCode = 0xA9;
                break;
            case ID_MEM_W:
                m_nAreaCode = 0xB4;
                break;
            case ID_MEM_SW:
                m_nAreaCode = 0xB5;
                break;
            case ID_MEM_R:
                m_nAreaCode = 0xAF;
                break;
            case ID_MEM_Z:
                m_nAreaCode = 0xCC;
                break;
            case ID_MEM_TN:
                m_nAreaCode = 0xC2;
                break;
            case ID_MEM_SN:
                m_nAreaCode = 0xC8;
                break;
            case ID_MEM_CN:
                m_nAreaCode = 0xC5;
                break;

            }
        }
        else
        {
            switch(m_registerType)
            {
            default:
            case ID_MEM_X:
                m_nAreaCode = 'X' * 256 + ' ';
                break;
            case ID_MEM_Y:
                m_nAreaCode = 'Y' * 256 + ' ';
                break;
            case ID_MEM_DX:
                m_nAreaCode = 'D' * 256 + 'X';
                break;
            case ID_MEM_DY:
                m_nAreaCode = 'D' * 256 + 'Y';
                break;
            case ID_MEM_B:
                m_nAreaCode = 'B' * 256 + ' ';
                break;
            case ID_MEM_SB:
                m_nAreaCode = 'S' * 256 + 'B';
                break;
            case ID_MEM_M:
                m_nAreaCode = 'M' * 256 + ' ';
                break;
            case ID_MEM_SM:
                m_nAreaCode = 'S' * 256 + 'M';
                break;
            case ID_MEM_L:
                m_nAreaCode = 'L' * 256 + ' ';
                break;
            case ID_MEM_F:
                m_nAreaCode = 'F' * 256 + ' ';
                break;
            case ID_MEM_V:
                m_nAreaCode = 'V' * 256 + ' ';
                break;
            case ID_MEM_S:
                m_nAreaCode = 'S' * 256 + ' ';
                break;
            case ID_MEM_TS:
                m_nAreaCode = 'T' * 256 + 'S';
                break;
            case ID_MEM_TC:
                m_nAreaCode = 'T' * 256 + 'C';
                break;
            case ID_MEM_SS:
                m_nAreaCode = 'S' * 256 + 'S';
                break;
            case ID_MEM_SC:
                m_nAreaCode = 'S' * 256 + 'C';
                break;
            case ID_MEM_CS:
                m_nAreaCode = 'C' * 256 + 'S';
                break;
            case ID_MEM_CC:
                m_nAreaCode = 'C' * 256 + 'C';
                break;
            case ID_MEM_D:
                m_nAreaCode = 'D' * 256 + ' ';
                break;
            case ID_MEM_SD:
                m_nAreaCode = 'S' * 256 + 'D';
                break;
            case ID_MEM_W:
                m_nAreaCode = 'W' * 256 + ' ';
                break;
            case ID_MEM_SW:
                m_nAreaCode = 'S' * 256 + 'W';
                break;
            case ID_MEM_R:
                m_nAreaCode = 'R' * 256 + ' ';
                break;
            case ID_MEM_Z:
                m_nAreaCode = 'Z' * 256 + ' ';
                break;
            case ID_MEM_TN:
                m_nAreaCode = 'T' * 256 + 'N';
                break;
            case ID_MEM_SN:
                m_nAreaCode = 'S' * 256 + 'N';
                break;
            case ID_MEM_CN: //CN
                m_nAreaCode = 'C' * 256 + 'N';
                break;

            }
        }

        if (m_registerType ==ID_MEM_D||
                m_registerType ==ID_MEM_SD||
                    m_registerType ==ID_MEM_W||
                        m_registerType ==ID_MEM_SW||
                            m_registerType ==ID_MEM_R||
                                m_registerType ==ID_MEM_Z||
                                    m_registerType ==ID_MEM_TN||
                                        m_registerType ==ID_MEM_SN||
                                            m_registerType ==ID_MEM_CN)

        {
            m_mdimStart = m_startAddress;
            m_nRevLen = (m_endAddress - m_startAddress+1) * 2;

            if (m_registerType == ID_MEM_CN && device->protocalType() == 1 && m_startAddress > 199)
            {
                m_nRevLen = (m_endAddress - m_startAddress+1) *4;
            }
        }
        else
        {
            qint32 ntemp = 0,ntem2 = 0;

            ntemp = m_startAddress%16;
            m_mdimStart = m_startAddress - ntemp;

            ntemp = m_endAddress - m_mdimStart+1;
            ntem2 = ntemp/16;
            if (ntemp%16 || ntemp == 0)
            {
                ntem2++;
            }

            m_nRevLen = ntem2*2;
        }
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
    this->setStrStartAddress(((CMDDataBlock*)otherDataBlock)->strStartAddress());
    this->setStrEndAddress(((CMDDataBlock*)otherDataBlock)->strEndAddress ());
    this->setStrRegisterType(((CMDDataBlock*)otherDataBlock)->strRegisterType());
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }
 }

//开始地址
void CMDDataBlock::setStartAddress(qint32 address)
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

qint32 CMDDataBlock::startAddress()
{
    return m_startAddress;
}

//结束地址
void CMDDataBlock::setEndAddress(qint32 address)
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

qint32 CMDDataBlock::endAddress()
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

qint16 CMDDataBlock::registerType()
{
    return m_registerType;
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
bool CMDDataBlock::getDataBufferUShortValue(qint16 internalAddress,  quint16 &data)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);

    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }

    if (internalAddress < m_dataBufferSize-1 && internalAddress>=0)
    {
        quint8 tempbuffer[4];
        tempbuffer[0] = m_dataBuffer[internalAddress ];
        tempbuffer[1] =  m_dataBuffer[internalAddress+ 1];
        memcpy(&data,tempbuffer,2);
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
    qint32 tempDataBufferSize = 0;
    CMDDevice *device = (CMDDevice*)CBaseDataBlock::device();
    if(device)
    {
        if (m_registerType ==ID_MEM_D||
                m_registerType ==ID_MEM_SD||
                    m_registerType ==ID_MEM_W||
                        m_registerType ==ID_MEM_SW||
                            m_registerType ==ID_MEM_R||
                                m_registerType ==ID_MEM_Z||
                                    m_registerType ==ID_MEM_TN||
                                        m_registerType ==ID_MEM_SN||
                                            m_registerType ==ID_MEM_CN)

        {
            tempDataBufferSize = (m_endAddress - m_startAddress+1) * 2;

            if (m_registerType == ID_MEM_CN && device->protocalType() == 1 && m_startAddress > 199)
            {
                tempDataBufferSize = (m_endAddress - m_startAddress+1) *4;
            }
        }
        else
        {
            qint32 ntemp = 0,ntem2 = 0,nTempSimStart= 0;

            ntemp = m_startAddress%16;
            nTempSimStart = m_startAddress - ntemp;

            ntemp = m_endAddress - nTempSimStart+1;
            ntem2 = ntemp/16;
            if (ntemp%16 || ntemp == 0)
            {
                ntem2++;
            }
            tempDataBufferSize = ntem2*2;
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
            m_dataBufferSize = tempDataBufferSize;
        }
    }

}

bool CMDDataBlock::checkVarAddress(IMDRTDBVariable* nsVar,QString &error)
{
    bool ret = false;
    if (NULL == nsVar)
    {
        return ret;
    }
    if (nsVar->address().isNull() || nsVar->address().isEmpty())
    {
        error = tr("变量地址不能为空!");
        return ret;
    }
    if(verifyFlag() != FLAG_RIGHT)
    {
        error = tr("数据块配置错误!");
        return ret;
    }
    QT_TRY
    {
        nsVar->setInternalAddress(-1);
        nsVar->setInternalSubAddress(-1);

        QStringList addressSplit = nsVar->address().split(".");
        if (addressSplit.count() < 2)
        {
            error = tr("变量地址格式错误!");
            return ret;
        }

        QString strRegisterAndAddress=addressSplit[1];
        QStringList registerAndAddressSplit = strRegisterAndAddress.split("@");
        if(registerAndAddressSplit.count()<2)
        {
            error = tr("变量地址格式错误!");
            return ret;
        }
        QString strRegister = registerAndAddressSplit[0];

        QString strRegisterCompared = "";
        switch (m_registerType)
        {
        case ID_MEM_X:
                strRegisterCompared = "X";
                break;
        case ID_MEM_Y:
                strRegisterCompared = "Y";
                break;
        case ID_MEM_DX:
                strRegisterCompared = "DX";
                break;
        case ID_MEM_DY:
                strRegisterCompared = "DY";
                break;
        case ID_MEM_M:
                strRegisterCompared = "M";
                break;
        case ID_MEM_D:
                strRegisterCompared = "D";
                break;
        case ID_MEM_W:
                strRegisterCompared = "W";
                break;
        case ID_MEM_B:
                strRegisterCompared = "B";
                break;
        case ID_MEM_SB:
                strRegisterCompared = "SB";
                break;
        case ID_MEM_SM:
                strRegisterCompared = "SM";
                break;
        case ID_MEM_L:
                strRegisterCompared = "L";
                break;
        case ID_MEM_F:
                strRegisterCompared = "F";
                break;
        case ID_MEM_V:
                strRegisterCompared = "V";
                break;
        case ID_MEM_S:
                strRegisterCompared = "S";
                break;
        case ID_MEM_TS:
                strRegisterCompared = "TS";
                break;
        case ID_MEM_TC:
                strRegisterCompared = "TC";
                break;
        case ID_MEM_SS:
                strRegisterCompared = "SS";
                break;
        case ID_MEM_SC:
                strRegisterCompared = "SC";
                break;
        case ID_MEM_CS:
                strRegisterCompared = "CS";
                break;
        case ID_MEM_CC:
                strRegisterCompared = "CC";
                break;
        case ID_MEM_SD:
                strRegisterCompared = "SD";
                break;
        case ID_MEM_SW:
                strRegisterCompared = "SW";
                break;
        case ID_MEM_R:
                strRegisterCompared = "R";
                break;
        case ID_MEM_Z:
                strRegisterCompared = "Z";
                break;
        case ID_MEM_TN:
                strRegisterCompared = "TN";
                break;
        case ID_MEM_SN:
                strRegisterCompared = "SN";
                break;
        case ID_MEM_CN:
                strRegisterCompared = "CN";
                break;
        default:
            error = tr("变量地址中寄存器错误!");
            return ret;
            break;
        }
        if(strRegisterCompared!=strRegister)
        {
            error = tr("变量地址中寄存器错误!");
            return ret;
        }
        QString strAddress = registerAndAddressSplit[1];
        qint32 nAddress = 0;
        CMDDevice *device = (CMDDevice*)CBaseDataBlock::device();
        if(device==NULL)
        {
            return ret;
        }
        if((device->xyRegisterFormat() == false)&&
                (m_registerType==ID_MEM_X||
                    m_registerType==ID_MEM_Y))
        {
            bool ok = false;
            nAddress = (qint32)strAddress.toUpper().toInt(&ok,8);
            if(ok == false)
            {
                error = tr("变量地址格式错误!");
                return ret;
            }
        }
        else
        {
            if(m_registerType==ID_MEM_X||
                    m_registerType==ID_MEM_Y||
                        m_registerType==ID_MEM_DX||
                            m_registerType==ID_MEM_DY||
                                m_registerType==ID_MEM_B||
                                    m_registerType==ID_MEM_SB||
                                        m_registerType==ID_MEM_W||
                                            m_registerType==ID_MEM_SW)
            {
                bool ok = false;
                nAddress = (qint32)strAddress.toUpper().toInt(&ok,16);
                if(ok == false)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
            }
            else
            {
                bool ok = false;
                nAddress = (qint32)strAddress.toUpper().toInt(&ok,10);
                if(ok == false)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
            }
        }
        if(nAddress <m_startAddress||nAddress>m_endAddress)
        {
            error = tr("变量地址超限");
            return ret;
        }
        switch (m_registerType)
        {
        case ID_MEM_X:
        case ID_MEM_Y:
        case ID_MEM_DX:
        case ID_MEM_DY:
        case ID_MEM_M:
        case ID_MEM_B:
        case ID_MEM_SB:
        case ID_MEM_SM:
        case ID_MEM_L:
        case ID_MEM_F:
        case ID_MEM_V:
        case ID_MEM_S:
        case ID_MEM_TS:
        case ID_MEM_TC:
        case ID_MEM_SS:
        case ID_MEM_SC:
        case ID_MEM_CS:
        case ID_MEM_CC:
        {
            qint32 ntemp = 0,ntem2 = 0,nTempSimStart= 0;

            ntemp = m_startAddress%16;
            nTempSimStart = m_startAddress - ntemp;

            ntemp = nAddress - nTempSimStart+1;
            ntem2 = ntemp/16;
            if (ntemp%16 || ntemp == 0)
            {
                ntem2++;
            }
            qint32 internalAddress  = (ntem2-1)*2;
            qint32 internalSubAddress = nAddress%16;
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                if((internalAddress+2)>m_dataBufferSize)
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
                    internalSubAddress =15;
                }

            }
                break;
            default:
                error = tr("变量类型错误!");
                return ret;
                break;
            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);
        }
                break;
        case ID_MEM_D:
        case ID_MEM_W:
        case ID_MEM_SD:
        case ID_MEM_SW:
        case ID_MEM_R:
        case ID_MEM_Z:
        case ID_MEM_TN:
        case ID_MEM_SN:
        {
            qint32 internalAddress = (nAddress - m_startAddress)*2;
            qint32 internalSubAddress = -1;
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                if (addressSplit.count() > 2)
                {
                    internalSubAddress=(qint16)addressSplit[2].toUInt();
                }
                else
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                if ((internalAddress+2) > m_dataBufferSize)
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
            case MDDataType::Int16:
            case MDDataType::UInt16:
            {
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
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
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
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
            case MDDataType::Double:
            {
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                if ((internalAddress+8) > m_dataBufferSize)
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
                if (addressSplit.count() > 2)
                {
                    error = tr("变量地址格式错误!");
                    return ret;
                }
                if ((internalAddress+1) >= m_dataBufferSize)
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
                return ret;
                break;
            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);
        }
                break;
        case ID_MEM_CN:
        {
            qint32 internalAddress = -1;
            qint32 internalSubAddress = -1;
            if (m_registerType == ID_MEM_CN && device->protocalType() == 1 && m_startAddress > 199)
            {
                internalAddress = (nAddress - m_startAddress) *4;
                switch (nsVar->originalDataType())
                {
                case MDDataType::Int32:
                case MDDataType::UInt32:
                case MDDataType::Float:
                {
                    if (addressSplit.count() > 2)
                    {
                        error = tr("变量地址格式错误!");
                        return ret;
                    }
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
                    return ret;
                    break;
                }
            }
            else
            {
                internalAddress = (nAddress - m_startAddress) *2;
                switch (nsVar->originalDataType())
                {

                case MDDataType::Bool:
                {
                    if (addressSplit.count() > 2)
                    {
                        internalSubAddress=(qint16)addressSplit[2].toUInt();
                    }
                    else
                    {
                        error = tr("变量地址格式错误!");
                        return ret;
                    }
                    if ((internalAddress+2) > m_dataBufferSize)
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
                case MDDataType::Int16:
                case MDDataType::UInt16:
                {
                    if (addressSplit.count() > 2)
                    {
                        error = tr("变量地址格式错误!");
                        return ret;
                    }
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
                    if (addressSplit.count() > 2)
                    {
                        error = tr("变量地址格式错误!");
                        return ret;
                    }
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
                /*case MDDataType::String:
                {
                    if (addressSplit.count() > 2)
                    {
                        return ret;
                    }
                    if ((internalAddress+1) >= m_dataBufferSize)
                    {
                        internalAddress = -1;
                    }
                    else
                    {
                        ret = true;
                    }
                }
                    break;*/
                default:
                    error = tr("变量类型错误!");
                    return ret;
                    break;
                }
            }
            nsVar->setInternalAddress(internalAddress);
            nsVar->setInternalSubAddress(internalSubAddress);
        }
            break;
        default:
            return ret;
            break;
        }

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
    CMDDevice *device = (CMDDevice*)CBaseDataBlock::device();
    if(device==NULL)
    {
        return;
    }
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
        unsigned char tempBuffer[8];
        switch (m_registerType)
        {
        case ID_MEM_X:
        case ID_MEM_Y:
        case ID_MEM_DX:
        case ID_MEM_DY:
        case ID_MEM_M:
        case ID_MEM_B:
        case ID_MEM_SB:
        case ID_MEM_SM:
        case ID_MEM_L:
        case ID_MEM_F:
        case ID_MEM_V:
        case ID_MEM_S:
        case ID_MEM_TS:
        case ID_MEM_TC:
        case ID_MEM_SS:
        case ID_MEM_SC:
        case ID_MEM_CS:
        case ID_MEM_CC:
        {
            tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
            tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 1];
            if (nsVar->originalDataType() == MDDataType::Bool)
            {
                quint16 tempValue = 0;
                memcpy(&tempValue,tempBuffer,2);
                quint16 dataValue = tempValue & (1<<nsVar->internalSubAddress());
                if (dataValue > 0)
                {
                    varDataValue = true;
                }
                else
                {
                    varDataValue = false;
                }
                bRightValue = true;
            }
            else
            {
                bDataType = true;
            }

        }
            break;
        case ID_MEM_D:
        case ID_MEM_W:
        case ID_MEM_SD:
        case ID_MEM_SW:
        case ID_MEM_R:
        case ID_MEM_Z:
        case ID_MEM_TN:
        case ID_MEM_SN:
        {
            tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
            tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 1];
            tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 2];
            tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 3];
            tempBuffer[4] = m_dataBuffer[nsVar->internalAddress() + 4];
            tempBuffer[5] = m_dataBuffer[nsVar->internalAddress() + 5];
            tempBuffer[6] = m_dataBuffer[nsVar->internalAddress() + 6];
            tempBuffer[7] = m_dataBuffer[nsVar->internalAddress() + 7];
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                quint16 tempValue = 0;
                memcpy(&tempValue,tempBuffer,2);
                quint16 dataValue = tempValue & (1<<nsVar->internalSubAddress());
                if (dataValue > 0)
                {
                    varDataValue = true;
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
            case MDDataType::Float:
            {
                float tempValue=0;
                memcpy(&tempValue,tempBuffer,4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::Double:
            {
                double tempValue=0;
                memcpy(&tempValue,tempBuffer,8);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::String:
            {
                char strTempBuffer[1024];
                memset(strTempBuffer,0,sizeof(strTempBuffer));
                qint32 nLength = (m_endAddress-nsVar->internalAddress()/2 -m_startAddress)*2;

                qint16 nRegulationSize = 0;
                memcpy(&nRegulationSize,&m_dataBuffer[nsVar->internalAddress()],2);

                if(nRegulationSize>nLength)
                {
                    nRegulationSize = nLength;
                }

                memcpy(strTempBuffer,&m_dataBuffer[nsVar->internalAddress()+2],nRegulationSize);
                QString string(strTempBuffer);
                varDataValue =string;
                bRightValue = true;
            }
                break;
            default:
                bDataType = true;
                break;
            }
        }
            break;
        case ID_MEM_CN:
        {
            tempBuffer[0] = m_dataBuffer[nsVar->internalAddress() + 0];
            tempBuffer[1] = m_dataBuffer[nsVar->internalAddress() + 1];
            tempBuffer[2] = m_dataBuffer[nsVar->internalAddress() + 2];
            tempBuffer[3] = m_dataBuffer[nsVar->internalAddress() + 3];
            switch (nsVar->originalDataType())
            {
            case MDDataType::Bool:
            {
                if (device->protocalType() == 1 && m_startAddress > 199)
                {
                    bDataType = true;
                }
                else
                {
                    quint16 tempValue = 0;
                    memcpy(&tempValue,tempBuffer,2);
                    quint16 dataValue = tempValue & (1<<nsVar->internalSubAddress());
                    if (dataValue > 0)
                    {
                        varDataValue = true;
                    }
                    else
                    {
                        varDataValue = false;
                    }
                    bRightValue = true;
                }

            }
                break;
            case MDDataType::Int16:
            {
                if (device->protocalType() == 1 && m_startAddress > 199)
                {
                    bDataType = true;
                }
                else
                {
                    qint16 tempValue=0;
                    memcpy(&tempValue,tempBuffer,2);
                    varDataValue =tempValue;
                    bRightValue = true;
                }
            }
                break;
            case MDDataType::UInt16:
            {
                if (device->protocalType() == 1 && m_startAddress > 199)
                {
                    bDataType = true;
                }
                else
                {
                    quint16 tempValue=0;
                    memcpy(&tempValue,tempBuffer,2);
                    varDataValue =tempValue;
                    bRightValue = true;
                }
            }
                break;
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
            case MDDataType::Float:
            {
                float tempValue=0;
                memcpy(&tempValue,tempBuffer,4);
                varDataValue =tempValue;
                bRightValue = true;
            }
                break;
            case MDDataType::String:
            {
                qint32 nLength = 0;
                 if (device->protocalType() == 1 && m_startAddress > 199)
                 {
                    nLength = (m_endAddress-nsVar->internalAddress()/4 -m_startAddress +1)*4;
                 }
                 else
                 {
                    nLength = (m_endAddress-nsVar->internalAddress()/2 -m_startAddress +1)*2;
                 }
                 /*qint16 nLengthSet = 0;
                 memcpy(&nLengthSet,tempBuffer,2);
                 if(nLengthSet>nLength-2)
                 {
                     nLengthSet = nLength-2;
                 }*/
                 char strTempBuffer[1024];
                 memset(strTempBuffer,0,sizeof(strTempBuffer));
                 memcpy(strTempBuffer,&m_dataBuffer[nsVar->internalAddress()+2],nLength-2);
                 QString string(strTempBuffer);
                 varDataValue =string;
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
            bRightValue = false;
            break;
        }
        if (true == bRightValue)
        {
           bool bChangeFlag = nsVar->refreshData(varDataValue, timeStamp(), quality(), false);
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
    CMDDevice *dev = (CMDDevice *)CBaseDataBlock::device();
    if(NULL == cmd || m_dataBufferSize<=0||NULL ==dev)
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
           if(dev->protocalType()==0)
           {
               memcpy(m_dataBuffer,cmd->getReceiveData()+11,m_dataBufferSize);
           }
           else
           {
               memcpy(m_dataBuffer,cmd->getReceiveData()+2,m_dataBufferSize);
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
        this->onReceiveCommand(false, cmd->getCommandResult());
    }

}
void CMDDataBlock::getVariableAddressList(QStringList &varAddressList)
{
    if(verifyFlag()==FLAG_RIGHT)
    {
        QString strTempStartAddress = "";
        QString strTempEndAddress =  "";
        CMDDevice *device = (CMDDevice*)CBaseDataBlock::device();
        if(device!=NULL)
        {
            if((device->xyRegisterFormat() == false)&&
                    (m_registerType==ID_MEM_X||
                     m_registerType==ID_MEM_Y))
            {
                strTempStartAddress = QString::number(m_startAddress,8);
                strTempEndAddress  = QString::number(m_endAddress,8);
            }
            else
            {
                if(m_registerType==ID_MEM_X||
                        m_registerType==ID_MEM_Y||
                        m_registerType==ID_MEM_DX||
                        m_registerType==ID_MEM_DY||
                        m_registerType==ID_MEM_B||
                        m_registerType==ID_MEM_SB||
                        m_registerType==ID_MEM_W||
                        m_registerType==ID_MEM_SW)
                {
                    strTempStartAddress = QString::number(m_startAddress,16);
                    strTempEndAddress  = QString::number(m_endAddress,16);
                }
                else
                {
                    strTempStartAddress = QString::number(m_startAddress,10);
                    strTempEndAddress  = QString::number(m_endAddress,10);
                }
            }
        }
        QString strRegister = "";
        switch (m_registerType)
        {
        default:
        case ID_MEM_X:
            strRegister = "X";
            break;
        case ID_MEM_Y:
            strRegister = "Y";
            break;
        case ID_MEM_DX:
            strRegister = "DX";
            break;
        case ID_MEM_DY:
            strRegister = "DY";
            break;
        case ID_MEM_M:
            strRegister = "M";
            break;
        case ID_MEM_D:
            strRegister = "D";
            break;
        case ID_MEM_W:
            strRegister = "W";
            break;
        case ID_MEM_B:
            strRegister = "B";
            break;
        case ID_MEM_SB:
            strRegister = "SB";
            break;
        case ID_MEM_SM:
            strRegister = "SM";
            break;
        case ID_MEM_L:
            strRegister = "L";
            break;
        case ID_MEM_F:
            strRegister = "F";
            break;
        case ID_MEM_V:
            strRegister = "V";
            break;
        case ID_MEM_S:
            strRegister = "S";
            break;
        case ID_MEM_TS:
            strRegister = "TS";
            break;
        case ID_MEM_TC:
            strRegister = "TC";
            break;
        case ID_MEM_SS:
            strRegister = "SS";
            break;
        case ID_MEM_SC:
            strRegister = "SC";
            break;
        case ID_MEM_CS:
            strRegister = "CS";
            break;
        case ID_MEM_CC:
            strRegister = "CC";
            break;
        case ID_MEM_SD:
            strRegister = "SD";
            break;
        case ID_MEM_SW:
            strRegister = "SW";
            break;
        case ID_MEM_R:
            strRegister = "R";
            break;
        case ID_MEM_Z:
            strRegister = "Z";
            break;
        case ID_MEM_TN:
            strRegister = "TN";
            break;
        case ID_MEM_SN:
            strRegister = "SN";
            break;
        case ID_MEM_CN:
            strRegister = "CN";
            break;
        }
        QString addressTemp = QString("%1.%2@%3--%4.%5@%6").arg(name())
                .arg(strRegister)
                .arg(strTempStartAddress.toUpper())
                .arg(name())
                .arg(strRegister)
                .arg(strTempEndAddress.toUpper());
        varAddressList.append(addressTemp);
    }
    else
    {
        QString addressTemp = QString("数据块%1配置错误!").arg(name());
        varAddressList.append(addressTemp);
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
        else if(0==QString::compare(element.tagName(),"RegisterType", Qt::CaseInsensitive))
        {
            QString str = element.text();
            this->setStrRegisterType(str);
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
        else if(0==QString::compare(element.tagName(),"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(element.text().toInt());
        }
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
        else if(0==QString::compare(parameterName,"RegisterType", Qt::CaseInsensitive))
        {
            QString str = parameterValue;
            this->setStrRegisterType(str);
        }
        else if(0==QString::compare(parameterName,"StartAddress", Qt::CaseInsensitive))
        {
            QString str = parameterValue;
            this->setStrStartAddress(str);
        }
        else if(0==QString::compare(parameterName,"EndAddress", Qt::CaseInsensitive))
        {
            QString str = parameterValue;
            this->setStrEndAddress(str);
        }
        else if(0==QString::compare(parameterName,"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(parameterValue.toInt());
        }
    }
    this->overallVerify();

    if(verifyFlag()==FLAG_RIGHT)
    {
        this->calculateDataBufferSize();
    }
}
qint32 CMDDataBlock::areaCode()
{
    return m_nAreaCode;
}

void CMDDataBlock::setAreaCode(qint32 areacode)
{
    m_nAreaCode = areacode;
}
qint32 CMDDataBlock::simStar()
{
    return m_mdimStart;
}

void CMDDataBlock::setSimStar(qint32 simstar)
{
    m_mdimStart = simstar;
}

qint32 CMDDataBlock::revLen()
{
    return m_nRevLen;
}

void CMDDataBlock::setRevLen(qint32 revlen)
{
    m_nRevLen = revlen;
}
int CMDDataBlock::verifyRegisterFormat(QString strRegister,qint16 &nRegister,int &addressFormat,qint16 &nLength)
{
    CMDDevice *device = (CMDDevice*)CBaseDataBlock::device();
    if(device)
    {
        bool okRegister = false;
        qint16 tempRegister = strRegister.toInt(&okRegister,10);

        if((okRegister==false)||(tempRegister<0||tempRegister>26))
            return REGISTER_FORMAT_ERROR;//!寄存器格式校验错误
        else
        {
            if((device->xyRegisterFormat() == false)&&
                    (tempRegister==ID_MEM_X||
                     tempRegister==ID_MEM_Y))
            {
                if(device->protocalType()==0)
                {
                    nRegister = tempRegister;
                    addressFormat = 8;
                    nLength = 4800;
                }
                else
                {
                    nRegister = tempRegister;
                    addressFormat = 8;
                    nLength = 496;
                }

            }
            else
            {
                if(tempRegister==ID_MEM_D||
                        tempRegister==ID_MEM_SD||
                        tempRegister==ID_MEM_W||
                        tempRegister==ID_MEM_SW||
                        tempRegister==ID_MEM_R||
                        tempRegister==ID_MEM_Z||
                        tempRegister==ID_MEM_TN||
                        tempRegister==ID_MEM_SN||
                        tempRegister==ID_MEM_CN)
                {
                    if(device->protocalType()==0)
                        nLength = 600;
                    else
                        nLength = 256;
                    nRegister = tempRegister;
                    addressFormat = 10;

                    if(tempRegister==ID_MEM_W||
                            tempRegister==ID_MEM_SW)
                    {
                        addressFormat = 16;
                    }
                }
                else
                {
                    if(device->protocalType()==0)
                        nLength = 4800;
                    else
                        nLength = 496;
                    nRegister = tempRegister;
                    addressFormat = 10;
                    if(tempRegister==ID_MEM_X||
                            tempRegister==ID_MEM_Y||
                            tempRegister==ID_MEM_DX||
                            tempRegister==ID_MEM_DY||
                            tempRegister==ID_MEM_B||
                            tempRegister==ID_MEM_SB)
                    {
                        addressFormat = 16;
                    }
                }
            }
        }
    }
    return REGISTER_FORMAT_RIGHT;
}

int CMDDataBlock::verifyAddressFormat(QString strAddress,qint32 &nAddress,qint16 addressFormat)
{
    bool okAddress = false;
    qint32 tempAddress = 0;

    tempAddress = strAddress.toInt(&okAddress,addressFormat);

    if((okAddress == false)||(tempAddress<0))
        return ADDRESS_FORMAT_ERROR;//!地址格式校验错误
    else
        nAddress = tempAddress;
    return ADDRESS_FORMAT_RIGHT;
}
int CMDDataBlock::verifyLength(qint32 startAddress,qint32 endAddress,qint16 nLength)
{
    if(startAddress > endAddress)
    {
        return LENGTH_NEGATIVENUMBER_ERROR;//!开始地址大于结束地址
    }
    else
    {
        qint32 nLengthTemp = endAddress - startAddress+1;
        if(nLengthTemp<=0||nLengthTemp>nLength)
        {
            return LENGTH_OVERRUN_ERROR;//!超过最大长度
        }
    }
    return LENGTH_RIGHT;
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

        qint32 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(this->strStartAddress(),nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            this->setVerifyFlag(FLAG_STARTADDRESS_ERROR);//!开始地址错误
            return;
        }

        qint32 nTempEndAddress = 0;

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
        m_registerType_Pro = (MDRegister_Pro)27;
        this->setVerifyFlag(FLAG_REGISTER_ERROR);//!寄存器错误
        return;
    }
}
QString CMDDataBlock::registerIndexToString(QString index)
{
    QString strRegisterIndexToString = index;
    if(index == "0")
    {
        strRegisterIndexToString = "X";
    }
    else if(index == "1")
    {
        strRegisterIndexToString = "Y";
    }
    else if(index == "2")
    {
        strRegisterIndexToString = "DX";
    }
    else if(index == "3")
    {
        strRegisterIndexToString = "DY";
    }
    else if(index == "4")
    {
        strRegisterIndexToString = "M";
    }
    else if(index == "5")
    {
        strRegisterIndexToString = "D";
    }
    else if(index == "6")
    {
        strRegisterIndexToString = "W";
    }
    else if(index == "7")
    {
        strRegisterIndexToString = "B";
    }
    else if(index == "8")
    {
        strRegisterIndexToString = "SB";
    }
    else if(index == "9")
    {
        strRegisterIndexToString = "SM";
    }
    else if(index == "10")
    {
        strRegisterIndexToString = "L";
    }
    else if(index == "11")
    {
        strRegisterIndexToString = "F";
    }
    else if(index == "12")
    {
        strRegisterIndexToString = "V";
    }
    else if(index == "13")
    {
        strRegisterIndexToString = "S";
    }
    else if(index == "14")
    {
        strRegisterIndexToString = "TS";
    }
    else if(index == "15")
    {
        strRegisterIndexToString = "TC";
    }
    else if(index == "16")
    {
        strRegisterIndexToString = "SS";
    }
    else if(index == "17")
    {
        strRegisterIndexToString = "SC";
    }
    else if(index == "18")
    {
        strRegisterIndexToString = "CS";
    }
    else if(index == "19")
    {
        strRegisterIndexToString = "CC";
    }
    else if(index == "20")
    {
        strRegisterIndexToString = "SD";
    }
    else if(index == "21")
    {
        strRegisterIndexToString = "SW";
    }
    else if(index == "22")
    {
        strRegisterIndexToString = "R";
    }
    else if(index == "23")
    {
        strRegisterIndexToString = "Z";
    }
    else if(index == "24")
    {
        strRegisterIndexToString = "TN";
    }
    else if(index == "25")
    {
        strRegisterIndexToString = "SN";
    }
    else if(index == "26")
    {
        strRegisterIndexToString = "CN";
    }
    return strRegisterIndexToString;
}
QString CMDDataBlock::registerStringToIndex(QString string)
{
    QString strRegisterStringToIndex = string;
    if(0==QString::compare(string,"X", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "0";
    }
    else if(0==QString::compare(string,"Y", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "1";
    }
    else if(0==QString::compare(string,"DX", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "2";
    }
    else if(0==QString::compare(string,"DY", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "3";
    }
    else if(0==QString::compare(string,"M", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "4";
    }
    else if(0==QString::compare(string,"D", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "5";
    }
    else if(0==QString::compare(string,"W", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "6";
    }
    else if(0==QString::compare(string,"B", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "7";
    }
    else if(0==QString::compare(string,"SB", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "8";
    }
    else if(0==QString::compare(string,"SM", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "9";
    }
    else if(0==QString::compare(string,"L", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "10";
    }
    else if(0==QString::compare(string,"F", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "11";
    }
    else if(0==QString::compare(string,"V", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "12";
    }
    else if(0==QString::compare(string,"S", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "13";
    }
    else if(0==QString::compare(string,"TS", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "14";
    }
    else if(0==QString::compare(string,"TC", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "15";
    }
    else if(0==QString::compare(string,"SS", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "16";
    }
    else if(0==QString::compare(string,"SC", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "17";
    }
    else if(0==QString::compare(string,"CS", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "18";
    }
    else if(0==QString::compare(string,"CC", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "19";
    }
    else if(0==QString::compare(string,"SD", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "20";
    }
    else if(0==QString::compare(string,"SW", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "21";
    }
    else if(0==QString::compare(string,"R", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "22";
    }
    else if(0==QString::compare(string,"Z", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "23";
    }
    else if(0==QString::compare(string,"TN", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "24";
    }
    else if(0==QString::compare(string,"SN", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "25";
    }
    else if(0==QString::compare(string,"CN", Qt::CaseInsensitive))
    {
        strRegisterStringToIndex = "26";
    }
    return strRegisterStringToIndex;
}
//**********************************属性框翻译与提示***************************//
void CMDDataBlock::initTranslateNameAndPropertyType()
{
    m_translateName.insert(QString("GUID_DataBlock"),QString("数据块"));
    m_translateName.insert(QString("Register"),QString("寄存器"));
    m_propertyTip.insert(QString("Register"),QString("寄存器类型为NONE时,寄存器错误!"));
    m_translateName.insert(QString("StartAddress"),QString("开始地址"));
    m_propertyTip.insert(QString("StartAddress"),QString("寄存器正确:开始地址不能大于结束地址.\
                                                         寄存器错误:地址无意义. \
                                                         地址格式：X,Y,DX,DY,B,SB,W,SW：16进制.\
                                                         注：X/Y由设备参数中(XY寄存器地址格式)确定为8进制或16进制.\
                                                         上述软元件类型以外：10进制.\
                                                         Q3E协议：位寄存器最大长度：4800,字寄存器最大长度：600.\
                                                         A1E协议：位寄存器最大长度：496,字寄存器最大长度：256.\
                                                         具体以各系列手册为准."));
    m_translateName.insert(QString("EndAddress"),QString("结束地址"));
    m_propertyTip.insert(QString("EndAddress"),QString("寄存器正确:结束地址不能小于开始地址.\
                                                       寄存器错误:地址无意义.\
                                                       地址格式：X,Y,DX,DY,B,SB,W,SW：16进制.\
                                                       注：X/Y由设备参数中(XY寄存器地址格式)确定为8进制或16进制.\
                                                       上述软元件类型以外：10进制.\
                                                       Q3E协议：位寄存器最大长度：4800,字寄存器最大长度：600.\
                                                       A1E协议：位寄存器最大长度：496,字寄存器最大长度：256.\
                                                       具体以各系列手册为准."));
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

            qint32 nTempStartAddress = 0;

            int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);


            qint32 nTempEndAddress = 0;

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
        qint32 nTempStartAddress = 0;

        int tempStartAddressFlag = verifyAddressFormat(address,nTempStartAddress,nTempAddressFormat);
        if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            return;
        }

        qint32 nTempEndAddress = 0;

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

        qint32 nTempEndAddress = 0;

        int tempEndAddressFlag = verifyAddressFormat(address,nTempEndAddress,nTempAddressFormat);
        if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            return;
        }

        qint32 nTempStartAddress = 0;

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
            qint32 nTempStartAddress = 0;

            int tempStartAddressFlag = verifyAddressFormat(m_startAddress_Pro,nTempStartAddress,nTempAddressFormat);
            if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
            {
                dev->getPropertyBrowse()->setPropertyBackColor("StartAddress",Qt::red);
            }

            qint32 nTempEndAddress = 0;

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

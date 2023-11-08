#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"

//********************PKX20151104********************//
OMRRegInform OMRRegInform_CJ1M[MAX_NUMBER_MEMAREA] =
{
    {ID_MEM_AR,"AR","RJ","WJ",0x02},
    {ID_MEM_DM,"DM","RD","WD",0x02},
    {ID_MEM_EM,"EM","RE00","WE00",0x04},
    {ID_MEM_HR,"HR","RH","WH",0x02},
    {ID_MEM_IR,"IR","RR","WR",0x02},
    {ID_MEM_LR,"LR","RL","WL",0x02},
    {ID_MEM_PV,"PV","RC","WC",0x02},
    {ID_MEM_TC,"TC","RG","",0x02},
};
OMRRegInform* getOMRRegInform(qint16 nRegType)
{
    if((nRegType >= 0)&&(nRegType<MAX_NUMBER_MEMAREA))
    {
        return &OMRRegInform_CJ1M[nRegType];
    }
    else
    {
        return NULL;
    }
}
//***************************************************//

CMDProtocolCommand::CMDProtocolCommand()
{
     m_readCmdDataLength = 0;
     m_ushort_Extra_Length_Read = 0;
     setReceiveBufferSize(613);//PKX20151110
     m_tempReceiveBufferIndex = 0;//PKX20151111
     m_splitFlag = 0;//PKX20151111
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

    //************************PKX20151104************************//
    /*!
     * \section
     * 要求读回的数据超过plc规定一包读回的长度时
     * 第二报以后的数据需要发送分包读取命令(0x0D,0x0A)
     */
    if(m_splitFlag != 0)
    {
        setReceiveDataLength(0);
        m_sendData[0] = 0x0D;

        m_sendData[1] = 0x0A;
        setCommandLength(2);
    }
    else
    {
        setReceiveDataLength(0);
        quint16 nLengthCounter = 0;
        OMRRegInform* pointer_OMRRegInform = getOMRRegInform(db->registerType());//!获取相应寄存器信息
        if(NULL == pointer_OMRRegInform )
        {
            return bRet;
        }
        m_sendData[0] = 0x40;//!起始符

        m_sendData[1] = dev->station()/10 + 0x30;//!站号

        m_sendData[2] = dev->station()%10 + 0x30;//!站号
        quint8 nCommandLength = pointer_OMRRegInform->m_uint8_ReadWriteCommand_Length;//!获取读命令长度
        for(int i = 0 ;i < nCommandLength;i++)
        {
            m_sendData[3+i] = (quint8)pointer_OMRRegInform->m_char_Register_ReadCommand[i];//!读命令码赋值给发送buffer
        }

        qint16 nStartAddress = db->startAddress();
        /*!
          *\section
          * 开始地址由高到低赋值给发送buffer
          */
        m_sendData[nCommandLength+3] = nStartAddress/1000 + 0x30;

        m_sendData[nCommandLength+4] = (nStartAddress%1000)/100 + 0x30;

        m_sendData[nCommandLength+5] = (nStartAddress%100)/10 + 0x30;

        m_sendData[nCommandLength+6] = nStartAddress%10 + 0x30;

        qint16 AddressLength = db->endAddress() - db->startAddress() +1;
        if(pointer_OMRRegInform->m_uint_UIRegister_ID == ID_MEM_EM)
        {
            m_readCmdDataLength = (ushort)(AddressLength*4);//!返回数据的长度
            m_ushort_Extra_Length_Read = 13;//!返回除数据外的长度
        }
        else if(pointer_OMRRegInform->m_uint_UIRegister_ID == ID_MEM_TC)
        {
            m_readCmdDataLength = (ushort)(AddressLength);//!返回数据的长度
            m_ushort_Extra_Length_Read = 11;//!返回除数据外的长度
        }
        else
        {
            m_readCmdDataLength = (ushort)(AddressLength*4);//!返回数据的长度
            m_ushort_Extra_Length_Read = 11;//!返回除数据外的长度
        }
        /*!
          *\section
          * 需要读取的数据长度由高到低赋值给发送buffer
          */
        m_sendData[nCommandLength+7] = AddressLength/1000 + 0x30;

        m_sendData[nCommandLength+8] = (AddressLength%1000)/100 + 0x30;

        m_sendData[nCommandLength+9] = (AddressLength%100)/10 + 0x30;

        m_sendData[nCommandLength+10] = AddressLength%10 + 0x30;

        nLengthCounter = nCommandLength + 11;



        //quint16 nRead_XOR_Result = XOR((uchar*)m_sendData,nLengthCounter);

        //m_sendData[nCommandLength+11] = (nRead_XOR_Result>>8) & 0xFF;

        //m_sendData[nCommandLength+12] = nRead_XOR_Result& 0xFF;

        quint8 btXORLow,btXORHigh;
        XOR(m_sendData,nLengthCounter,btXORLow,btXORHigh);//!异或校验

        m_sendData[nCommandLength+11] = btXORHigh;//!异或校验结果高位赋值给发送buffer

        m_sendData[nCommandLength+12] = btXORLow;//!异或校验结果低位赋值给发送buffer

        m_sendData[nCommandLength+13] = 0x2A;//!结束符

        m_sendData[nCommandLength+14] = 0x0D;//!结束符

        nLengthCounter = nCommandLength + 11 + 4;

        setCommandLength(nLengthCounter);
    }

    bRet = true;
    return bRet;
    //*********************************************************//
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
    //****************************PKX20151106*****************************//
    OMRRegInform *pOMRRegInform = getOMRRegInform(db->registerType());
    if(pOMRRegInform == NULL)
    {
        return bRet;
    }
    if(pOMRRegInform->m_uint_UIRegister_ID == ID_MEM_TC)//!TC寄存器不能进行写操作
    {
        return bRet;
    }
    else if(pOMRRegInform->m_uint_UIRegister_ID == ID_MEM_AR)//!AR寄存器地址447前不能进行写操作
    {

        if((db->startAddress()+nsVar->internalAddress() /2)<=447)
        {
            return bRet;
        }
    }
    setCommandLength(0);
    m_sendData[0] = 0x40;//!起始符

    m_sendData[1] = dev->station()/10 + 0x30;//!站号高位

    m_sendData[2] = dev->station()%10 + 0x30;//!站号低位

    quint8 nCommandLength = pOMRRegInform->m_uint8_ReadWriteCommand_Length;
    for(int i = 0 ;i < nCommandLength;i++)
    {
        m_sendData[3+i] = (quint8)pOMRRegInform->m_char_Register_WriteCommand[i];//!写命令码
    }

    qint16 nWriteAddress = db->startAddress() + nsVar->internalAddress()/2;
    /*!
      *\section
      * 写入地址由高到低赋值给发送buffer
      */
    m_sendData[nCommandLength+3] = nWriteAddress/1000 + 0x30;

    m_sendData[nCommandLength+4] = (nWriteAddress%1000)/100 + 0x30;

    m_sendData[nCommandLength+5] = (nWriteAddress%100)/10 + 0x30;

    m_sendData[nCommandLength+6] = nWriteAddress%10 + 0x30;
    quint16 nLengthCounter = 0;
    /*!
      *\section
      * 写入值获取
      */
    switch(nsVar->originalDataType())
    {
    case MDDataType::Bool:
    {
        ushort dataValue = 0;
        if (db->getDataBufferUShortValue(nsVar->internalAddress(), dataValue))
        {
            if (nsVar->originalValue().toBool() == true)
            {
                dataValue = CBitFuntons::setBit(dataValue, nsVar->internalSubAddress());
            }
            else
            {
                dataValue = CBitFuntons::clearBit(dataValue, nsVar->internalSubAddress());
            }
        }
        for(int i = 0;i<4;i++)
        {
            quint8 temp =  (quint8)(dataValue>>i*4) & 0xF;
            if(temp<10)
            {
                temp = temp +0x30;
            }
            else
            {
                 temp = temp +0x37;
            }
            m_sendData[nCommandLength+10-i] = temp;
        }
        nLengthCounter = nCommandLength +11;
    }
        break;
    case MDDataType::Int16:
    {
        short dataValue = (short)(nsVar->originalValue().toInt());
        for(int i = 0;i<4;i++)
        {
            quint8 temp =  (quint8)(dataValue>>i*4) & 0xF;
            if(temp<10)
            {
                temp = temp +0x30;
            }
            else
            {
                 temp = temp +0x37;
            }
            m_sendData[nCommandLength+10-i] = temp;
        }
        nLengthCounter = nCommandLength +11;
    }
        break;
    case MDDataType::UInt16:
    {
        //int  a   =  nsVar->originalValue().toInt();
        ushort dataValue = (ushort)(nsVar->originalValue().toUInt());
        for(int i = 0;i<4;i++)
        {
            quint8 temp =  (quint8)(dataValue>>i*4) & 0xF;
            if(temp<10)
            {
                temp = temp +0x30;
            }
            else
            {
                 temp = temp +0x37;
            }
            m_sendData[nCommandLength+10-i] = temp;
        }
        nLengthCounter = nCommandLength +11;

    }
        break;
    case MDDataType::Int32:
    {
        int dataValue = (int)(nsVar->originalValue().toInt());
        for(int i = 0;i<8;i++)
        {
            quint8 temp = (quint8) (dataValue>>i*4) & 0xF;
            if(temp<10)
            {
                temp = temp +0x30;
            }
            else
            {
                 temp = temp +0x37;
            }
            if(i<=3)
            {
                m_sendData[nCommandLength+10-i] = temp;
            }
            else
            {
                m_sendData[nCommandLength+18-i] = temp;
            }
        }
        nLengthCounter = nCommandLength +15;
    }
        break;
    case MDDataType::UInt32:
    {
        uint dataValue = (uint)(nsVar->originalValue().toUInt());
        for(int i = 0;i<8;i++)
        {
            quint8 temp = (quint8) (dataValue>>i*4) & 0xF;
            if(temp<10)
            {
                temp = temp +0x30;
            }
            else
            {
                 temp = temp +0x37;
            }
            if(i<=3)
            {
                m_sendData[nCommandLength+10-i] = temp;
            }
            else
            {
                m_sendData[nCommandLength+18-i] = temp;
            }
        }
        nLengthCounter = nCommandLength +15;
    }
        break;
    case MDDataType::Float:
    {
        float dataValue = (float)(nsVar->originalValue().toFloat());
        quint8 *ptempfloat = (quint8*)&dataValue;
        quint8 temp[2] ;
        for(int i = 0;i<4;i++)
        {
            temp[0] = (quint8) (*(ptempfloat +i)) & 0xF;
            temp[1] = (quint8) (*(ptempfloat +i)>>4) & 0xF;
            for(int j = 0;j<2;j++)
            {
                if(temp[j]<10)
                {
                    temp[j] = temp[j] +0x30;
                }
                else
                {
                     temp[j] = temp[j] +0x37;
                }
             }
            if(i == 0)
            {
                m_sendData[nCommandLength+10] = temp[0];
                m_sendData[nCommandLength+10 -1] = temp[1];
            }
            else if(i == 1)
            {
                m_sendData[nCommandLength+10-2] = temp[0];
                m_sendData[nCommandLength+10 -3] = temp[1];
            }
            else if(i == 2)
            {
                m_sendData[nCommandLength+14] = temp[0];
                m_sendData[nCommandLength+14-1] = temp[1];
            }
            else
            {
                m_sendData[nCommandLength+14-2] = temp[0];
                m_sendData[nCommandLength+14-3] = temp[1];
            }
        }
        nLengthCounter = nCommandLength +15;
    }
        break;
    case MDDataType::String:
    {
        m_sendData[nCommandLength+3] = (nWriteAddress+1)/1000 + 0x30;

        m_sendData[nCommandLength+4] = ((nWriteAddress+1)%1000)/100 + 0x30;

        m_sendData[nCommandLength+5] = ((nWriteAddress+1)%100)/10 + 0x30;

        m_sendData[nCommandLength+6] = (nWriteAddress+1)%10 + 0x30;

        QString tempstring= nsVar->originalValue().toString();
        QByteArray tempstring1 = tempstring.toLatin1();
        char* tempstring2 = tempstring1.data();
        quint8 *pstrWrite = (quint8 *)tempstring2;

        int  nstrWtiteSize = tempstring.size();

        quint16 nRegulationSize = 0;
        db->getDataBufferUShortValue(nsVar->internalAddress(), nRegulationSize);

        if(nRegulationSize>=(db->endAddress()-nWriteAddress)*2)
        {
          nRegulationSize = (db->endAddress()-nWriteAddress)*2;
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
        quint8 temp[2];
        for(int i = 0;i<nstrWtiteSize;i++)
        {
            temp[0] = (quint8) (*(pstrWrite +i)) & 0xF;
            temp[1] = (quint8) (*(pstrWrite +i)>>4) & 0xF;
            for(int j = 0;j<2;j++)
            {
                if(temp[j]<10)
                {
                    temp[j] = temp[j] +0x30;
                }
                else
                {
                    temp[j] = temp[j] +0x37;
                }
            }

            if(i == (nstrWtiteSize - 2))
            {
                if(bOddOrEven)
                {
                    m_sendData[nCommandLength+7+2*i+3] = temp[0];
                    m_sendData[nCommandLength+7+2*i+2] = temp[1];
                }
                else
                {
                    if(bLenghtEqual == 1)
                    {
                        m_sendData[nCommandLength+7+2*i+3] = temp[0];
                        m_sendData[nCommandLength+7+2*i+2] = temp[1];
                    }
                    else
                    {
                        m_sendData[nCommandLength+7+2*i+3] = 0x30;
                        m_sendData[nCommandLength+7+2*i+2] = 0x30;
                    }
                }
            }
            if(i == (nstrWtiteSize - 1))
            {
                if(bOddOrEven)
                {
                    m_sendData[nCommandLength+7+2*(i-1)+1] = 0x30;
                    m_sendData[nCommandLength+7+2*(i-1)+0] = 0x30;
                }
                else
                {
                    if(bLenghtEqual == 1)
                    {
                        m_sendData[nCommandLength+7+2*(i-1)+1] = temp[0];
                        m_sendData[nCommandLength+7+2*(i-1)+0] = temp[1];
                    }
                    else
                    {
                        m_sendData[nCommandLength+7+2*(i-1)+1] = 0x30;
                        m_sendData[nCommandLength+7+2*(i-1)+0] = 0x30;
                    }
                }
            }
            else
            {
                if(i%2 ==0)
                {
                    m_sendData[nCommandLength+7+2*i+3] = temp[0];
                    m_sendData[nCommandLength+7+2*i+2] = temp[1];
                }
                else
                {
                    m_sendData[nCommandLength+7+2*(i-1)+1] = temp[0];
                    m_sendData[nCommandLength+7+2*(i-1)+0] = temp[1];
                }
            }
        }

        nLengthCounter = nCommandLength +7+nstrWtiteSize*2;

    }
        break;
    default:
        break;

    }
    if(nLengthCounter == 0)
    {
        return bRet;
    }
    else
    {
        quint8 btXORLow,btXORHigh;
        XOR(m_sendData,nLengthCounter,btXORLow,btXORHigh);

        m_sendData[nLengthCounter] = btXORHigh;//!异或校验结果高位

        m_sendData[nLengthCounter+1] = btXORLow;//!异或校验结果低位

       // quint16 nWrite_XOR_Result = XOR((uchar*)m_sendData,nLengthCounter);

       // m_sendData[nLengthCounter] = (nWrite_XOR_Result>>8) & 0xFF;

       // m_sendData[nLengthCounter+1] = nWrite_XOR_Result& 0xFF;

        m_sendData[nLengthCounter+2] = 0x2A;//!结束符

        m_sendData[nLengthCounter+3] = 0x0D;//!结束符

        nLengthCounter = nLengthCounter +4;

         setCommandLength(nLengthCounter);

        bRet = true;
    }
    return bRet;
    //**************************************************************************************//
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
    OMRRegInform *pOMRRegInform = getOMRRegInform(db->registerType());
    if(pOMRRegInform == NULL)
    {
        return bRet;
    }
    if(pOMRRegInform->m_uint_UIRegister_ID == ID_MEM_TC)
    {
        return bRet;
    }
    setCommandLength(0);
    if(db->currentPackBlockWrite() == 0)
    {
        unsigned char *dataDatabuffer =new unsigned char[db->getDataBufferSize()] ;
        db->getDataBuffer(dataDatabuffer, 0, db->getDataBufferSize());

        QString strAddr="";
        uint nMinAddress = 0;
        uint nMaxAddress = 0;
        uint nAddress = 0;
        uint nItemOff = 0;
        IMDRTDBVariable *nsVar=NULL;
        /*!
          *\section
          * 获取要批量写入的最小地址与最大地址，
          * 同时将从缓存中获取到的最小地址--最大地址
          * 的值放到临时buffer中，将需要修改的值在临时
          * buffer中改掉
          */
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

                nAddress = (ushort)(db->startAddress() + nAddress / 2);

                if(n ==0 )
                {
                    nMinAddress = nAddress ;
                    nMaxAddress = nAddress ;
                }
                nItemOff = (nAddress - db->startAddress())*2;
                switch (nsVar->originalDataType())
                {
                case MDDataType::Bool:
                    {
                        ushort tempValue=0;
                        unsigned char tempArray[2];
                        tempArray[0] = dataDatabuffer[nItemOff + 1];
                        tempArray[1] = dataDatabuffer[nItemOff];
                        memcpy(&tempValue,tempArray,2);
                        if (nsVar->originalValue().toBool() == true)
                        {
                            tempValue = CBitFuntons::setBit(tempValue, nSubAddress);
                        }
                        else
                        {
                            tempValue = CBitFuntons::clearBit(tempValue, nSubAddress);
                        }
                        dataDatabuffer[nItemOff + 1] = (unsigned char)(tempValue&0xFF);
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);

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
                        qint16 tempValue=(qint16)(nsVar->originalValue().toInt());
                        dataDatabuffer[nItemOff + 1] = (unsigned char)(tempValue&0xFF);
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
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
                        quint16 tempValue=(quint16)(nsVar->originalValue().toUInt());
                        dataDatabuffer[nItemOff + 1] = (unsigned char)(tempValue&0xFF);
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
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
                        int tempValue=(int)(nsVar->originalValue().toInt());
                        dataDatabuffer[nItemOff+1] = (unsigned char)(tempValue&0xFF);
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                        dataDatabuffer[nItemOff+3] = (unsigned char)((tempValue>>16)&0xFF);
                        dataDatabuffer[nItemOff+2] = (unsigned char)((tempValue>>24)&0xFF);
                        if(nMinAddress>nAddress)
                        {
                            nMinAddress = nAddress;
                        }
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }

                    }
                    break;
                case MDDataType::UInt32:
                    {
                        uint tempValue=(uint)(nsVar->originalValue().toUInt());
                        dataDatabuffer[nItemOff+1] = (unsigned char)(tempValue&0xFF);
                        dataDatabuffer[nItemOff] = (unsigned char)((tempValue>>8)&0xFF);
                        dataDatabuffer[nItemOff+3] = (unsigned char)((tempValue>>16)&0xFF);
                        dataDatabuffer[nItemOff+2] = (unsigned char)((tempValue>>24)&0xFF);
                        if(nMinAddress>nAddress)
                        {
                            nMinAddress = nAddress;
                        }
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                    break;
                case MDDataType::Float:
                    {
                        float tempValue=(float)(nsVar->originalValue().toFloat());
                        unsigned char *pTempVale = (unsigned char *)&tempValue;
                        dataDatabuffer[nItemOff+1] = *pTempVale ;
                        dataDatabuffer[nItemOff] = *(pTempVale+1) ;
                        dataDatabuffer[nItemOff+3] = *(pTempVale+2);
                        dataDatabuffer[nItemOff+2] = *(pTempVale+3);
                        if(nMinAddress>nAddress)
                        {
                            nMinAddress = nAddress;
                        }
                        if(nMaxAddress<nAddress+1)
                        {
                            nMaxAddress = nAddress+1;
                        }
                    }
                    break;
                case MDDataType::String:
                    {
                        QString tempstring= nsVar->originalValue().toString();
                        QByteArray tempstring1 = tempstring.toLatin1();
                        char* tempstring2 = tempstring1.data();
                        unsigned char  *pstrWrite = (unsigned char *)tempstring2;
                        int  nstrWtiteSize = tempstring.size();
                        if(nstrWtiteSize >(db->endAddress()-db->startAddress())*2)
                        {
                            nstrWtiteSize = (db->endAddress()-db->startAddress())*2;
                        }
                        for(int i = 0;i<nstrWtiteSize;i++)
                        {
                            if(nstrWtiteSize%2 ==0)
                            {
                                if(i%2==0)
                                {
                                    dataDatabuffer[nItemOff +i ] = *(pstrWrite+i +1);
                                }
                                else
                                {
                                    dataDatabuffer[nItemOff +i ] = *(pstrWrite+i-1);
                                }
                            }
                            else
                            {
                                if(i == (nstrWtiteSize-1))
                                {
                                        dataDatabuffer[nItemOff +i ] = 0;
                                        dataDatabuffer[nItemOff +i +1] = *(pstrWrite+i );
                                }
                                else
                                {
                                    if(i%2==0)
                                    {
                                        dataDatabuffer[nItemOff +i ] = *(pstrWrite+i +1);
                                    }
                                    else
                                    {
                                        dataDatabuffer[nItemOff +i ] = *(pstrWrite+i-1);
                                    }
                                }
                            }
                        }
                        if(nstrWtiteSize%2==0)
                        {
                            dataDatabuffer[nItemOff+nstrWtiteSize] = 0;
                            dataDatabuffer[nItemOff+nstrWtiteSize+1] = 0;
                        }
                        else
                        {
                            dataDatabuffer[nItemOff+nstrWtiteSize+1] = 0;
                            dataDatabuffer[nItemOff+nstrWtiteSize+2] = 0;
                        }
                        if(nMinAddress>nAddress)
                        {
                            nMinAddress = nAddress;
                        }
                        if(nstrWtiteSize%2==0)
                        {
                            if(nMaxAddress<(nAddress+nstrWtiteSize/2 +1))
                            {
                                nMaxAddress = nAddress+nstrWtiteSize/2 +1;
                            }
                        }
                        else
                        {
                            if(nMaxAddress<(nAddress+(nstrWtiteSize+1)/2+1))
                            {
                                nMaxAddress = nAddress+(nstrWtiteSize+1)/2+1;
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

        if(nMinAddress < db->startAddress()|| nMinAddress > db->endAddress()||
           nLength<=0|| nLength > db->endAddress()-db->startAddress()+1)
        {
            if(NULL!=dataDatabuffer)
            {
                delete dataDatabuffer;
            }
            return bRet;
        }
        else
        {
            /*!
              *\section
              * 批量写时如果数据量超过每包要求长度，
              * 需要分包批量写入。下面一段代码计算需要
              * 写多少包以及设置起始地址结束地址总包数。
              */
            uint nBag = (nMaxAddress - nMinAddress+1)/29;
            uint nLastCount = (nMaxAddress - nMinAddress+1)%29;
            if(nLastCount>0)
            {
                nBag = nBag + 1;
            }
            db->setStartAddressBlockWrite(nMinAddress);
            db->setEndAddressBlockWrite(nMaxAddress);
            db->setPackBlockWrite(nBag);

            quint8 nCommandLength = pOMRRegInform->m_uint8_ReadWriteCommand_Length;
            /*!
              *\section
              * 每包数据重新解析（临时buffer中数据每byte数据转成相应的ascall码）。
              */
            for(int i = 1;i<=db->packBlockWrite();i++)
            {
                quint8 tempBuffer[256];
                memset(tempBuffer,0,256);
                quint16 nLength = 0;
                if(i == 1)
                {
                    tempBuffer[0] = 0x40;

                    tempBuffer[1] = dev->station()/10 + 0x30;

                    tempBuffer[2] = dev->station()%10 + 0x30;

                    for(int j = 0 ;j< nCommandLength;j++)
                    {
                        tempBuffer[3+j] = (quint8)pOMRRegInform->m_char_Register_WriteCommand[j];
                    }
                    tempBuffer[nCommandLength+3] = nMinAddress/1000 + 0x30;

                    tempBuffer[nCommandLength+4] = (nMinAddress%1000)/100 + 0x30;

                    tempBuffer[nCommandLength+5] = (nMinAddress%100)/10 + 0x30;

                    tempBuffer[nCommandLength+6] = nMinAddress%10 + 0x30;

                    nLength = nLength +nCommandLength + 7;
                 }
                if(i ==db->packBlockWrite() && nLastCount>0)
                {
                    for(int dataLength = 0;dataLength<nLastCount*2;dataLength++)
                    {
                        quint8 temp[2];
                        temp[0] = (quint8) (dataDatabuffer[(nMinAddress-db->startAddress()+29*(i-1))*2+dataLength]& 0xF);
                        temp[1] = (quint8) ((dataDatabuffer[(nMinAddress-db->startAddress()+29*(i-1))*2+dataLength]>>4) & 0xF);
                        for(int splittwo = 0;splittwo<2;splittwo++)
                        {
                            if(temp[splittwo]<10)
                            {
                                temp[splittwo] = temp[splittwo] + 0x30;
                            }
                            else
                            {
                                temp[splittwo] = temp[splittwo] + 0x37;
                            }
                        }
                        tempBuffer[nLength+2*dataLength+1] = temp[0];
                        tempBuffer[nLength+2*dataLength] = temp[1];
                    }
                    nLength =nLength + nLastCount*4;
                }
                else
                {
                    for(int dataLength = 0;dataLength<29*2;dataLength++)
                    {
                        quint8 temp[2];
                        temp[0] = (quint8) (dataDatabuffer[(nMinAddress-db->startAddress()+29*(i-1))*2+dataLength]& 0xF);
                        temp[1] = (quint8) ((dataDatabuffer[(nMinAddress-db->startAddress()+29*(i-1))*2+dataLength]>>4) & 0xF);
                        for(int splittwo = 0;splittwo<2;splittwo++)
                        {
                            if(temp[splittwo]<10)
                            {
                                temp[splittwo] = temp[splittwo] + 0x30;
                            }
                            else
                            {
                                temp[splittwo] = temp[splittwo] + 0x37;
                            }
                        }
                        tempBuffer[nLength+1+2*dataLength] = temp[0];
                        tempBuffer[nLength+2*dataLength] = temp[1];
                    }
                    nLength =nLength + 29*4 ;
                }
                quint8 btXORLow,btXORHigh;
                XOR(tempBuffer,nLength,btXORLow,btXORHigh);

                tempBuffer[nLength] = btXORHigh;//!异或校验高位

                tempBuffer[nLength+1] = btXORLow;//!异或校验低位
                /*quint16 nWrite_XOR_Result = XOR((uchar*)tempBuffer,nLength);
                tempBuffer[nLength+1] = (nWrite_XOR_Result>>8) & 0xFF;
                tempBuffer[nLength+2] = nWrite_XOR_Result& 0xFF;*/
                nLength = nLength+2;

                if(i ==db->packBlockWrite())//!数据不超一包长度结束符以及最后一包结束符
                {
                    tempBuffer[nLength] = 0x2A;
                    tempBuffer[nLength+1] = 0x0D;
                    nLength = nLength+2;
                    db->setCurrentPackBlockWrite(1);
                }
                else//!数据分包结束符
                {
                    tempBuffer[nLength] = 0x0D;
                    nLength = nLength+1;
                }
                db->cpyTempDBWriteBuffer(tempBuffer,nLength,i-1);
            }
        }
        if(NULL!=dataDatabuffer)
        {
            delete dataDatabuffer;
            dataDatabuffer = NULL;
        }
    }
    quint8 *pSendData = db->tempDBWtiteBuffer(db->currentPackBlockWrite()-1);
    quint16 sendLength = 0;
    quint8 nCommandLength = pOMRRegInform->m_uint8_ReadWriteCommand_Length;
    /*!
      *\section
      * 将构建号的数据包，写入plc。
      */
    if(db->currentPackBlockWrite()<db->packBlockWrite())
    {
        if(db->currentPackBlockWrite() == 1)
        {
            sendLength = 116+nCommandLength+10;
            memcpy(m_sendData,pSendData,sendLength);
        }
        else
        {
            sendLength = 116+3;
            memcpy(m_sendData,pSendData,sendLength);
        }

    }
    else if(db->currentPackBlockWrite()==db->packBlockWrite())
    {
        if(db->currentPackBlockWrite() == 1)
        {
            sendLength = 11+nCommandLength + (db->endAddressBlockWrite() - db->startAddressBlockWrite()+1)*4;
            memcpy(m_sendData,pSendData,sendLength);
        }
        else
        {
            sendLength = 4+( (db->endAddressBlockWrite() - db->startAddressBlockWrite()+1)%29)*4;
            memcpy(m_sendData,pSendData,sendLength);
        }
    }
    setCommandLength(sendLength);
    bRet = true;
    return bRet;
}
quint8* CMDProtocolCommand::getReceiveData()
{
    return m_receiveData;
}
/*!
 * \brief CMDProtocolCommand::parseCommand
    *  现象：当停掉驱动重新运行的时候：
    * 有些数据块会出现协议错误或者超时的现象，现象正常。
    * 原因：获取数据长度过长的时候：
    * 发送读寄存器命令的时候，需要分包发送，未全部发送完
    * 停止运行，plc默认需要命令是分包的下一条，而本驱动却
    * 从第一个数据块开始构建命令，因此报协议错误，协议超时。
 */
ProtocolCommandResult CMDProtocolCommand::parseCommand()
{
    setCommandResult(ProtocolCommandResult::Error);
    CMDDevice *dev = (CMDDevice*)getDevice();
    CMDDataBlock *db = (CMDDataBlock*)getDataBlock();
    if (NULL == dev || NULL == db)
    {
        return getCommandResult();
    }
    //**************************PKX20151104****************************//

    if (getCommandType() == ProtocolCommandType::Read)
    {
        if (getReceiveDataLength() > getReceiveBufferSize())//!收到的数据长度不能超过设置允许收到的长度。
        {
            setReceiveDataLength(0);
            return getCommandResult();
        }
        if(m_receiveData[getReceiveDataLength()-1] == 0x0D)//!无论时候分包结束符一定时0x0D结尾
        {
            if(m_receiveData[getReceiveDataLength()-2] != 0x2A)//!倒数第二位如果不是0x2A：说明不是最后一包
            {
                if(m_splitFlag == 0)//!判断当前包时候是第一包数据，==0时是第一包数据进行发送buffer与返回buffer包头校验
                {
                    /*!
                      *\section
                      *第一包：包头校验
                      */
                    if ((m_sendData[0] != m_receiveData[0])||(m_sendData[1] != m_receiveData[1])||
                            (m_sendData[2] != m_receiveData[2])||(m_sendData[3] != m_receiveData[3])||
                            (m_sendData[4] != m_receiveData[4] )
                            )
                    {
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }
                     if(db->registerType() == ID_MEM_EM)
                     {
                        if((m_sendData[5] != m_receiveData[5])||(m_sendData[6] != m_receiveData[6])||
                                (m_receiveData[7]!= 0x30) ||(m_receiveData[8]!= 0x30)
                                )
                        {
                            setReceiveDataLength(0);
                            return getCommandResult();
                        }
                    }
                    else
                    {
                        if((m_receiveData[5]!= 0x30) ||(m_receiveData[6]!= 0x30))
                        {
                            setReceiveDataLength(0);
                            return getCommandResult();
                        }
                    }
                     /*!
                       *\section
                       *异或码校验
                       */
                     quint8 nXORLow1 = m_receiveData[getReceiveDataLength() - 2];
                     quint8 nXORHigh1 = m_receiveData[getReceiveDataLength() - 3];

                     quint8 nXORLow2= 0;
                     quint8 nXORHigh2 = 0;
                     XOR(m_receiveData,getReceiveDataLength()-3,nXORLow2,nXORHigh2);
                     if (nXORLow1 != nXORLow2 || nXORHigh1 != nXORHigh2)
                     {
                         setReceiveDataLength(0);
                         return getCommandResult();
                     }
                     memcpy(&m_tempReceiveBuffer[m_tempReceiveBufferIndex] , m_receiveData , getReceiveDataLength()-3);

                     m_tempReceiveBufferIndex = m_tempReceiveBufferIndex + getReceiveDataLength() - 3;
                     if(m_tempReceiveBufferIndex < m_readCmdDataLength + m_ushort_Extra_Length_Read)
                     {
                         m_splitFlag ++;
                         setReceiveDataLength(0);
                         setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                         return getCommandResult();
                     }
                     else
                     {
                         setReceiveDataLength(0);
                         return getCommandResult();
                     }
                }
                else if(m_splitFlag<5)//!不时第一包数据不需要进行包头校验
                {
                    /*!
                      *\section
                      *异或码校验
                      */
                    quint8 nXORLow1 = m_receiveData[getReceiveDataLength() - 2];
                    quint8 nXORHigh1 = m_receiveData[getReceiveDataLength() - 3];

                    quint8 nXORLow2= 0;
                    quint8 nXORHigh2 = 0;
                    XOR(m_receiveData,getReceiveDataLength()-3,nXORLow2,nXORHigh2);
                    if (nXORLow1 != nXORLow2 || nXORHigh1 != nXORHigh2)
                    {
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }
                    memcpy(&m_tempReceiveBuffer[m_tempReceiveBufferIndex] , m_receiveData , getReceiveDataLength()-3);

                    m_tempReceiveBufferIndex = m_tempReceiveBufferIndex + getReceiveDataLength() - 3;
                    if(m_tempReceiveBufferIndex < m_readCmdDataLength + m_ushort_Extra_Length_Read)
                    {
                        m_splitFlag ++;
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::RightAndNextCmd);
                        return getCommandResult();
                    }
                    else
                    {
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }

                }
                else
                {
                    setReceiveDataLength(0);
                    return getCommandResult();
                }

            }
            else//!最后一包或者第一包收回的数据
            {

                if(m_splitFlag == 0)
                {
                    /*!
                      *\section
                      *共一包数据包头校验
                      */
                    if ((m_sendData[0] != m_receiveData[0])||(m_sendData[1] != m_receiveData[1])||
                            (m_sendData[2] != m_receiveData[2])||(m_sendData[3] != m_receiveData[3])||
                            (m_sendData[4] != m_receiveData[4] )
                            )
                    {
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }
                     if(db->registerType() == ID_MEM_EM)
                     {
                        if((m_sendData[5] != m_receiveData[5])||(m_sendData[6] != m_receiveData[6])||
                                (m_receiveData[7]!= 0x30) ||(m_receiveData[8]!= 0x30)
                                )
                        {
                            setReceiveDataLength(0);
                            return getCommandResult();
                        }
                    }
                    else
                    {
                        if((m_receiveData[5]!= 0x30) ||(m_receiveData[6]!= 0x30))
                        {
                            setReceiveDataLength(0);
                            return getCommandResult();
                        }
                    }
                     /*!
                       *\section
                       *异或码校验
                       */
                     quint16 nXORLow1 = m_receiveData[getReceiveDataLength() - 3];
                     quint16 nXORHigh1 = m_receiveData[getReceiveDataLength() - 4];

                     quint8 nXORLow2= 0;
                     quint8 nXORHigh2 = 0;
                     XOR(m_receiveData,getReceiveDataLength()-4,nXORLow2,nXORHigh2);

                     if (nXORLow1 != nXORLow2 || nXORHigh1 != nXORHigh2)
                     {
                         setReceiveDataLength(0);
                         return getCommandResult();
                     }
                     memcpy(&m_tempReceiveBuffer[m_tempReceiveBufferIndex] , m_receiveData , getReceiveDataLength());

                     m_tempReceiveBufferIndex = m_tempReceiveBufferIndex + getReceiveDataLength();
                     if(m_tempReceiveBufferIndex == m_readCmdDataLength + m_ushort_Extra_Length_Read)
                     {
                         setReceiveDataLength(0);
                         setCommandResult(ProtocolCommandResult::Right);
                         return getCommandResult();
                     }
                     else
                     {
                         setReceiveDataLength(0);
                         return getCommandResult();
                     }
                }
                else
                {
                    /*!
                      *\section
                      *异或码校验
                      */
                    quint16 nXORLow1 = m_receiveData[getReceiveDataLength() - 3];
                    quint16 nXORHigh1 = m_receiveData[getReceiveDataLength() - 4];

                    quint8 nXORLow2= 0;
                    quint8 nXORHigh2 = 0;
                    XOR(m_receiveData,getReceiveDataLength()-4,nXORLow2,nXORHigh2);
                    if (nXORLow1 != nXORLow2 || nXORHigh1 != nXORHigh2)
                    {
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }
                    memcpy(&m_tempReceiveBuffer[m_tempReceiveBufferIndex] , m_receiveData , getReceiveDataLength());

                    m_tempReceiveBufferIndex = m_tempReceiveBufferIndex + getReceiveDataLength();
                    if(m_tempReceiveBufferIndex == m_readCmdDataLength + m_ushort_Extra_Length_Read)
                    {
                        setReceiveDataLength(0);
                        setCommandResult(ProtocolCommandResult::Right);
                        return getCommandResult();
                    }
                    else
                    {
                        setReceiveDataLength(0);
                        return getCommandResult();
                    }
                }
            }
        }
        else//!如果结束符不是0x0D结尾可能需要分包收取数据，如果收到的数据大于想要的数据长度返回错误。
        {
            if (getReceiveDataLength() >= (m_readCmdDataLength + m_ushort_Extra_Length_Read))
            {
                setReceiveDataLength(0);
                return getCommandResult();
            }
            else if (getReceiveDataLength() < (m_readCmdDataLength + m_ushort_Extra_Length_Read))
            {
                setCommandResult(ProtocolCommandResult::SplitData);
                return getCommandResult();
            }
        }


    }
    else
    {
        if(db->currentPackBlockWrite()==0)
        {
            /*!
              *\section
              *单写相应处理
              */
            if(db->registerType() == ID_MEM_EM)
            {
                if (getReceiveDataLength() > 13)
                {
                    setReceiveDataLength(0);
                    return getCommandResult();
                }
                else if (getReceiveDataLength() < 13)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }

            }
            else
            {
                if (getReceiveDataLength() > 11)
                {
                    setReceiveDataLength(0);
                    return getCommandResult();
                }
                else if (getReceiveDataLength() < 11)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
                }
            }
            if ((m_sendData[0] != m_receiveData[0])||(m_sendData[1] != m_receiveData[1])||
                    (m_sendData[2] != m_receiveData[2])||(m_sendData[3] != m_receiveData[3])||
                    (m_sendData[4] != m_receiveData[4] )
                    )
            {
                setReceiveDataLength(0);
                return getCommandResult();
            }
             if(db->registerType() == ID_MEM_EM)
             {
                if((m_sendData[5] != m_receiveData[5])||(m_sendData[6] != m_receiveData[6])||
                        (m_receiveData[7]!= 0x30) ||(m_receiveData[8]!= 0x30)
                        )
                {
                    setReceiveDataLength(0);
                    return getCommandResult();
                }
            }
            else
            {
                if((m_receiveData[5]!= 0x30) ||(m_receiveData[6]!= 0x30))
                {
                    setReceiveDataLength(0);
                    return getCommandResult();
                }
            }
             quint8 nXORLow1 = m_receiveData[getReceiveDataLength() - 3];
             quint8 nXORHigh1 = m_receiveData[getReceiveDataLength() - 4];
             quint8 nXORLow2= 0;
             quint8 nXORHigh2 = 0;
             XOR(m_receiveData,getReceiveDataLength()-4,nXORLow2,nXORHigh2);
             if (nXORLow1 != nXORLow2 || nXORHigh1 != nXORHigh2)
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
            /*!
              *\section
              *批量写相应的处理
              */
            if(db->currentPackBlockWrite() == db->packBlockWrite())
            {
                 if(db->registerType() == ID_MEM_EM)
                 {
                     if (getReceiveDataLength() < 13)
                     {
                         setCommandResult(ProtocolCommandResult::SplitData);
                         return getCommandResult();
                     }
                 }
                 else
                 {
                     if (getReceiveDataLength() < 11)
                     {
                         setCommandResult(ProtocolCommandResult::SplitData);
                         return getCommandResult();
                     }
                 }

            }
            else
            {
                if (getReceiveDataLength() < 1)
                {
                    setCommandResult(ProtocolCommandResult::SplitData);
                    return getCommandResult();
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

    }
    return getCommandResult();
    //*****************************************************************//

}
//*********************PKX20151104*******************//
void CMDProtocolCommand::XOR(quint8 buffer[], int len,quint8 &btXORLow,quint8 &btXORHigh)
{
    if(len>0)
    {
        quint8 checksum = buffer[0];
        for (int i = 1;  i<len;  i++)
        {
            checksum = checksum ^ buffer[i];
        }
        QString strChecksum=QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper();
        btXORLow = (quint8)strChecksum.at(1).toLatin1();
        btXORHigh = (quint8)strChecksum.at(0).toLatin1();

        /*
        QSettings *configIniWrite = new QSettings("C:\\hahaya.ini", QSettings::IniFormat);
        QString str = QString("%1:%2:%3:%4").arg(len).arg(strChecksum).arg(btXORLow).arg(btXORHigh);
        configIniWrite->setValue("/ip/nXOR", str);
        delete configIniWrite;
        */
    }

      /*
    uchar checksum = 0;
    uchar checksumH,checksumL;
    for (int i = 0;  i<len;  i++)
    {
        checksum = checksum ^ buffer[i];
    }


    checksumH = (checksum  >> 4) & 0x0F;
    checksumL = checksum & 0x0F;

    if (checksumH < 10)
        checksumH = checksumH  +  '0';
    else
        checksumH = (checksumH - 10 ) +  'A';

    if (checksumL < 10)
        checksumL = checksumL  +  '0';
    else
        checksumL = (checksumL - 10 )+  'A';

    return ((checksumH<<8) | checksumL);
    */
}
quint8* CMDProtocolCommand::tempReceiveBuffer()
{
    return m_tempReceiveBuffer;
}
quint16 CMDProtocolCommand::splitFalg()
{
    return m_splitFlag;
}
void CMDProtocolCommand::setsplitFalg(quint16 splitflag)
{
    m_splitFlag = splitflag;
}
//***************************************************//

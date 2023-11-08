#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include <QMap>

//#define MOD_READ_COILS           0x01     //! protocol cmd 1
//#define MOD_READ_DIGITALS        0x02     //! command 2 (read inputs)
//#define MOD_READ_HOLDING         0x03
//#define MOD_READ_REGISTERS       0x04      //! analog input registers
////! writes
//#define MOD_WRITE_SINGLE_COIL    0x05     //! un-implemented?
//#define MOD_WRITE_SINGLEHOLDING  0x06     //! tested, preset single register
//#define MOD_WRITE_MULTIPLE_COILS 0x0F
//#define MOD_WRITE_HOLDING        0x10
////! Extended registers
//#define MOD_READ_EXTENDED        0x14
//#define MOD_WRITE_EXTENDED       0x15

//#define MAX_NUMBER_MEMAREA	5		//! Total number of MemAreas



class CMDProtocolCommand : public CBaseProtocolCommand
{
public:
    CMDProtocolCommand();
    ~CMDProtocolCommand();
public:
    /*!
     * \brief getSendData:
        * 获取发送的数据.
     */
    quint8* getSendData();
    /*!
     * \brief getReceiveData:
        * 获取返回的数据.
     */
    quint8* getReceiveData();
    /*!
     * \section:
        * 命令构建:读命令，写命令，批量写命令.
     */
    void setupCommand();
    bool setupReadCommand();
    bool setupWriteSingleVariableCommand();
    bool setupWriteMultiVariableCommand();
    /*!
     * \brief parseCommand:
        * 对返回命令进行解析.
     */
    ProtocolCommandResult parseCommand();
public:
    void  setTransactionID(ushort transactionID);
    quint16 getTransactionID();
    quint8* tempReceiveBuffer();//!返回临时buffer指针
private:
    quint16 m_transactionID;          //!传输标识符(命令序号）
    quint8 m_sendData[8200];            //!发送缓冲区
    quint8 m_receiveData[1200];         //!接收缓冲区
    quint16 m_readCmdDataLength;       //!需要读取的数据长度
    int m_saveOffset;


};

#endif // MDPROTOCOLCOMMAND_H

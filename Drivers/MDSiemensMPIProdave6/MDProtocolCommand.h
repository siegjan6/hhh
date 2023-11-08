#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"


#define RACK_SLOT        0
#define TSAP             1

#define ID_MEM_I         0
#define ID_MEM_Q         1
#define ID_MEM_M         2
#define ID_MEM_DB        3
#define ID_MEM_C         4
#define ID_MEM_T         5

#define MAX_NUMBER_MEMAREA	6		//!寄存器总数


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
private:
    quint16 m_transactionID;          //!传输标识符
    quint8 m_sendData[256];            //!发送缓冲区
    quint8 m_receiveData[1460];         //!接收缓冲区//PKX  256
    quint16 m_readCmdDataLength;       //!需要读取的数据长度
};

#endif // MDPROTOCOLCOMMAND_H

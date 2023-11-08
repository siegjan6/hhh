#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#define ID_MEM_X        0
#define ID_MEM_Y        1
#define ID_MEM_DX      2
#define ID_MEM_DY      3
#define ID_MEM_M       4
#define ID_MEM_D        5
#define ID_MEM_W       6
#define ID_MEM_B        7
#define ID_MEM_SB        8
#define ID_MEM_SM       9
#define ID_MEM_L        10
#define ID_MEM_F        11
#define ID_MEM_V        12
#define ID_MEM_S       13
#define ID_MEM_TS        14
#define ID_MEM_TC        15
#define ID_MEM_SS        16
#define ID_MEM_SC        17
#define ID_MEM_CS        18
#define ID_MEM_CC       19
#define ID_MEM_SD        20
#define ID_MEM_SW        21
#define ID_MEM_R        22
#define ID_MEM_Z        23
#define ID_MEM_TN        24
#define ID_MEM_SN        25
#define ID_MEM_CN        26



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
    quint16 m_transactionID;          //!传输标识符(命令序号）
    quint8 m_sendData[1300];            //!发送缓冲区
    quint8 m_receiveData[1300];         //!接收缓冲区
    quint16 m_readCmdDataLength;       //!需要读取的数据长度
};

#endif // MDPROTOCOLCOMMAND_H

#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#define SECP_PROTOCOL_VER    10
#define SECP_CMD_RD_TAG			0X01
#define SECP_CMD_RD_DATA		0X02
#define SECP_CMD_WR_DATA		0X03
#define MAX_TAG_NUM                2048

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
    quint8* tempReceiveBuffer();
private:
    quint16 m_transactionID;          //!传输标识符(命令序号）
    quint8 m_sendData[4096];            //!发送缓冲区
    quint8 m_receiveData[4096];         //!接收缓冲区
    quint16 m_readCmdDataLength;       //!需要读取的数据长度
    bool      m_bFirstPack;                         //!收回数据判断第一包标志位
    quint8 m_tempReceiveBuffer[4096];//!临时数据缓冲区
public:
    QMap<qint16,QVariant>tempIDAndVariantMap;//!临时数据缓冲区
};

#endif // MDPROTOCOLCOMMAND_H

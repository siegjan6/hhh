#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

/*!
 * \brief :
    * PLC类型.
 */
#define PLC_FX                0
#define PLC_FX0              1
#define PLC_FX0N           2
#define PLC_FX2N           3

/*!
 * \brief :
    * 寄存器类型.
 */
#define ID_MEM_X           0
#define ID_MEM_Y           1
#define ID_MEM_M          2
#define ID_MEM_S           3
#define ID_MEM_CS         4
#define ID_MEM_TS         5
#define ID_MEM_D          6
#define ID_MEM_C          7
#define ID_MEM_T           8


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
    /*!
     * \brief BCC:
        * 校验.
     */
    unsigned char BCC(int nLength,quint8 *pBCC);
    /*!
     * \brief :
        * 获取设置包头标志位.
     */
    bool bagHeadFlag();
    void setbagHeadFlag(bool baghead);

private:
    quint8 m_sendData[600];            //!发送缓冲区
    quint8 m_receiveData[600];         //!接收缓冲区
    unsigned short m_readCmdDataLength;       //!需要读取的数据长度
    bool    m_bRemovBagHeadFlag;                     //!包头可能不是0x02,去掉包头
};

#endif // MDPROTOCOLCOMMAND_H

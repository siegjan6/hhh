#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

#define ID_MEM_X        0
#define ID_MEM_Y        1
#define ID_MEM_M       2
#define ID_MEM_S        3
#define ID_MEM_TS      4
#define ID_MEM_CS      5
#define ID_MEM_TN      6
#define ID_MEM_CN      7
#define ID_MEM_D        8
#define ID_MEM_R        9

#define MAX_NUMBER_REGISTER 10
typedef struct
{
    int m_nIndex;
    char m_strName[3];
    char m_strRead[3];
    char m_strWrite[3];
    //WORD m_Length;
}struRegisterArea;



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

private:
    quint8 m_sendData[600];            //!发送缓冲区
    quint8 m_receiveData[600];         //!接收缓冲区
    unsigned short m_readCmdDataLength;       //!需要读取的数据长度
};
extern struRegisterArea*GetRegisterStruct(int iIndex);
#endif // MDPROTOCOLCOMMAND_H

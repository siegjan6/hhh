#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

//***********************PKX20151104***********************//
#define ID_MEM_AR  0
#define ID_MEM_DM  1
#define ID_MEM_EM  2
#define ID_MEM_HR  3
#define ID_MEM_IR  4
#define ID_MEM_LR  5
#define ID_MEM_PV  6
#define ID_MEM_TC  7

#define MAX_NUMBER_MEMAREA	8
/*!
  *\section
  * 构建plc相应数据结构
  */
typedef struct
{
    quint8 m_uint_UIRegister_ID;
    char    m_char_Register_Name[3];
    char    m_char_Register_ReadCommand[5];
    char    m_char_Register_WriteCommand[5];
    quint8 m_uint8_ReadWriteCommand_Length;

}OMRRegInform;

//*********************************************************//



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
    quint8 m_sendData[256];            //!发送缓冲区
    quint8 m_receiveData[613];         //!接收缓冲区
    unsigned short m_readCmdDataLength;       //!需要读取的数据长度
//*******************PXK20151104*******************//
private:
    /*!
     * \brief XOR:
        * 异或校验.
     */
    void XOR(quint8 buffer[], int len,quint8 &btXORLow,quint8 &btXORHigh);//异或校验
public:
    ushort m_ushort_Extra_Length_Read;//!收回数据包除数据外的长度

    quint8 m_tempReceiveBuffer[613];//!临时数据缓冲区

    quint16 m_tempReceiveBufferIndex;//!数据分包返回时放到临时buffer位置

    quint16 m_splitFlag;//!分包标志位
public:
    quint8* tempReceiveBuffer();//!返回临时buffer指针
    quint16 splitFalg();//!获取当前分包标志位
    void setsplitFalg(quint16 splitflag);//!设置分包标志位

//*************************************************//
};
extern OMRRegInform* getOMRRegInform(qint16 nRegType);
#endif // MDPROTOCOLCOMMAND_H

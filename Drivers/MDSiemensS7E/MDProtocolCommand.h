#ifndef MDPROTOCOLCOMMAND_H
#define MDPROTOCOLCOMMAND_H
#include "BaseProtocolCommand.h"
#include "MDDevice.h"
#include "MDDataBlock.h"

/*!
  *\section
  * PLC类型
  */
#define PLC_1200         0
#define PLC_1500         1
#define PLC_300           2
#define PLC_400           3
/*!
  *\section
  * 西门子1200 1500 300 400 总体寄存器类型
  */
#define ID_MEM_V        0
#define ID_MEM_I         1
#define ID_MEM_Q        2
#define ID_MEM_AI       3
#define ID_MEM_AQ      4
#define ID_MEM_T         5
#define ID_MEM_C         6
#define ID_MEM_HC      7
#define ID_MEM_M        8
#define ID_MEM_SM      9
#define ID_MEM_S        10
#define ID_MEM_PI       11
#define ID_MEM_PQ     12
#define ID_MEM_DB     13
/*!
  *\section
  * 西门子1200 1500 300 400 总体数据类型
  */
#define S7_DT_Bool              0x01
#define S7_DT_Byte              0x02
#define S7_DT_Word            0x04
#define S7_DT_DWord         0x06
#define S7_DT_Counter       0x1E
#define S7_DT_Timer           0x1F
#define S7_DT_HCounter     0x20

#define MAX_NUMBER_MEMAREA	14		//!寄存器总数
/*!
  *\section
  * 西门子1200 1500 300 400 总体数据结构
  */
typedef struct
{
    qint32 m_nRegType;
    char    m_strName[3];
    quint8 m_ustrRegCode;
    quint8 m_ustrDataType;
}S7RegInform;


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
private:
    /*!
     * \section
     * 通信初始化
     */
    quint16 BuildPLCServerMSG(quint8 local1,quint8 local2,quint8 remote1,quint8 remote2);
    quint16 BuildPLCConnectMSG(quint16 iRanMsgID,CMDDevice *dev);
    /*!
     * \section
     * 通信读写命令构建
     */
    quint16 BuildRead(S7RegInform*pSRegInform,qint16 iMemAdd,quint16 iElements,quint16 iRanMsgID,CMDDataBlock*pdatablock,CMDDevice *dev);
    quint16 BuildWrite(S7RegInform*pSRegInform,IMDRTDBVariable *nsVar,CMDDataBlock*pdatablock ,quint16 iRanMsgID,quint16 Elements);
    quint16 BuildWrite300And400(S7RegInform*pSRegInform,IMDRTDBVariable *nsVar,CMDDataBlock*pdatablock ,quint16 iRanMsgID,quint16 Elements);
    quint16 BuildDBWrite(S7RegInform*pSRegInform,uint iMemAdd,CMDDataBlock*pdatablock ,quint16 iRanMsgID,uint Elements,unsigned char *dataBuffer);
    quint16 BuildDBWrite300And400(S7RegInform*pSRegInform,uint iMemAdd,CMDDataBlock*pdatablock ,quint16 iRanMsgID,uint Elements,unsigned char *dataBuffer);
};
extern S7RegInform* GetS7RegInform(qint16 nRegType,qint16 nPLCType);//PKX

#endif // MDPROTOCOLCOMMAND_H

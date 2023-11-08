#ifndef MDDATABLOCK_H
#define MDDATABLOCK_H
#include "BaseDataBlock.h"
#include <QDomElement>

class CMDDataBlock:public CBaseDataBlock
{
public:
    CMDDataBlock();
    ~CMDDataBlock();
public:
    bool start();
    bool stop();

public:
     void copy(IDataBlock *otherDataBlock);
     /*!
      * \brief setStartAddress:
        *设置开始地址
      * \param address:
        *qint16型开始地址
      */
     void setStartAddress(qint16 address);
     /*!
      * \brief startAddress:
        *获取开始地址
      * \return:
        *返回qint16型开始地址
      */
     qint16 startAddress();
     /*!
      * \brief setEndAddress:
        *设置结束地址
      * \param address:
        *qint16型结束地址
      */
     void setEndAddress(qint16 address);
     /*!
      * \brief endAddress:
        *获取结束地址
      * \return:
        *返回qint16型结束地址
      */
     qint16 endAddress();
     /*!
      * \brief setRegisterType:
        *设置寄存器类型
      * \param registerType:
        *qint16型寄存器类型
      */
     void setRegisterType(qint16 registerType);
     /*!
      * \brief registerType:
        *获取寄存器类型
      * \return:
        *返回qint16型寄存器类型
      */
     qint16 registerType();
public:
     /*!
      * \brief createCommand:
        *创建命令,设置命令与数据块及设备的关联
      * \param cmdType:
        * 命令的类型(读命令,写命令,批量写命令)
      * \return
        *派生类指针传递给基类.
      */
     CBaseProtocolCommand *createCommand(ProtocolCommandType cmdType);
     /*!
      * \brief parseCommand:
        * 命令分析,变量值更新.
      * \param cmd:
        * 基类指针,获取返回命令.
      */
     void parseCommand(CBaseProtocolCommand *cmd);

public:
     /*!
     * \brief checkVarAddress:
        * 变量地址校验.
     * \param nsVar:
        * 变量类指针,获取定义的变量信息.
     * \param error:
        * 无.
     * \return:
        * 当前变量地址校验结果.
     */
    bool checkVarAddress(IMDRTDBVariable* nsVar,QString &error);
    /*!
     * \brief initialVariables:
        *变量初始化
     */
    void initialVariables();
    /*!
     * \brief getDataBufferUShortValue:
        * 从数据缓冲区获取UShort值.
     * \param internalAddress:
        *变量对应在数据缓冲区的偏移地址.
     * \param data:
        *ushort值传递.
     * \return:
        *值获取是否成功.
     */
    bool getDataBufferUShortValue(int internalAddress,  ushort &data);
    /*!
     * \brief refreshVariables:
        * 更新变量.
     */
    void refreshVariables();
    /*!
     * \brief refreshVariablesQuality:
        * 品质变化时更新变量.
     */
    void refreshVariablesQuality();
public:
    /*!
     * \brief getDataBuffer:
        * 批量写的时候获取数据缓冲区
     * \param dataBuf:
        * 数据缓冲区将要拷贝到的buffer.
     * \param offset:
        * 数据缓冲区开始拷贝的位置.
     * \param length:
        * 数据缓冲区拷贝的长度.
     * \return
        * 拷贝是否成功.
     */
    bool getDataBuffer(quint8 *dataBuf, qint16 offset, qint16 length);
    /*!
     * \brief getDataBufferSize:
        * 获取数据缓冲区的总长度.
     * \return:
        * 返回数据缓冲区总长度.
     */
    qint16 getDataBufferSize();
private:
    /*!
     * \brief calculateDataBufferSize:
        * 计算数据缓冲区需要的长度.
     */
    void calculateDataBufferSize();
private:
    qint16 m_dataBufferSize;    //!数据缓冲区大小
    unsigned char *m_dataBuffer;//!数据缓冲区
    QMutex m_dataBufferMutex;

private:
    qint16 m_registerType;      //!寄存器类型
    qint16 m_startAddress;      //!开始地址
    qint16 m_endAddress;        //!结束地址
    QString m_szReadCMD;      //!plc读命令
    QString m_szWriteCMD;      //!plc写命令
    qint16 m_nOffsetAddr;     //!不同plc对应寄存器偏移地址
    qint16 m_nBitOffset;
public:
    /*!
     * \brief :
        * 读写命令设置与获取.
     */
    void setCmdRead(QString readCmd);
    void setCmdWrite(QString writeCmd);
    QString cmdRead();
    QString cmdWrite();
    void setOffsetAddr(qint16 offsetAddr);
    qint16 offsetAddr();
    void setBitOffsetAddr(qint16 bitOffsetAddr);
    qint16 bitOffsetAddr();

public:
    /*!
     * \brief getVariableAddressList:
        * 变量表地址映射函数.
     * \param varAddressList:
        * 变量表
     */
    void getVariableAddressList(QStringList &varAddressList);
public:
    /*!
     * \brief saveConfiguration:
        * 配置写入.
     */
    void saveConfiguration(QDomDocument &doc,QDomElement &parentElement);
    /*!
     * \brief readConfiguration:
        * 配置读出.
     */
    void readConfiguration(QDomElement &domElement);
/*!
\section 运行时动态添加数据块
*/
public:
    /*!
     动态设置参数
     \param deviceParameterMap 数据块参数。
     \returns 无。
     \note
     */
    void dynamicSetParameter(const QVariantMap &dataBlockMap);
};




#endif // MDDATABLOCK_H

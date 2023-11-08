#ifndef MDDATABLOCK_H
#define MDDATABLOCK_H
#include "BaseDataBlock.h"
#include <QDomElement>

class CMDDataBlock:public CBaseDataBlock
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)//!翻译
    Q_PROPERTY(QMap propertyTip READ propertyTip) //!提示
    //*******************************属性框*******************************//
    Q_PROPERTY(QString GUID_DataBlock)
    Q_ENUMS(MDRegister_Pro)
    Q_PROPERTY(MDRegister_Pro Register READ register_Pro WRITE setRegister_Pro)
    Q_PROPERTY(QString StartAddress READ startAddress_Pro WRITE setStartAddress_Pro)
    Q_PROPERTY(QString EndAddress READ endAddress_Pro WRITE setEndAddress_Pro)
    //*******************************************************************//
public:
    CMDDataBlock();
    ~CMDDataBlock();
    enum class MDRegister_Pro
    {
        CoilStatus = 0,
        InputStatus = 1,
        HoldingRegister = 2,
        InputRegister = 3,
        NONE = 4,
    };
public:
    void initTranslateNameAndPropertyType();
    QMap<QString,QVariant> translateName() const;//!翻译函数
    QMap<QString, QVariant> propertyTip() const;//!提示框函数
    //******************************属性框函数*****************************//
    MDRegister_Pro register_Pro();
    void setRegister_Pro(MDRegister_Pro reg);

    QString startAddress_Pro();
    void setStartAddress_Pro(QString address);

    QString endAddress_Pro();
    void setEndAddress_Pro(QString address);
    //*******************************************************************//
private slots:
    void propertyLoadFinish_Pro();
private:
    //******************************属性框变量*****************************//
    QMap<QString,QVariant>       m_translateName;
    QMap<QString,QVariant>        m_propertyTip;
    MDRegister_Pro m_registerType_Pro;
    QString m_startAddress_Pro;
    QString m_endAddress_Pro;
    //!初始报错标志位
    bool m_bError;
    //*******************************************************************//
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
     void setStrStartAddress(QString address);

     QString strStartAddress();

     void setStrEndAddress(QString address);

     QString strEndAddress();

     void setStrRegisterType(QString registerType);

     QString strRegisterType();
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
    bool getDataBufferUShortValue(qint16 internalAddress,  quint16 &data);
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

    int refreshVariable(QString varpath, QVariant value);
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
    quint8* getDataBuffer();
    /*!
     * \brief getDataBufferSize:
        * 获取数据缓冲区的总长度.
     * \return:
        * 返回数据缓冲区总长度.
     */
    qint16 getDataBufferSize();

    QString registerIndexToString(QString index);
    QString registerStringToIndex(QString string);
public:
    /*!
     * \brief calculateDataBufferSize:
        * 计算数据缓冲区需要的长度.
     */
    void calculateDataBufferSize();
private:
    qint16 m_dataBufferSize;    //!数据缓冲区大小
    quint8 *m_dataBuffer;//!数据缓冲区
    QMutex m_dataBufferMutex;

private:
    qint16 m_registerType;      //!寄存器类型
    qint16 m_startAddress;      //!开始地址
    qint16 m_endAddress;        //!结束地址
    QString m_strRegisterType;//!字符串类型寄存器
    QString m_strStartAddress;//!字符串类型开始地址
    QString m_strEndAddress;//!字符串类型结束地址
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
    void serialize(CSortJsonObject& json);
    /*!
     * \brief readConfiguration:
        * 配置读出.
     */
    void readConfiguration(QDomElement &domElement);
    void deserialize(const QJsonObject &json);
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
public:
    int verifyRegisterFormat(QString strRegister,qint16 &nRegister,int &addressFormat,qint16 &nLength);
    int verifyAddressFormat(QString strAddress,qint16 &nAddress,qint16 addressFormat);
    int verifyLength(qint16 startAddress,qint16 endAddress,qint16 nLength);
    void overallVerify();

public:
     bool asyncWriteVariable(IMDRTDBVariable *nsVar);
     bool asyncWriteVariables(QList<IMDRTDBVariable*>& nsVarList);
};




#endif // MDDATABLOCK_H

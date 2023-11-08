#ifndef MDDATABLOCK_H
#define MDDATABLOCK_H
#include "BaseDataBlock.h"
#include <QDomElement>
#include <QList>
#include <QMap>

#define ID_MEM_ORDER_X    0
#define ID_MEM_ORDER_Y    1
#define ID_MEM_ORDER_S    2
#define ID_MEM_ORDER_M    3
#define ID_MEM_ORDER_SPM  4
#define ID_MEM_ORDER_TS   5
#define ID_MEM_ORDER_CS16 6
#define ID_MEM_ORDER_CS32 7
#define ID_MEM_ORDER_T    8
#define ID_MEM_ORDER_C32  9
#define ID_MEM_ORDER_D    10
#define ID_MEM_ORDER_SPD  11
#define ID_MEM_ORDER_C16  12

#define ID_REG_RANGE_ORDER_X    0
#define ID_REG_RANG_ORDER_Y     1
#define ID_REG_RANG_ORDER_S     2
#define ID_REG_RANG_ORDER_M     3
#define ID_REG_RANG_ORDER_SPM   4
#define ID_REG_RANG_ORDER_T     5
#define ID_REG_RANG_ORDER_C16   6
#define ID_REG_RANG_ORDER_C32   7
#define ID_REG_RANG_ORDER_D     8
#define ID_REG_RANG_ORDER_SPD   9
//!寄存器 开始地址 结束地址 偏移地址数据结构

class MDDataBlockConfig
{
public:
    MDDataBlockConfig(){}
    ~MDDataBlockConfig(){}
public:
    QString m_strRegister;
    QString m_strStartAddress;
    QString m_strEndAddress;

    qint16 m_nRegister;
    qint32 m_mdtartAddress;
    qint32 m_nEndAddress;
    qint16 m_nArrayLength;
    qint32 m_nArraySubScript;
    qint32 m_nOffSet;

};

typedef QList<MDDataBlockConfig*> listNoOrderRegistersConfig;

typedef QMap<QString,listNoOrderRegistersConfig> mapOrderRegistersConfig;

typedef QMap<int,listNoOrderRegistersConfig> mapInitialCommandPackage;

class CMDDataBlock:public CBaseDataBlock
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)//!翻译
    Q_PROPERTY(QMap propertyTip READ propertyTip) //!提示
    //*******************************属性框*******************************//
    Q_PROPERTY(QString GUID_DataBlock)
    Q_PROPERTY(QString DILRegistersConfig READ registersConfig_Pro WRITE setRegistersConfig_Pro)
    //*******************************************************************//
public:
    CMDDataBlock();
    ~CMDDataBlock();
public:
    void initTranslateNameAndPropertyType();
    QMap<QString,QVariant> translateName() const;//!翻译函数
    QMap<QString, QVariant> propertyTip() const;//!提示框函数

    QString registersConfig_Pro();
    void setRegistersConfig_Pro(QString registerConfig);
private:
    //******************************属性框变量*****************************//
    QMap<QString,QVariant>       m_translateName;
    QMap<QString,QVariant>        m_propertyTip;
    QString m_registersConfig_Pro;
    //*******************************************************************//
public:
    bool start();
    bool stop();
public:
    void copy(IDataBlock *otherDataBlock);

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
    bool getDataBuffer(quint8 *dataBuf, qint32 offset, qint32 length);
    /*!
     * \brief getDataBufferSize:
        * 获取数据缓冲区的总长度.
     * \return:
        * 返回数据缓冲区总长度.
     */
    qint32 getDataBufferSize();
    /*!
     * \brief calculateDataBufferSize:
        * 计算数据缓冲区需要的长度.
     */
    void calculateDataBufferSize(qint32 bufferSize);
    /*!
     * \brief setListRegistersConfigDownLoad:
        *当前数据块所有寄存器配置设置
     */
    void setListRegistersConfigDownLoad(QString strConfig);

    /*!
     * \brief setListRegistersConfig:
        *当前数据块所有寄存器获取
     */

    void listRegistersConfig(listNoOrderRegistersConfig &config);
    /*!
     * \brief listRegistersConfigToFile:
        *存文件时返回字符串
     */
    QString listRegistersConfigToFile();

    void listRegistersCongigClear();

    void setMapRegistersConfig(listNoOrderRegistersConfig &config);

    void setInitialMapPackage(listNoOrderRegistersConfig &config);

    void InitialMapPackage(listNoOrderRegistersConfig &config,int currentPackage);

    void setOverallPackage(int package);
    int overallPackage();

    void setCurrentPackage(int currentPackage);
    int currentPackage();

    quint8 *pointerOfDataBuffer();

    void insertToVarForStringMap(QString mapKey,QString reg,qint32 address,qint32 &offset);

    qint32 endAddressFromVarForStringMap(QString mapKey);

    bool verifyLoadSpace(listNoOrderRegistersConfig &config);

    void setPackBlockWrite(uint allpack);
    uint  packBlockWrite();

    void setCurrentPackBlockWrite(uint currentpack);
    uint  currentPackBlockWrite();

    void cpyTempDBWriteBuffer(quint8 * pbuffer,uint cpyLength,int nwei);
    quint8 * tempDBWtiteBuffer(int nwei);

private:
    qint32 m_dataBufferSize;    //!数据缓冲区大小
    quint8 *m_dataBuffer;//!数据缓冲区
    QMutex m_dataBufferMutex;
    int m_overallPackage;
    int m_currentPackage;

    uint m_packDBCount;//!分包总数
    uint m_currentDBPack;//!当前包
    quint8 m_DBWriteArray[2][8200];//!每包数据
private:
    mapOrderRegistersConfig mapDataRegistersConfig;
    listNoOrderRegistersConfig listDataRegistersConfig;
    mapInitialCommandPackage  mapInitialPackage;
    QMap<QString,quint32> mapVarForString;
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
    //!寄存器 地址 长度 校验
    int verifyRegisterFormat(QString strRegister,int &addressFormat);
    int verifyAddressFormat(QString strAddress,qint32 &nAddress,qint16 addressFormat);
    int verifyLength(qint32 startAddress,qint32 endAddress);
    //!整个数据块的校验
    void overallVerify(listNoOrderRegistersConfig &config);
    //!通信获取各寄存器最大点数,通过verifyRegisterAddress校验设置地址是否超限
    bool verifyRegisterAddress(qint32 maxPoint,QString strFindRegister,QString strFindStatusRegister = "");
    bool verifyVarAddress(QString reg,qint32 address,qint32 &offset,qint32 &subOffset);
};




#endif // MDDATABLOCK_H

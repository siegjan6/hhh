#ifndef MDDATABLOCK_H
#define MDDATABLOCK_H
#include "BaseDataBlock.h"
#include "MDDevice.h"
#include <QDomElement>

#define DB_NUMBER_RIGHT 0
#define DB_NUMBER_ERROR 1

#define FLAG_DB_NUMBER_ERROR 6

class CMDDataBlock:public CBaseDataBlock
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)//!翻译
    Q_PROPERTY(QMap propertyTip READ propertyTip) //!提示
    //*******************************属性框*******************************//
    Q_PROPERTY(QString GUID_DataBlock)
    Q_ENUMS(MDRegister_Pro)
    Q_PROPERTY(MDRegister_Pro Register READ register_Pro WRITE setRegister_Pro)
    Q_PROPERTY(QString DBRegisterNum READ dbRegisterNum_Pro WRITE setDBRegisterNum_Pro)
    Q_PROPERTY(QString StartAddress READ startAddress_Pro WRITE setStartAddress_Pro)
    Q_PROPERTY(QString EndAddress READ endAddress_Pro WRITE setEndAddress_Pro)
    //*******************************************************************//
public:
    CMDDataBlock();
    ~CMDDataBlock();
    enum class MDRegister_Pro
    {
        V = 0,
        I = 1,
        Q = 2,
        AI = 3,
        AQ = 4,
        T = 5,
        C = 6,
        HC = 7,
        M = 8,
        SM = 9,
        S = 10,
        PI = 11,
        PQ = 12,
        DB = 13,
        NONE = 14,
    };
public:
    void initTranslateNameAndPropertyType();
    QMap<QString,QVariant> translateName() const;//!翻译函数
    QMap<QString, QVariant> propertyTip() const;//!提示框函数
    //******************************属性框函数*****************************//
    MDRegister_Pro register_Pro();
    void setRegister_Pro(MDRegister_Pro reg);

    QString dbRegisterNum_Pro();
    void setDBRegisterNum_Pro(QString num);

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
    QString m_dbRegisterNum_Pro;
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
     qint16 getStartAddress();
     /*!
      * \brief setEndAddress:
        *设置结束地址
      * \param address:
        *qint16型结束地址
      */
     void setEndAddress(qint16 address);
     qint16 getEndAddress();
     //寄存器类型
     void setRegisterType(qint16 registerType);
     qint16 getRegisterType();
     //DB数据块编号
     void setDBRegisterNum(qint16 DBregisterNum);//PKX
     qint16 getDBRegisterNum();//PKX
public:
     void setStrStartAddress(QString address);

     QString strStartAddress();

     void setStrEndAddress(QString address);

     QString strEndAddress();

     void setStrRegisterType(QString registerType);

     QString strRegisterType();

     void setstrDBNumber(QString dbnumber);

     QString strDBNumber();
//命令管理
public:
     //创建命令
     CBaseProtocolCommand *createCommand(ProtocolCommandType cmdType);
     //分析命令:变量值的更新函数中完成
     void parseCommand(CBaseProtocolCommand *cmd);
//变量管理
public:
    //变量地址校验
    bool checkVarAddress(IMDRTDBVariable* nsVar,QString &error);
    //变量初始化
    void initialVariables();
    //从数据缓冲区获取UShort值
    bool getDataBufferUShortValue(qint16 internalAddress,  quint16 &data);
    //更新变量
    void refreshVariables();
    //品质变化时更新变量
    void refreshVariablesQuality();
    //*************************PKX***********************//
    void variablesAddressReflect(QStringList &varAddressList);
    //***************************************************//
//数据缓冲区
public:
    //获取数据缓冲区
    bool getDataBuffer(quint8 *dataBuf, qint16 offset, qint16 length);
    //数据缓冲区大小
    qint16 getDataBufferSize();
    QString registerIndexToString(QString index);
    QString registerStringToIndex(QString string);
public:
    void calculateDataBufferSize();
private:
    qint16 m_dataBufferSize;    //数据缓冲区大小
    quint8 *m_dataBuffer;//数据缓冲区
    QMutex m_dataBufferMutex;
//扩展的属性
private:
    qint16 m_registerType;      //寄存器类型
    qint16 m_startAddress;      //开始地址
    qint16 m_endAddress;        //结束地址
    qint16 m_DBRegisterNum; //DB数据块编号//PKX
    QString m_strRegisterType;//!字符串类型寄存器
    QString m_strStartAddress;//!字符串类型开始地址
    QString m_strEndAddress;//!字符串类型结束地址
    QString m_strDBRegisterNum;//!字符串类型DB块编号
public:
    void saveConfiguration(QDomDocument &doc,QDomElement &parentElement);
    void serialize(CSortJsonObject& json);
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
    int verifyDBNumber(QString strNumber,qint16 &nNumber);
    void overallVerify();

};




#endif // MDDATABLOCK_H

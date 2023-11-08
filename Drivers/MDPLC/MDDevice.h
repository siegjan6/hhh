#ifndef MDDEVICE_H
#define MDDEVICE_H
#include "BaseDevice.h"
#include <QJsonObject>
#include <QJsonArray>
#define PROCESS_GETCONFIG 0
#define PROCESS_INITIAL   1
#define PROCESS_GETDATA   2
class CMDDevice:public CBaseDevice
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)//!翻译
    Q_PROPERTY(QMap propertyTip READ propertyTip) //!提示
    //***********************************通信参数**********************************//
    Q_PROPERTY(QString GUID_CommunicationParameter)
    Q_ENUMS(MDCommunicationType_Pro)
    Q_PROPERTY(MDCommunicationType_Pro CommunicationTypeP READ communicationType_Pro WRITE setCommunicationType_Pro)
    //!串口
    Q_PROPERTY(QString SerialPort READ serialPort_Pro WRITE setSerialPort_Pro)
    Q_ENUMS(MDSerialBaud_Pro)
    Q_PROPERTY(MDSerialBaud_Pro SerialBaud READ serialBaud_Pro WRITE setSerialBaud_Pro)
    Q_ENUMS(MDSerialDataBit_Pro)
    Q_PROPERTY(MDSerialDataBit_Pro SerialDataBit READ serialDataBit_Pro WRITE setSerialDataBit_Pro)
    Q_ENUMS(MDSerialStopBit_Pro)
    Q_PROPERTY(MDSerialStopBit_Pro SerialStopBit READ serialStopBit_Pro WRITE setSerialStopBit_Pro)
    Q_ENUMS(MDSerialParityBit_Pro)
    Q_PROPERTY(MDSerialParityBit_Pro SerialParityBit READ serialParityBit_Pro WRITE setSerialParityBit_Pro)
    //!以太
    Q_ENUMS(MDClientOrServer_Pro)
    Q_PROPERTY(MDClientOrServer_Pro  ClientOrServer READ clientOrServer_Pro WRITE setcClientOrServer_Pro)
    Q_PROPERTY(QString TcpOrUdpIP READ tcpOrUdpIP_Pro WRITE setTcpOrUdpIP_Pro)
    Q_PROPERTY(QString TcpOrUdpRemotePort READ tcpOrUdpRemotePort_Pro WRITE setTcpOrUdpRemotePort_Pro)
    Q_PROPERTY(QString TcpOrUdpLocalPort READ tcpOrUdpLocalPort_Pro WRITE setTcpOrUdpLocalPort_Pro)
    //!USB
    Q_PROPERTY(QString UsbParameter READ usbParameter_Pro WRITE setUsbParameter_Pro)
    //!板卡
    Q_PROPERTY(QString BoardParameter READ boardParameter_Pro WRITE setBoardParameter_Pro)
    //!其他
    Q_PROPERTY(QString OtherParameter READ otherParameter_Pro WRITE setOtherParameter_Pro)
    //******************************************************************************//

    //***********************************设备参数*************************************//
    Q_PROPERTY(QString GUID_DeviceParameter)
    Q_PROPERTY(QString CommunicationProtocol READ communicationProtocol_Pro)
    Q_PROPERTY(QString ReplyTimeout READ replyTimeout_Pro WRITE setReplyTimeout_Pro)
    Q_PROPERTY(QString Retries READ retries_Pro WRITE setRetries_Pro)
    Q_PROPERTY(QString Station READ station_Pro WRITE setStation_Pro)
    //*******************************************************************************//
public:
    CMDDevice();
    ~CMDDevice();
    //************************属性框枚举类-通信参数*************************//
    enum class  MDCommunicationType_Pro
    {
        Serial = 0,     //串口通信
        TCP = 1,        //TCP通信
        UDP = 2,        //UDP通信
        USB = 3,		//USB
        Board = 4,      //板卡
        Other = 5,	    //其它
    };

    enum class  MDSerialBaud_Pro
    {
        Baud_115200 = 0,
        Baud_57600 = 1,
        Baud_38400 = 2,
        Baud_19200 = 3,
        Baud_9600 = 4,
        Baud_4800 = 5,
        Baud_2400 = 6,
        Baud_1200 = 7,
    };
    enum class  MDSerialDataBit_Pro
    {
        DataBit_5 = 0,
        DataBit_6 = 1,
        DataBit_7 = 2,
        DataBit_8 = 3,
    };
    enum class  MDSerialStopBit_Pro
    {
        StopBit_1 = 0,
        StopBit_1_5 = 1,
        StopBit_2 = 2,
    };
    enum class  MDSerialParityBit_Pro
    {
        NONE = 0,
        ODD = 1,
        EVEN = 2,
        MARK = 3,
        SPACE = 4,
    };
    enum class  MDClientOrServer_Pro
    {
        Client = 0,
        Server = 1,
    };
    //***********************************************************//
public:
    void initTranslateNameAndPropertyType();
    QMap<QString,QVariant> translateName() const;//!翻译函数
    QMap<QString, QVariant> propertyTip() const;//!提示框函数
    //************************属性框函数-通信参数***************************//
    MDCommunicationType_Pro communicationType_Pro();//!通信类型读函数
    void setCommunicationType_Pro(MDCommunicationType_Pro model);//!通信类型写函数
    //!串口函数
    QString serialPort_Pro();
    void setSerialPort_Pro(QString port);

    MDSerialBaud_Pro serialBaud_Pro();
    void setSerialBaud_Pro(MDSerialBaud_Pro baud);

    MDSerialDataBit_Pro serialDataBit_Pro();
    void setSerialDataBit_Pro(MDSerialDataBit_Pro dataBit);

    MDSerialStopBit_Pro serialStopBit_Pro();
    void setSerialStopBit_Pro(MDSerialStopBit_Pro stopBit);

    MDSerialParityBit_Pro serialParityBit_Pro();
    void setSerialParityBit_Pro(MDSerialParityBit_Pro parityBit);
    //!以太函数
    MDClientOrServer_Pro clientOrServer_Pro();
    void setcClientOrServer_Pro(MDClientOrServer_Pro clientOrServer);

    QString tcpOrUdpIP_Pro();
    void setTcpOrUdpIP_Pro(QString ip);

    QString tcpOrUdpLocalPort_Pro();
    void setTcpOrUdpLocalPort_Pro(QString localPort);

    QString tcpOrUdpRemotePort_Pro();
    void setTcpOrUdpRemotePort_Pro(QString remotePort);
    //!USB
    QString usbParameter_Pro();
    void setUsbParameter_Pro(QString parameter);
    //!板卡
    QString boardParameter_Pro();
    void setBoardParameter_Pro(QString parameter);
    //!其他
    QString otherParameter_Pro();
    void setOtherParameter_Pro(QString parameter);
    //!根据通信类型屏蔽其他通信配置
    void hideAllAndShowOne_Pro(CommunicationType comm);
    //*******************************************************************//
    //************************属性框函数-设备参数***************************//
    //!通信协议
    QString communicationProtocol_Pro();
    //!超时时间
    QString replyTimeout_Pro();
    void setReplyTimeout_Pro(QString timeout);
    //!重试次数
    QString retries_Pro();
    void setRetries_Pro(QString retries);
    //!站号
    QString station_Pro();
    void setStation_Pro(QString station);
    //*******************************************************************//
private slots:
    void propertyLoadFinish_Pro();

private:
    //************************属性框变量-通信参数***************************//
    QMap<QString,QVariant>       m_translateName;
    QMap<QString,QVariant>        m_propertyTip;
    MDCommunicationType_Pro m_communicationModel;//!通信类型变量
    //!串口变量
    QString m_serialPort;
    MDSerialBaud_Pro m_serialBaud;
    MDSerialDataBit_Pro m_serialDataBit;
    MDSerialStopBit_Pro m_serialStopBit;
    MDSerialParityBit_Pro m_serialParityBit;
    //!以太
    MDClientOrServer_Pro m_clientOrServer;
    QString m_tcpOrUdpIP;
    QString m_tcpOrUdpLocalPort;
    QString m_tcpOrUdpRemotePort;
    //!USB
    QString m_usbParameter;
    //!板卡
    QString m_boardParameter;
    //!其他
    QString m_otherParameter;
    //!初始隐藏标志位
    bool m_bHide;
    //******************************************************************//

    //************************属性框变量-设备参数***************************//
    QString m_replyTimeout_Pro;
    QString m_retries_Pro;
    QString m_station_Pro;
    QString m_commProtocol_Pro;
    //******************************************************************//
public:
   bool start();
   void copy(IDevice *otherDevice);
   /*!
     * \brief station:
        *  站号获取.
     */
    qint16 station();
    /*!
      * \brief setStation:
         *  设置站号.
      */
    void setStation(qint16 station);
    /*!
      * \brief configDecice:
         *  配置device界面.
      */
    void configDecice(void *parent = 0);
    /*!
     * \brief createDataBlock
        * 创建数据块.
     */
    IDataBlock *createDataBlock(void *parent = 0,bool config = true);
    /*!
     * \brief configDataBlock:
        * 配置数据块.
     */
    bool configDataBlock(IDataBlock *dataBlock,void *parent = 0);
    /*!
     * \brief commType:
        * 获取通信类型.
     */
    CommunicationType commType();
    /*!
     * \brief setCommType:
        * 设置通信类型.
     */
    void setCommType(CommunicationType commType);
    /*!
     * \brief commParameter:
        * 获取通信参数（如串口：COM口，Baud等）.
     */
    QString commParameter();
    /*!
     * \brief setCommParameter:
        * 设置通信参数（如串口：COM口，Baud等）.
     */
    void setCommParameter(const QString &commParameter);
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
     * \brief
        * 设置获取通信流程标志位
     */
    void setCommunicationProcessFlag(qint16 communication);
    qint16 communicationProcessFlag();

    QString commTypeEnumToString(CommunicationType commtype);
    CommunicationType commTypeStringToEnum(QString strcommtype);

    QString commParameterIndexToString(QString parameter);
    QString commParameterStringToIndex(QString parameter);
private:
    qint16 m_station;//!站号
    QString m_commParameter;//!通信参数
    CommunicationType m_commType;//!通信类型
    qint16 m_communicationProcessFlag;//!通信步骤0--获取寄存配置 1--初始化 2--正常读数据

public:
    /*!
     * \brief addVariable:
        * 添加变量.
     */
    bool addVariable(IMDRTDBVariable* nsVar,QString &error);
    /*!
     * \brief removeVariable:
        * 移除变量.
     */
    bool removeVariable(IMDRTDBVariable* nsVar);
    /*!
     * \brief checkVarAddress:
        * 验证变量地址.
     */
    bool checkVarAddress(IMDRTDBVariable* nsVar,QString &error);
    /*!
     * \brief getVariableAddressList:
        * 获取可用的变量地址链表.
     */
    void getVariableAddressList(QStringList &varAddressList);

/*!
\section 运行时动态添加设备
*/
public:

    /*!
     动态设置参数
     \param deviceParameterMap 设备参数。
     \returns 无。
     \note
     */
    void dynamicSetParameter(const QVariantMap &deviceParameterMap);
};




#endif // MDDEVICE_H

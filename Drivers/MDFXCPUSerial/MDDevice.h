#ifndef MDDEVICE_H
#define MDDEVICE_H
#include "BaseDevice.h"

class CMDDevice:public CBaseDevice
{
public:
    CMDDevice();
    ~CMDDevice();
public:
    void copy(IDevice *otherDevice);
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
    void setCommParameter(const QString & commParameter);
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
     * \brief PLCType:
        * 获取plc类型.
     */
    qint16 PLCType();
    /*!
     * \brief setPLCType:
        * 设置plc类型.
     */
    void setPLCType(qint16 PLCType);
private:
    QString m_commParameter;//!通信参数
    CommunicationType m_commType;//!通信类型

    qint16 m_PLCType;
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

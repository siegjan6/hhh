#ifndef MDDATABLOCK_H
#define MDDATABLOCK_H
#include "BaseDataBlock.h"
#include <QDomElement>
#include<QMap>

class CMDDataBlock:public CBaseDataBlock
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)//!翻译
    Q_PROPERTY(QMap propertyTip READ propertyTip) //!提示
    //*******************************属性框*******************************//
    Q_PROPERTY(QString GUID_DataBlock)
    Q_PROPERTY(QString DILTabVar READ tabVar_Pro WRITE setTabVar_Pro)
    //*******************************************************************//
public:
    CMDDataBlock();
    ~CMDDataBlock();
public:
    void initTranslateNameAndPropertyType();
    QMap<QString,QVariant> translateName() const;//!翻译函数
    QMap<QString, QVariant> propertyTip() const;//!提示框函数
    QString tabVar_Pro();
    void setTabVar_Pro(QString tabVar);
private:
    //******************************属性框变量*****************************//
    QMap<QString,QVariant>       m_translateName;
    QMap<QString,QVariant>        m_propertyTip;
    QString m_tabVar_Pro;
    //*******************************************************************//
public:
    bool start();
    bool stop();
public:
     void copy(IDataBlock *otherDataBlock);
     //初始化参数
     //void initialParameters(IDataBlock *db);

     /*!
      * \brief setTabNumber:
        *设置位号表序号
      * \param number:
        *qint16型位号表序号
      */
     void setTabNumber(qint16 number);
     /*!
      * \brief tabNumber:
        *获取位号表序号
      * \return:
        *返回qint16型位号表序号
      */
     qint16 tabNumber();
     /*!
      * \brief setDataTabNumber:
        *设置数据表序号
      * \param number:
        *qint16型位号表序号
      */
     void setDataTabNumber(qint16 number);
     /*!
      * \brief dataTabNumber:
        *获取位号表序号
      * \return:
        *返回qint16型数据表序号
      */
     qint16 dataTabNumber();

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
    //变量地址校验
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
    quint8 *m_dataBuffer;//!数据缓冲区

    QMutex m_dataBufferMutex;//!数据缓冲区读写保护
public:
    QMap<qint16,QVariant> m_dataBufferVariantMap;//!数据缓冲区
    quint8 m_tag[8];//!标签
private:
    qint16 m_tabNumber;//!位号表序号
    qint16 m_dataTabNumber;//!数据表序号
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

};




#endif // MDDATABLOCK_H

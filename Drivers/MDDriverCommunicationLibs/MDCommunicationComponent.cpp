#include "MDCommunicationComponent.h"
#include "MDTCPCommunicationObject.h"
#include "MDSerialPortCommunicationObject.h"
#include "MDUDPCommunicationObject.h"
#include "MDOPCUaCommunicationObject.h"
/*
CMDCommunicationComponent::CMDCommunicationComponent()
{
    m_pCurrentInstance = NULL;
    m_pCurrentEventCallBack = NULL;
    m_commParameter = "";
    m_primaryCommParameter = "";
    m_backupCommParameter="";
    m_responesTimeout = 3000;
    m_commType = CommunicationType::Serial;
}
CMDCommunicationComponent::~CMDCommunicationComponent()
{
    if(NULL != m_pCurrentInstance)
    {
        m_pCurrentInstance->stopConnect();
        delete m_pCurrentInstance;
    }
}
QString CMDCommunicationComponent::getCommParameter()
{
    return m_commParameter;
}

void  CMDCommunicationComponent::setCommParameter(QString commParameter)
{
    m_commParameter = commParameter;
    try
    {
        QStringList  param = m_commParameter.split(";");
        if (param.count() >= 2)
        {
            m_primaryCommParameter = param[0];
            m_backupCommParameter = param[1];
        }
        else if (param.count() >= 1)
        {
            m_primaryCommParameter = param[0];
        }
    }
    catch (...)
    {
    }
    m_primaryCommParameter=m_primaryCommParameter.trimmed();
    m_backupCommParameter=m_backupCommParameter.trimmed();
}
//通信类型
CommunicationType CMDCommunicationComponent::getCommType()
{
    return m_commType;
}

void CMDCommunicationComponent::setCommType(CommunicationType commType)
{
    m_commType = commType;
}

qint16 CMDCommunicationComponent::getResponesTimeout()
{
    return m_responesTimeout;
}

void CMDCommunicationComponent::setResponesTimeout(qint16 timeOut)
{
    m_responesTimeout = timeOut;
}
CommunicationStatus CMDCommunicationComponent::getCommStatus()
{
    if (NULL!=m_pCurrentInstance)
    {
        return m_pCurrentInstance->getCommStatus();
    }
    return CommunicationStatus::DisConnected;
}
void CMDCommunicationComponent::setCommStatus(CommunicationStatus commStatus)
{
    if (NULL!=m_pCurrentInstance)
    {
        m_pCurrentInstance->setCommStatus(commStatus);
    }
}

qint16 CMDCommunicationComponent::setEventCallback( IEventCallback *onEvent)
{
    m_pCurrentEventCallBack = onEvent;
    return 1;
}

qint16 CMDCommunicationComponent::switchChannel()
{
    int ret =0;
    if (NULL != m_pCurrentInstance)
    {
        m_pCurrentInstance->stopConnect();
        try
        {
            if((m_backupCommParameter.isEmpty()||m_backupCommParameter.isNull())== false &&
               QString::compare(m_pCurrentInstance->getCommParameter(),m_backupCommParameter,Qt::CaseInsensitive)!=0)
            {
                m_pCurrentInstance->setCommParameter(m_backupCommParameter);
            }
            else
            {
                m_pCurrentInstance->setCommParameter(m_primaryCommParameter);
            }
        }
        catch (...)
        {
            ret = 0;
        }
        ret = m_pCurrentInstance->startConnect();
    }
    return ret;
}

qint16 CMDCommunicationComponent::startConnect()
{
    qint16 ret = 1;
    if (getCommStatus() != CommunicationStatus::Connected)
    {
        try
        {
            if (NULL == m_pCurrentInstance)
            {
                switch (m_commType)
                {
                    case CommunicationType::Serial:
                        m_pCurrentInstance = new CMDSerialPortCommunicationObject();
                        break;
                    case CommunicationType::TCP:
                        m_pCurrentInstance = new CMDTCPCommunicationObject();
                        break;
                    case CommunicationType::UDP:
                        //mCurrentInstance=new _UDP();
                        break;
                    case CommunicationType::USB:
                        //mCurrentInstance=new _USB();
                        break;
                    case CommunicationType::Board:
                        //mCurrentInstance=new _Board();
                        break;
                    case CommunicationType::Other:
                        //mCurrentInstance=new _Other();
                        break;
                    default:
                        break;
                }
            }

            if (NULL != m_pCurrentInstance)
            {
                m_pCurrentInstance->setResponesTimeout(m_responesTimeout);
                m_pCurrentInstance->setCommParameter(m_primaryCommParameter);
                m_pCurrentInstance->setEventCallback(m_pCurrentEventCallBack);
                ret = m_pCurrentInstance->startConnect();
            }
            else
            {
                ret= 0;
            }
        }
        catch (...)
        {
            ret = 0;
        }
    }
    return ret;
}

qint16 CMDCommunicationComponent::stopConnect()
{
    if (NULL!=m_pCurrentInstance)
    {
        return m_pCurrentInstance->stopConnect();
    }
    return 0;
}

qint16 CMDCommunicationComponent::sendData(quint8 *buffer, qint16 count)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->sendData(buffer,count);
    }
    return 0;
}

qint16 CMDCommunicationComponent::sendData(quint8 *buffer, qint16 count, const  QObject *socketObj)
{
    if (NULL != m_pCurrentInstance)
    {
       // return m_pCurrentInstance->sendData(buffer, count, socketObj);
    }
    return 0;
}

qint16 CMDCommunicationComponent::getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->getData(buffer, startIndex,bufferSize);
    }
    return 0;
}

qint16 CMDCommunicationComponent::getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize, const QObject &mSocket)
{
    if (NULL != m_pCurrentInstance)
    {
        //return m_pCurrentInstance->getData(buffer, startIndex,bufferSize,mSocket);
    }
    return 0;
}
*/

CMDCommunicationComponent::CMDCommunicationComponent()
{
    m_pCurrentInstance = NULL;
    m_commParameter = "";
    m_primaryCommParameter = "";
    m_backupCommParameter="";
    m_commType = CommunicationType::Serial;
}
CMDCommunicationComponent::~CMDCommunicationComponent()
{
    if(NULL != m_pCurrentInstance)
    {
        m_pCurrentInstance->disConnect(1000);
        if(m_commType == CommunicationType::Serial)
        {
            delete (CMDSerialPortCommunicationObject*)m_pCurrentInstance;
        }
        else if(m_commType == CommunicationType::TCP||m_commType == CommunicationType::UDP)
        {
            delete (CMDTCPCommunicationObject*)m_pCurrentInstance;
        }
        else if(m_commType == CommunicationType::OPCUA)
        {
            delete (CMDOPCUaCommunicationObject*)m_pCurrentInstance;
        }

        m_pCurrentInstance= NULL;
    }
}
/*!
\section 获取、设置通信参数
*/
QString CMDCommunicationComponent::getCommParameter()
{
    return m_commParameter;
}
/*!
\section 以分号隔开主从参数
*/
void  CMDCommunicationComponent::setCommParameter(QString commParameter)
{
    m_commParameter = commParameter;
    try
    {
        QStringList  param = m_commParameter.split(";");
        if (param.count() >= 2)
        {
            m_primaryCommParameter = param[0];
            m_backupCommParameter = param[1];
        }
        else if (param.count() >= 1)
        {
            m_primaryCommParameter = param[0];
        }
    }
    catch (...)
    {
    }
    m_primaryCommParameter=m_primaryCommParameter.trimmed();
    m_backupCommParameter=m_backupCommParameter.trimmed();
}

/*!
\section 获取、设置通信介质
*/
CommunicationType CMDCommunicationComponent::getCommType()
{
    return m_commType;
}

void CMDCommunicationComponent::setCommType(CommunicationType commType)
{
    m_commType = commType;
}

/*!
\section 获取、设置通信状态
*/
CommunicationStatus CMDCommunicationComponent::getCommStatus()
{
    //!如果是other或Board,显示为连接状态
    //!如果COM口>=9000,默认为调用第三方的485，如Prodave的MPI
    if(m_commType == CommunicationType::Other || m_commType == CommunicationType::Board)
        return CommunicationStatus::Connected;

    if(m_commType == CommunicationType::Serial)
    {
        int index = m_commParameter.indexOf(',');
        int nNumIndex = -1;
        //获取数字
        for(int i = 0; i<index; i++)
        {
            if(m_commParameter.at(i) >= '0' && m_commParameter.at(i) <= '9')
            {
                nNumIndex = i;
                break;
            }
        }

        if(nNumIndex  != -1 || nNumIndex < index)
        {
            QString strcom = m_commParameter.mid(nNumIndex,index - nNumIndex);
            if(strcom.toInt() >= 9000)
            {
                m_commType = CommunicationType::Other;
                return CommunicationStatus::Connected;
            }
        }
    }


    if (NULL!=m_pCurrentInstance)
    {
        return m_pCurrentInstance->getCommStatus();
    }
    return CommunicationStatus::DisConnected;
}
void CMDCommunicationComponent::setCommStatus(CommunicationStatus commStatus)
{
    if (NULL!=m_pCurrentInstance)
    {
        m_pCurrentInstance->setCommStatus(commStatus);
    }
}

/*!
\section 切换通信通道
*/
void CMDCommunicationComponent::switchChannel()
{
    if (NULL != m_pCurrentInstance)
    {
        m_pCurrentInstance->disConnect(2000);
        try
        {
            if((m_backupCommParameter.isEmpty()||m_backupCommParameter.isNull())== false &&
               QString::compare(m_pCurrentInstance->getCommParameter(),m_backupCommParameter,Qt::CaseInsensitive)!=0)
            {
                m_pCurrentInstance->setCommParameter(m_backupCommParameter);
            }
            else
            {
                m_pCurrentInstance->setCommParameter(m_primaryCommParameter);
            }
            m_pCurrentInstance->connect(5000);
        }
        catch (...)
        {
        }
    }
}

QString CMDCommunicationComponent::getCurCommParam()
{
    if(NULL != m_pCurrentInstance)
         return m_pCurrentInstance->getCommParameter();
    else
         return "";
}

/*!
\section 通信接口连接及断开
*/
bool CMDCommunicationComponent::connect(qint32 msecs)
{
    bool ret = true;
    if (getCommStatus() != CommunicationStatus::Connected)
    {
        try
        {
            if (NULL == m_pCurrentInstance)
            {
                switch (m_commType)
                {
                    case CommunicationType::Serial:
                        m_pCurrentInstance = new CMDSerialPortCommunicationObject();
                        break;
                    case CommunicationType::TCP:
                        m_pCurrentInstance = new CMDTCPCommunicationObject();
                        break;
                    case CommunicationType::UDP:
                        m_pCurrentInstance = new CMDUDPCommunicationObject();
                        break;
                    case CommunicationType::OPCUA:
                        m_pCurrentInstance = new CMDOPCUaCommunicationObject();
                        break;
                    case CommunicationType::USB:
                        //mCurrentInstance=new _USB();
                        break;
                    case CommunicationType::Board:
                        //mCurrentInstance=new _Board();
                        break;
                    case CommunicationType::Other:
                        //mCurrentInstance=new _Other();
                        break;
                    default:
                        break;
                }
            }

            if (NULL != m_pCurrentInstance)
            {
                m_pCurrentInstance->setCommParameter(m_primaryCommParameter);
                //************************* Y2hw ************************
                //通信不正常时：关闭channel需要太长时间
                ret = m_pCurrentInstance->connect(msecs);

                //------------------------------------------------------
            }
            else
            {
                ret= false;
            }
        }
        catch (...)
        {
            ret = false;
        }
    }
    return ret;
}

bool CMDCommunicationComponent::disConnect(qint32 msecs)
{
    if (NULL!=m_pCurrentInstance)
    {

        return  m_pCurrentInstance->disConnect(msecs);
    }
    return false;
}
/*!
\section 发送数据
*/
bool CMDCommunicationComponent::writeData(quint8 *buffer, qint16 count,qint32 msecs)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->writeData(buffer,count,msecs);
    }
    return false;
}

/*!
\section 收数据。当协议解析数据包不完整时，下个包的数据存放时间是前面数据包的总长
*/
qint64 CMDCommunicationComponent::readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->readData(buffer, startIndex,bufferSize,msecs);
    }
    return 0;
}

bool CMDCommunicationComponent::subscribes(const std::vector<std::string> &entityBrowseNames)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->subscribes(entityBrowseNames);
    }
    return true;
}

bool CMDCommunicationComponent::registSubCallback(subCallbackFunc callback, void* contex)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->registSubCallback(callback, contex);
    }
    return true;
}

void CMDCommunicationComponent::asyncWriteVariable(QString address, MDDataType type, QVariant value)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->asyncWriteVariable(address, type, value);
    }
}

void CMDCommunicationComponent::asyncWriteVariables(QList<QString> addresses, QList<MDDataType> types, QList<QVariant> values)
{
    if (NULL != m_pCurrentInstance)
    {
        return m_pCurrentInstance->asyncWriteVariables(addresses, types, values);
    }
}

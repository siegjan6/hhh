#include "MDTCPCommunicationObject.h"
#include "QStringList"
#include <string.h>
/*
CMDTCPCommunicationObject::CMDTCPCommunicationObject()
{
    m_pTcpSocket=NULL;
    m_pCurrentEventCallBack=NULL;
    m_commParaeter="";
    m_localIP="";
    m_serverIP="";
    m_localPort=20000;
    m_serverPort=20000;
    m_mode="Client";
    m_recvLength =0;
    m_responesTimeout = 3000;
    m_commStatus = CommunicationStatus::DisConnected;
}

CMDTCPCommunicationObject::~CMDTCPCommunicationObject()
{
    if(NULL != m_pTcpSocket)
    {
        delete m_pTcpSocket;
        m_pTcpSocket = NULL;
    }
}
qint16 CMDTCPCommunicationObject::getResponesTimeout()
{
    return m_responesTimeout;
}

void CMDTCPCommunicationObject::setResponesTimeout(qint16 timeOut)
{
    m_responesTimeout = timeOut;
}
CommunicationStatus CMDTCPCommunicationObject::getCommStatus()
{
    return m_commStatus;
}
void CMDTCPCommunicationObject::setCommStatus(CommunicationStatus commStatus)
{
    m_commStatus = commStatus;
}
qint16 CMDTCPCommunicationObject::setEventCallback( IEventCallback *onEvent)
{
    m_pCurrentEventCallBack = onEvent;
    return 1;
}
void CMDTCPCommunicationObject::setCommParameter(QString commParameter)
{
    m_commParaeter.clear();
    m_commParaeter = commParameter;
    QStringList  paramList = m_commParaeter.split(',');//mode,目标ip,目标port,本地ip,本地port
    if(paramList.size()<5)
    {
        return;
    }
    m_mode = paramList[0];
    m_serverIP = paramList[1];
    if (paramList[2].length()>0)
    {
        m_serverPort = paramList[2].toInt();
    }
    else
    {
        m_serverPort = -1;
    }
    m_localIP = paramList[3];
    if (m_localIP.length()==0)
    {
        m_localIP = "localhost";
    }
    if (paramList[4].length() > 0)
    {
        m_localPort =paramList[4].toInt();
    }
    else
    {
        m_localPort = -1;
    }
}

QString CMDTCPCommunicationObject::getCommParameter()
{
    return m_commParaeter;
}

qint16 CMDTCPCommunicationObject::startConnect()
{
   qint16 ret=0;
   try
   {
        if(m_mode.compare("SERVER",Qt::CaseInsensitive)==0)
        {

        }
        else if(m_mode.compare("Client",Qt::CaseInsensitive)==0)
        {
            m_recvLength = 0;

            stopConnect();

            if(NULL == m_pTcpSocket)
            {
                m_pTcpSocket = new QTcpSocket();
            }

            //connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
            connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
            connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));

            setCommStatus(CommunicationStatus::Connecting);
            onEvent(CommunicationEventType::Connecting);
            m_pTcpSocket->connectToHost(m_serverIP,m_serverPort);
            if(m_pTcpSocket->waitForConnected(m_responesTimeout))
            {
                setCommStatus(CommunicationStatus::Connected);
                onEvent(CommunicationEventType::Connected);
            }
            else
            {
                stopConnect();
                setCommStatus(CommunicationStatus::DisConnected);
                onEvent(CommunicationEventType::DisConnected);
            }
        }
        else
        {
            setCommStatus(CommunicationStatus::DisConnected);
            onEvent(CommunicationEventType::DisConnected);
        }
    }
    catch(...)
    {
    }
    return ret;
}

qint16 CMDTCPCommunicationObject::stopConnect()
{
    try
    {
        m_recvLength = 0;
        if(NULL != m_pTcpSocket)
        {
           m_pTcpSocket->disconnectFromHost();
           m_pTcpSocket->waitForDisconnected();
           setCommStatus(CommunicationStatus::DisConnected);
           onEvent(CommunicationEventType::DisConnected);
        }
    }
    catch (...)
    {
        return 0;
    }
    return 1;
}

qint16 CMDTCPCommunicationObject::sendData(quint8 *buffer, qint16 count)
{
    qint16 ret=0;
    if(NULL != m_pTcpSocket &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
             qint64 nBytesWritten = m_pTcpSocket->write((char*)buffer, count);
             m_pTcpSocket->waitForBytesWritten();
             ret=1;
        }
        QT_CATCH(...)
        {

            ret=-1;
        }
    }
    return ret;
}

qint16 CMDTCPCommunicationObject::getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize)
{
    qint16 numOfBytes=0;
    if(NULL != m_pTcpSocket)
    {
        m_recvBufferMutex.lock();
        QT_TRY
        {
            if (m_recvLength + startIndex > bufferSize)
            {
                numOfBytes = -1;
            }
            else
            {
                memcpy(buffer+startIndex,m_recvBuffer,m_recvLength);
                numOfBytes = m_recvLength;
            }

       }
       QT_CATCH(...)
       {
       }
       m_recvLength = 0;
       m_recvBufferMutex.unlock();
    }
    return numOfBytes;
}

void CMDTCPCommunicationObject::dataReceived()
{
    if(NULL != m_pTcpSocket)
    {
        m_recvBufferMutex.lock();
        qint64 bytesAvailable = m_pTcpSocket->bytesAvailable();
        while(bytesAvailable>0)
        {
            char *recvBuffer = new char[bytesAvailable];
            qint64 recvLength = m_pTcpSocket->read(recvBuffer,bytesAvailable);

            if(m_recvLength+recvLength>65535)
            {
                m_recvLength = 0;
            }
            memcpy(m_recvBuffer+m_recvLength,recvBuffer,recvLength);
            m_recvLength = m_recvLength+recvLength;

            delete recvBuffer;

            bytesAvailable = m_pTcpSocket->bytesAvailable();
        }
        m_recvBufferMutex.unlock();
        onEvent(CommunicationEventType::ReceivedData);
    }
}

void CMDTCPCommunicationObject::slotDisconnected()
{
    m_recvLength = 0;
    setCommStatus(CommunicationStatus::DisConnected);
    onEvent(CommunicationEventType::DisConnected);
}
void CMDTCPCommunicationObject::onEvent(const CommunicationEventType eventType)
{
    if(NULL != m_pCurrentEventCallBack)
    {
       m_pCurrentEventCallBack->onEvent(eventType);
    }
}*/

CMDTCPCommunicationObject::CMDTCPCommunicationObject()
{
    m_pTcpSocket=NULL;
    m_commParaeter="";
    m_localIP="";
    m_serverIP="";
    m_localPort=20000;
    m_serverPort=20000;
    m_mode="Client";
    m_recvLength =0;
    m_commStatus = CommunicationStatus::DisConnected;

    m_pSubCallback = NULL;
}

CMDTCPCommunicationObject::~CMDTCPCommunicationObject()
{
    if(NULL != m_pTcpSocket)
    {
        m_pTcpSocket->deleteLater();
        //delete m_pTcpSocket;
        m_pTcpSocket = NULL;
    }
}
CommunicationStatus CMDTCPCommunicationObject::getCommStatus()
{
    return m_commStatus;
}
void CMDTCPCommunicationObject::setCommStatus(CommunicationStatus commStatus)
{
    m_commStatus = commStatus;
}
void CMDTCPCommunicationObject::setCommParameter(QString commParameter)
{
    m_commParaeter.clear();
    m_commParaeter = commParameter;
    QStringList  paramList = m_commParaeter.split(',');//mode,目标ip,目标port,本地ip,本地port
    if(paramList.size()<5)
    {
        return;
    }
    m_mode = paramList[0];
    m_serverIP = paramList[1];
    if (paramList[2].length()>0)
    {
        m_serverPort = paramList[2].toShort();
    }
    else
    {
        m_serverPort = -1;
    }
    m_localIP = paramList[3];
    if (m_localIP.length()==0)
    {
        m_localIP = "localhost";
    }
    if (paramList[4].length() > 0)
    {
        m_localPort =paramList[4].toInt();
    }
    else
    {
        m_localPort = -1;
    }
}

QString CMDTCPCommunicationObject::getCommParameter()
{
    return m_commParaeter;
}

bool CMDTCPCommunicationObject::connect(qint32 msecs)
{
   bool ret=false;
   try
   {
        if(m_mode.compare("SERVER",Qt::CaseInsensitive)==0)
        {

        }
        else if(m_mode.compare("Client",Qt::CaseInsensitive)==0)
        {
            m_recvLength = 0;

            disConnect(2000);
            if(m_serverPort == -1)
            {
                setCommStatus(CommunicationStatus::DisConnected);
                return false;
            }
            if(NULL == m_pTcpSocket)
            {
                m_pTcpSocket = new QTcpSocket();
            }
            setCommStatus(CommunicationStatus::Connecting);
            try
            {
                m_pTcpSocket->abort();
                m_pTcpSocket->connectToHost(m_serverIP, (quint16)m_serverPort);

                if(m_pTcpSocket->waitForConnected(msecs))
                {
                    setCommStatus(CommunicationStatus::Connected);
                    ret = true;
                }
                else
                {
                    disConnect(msecs);
                    setCommStatus(CommunicationStatus::DisConnected);
                }
            }
            catch(...)
            {
            }
        }
        else
        {
            setCommStatus(CommunicationStatus::DisConnected);
        }
    }
    catch(...)
    {
    }
    return ret;
}
bool CMDTCPCommunicationObject::disConnect(qint32 msecs)
{
    bool ret=false;
    try
    {
        m_recvLength = 0;
        if(NULL != m_pTcpSocket)
        {
           if(getCommStatus() == CommunicationStatus::Connected)
           {
                m_pTcpSocket->disconnectFromHost();
               setCommStatus(CommunicationStatus::DisConnecting);
               if(m_pTcpSocket->waitForDisconnected(msecs))
               {
                   ret = true;
               }
           }
           setCommStatus(CommunicationStatus::DisConnected);
        }
    }
    catch (...)
    {
    }
    return ret;
}

bool CMDTCPCommunicationObject::writeData(quint8 *buffer, qint16 count,qint32 msecs)
{
    bool ret=false;
    if(NULL != m_pTcpSocket &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
             if(count == m_pTcpSocket->write((char*)buffer, count))
             {
                 if(m_pTcpSocket->waitForBytesWritten(msecs))
                 {
                     ret=true;
                 }
             }
        }
        QT_CATCH(...)
        {
        }
    }
    return ret;
}
qint64 CMDTCPCommunicationObject::readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs)
{
    qint64 ret=0;
    if(NULL != m_pTcpSocket &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
            if (m_pTcpSocket->waitForReadyRead(msecs))
            {
                qint64 recvLength = m_pTcpSocket->read((char*)m_recvBuffer,4096);
                //QByteArray requestData = m_clientSocket->readAll();
                if(recvLength > 0)
                {
                    if (recvLength + startIndex > bufferSize)
                    {
                        ret = -1;
                    }
                    else
                    {
                        memcpy(buffer+startIndex,m_recvBuffer,recvLength);
                        ret = recvLength;
                    }
                }
            }
        }
        QT_CATCH(...)
        {
        }
    }
    return ret;
}

bool CMDTCPCommunicationObject::subscribes(const std::vector<std::string> &entityBrowseNames)
{
    return true;
}

bool CMDTCPCommunicationObject::registSubCallback(subCallbackFunc callback, void* contex)
{
    m_pSubCallback = callback;
    return true;
}

void CMDTCPCommunicationObject::asyncWriteVariable(QString address, MDDataType type, QVariant value)
{

}

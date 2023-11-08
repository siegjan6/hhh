#include "MDUDPCommunicationObject.h"
#include "QStringList"
#include <string.h>
#include <QUdpSocket>
CMDUDPCommunicationObject::CMDUDPCommunicationObject()
{
    m_pUdpSocket=NULL;
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

CMDUDPCommunicationObject::~CMDUDPCommunicationObject()
{
    if(NULL != m_pUdpSocket)
    {
        m_pUdpSocket->deleteLater();
        m_pUdpSocket = NULL;
    }
}
CommunicationStatus CMDUDPCommunicationObject::getCommStatus()
{
    return m_commStatus;
}
void CMDUDPCommunicationObject::setCommStatus(CommunicationStatus commStatus)
{
    m_commStatus = commStatus;
}
void CMDUDPCommunicationObject::setCommParameter(QString commParameter)
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

QString CMDUDPCommunicationObject::getCommParameter()
{
    return m_commParaeter;
}

bool CMDUDPCommunicationObject::connect(qint32 msecs)
{
   bool ret=false;
   try
   {
        m_pUdpSocket = new QUdpSocket();
        if(m_mode.compare("SERVER",Qt::CaseInsensitive)==0)
        {
            m_pUdpSocket->bind(m_localPort);//建立监听
        }
        setCommStatus(CommunicationStatus::Connected);
        ret = true;
    }
    catch(...)
    {
    }
    return ret;
}
bool CMDUDPCommunicationObject::disConnect(qint32 msecs)
{
    bool ret=false;
    try
    {
        m_recvLength = 0;
        if(NULL != m_pUdpSocket)
        {
           if(getCommStatus() == CommunicationStatus::Connected)
           {
               m_pUdpSocket->disconnectFromHost();
               setCommStatus(CommunicationStatus::DisConnecting);
               if(m_pUdpSocket->waitForDisconnected(msecs))
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

bool CMDUDPCommunicationObject::writeData(quint8 *buffer, qint16 count,qint32 msecs)
{
    bool ret=false;
    if(NULL != m_pUdpSocket &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
            if(m_mode.compare("CLIENT",Qt::CaseInsensitive)==0)
            {
                QHostAddress sender;
                sender.setAddress(m_serverIP);
                 if(count == m_pUdpSocket->writeDatagram((char*)buffer,count,sender, m_serverPort))
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
qint64 CMDUDPCommunicationObject::readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs)
{
    qint64 ret=0;
    if(NULL != m_pUdpSocket &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
            //if (m_pUdpSocket->waitForReadyRead(msecs))//Y2hw
            if (m_pUdpSocket->bytesAvailable() > 0)
            {
                qint64 recvLength = m_pUdpSocket->readDatagram((char*)m_recvBuffer,4096);
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

bool CMDUDPCommunicationObject::subscribes(const std::vector<std::string> &entityBrowseNames)
{
    return true;
}

bool CMDUDPCommunicationObject::registSubCallback(subCallbackFunc callback, void* contex)
{
    m_pSubCallback = callback;
    return true;
}

void CMDUDPCommunicationObject::asyncWriteVariable(QString address, MDDataType type, QVariant value)
{

}

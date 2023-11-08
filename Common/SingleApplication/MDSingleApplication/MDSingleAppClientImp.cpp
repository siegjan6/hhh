#include "MDSingleAppClientImp.h"
#include <QTextCodec>

#define TIME_OUT                (1000)    // 1000ms

CMDSingleAppClientImp::CMDSingleAppClientImp()
{
}

void CMDSingleAppClientImp::init(const QString& appServerName)
{
    m_serverName = appServerName;
}

void CMDSingleAppClientImp::release()
{
    m_localSocket.disconnectFromServer();
}

bool CMDSingleAppClientImp::tryToConnectToServer()
{
    bool bConnected = false;

    m_localSocket.connectToServer(m_serverName);
    if(m_localSocket.waitForConnected(TIME_OUT))
    {
         bConnected = true;
    }

    return bConnected;
}

bool CMDSingleAppClientImp::sendToServer(const QString& data)
{
    bool bOK = false;

    QLocalSocket::LocalSocketState connState = m_localSocket.state();
    if(QLocalSocket::LocalSocketState::UnconnectedState == connState)
    {
        m_localSocket.connectToServer(m_serverName);
        if(m_localSocket.waitForConnected(TIME_OUT))
        {

        }
        connState = m_localSocket.state();
    }

    if(QLocalSocket::LocalSocketState::ConnectedState == connState)
    {
        QTextStream ts(&m_localSocket);
        ts.setCodec("UTF-8");
        ts << data;
        ts.flush();

        bOK = m_localSocket.waitForBytesWritten();
    }

    return bOK;
}

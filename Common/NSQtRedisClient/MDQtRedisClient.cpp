/*!
\author chp 2016.06.13
\section
*/

#include "MDQtRedisClient.h"


using namespace RedisClient;

CMDQtRedisClient::CMDQtRedisClient(const QString& serverIp, int port)
{
    initRedisClient();
    m_serverIp = serverIp;
    m_serverPort = port;

    ConnectionConfig config(serverIp);
    config.setPort(port);
    m_pConnection = new Connection(config);
    //m_pConnection->setConnectionConfig(*m_pConfig);
    //!设置订阅命令和基本命令的槽
    connect(m_pConnection,SIGNAL(messageRet(QList<QByteArray>&)),this,SIGNAL(messageRet(QList<QByteArray>&)));
    connect(m_pConnection,SIGNAL(connected()),this,SIGNAL(connected()));
}
CMDQtRedisClient::~CMDQtRedisClient()
{
    if(m_pConnection != NULL)
    {
        if(m_pConnection->isConnected())
            m_pConnection->disconnect();

        delete m_pConnection;
    }
}

void CMDQtRedisClient::connectServer()
{
    m_pConnection->connect();
}

void CMDQtRedisClient::disconnect()
{
    m_pConnection->disconnect();
}

bool CMDQtRedisClient::isConnected()
{
    return m_pConnection->isConnected();
}
void CMDQtRedisClient::setTimeouts(uint connectionTimeout, uint commandExecutionTimeout)
{
    m_pConnection->setTimeouts(connectionTimeout,commandExecutionTimeout);
}

void CMDQtRedisClient::asyncSendCommand(QList<QByteArray> rawCmd, int db)
{
    if(m_pConnection != NULL && rawCmd.length() > 0)
    {
        m_pConnection->command(rawCmd,this,NULL,db);
    }
}
QList<QByteArray> CMDQtRedisClient::syncSendCommand(QList<QByteArray> rawCmd, int db)
{
    QList<QByteArray> msg;
    if(m_pConnection != NULL && rawCmd.length() > 0)
    {
        RedisClient::Response res = m_pConnection->commandSync(rawCmd,db);
        if(!res.isEmpty())
            msg.append(res.getValue().toByteArray());
    }
    return msg;
}

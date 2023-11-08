#ifndef _QQTREDISCLIENT_H_
#define _QQTREDISCLIENT_H_
#include <QObject>
#include <QtNetwork/QAbstractSocket>
#include "qredisclient/redisclient.h"

using namespace RedisClient;
class CMDQtRedisClient: public QObject
{
	Q_OBJECT
	
public:
    CMDQtRedisClient(const QString& serverIp, int port=6379);
    ~CMDQtRedisClient();

public:
    /*!
    \section 连接
    */
    void connectServer();
    void disconnect();
    bool isConnected();
    void setTimeouts(uint connectionTimeout, uint commandExecutionTimeout);
    /*!
    \section 异步发送
    */
    void asyncSendCommand(QList<QByteArray> rawCmd, int db = -1);

    /*!
    \section 同步发送
    */
    QList<QByteArray> syncSendCommand(QList<QByteArray> rawCmd, int db = -1);
    //!信号槽
signals:
    void messageRet(QList<QByteArray>& msg);
    void connected();
protected:
    Connection* m_pConnection;
    //ConnectionConfig* m_pConfig;
    QString m_serverIp;
    int     m_serverPort;
};

#endif

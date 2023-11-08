#include "MDRedisClient.h"
#include "qtredis.h"

CMDRedisClient::CMDRedisClient(QString host, int port)
{
    redisclient = new QtRedis(host,port);
    //connect(redisclient, SIGNAL(returnMessage()), this, SLOT(outMessage()));
    //connect(redisclient, SIGNAL(returnGeneralCmd(QStringList&)), this, SLOT(commonCmd(QStringList&)));
    connect(redisclient,SIGNAL(messageRet(QList<QByteArray>&)),this,SIGNAL(messageRet(QList<QByteArray>&)));
    connect(redisclient, SIGNAL(connected()), this, SLOT(socketconnected()));
    connect(redisclient, SIGNAL(disconnected()), this, SLOT(socketdisconnected()));
}
CMDRedisClient::~CMDRedisClient()
{
    QMutexLocker locker(&m_Threadmutex);
    if(redisclient != NULL)
    {
        delete redisclient;
        redisclient = NULL;
    }
}

bool CMDRedisClient::openConnection()
{
    QMutexLocker locker(&m_Threadmutex);
    if(redisclient != NULL)
    {
         redisclient->openConnection();
    }
    return true;
}

bool CMDRedisClient::isconnected()
{
    QMutexLocker locker(&m_Threadmutex);
    bool bConnected = false;
    if(redisclient != NULL)
    {
        bConnected = redisclient->isconnected();
    }
    return bConnected;
}
void  CMDRedisClient::asyncSendCommand(QList<QByteArray>& rawCmdList)
{
    QMutexLocker locker(&m_Threadmutex);
    if(redisclient != NULL && rawCmdList.length() > 0)
    {
        QByteArray result;
        result.append(QString("*%1\r\n").arg(rawCmdList.length()));

        for (QByteArray partArray : rawCmdList) {
            result.append("$");
            result.append(QString::number(partArray.size()));
            result.append("\r\n");
            result.append(partArray);
            result.append("\r\n");
        }
        redisclient->sendCommand(result);
    }
}
/*
CMDRedisClient::Reply CMDRedisClient::removeHead()
{
    QMutexLocker locker(&m_Threadmutex);
    CMDRedisClient::Reply res;
    QtRedis::Reply res0;
    if(redisclient != NULL)
    {
        res0 = redisclient->removeHead();
        res.type = res0.type;
        res.channel = res0.channel;
        res.value = res0.value;
        res.message = res0.message;
    }
    return res;
}*/
void CMDRedisClient::socketconnected()
{
    emit connected();
}

void CMDRedisClient::socketdisconnected()
{
    emit disconnected();
}


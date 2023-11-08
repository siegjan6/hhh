#ifndef MDREDISCLIENT_H
#define MDREDISCLIENT_H

#include <QObject>
#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>
#include <QMutex>


#if defined(MDREDISCLIENT_LIBRARY)
#  define MDREDISCLIENT_EXPORT Q_DECL_EXPORT
#else
#  define MDREDISCLIENT_EXPORT Q_DECL_IMPORT
#endif

class QtRedis;
class MDREDISCLIENT_EXPORT CMDRedisClient : public QObject
{
    Q_OBJECT
public:
    CMDRedisClient(QString host, int port);
     ~CMDRedisClient();

    typedef struct Reply {
        QString type;
        QString message;
        QString pattern;
        QString channel;
        QVariant value;
    } Reply;

    //连接
    bool openConnection();
    bool isconnected();

    //发送命令
    void  asyncSendCommand(QList<QByteArray>& rawCmdList);
    //获取订阅消息
    //Reply removeHead();

signals:
    void connected();
    void disconnected();

    void messageRet(QList<QByteArray>& msg);

private slots:
    void socketconnected();
    void socketdisconnected();
    //常用命令
    //void commonCmd(QStringList& ret);
    //订阅消息
    //void outMessage();
private:
    QtRedis* redisclient;
    QMutex m_Threadmutex;
};

#endif // MDREDISCLIENT_H

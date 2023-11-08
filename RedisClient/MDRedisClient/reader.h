#ifndef READER_H
#define READER_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QtNetwork/QTcpSocket>
#include <QSocketNotifier>
#include <QByteArray>

#include <QDebug>
#include <QMutex>

class Reader : public QObject
{
    Q_OBJECT

public:
    Reader(QString host, int port);
    ~Reader();

    void connectHost(const QString &host, quint16 port);
    bool isConnected() const;
    void disconnectHost();

    //订阅消息粘包时，缓冲到列表
    QVector<QStringList> responseVector;//pinkie
signals:
    //about socket
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);

    //subscribe message
    void subResponse();

    //general message
    void response(QVariant);

public slots:
    void socketConnected();
    void socketConnectionClosed();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    void sendData(const QByteArray &);

private:
    QTcpSocket *socket;
    bool reading;
    QSocketNotifier *notifier;
    QString host;
    int port;

    //防止多线程同时发送数据锁
    QMutex m_processedQueueMutex;

    //接收数据buffer
    QByteArray recvbuffer;

    //解析来自socket的原始数据
    void ParsePackage();
    //解析订阅消息，支持分包
    void ParseMessage();
};

#endif // READER_H

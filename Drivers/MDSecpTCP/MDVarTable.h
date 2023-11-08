#ifndef MDVARTABLE_H
#define MDVARTABLE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>


class CMDDevice;
class MDVarTable : public QObject
{
    Q_OBJECT
public:
    explicit MDVarTable(CMDDevice *pDev,QObject *parent = 0);
    virtual ~MDVarTable();
public:
    void setParameter(QString serverIP,qint16 serverPort,QString serverSlaveIP,qint16 serverSlavePort);
    void Connect();
    void ConnectSlave();
    void DisConnect();
    bool writeData(quint8 *writeBuffer, qint16 count);
    qint64 readData(quint8 *readBuffer,qint16 startIndex);

    qint16 queryDataTagList(qint16 dataTabNum);
    qint16 queryTagList(qint16 tabNum);
public:
    CMDDevice *pDevice;
    QTcpSocket *m_pSocket;
    QString    m_serverIP;
    qint16      m_serverPort;
    QString    m_serverSlaveIP;
    qint16      m_serverSlavePort;
    qint16      m_selectGetTabType;
    qint16      m_tabNO;
    qint16      m_dataTabNO;

    quint8      m_recvBuffer[4096];
    quint8      m_sendBuffer[4096];

    bool        slaveConnectFlag;
public:

    qint32         m_packSize;

    quint8         m_tagTabNum;

    quint8         m_dataTabNum;

    int     m_nCounter;

private:
    QTimer     *pTimer ;

signals:
    void sigGetVar();
public slots:
    void getVar();

    void slaveConnect(QAbstractSocket::SocketState state);

    void disConnectFromMain();
};

#endif // MDVARTABLE_H

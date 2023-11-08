#ifndef NSTCPCLIENT_H
#define NSTCPCLIENT_H

//#include <QTcpSocket>
#include "../../network/socket/qtcpsocket.h"
#include <QList>
#include <QString>
#include <QObject>

class NSTcpClient:public QTcpSocket
{
    Q_OBJECT

public:
     NSTcpClient(QObject *parent,int max_length = 4096);
    ~NSTcpClient();

    bool BeginConnect(QString ip_address,quint16 port);
    bool DisConnect();
    int SendData(QByteArray buffer, int count);
    int GetData(QByteArray &buffer, int max_length);

private:
    int max_length;
    QMutex *mutex;
    QList<QByteArray> m_buffer_sending;
    QList<qint64> m_buffer_sending_offset;
    QByteArray recv_buf;

    QTcpSocket *p_socket;
public slots:
    void some_data_sended(qint64);
    void some_data_recieved();
    void Run();
signals:
    void evt_recieved_callback();

};

#endif // NSTCPCLIENT_H

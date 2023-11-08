#ifndef NSNET_H
#define NSNET_H
#include "ICommunicationObject.h"
#include "ICommunicationObjectEx.h"
#include <QByteArray>
#include "NSTcpClient.h"

class NSTcp:public QObject,public ICommunicationObject//,public ICommunicationObjectEx
{
    Q_OBJECT
public:
    NSTcp();
    ~NSTcp();

    int Connect();
    int DisConnect();
    int SendData(QByteArray buffer);
    int GetData(QByteArray &buffer, int max_length);

    bool SetCommParameter(QString value);
    QString GetCommParameter();

    bool SetMaxLength(int max_length);


private:
     void SendEvent();
private:

    NSTcpClient *p_socket;

    QString    comm_paraeter;
    QString    local_ip;
    QString    remote_ip;
    int        local_port;
    int        remote_port;
    QString    protocol_tpye;
    QString    mode;
    int        max_length;

public slots:
       void on_recieved();

};

#endif // NSNET_H

#ifndef QTREDISCLIENT_H
#define QTREDISCLIENT_H
#include <QObject>
#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>
#if defined(QTREDISCLIENTDLL_LIBRARY)
#  define QTREDISCLIENTDLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTREDISCLIENTDLLSHARED_EXPORT Q_DECL_IMPORT
#endif

class QtRedis;
class QTREDISCLIENTDLLSHARED_EXPORT QtRedisClient : public QObject
{
    Q_OBJECT
public:
    QtRedisClient(QString host, int port);
     ~QtRedisClient();

    typedef struct Reply {
        QString type;
        QString message;
        QString pattern;
        QString channel;
        QVariant value;
    } Reply;

    bool openConnection();
    bool isconnected();

    void  sendCommand(QString);

    Reply removeHead();

signals:
    void connected();
    void disconnected();
    void generalCmd(Reply);
    void subMessage();

private slots:
    void socketconnected();
    void socketdisconnected();
    void commonCmd(Reply);
    void outMessage();
private:
    QtRedis* redisclient;
};

#endif // QTREDISCLIENT_H

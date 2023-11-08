/*!
\author chp 2015.01.08
\section 封装Python的Redis客户端
\需要的库支持：PythonQt(Qt源码) + RedisCluster(Python源码)
\相关测试：
\1.空格
\2.中文
\3.订阅命令：无返回结果
\4.订阅消息：推送比接收快时不丢失，接收频率当前值：100ms
\5.断开网线后可自动重连
\6.订阅和普通命令socket不同，所有上层只需建立一个该对象
*/

#ifndef MDPYTHONREDISCLIENT_H
#define MDPYTHONREDISCLIENT_H

#include <QObject>
#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>
#include <QMutex>

#include <QVariant>
#include <QTimer>
#include <QtWidgets/QTextEdit.h>
//#include "gui/PythonQtScriptingConsole.h"


class PythonQtScriptingConsole;
#if defined(MDPYTHONREDISCLIENT_LIBRARY)
#  define MDPYTHONREDISCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDPYTHONREDISCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

class CMDPythonAMQClient : public QObject
{
    Q_OBJECT
public:
    CMDPythonAMQClient(QString host, int port, QString auth, QString password);
    virtual ~CMDPythonAMQClient();
    //!连接Redis服务器
    bool openConnection();

    //!发送消息
    void  setProduceName(QString& name , bool queue);
    void  produceMsg(QString& msg);

    //!订阅
    void  subscribeMsg(QString& name, bool queue);

public Q_SLOTS:
    //!收到Python的订阅信息回调
    void recvSubscribeMessage(QVariant message);
    void error(QVariant message);
    void onConnected();
    //!内部触发收订阅回调
    void TimerOut();
signals:
    //!上层信号
    void recvSubMessage(QVariant message);

private:    
    //!Python控制台对象，调试用
    PythonQtScriptingConsole* m_pythonConsole;
    //!触发订阅消息回调定时器
    QTimer *m_timer;
    QString m_host;
    int m_port;
    QString m_auth;
    QString m_password;

    QString m_name;
    bool    m_queueClient;
    bool    m_produce;
};

#endif // MDPYTHONREDISCLIENT_H

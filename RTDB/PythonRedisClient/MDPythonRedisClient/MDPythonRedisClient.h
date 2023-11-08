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



#if defined(MDPYTHONREDISCLIENT_LIBRARY)
#  define MDPYTHONREDISCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDPYTHONREDISCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

class CMDPythonRedisClient : public QObject
{
    Q_OBJECT
public:
    CMDPythonRedisClient();
    virtual ~CMDPythonRedisClient();
    //!连接Redis服务器
    bool openConnection(QVector<QString> hostArr, QVector<int>& portArr, bool cluster);

    //!发送命令 命令以QStringList组织，避免空格问题
    QVariant  sendCommand(QStringList& cmd);

public Q_SLOTS:
    //!收到Python的订阅信息回调
    void recvSubscribeMessage(QVariant message);

    //!内部触发收订阅回调
    void TimerOut();
signals:
    //!上层信号
    void recvSubMessage(QVariant message);

private:    
    //!Python控制台对象，调试用
    //PythonQtScriptingConsole* m_pythonConsole;
    //!触发订阅消息回调定时器
    QTimer *m_timer;

};

#endif // MDPYTHONREDISCLIENT_H

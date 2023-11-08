#ifndef OPCUA_CLIENT_H
#define OPCUA_CLIENT_H

#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>

extern "C"{
#include "pch_open62541.h"
}

extern QMutex  mutex;//互斥锁
extern QWaitCondition  newdataAvailable;//通知
extern UA_Variant value;//变量可以保存任何类型的标量值和数组


/**
 * @brief Opcua客户端线程
 *
 */
class Opcua_Client : public QObject
{
    Q_OBJECT
    QThread m_thread;

public:
    explicit Opcua_Client(QObject *parent = 0);
    ~Opcua_Client();
protected slots:
    void tmain();// 线程入口函数
signals:
    void VariableValue(UA_Variant value);//变量节点的属性
    void testSingnal(int test);
public://线程管理
    void start();
    bool isRunning();
    void terminate();
    void exit(int c);
    void wait();
    void StopClientThread(); //结束线程循环

public://客户端和远程服务器之间的交互

    /*客户端管理*/
    static void stopHandler(int sign);

    void CreatOpcuaClient();//创建客户端
    UA_StatusCode ConnectToSever();//连接端

    void DeleteOpcuaClient();//把客户内部连接断开

    /*数据交互*/
    void Client_readValueAttribute();//读取变量节点的value属性
    void Client_writeValueAttribute();//对server里的变量节点value属性写入一个新值
    void Client_clearVariant();//value属性

    void addVariable(UA_Server *server);//添加变量到服务器
    void ClientReadServerValue();//读服务端变量值
    void ClientWriteServerValue();//写服务端变量值


public://客户端和本地服务器之间的数据传递

    void ClientReadValue();//读取从服务端获得的变量值


public:
    static UA_Boolean running;
    UA_NodeId nodeId;

    UA_Client *client;
    UA_StatusCode retval;


    /*客户端和远程服务器之间的交互*/

    /*对线程的管理*/
    bool m_stop = false; //停止线程
    bool m_read_value = true;//读取服务器变量
    bool m_write_value = true;//写入服务器变量

    QMutex m_mutex;//互斥量
    UA_Server *m_server;

    /*客户端和本地服务器之间的数据传递*/

    UA_Variant value_to_lserver;//传递给本地server的值


private:
    char* sname = (char*)"the.answer";
};



#endif // OPCUA_CLIENT_H

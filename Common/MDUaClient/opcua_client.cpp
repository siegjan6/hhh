#include "opcua_client.h"
//#include "opcua_server.h"

UA_Boolean Opcua_Client::running = true;

void Opcua_Client::stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

QMutex  mutex;//互斥锁
QWaitCondition  newdataAvailable;//通知
UA_Variant value;//变量可以保存任何类型的标量值和数组


//stopHandler捕获程序,在操作系统尝试关闭时接收信号（中断）
Opcua_Client::Opcua_Client(QObject *parent):
    QObject(parent)
{
    moveToThread(&m_thread);

    connect(&m_thread, SIGNAL(started()), this, SLOT(tmain()));


    nodeId= UA_NODEID_STRING(1,sname);

    client = nullptr;
    retval = 0;

    /*初始化value为0*/
    UA_Int32 data = 0;
    UA_Variant_init(&value);
    UA_Variant_setScalar(&value, &data, &UA_TYPES[UA_TYPES_INT32]);

}

Opcua_Client::~Opcua_Client()
{
    nodeId= UA_NODEID_STRING(1,sname);

    client = nullptr;
    retval = 0;

    m_thread.wait();// 同步设计

}
void Opcua_Client::tmain()
{
    /* 创建客户端接受连接 */
    CreatOpcuaClient();

    /* 创建客户端接受连接 */
    ConnectToSever();


    while(!m_stop)//循环主体
    {

        mutex.lock();

        /* 读取节点的 value 属性*/
        Client_readValueAttribute();

        mutex.unlock();
        newdataAvailable.wakeAll();//唤醒所有线程，有新数据了

        m_thread.sleep(3);

    }
    m_thread.quit();

}

void Opcua_Client::start()
{
    m_thread.start();// m_thread线程开启，定义的tmain函数被调用(run函数也被调用,开启事件循环)
}

bool Opcua_Client::isRunning()
{
    return m_thread.isRunning();
}

void Opcua_Client::terminate()
{
    m_thread.terminate();
}

void Opcua_Client::exit(int c)
{
    m_thread.exit(c);
}

void Opcua_Client::wait()
{
    m_thread.wait();
}


void Opcua_Client::CreatOpcuaClient()
{
    client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
}

void Opcua_Client::DeleteOpcuaClient()
{
    /*把客户内部连接断开*/
    UA_Client_delete(client);

    qDebug()<<EXIT_SUCCESS;

}

UA_StatusCode Opcua_Client::ConnectToSever()
{
    retval = UA_Client_connect(client, "opc.tcp://192.168.5.120:4840");
    if(retval != UA_STATUSCODE_GOOD) {
          UA_Client_delete(client);
          qDebug()<<(int)retval<<"----connect fail----->";
          return (int)retval;
    }
}

void Opcua_Client::Client_readValueAttribute()
{
    /* 读取节点的 value 属性。Ua_client_readvalueattribute 是作为ua_client_service_read 提供的原始读取服务的包装器。 */
     UA_Variant_init(&value);

     /* 读取Server变量节点值 */

      // 变量的NodeId是string类型
      nodeId = UA_NODEID_STRING(1,sname);

      if(retval == UA_STATUSCODE_GOOD)
      {
          //使用UA_Client_readValueAttribute()去读取变量值
          retval = UA_Client_readValueAttribute(client,nodeId,&value);

          // 变量的数据类型是UA_Int32，第二个是判断UA_Variant里是否有UA_Int32类型的数据
          if(retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value,&UA_TYPES[UA_TYPES_INT32]))
          {
             UA_Int32 variableValue = *(UA_Int32 *)value.data;
             UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Variable Value From Single Server is:%d\n",variableValue);
          }
      }
}

void Opcua_Client::Client_writeValueAttribute()
{
    /* 对server里的变量写入一个新值 */
    nodeId = UA_NODEID_STRING(1, sname);

    UA_Int32 change = 100;
    UA_Variant newValue;
    UA_Variant_init(&newValue);
    UA_Variant_setScalar(&newValue, &change, &UA_TYPES[UA_TYPES_INT32]);
    retval = UA_Client_writeValueAttribute(client, nodeId,&newValue);

}

void Opcua_Client::Client_clearVariant()
{
    /* 清理value */
    UA_Variant_clear(&value);
}

void Opcua_Client::addVariable(UA_Server *server)
{
    /* 定义myInteger变量节点的属性 */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 myInteger = 42;

    UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT((char*)"en-US",sname);
    attr.displayName = UA_LOCALIZEDTEXT((char*)"en-US",sname);
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;


    /* 将该变量节点添加到信息模型中 */
    UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1,sname);
    UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1,sname);
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0,UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0,UA_NS0ID_ORGANIZES);
    UA_Server_addVariableNode(server,myIntegerNodeId,parentNodeId,
                              parentReferenceNodeId,myIntegerName,
                              UA_NODEID_NUMERIC(0,UA_NS0ID_BASEDATAVARIABLETYPE),
                              attr,NULL,NULL);
}


void Opcua_Client::StopClientThread()
{//停止线程
    m_stop=true;
}

void Opcua_Client::ClientReadServerValue()
{//读服务端变量值
    m_read_value=false;
}

void Opcua_Client::ClientWriteServerValue()
{//写服务端变量值
    m_write_value=false;
}

void Opcua_Client::ClientReadValue()
{

}



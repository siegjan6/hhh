#include "MDPythonAMQClient.h"
#include <QTextCodec>//支持中文
#include "include/PythonQt.h"
#include "gui/PythonQtScriptingConsole.h"
//sys.setdefaultencoding('utf-8')
#include <QThread>
PythonQtObjectPtr m_pythonOb;
PythonQtObjectPtr m_pythonObFactory;
CMDPythonAMQClient::CMDPythonAMQClient(QString host, int port, QString auth, QString password)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //PythonQt::init();
    PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
    m_pythonObFactory = PythonQt::self()->getMainModule();
    //m_pythonConsole = new PythonQtScriptingConsole(NULL,m_pythonObFactory);
    m_pythonObFactory.evalScript(QString("import sys\n"));
    m_timer = NULL;
    m_host = host;
    m_port = port;
    m_auth = auth;
    m_password = password;
    m_queueClient = false;
    m_name = "";
    m_produce = false;


    m_pythonObFactory.evalFile(":amq.py");
    Q_ASSERT(!m_pythonObFactory.isNull());
    //python 控制台
    m_pythonConsole = new PythonQtScriptingConsole(NULL,m_pythonObFactory);
    //显示python 控制台
    m_pythonConsole->appendCommandPrompt();
    m_pythonConsole->show();

    if(m_pythonOb.isNull())
    {
        m_pythonOb = m_pythonObFactory.evalScript("AMQPythonClient()\n",Py_eval_input);
        Q_ASSERT(!m_pythonOb.isNull());
        //m_pythonOb.addObject("MDAMQClient", this);
        m_pythonOb.call("onInit",QVariantList());
//         if(m_auth.isEmpty() || m_password.isEmpty())
//            m_pythonOb.call("onInit",QVariantList()<< "127.0.0.1" << 61613 << "None" << "None");
//         else
//            m_pythonOb.call("onInit",QVariantList()<< m_host << m_port << m_auth << m_password);
    }
}

CMDPythonAMQClient::~CMDPythonAMQClient()
{
    Q_ASSERT(!m_pythonOb.isNull());
    if(m_produce == false && !m_name.isEmpty())
        m_pythonOb.call("unsubscribe",QVariantList() << m_name);

    m_pythonOb.call("release",QVariantList());
}

//!连接Redis服务器
bool CMDPythonAMQClient::openConnection()
{
    if(!m_pythonOb.isNull())
    {
        // register the type with QMetaType
        //qRegisterMetaType<CMDPythonAMQClient>("CMDPythonAMQClient");
        // add a wrapper object for the new variant type
        //PythonQt::self()->registerCPPClass("CMDPythonAMQClient","","amq", PythonQtCreateObject<CustomObjectWrapper>);


         m_pythonOb.call("connect",QVariantList());

         m_timer = new QTimer(this);
         connect(m_timer,SIGNAL(timeout()),this,SLOT(TimerOut()));
         //m_timer->start(10);         
    }
}
/*
//!发送命令
QVariant  CMDPythonAMQClient::sendCommand(QStringList& cmd)
{
    if(!m_pythonOb.isNull() && cmd.length() > 0)
    {
        QVariant ret;

        QString firststrr = cmd.at(0);
        firststrr = firststrr.toUpper();
        QString ccmd = firststrr;
        for(int i = 1; i<cmd.count(); i++)
        {
            ccmd.append("@");
            ccmd.append(cmd.at(i));
        }
        //判断是否是订阅消息
        if(firststrr.indexOf("SUBSCRIBE") == 0)
        {
            if(cmd.length() > 1)
            {
                if(m_timer == NULL)
                {
                    m_timer = new QTimer(this);
                    connect(m_timer,SIGNAL(timeout()),this,SLOT(TimerOut()));
                    m_timer->start(100);
                }
                ret = m_pythonOb.call("subscribe",QVariantList() <<  cmd.at(1));
            }
        }
        else
        {
            ret = m_pythonOb.call("sendCommand",QVariantList() <<  ccmd );
        }
        return ret;
    }
    else
    {
        return QVariant();
    }
}
*/
void CMDPythonAMQClient::TimerOut()
{
    bool bConnected = false;
    QVariant ret;
    if(!m_pythonOb.isNull())
    {
         ret = m_pythonOb.call("isConnected",QVariantList());
         bConnected = ret.toBool();
         int mm = 9;
    }
}
//cmd
void  CMDPythonAMQClient::setProduceName(QString& name , bool queue)
{
    if(queue)
        m_name = "/queue/" + name;
    else
        m_name = "/topic/" + name;
    m_queueClient = queue;
    m_produce = true;
}

void  CMDPythonAMQClient::produceMsg(QString& msg)
{
    if(m_queueClient)
    {
        m_pythonOb.call("produceMsg",QVariantList() << m_name << msg);
    }
    else
    {
        m_pythonOb.call("produceMsg",QVariantList() << m_name << msg);
    }
}

void  CMDPythonAMQClient::subscribeMsg(QString& name, bool queue)
{
    QString str = "";
    if(queue)
        str = "/queue/" + name;
    else
        str = "/topic/" + name;
    m_name = name;
    m_pythonOb.call("subscribe",QVariantList() << str);
    m_produce = false;
}

//slots
void CMDPythonAMQClient::recvSubscribeMessage(QVariant message)
{
    QString msg = message.toString();
    emit recvSubMessage(msg);
}

void CMDPythonAMQClient::error(QVariant message)
{
    QString msg = message.toString();
}

void CMDPythonAMQClient::onConnected()
{
    int m = 9;//1234
}


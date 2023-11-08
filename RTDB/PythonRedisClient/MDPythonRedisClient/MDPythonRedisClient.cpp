#include "MDPythonRedisClient.h"
#include <QTextCodec>//支持中文
#include "include/PythonQt.h"

PythonQtObjectPtr m_pythonOb;
PythonQtObjectPtr m_pythonObFactory;
CMDPythonRedisClient::CMDPythonRedisClient()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    PythonQt::init();
    m_pythonObFactory = PythonQt::self()->getMainModule();
    //m_pythonConsole = new PythonQtScriptingConsole(NULL,m_pythonObFactory);
    m_pythonObFactory.evalScript(QString("import sys\n"));
    m_timer = NULL;
}

CMDPythonRedisClient::~CMDPythonRedisClient()
{
    Q_ASSERT(!m_pythonOb.isNull());
    m_pythonOb.call("release",QVariantList());
}

void CMDPythonRedisClient::recvSubscribeMessage(QVariant message)
{
    emit recvSubMessage(message);
}

//!连接Redis服务器
bool CMDPythonRedisClient::openConnection(QVector<QString> hostArr, QVector<int>& portArr, bool cluster)
{
    m_pythonObFactory.evalFile(":Redis.py");
    Q_ASSERT(!m_pythonObFactory.isNull());
    //python 控制台
    //m_pythonConsole = new PythonQtScriptingConsole(NULL,m_pythonObFactory);
    //显示python 控制台
    //m_pythonConsole->appendCommandPrompt();
    // m_pythonConsole->show();
    //检查IP，Port个数
    if(hostArr.length() == portArr.length() && hostArr.length() > 0)
    {
        if(m_pythonOb.isNull())
        {
             m_pythonOb = m_pythonObFactory.evalScript("RedispyClient()\n",Py_eval_input);
             Q_ASSERT(!m_pythonOb.isNull());
             m_pythonOb.addObject("MDRedisClient", this);

             if(cluster)
             {
                QString strhost = hostArr[0];
                QString strport = QString::number(portArr[0]);
                for(int i = 1; i<hostArr.length(); i++)
                {
                    strhost.append("@");
                    strhost.append(hostArr[i]);

                    strport.append("@");
                    strport.append(QString::number(portArr[i]));
                }
                m_pythonOb.call("onInit",QVariantList()<< strhost << strport << "True");
             }
             else
             {
                m_pythonOb.call("onInit",QVariantList()<< hostArr[0] << portArr[0] << "False");
             }
        }
    }
}

//!发送命令
QVariant  CMDPythonRedisClient::sendCommand(QStringList& cmd)
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

void CMDPythonRedisClient::TimerOut()
{
    if(!m_pythonOb.isNull())
    {
        m_pythonOb.call("triggerRecvSubscribeMsg",QVariantList());
    }
}

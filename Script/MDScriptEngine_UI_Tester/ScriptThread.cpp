#include "ScriptThread.h"

// 命令队列处理线程
CScriptThread::CScriptThread(QObject *parent) :
    QThread(parent)
{

}

CScriptThread::~CScriptThread()
{
}

void CScriptThread::run()
{
    // QScopedPointer<CNotificationHandler> handlerSP(m_handler);

    load();

    exec();

    unload();
}

void CScriptThread::load()
{
    m_scriptEngine = new CMDScriptEngine();
    m_scriptEngine->init(true);

//    m_userExpert = new CMDUserWrapperExpert();
//    m_userExpert->setScriptEngine(m_scriptEngine);

//    m_user = new CMDSimpleUser();
//    m_user->setName("u1");
//    m_userExpert->addWrapper("u1", m_user);

//    QList<CMDObjectWrapper*> wrappers;
//    m_userExpert->Wrappers(wrappers);
//    m_scriptEngine->addObjects(wrappers);

//    QString source = "var = 3";//getCode();
//    m_scriptEngine->run(source);
}

void CScriptThread::unload()
{
    m_scriptEngine->release();
}

QString CScriptThread::getCode()
{
    QString source(
                "var = u1.GetUID();"
"u1.doWork('u1', 8);"
    "function u1_onChanged(w)"
   "{"
       "u1.GetUID();"
   "}");
    return source;
}

void CScriptThread::fireEvent()
{
//    QList<QVariant> paras;
//    paras.append(QVariant(666));
//    m_userExpert->fireEvent("u1", "onChanged", paras);
}

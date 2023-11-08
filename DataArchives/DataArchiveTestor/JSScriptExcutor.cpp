#include "JSScriptExcutor.h"
#include <QtScript/QScriptEngine>
#include <QMessageBox>
//#include "jsdataarchivetestor.h"
#include "jsoutputwindow.h"
#include "scriptcasegroupmgr.h"

JSScriptExcutor * JSScriptExcutor::m_pExecutor = 0;

JSScriptExcutor::JSScriptExcutor(QObject *parent)
    :QThread(parent),
      m_bActived(false),
      m_bStop(false)
{
    m_pJSEngine = new QScriptEngine;
    //m_pJSDATestor = new JSDataArchivesTestor;
    m_pJSOutputWin = new JSOutputwindow;
}

JSScriptExcutor::~JSScriptExcutor()
{
    m_bStop = true;
    m_Condition.wakeOne();
    wait();

    delete m_pJSEngine;
    //delete m_pJSDATestor;
    delete m_pJSOutputWin;
}

JSScriptExcutor * JSScriptExcutor::GetInst()
{
    if(!m_pExecutor)
    {
        m_pExecutor = new JSScriptExcutor();
        m_pExecutor->Init();
    }

    return m_pExecutor;
}

void JSScriptExcutor::Init()
{
    //QScriptValue testorObjValue = m_pJSEngine->newQObject(m_pJSDATestor);
    //m_pJSEngine->globalObject().setProperty("dataArchivesTestor", testorObjValue);

    QScriptValue outputObjValue = m_pJSEngine->newQObject(m_pJSOutputWin);
    m_pJSEngine->globalObject().setProperty("outputwindow", outputObjValue);

    start(); //启动线程
        //eng.evaluate("jmonkey.ConvertFromXMLToObj('c:/uu.txt');"
         //                     "jmonkey.StartTest();"
         //                     "jmonkey.age = 9;");

        //qDebug()<<"monkey age = "<<monkey.getAge();
}

QScriptEngine * JSScriptExcutor::GetScriptEngine()
{
    return m_pJSEngine;
}

void JSScriptExcutor::Destroy()
{
    delete m_pExecutor;
    m_pExecutor = 0;
}

//!压入脚本用例名称 此实例在另一个线程中运行对应脚本
bool JSScriptExcutor::PushScriptCaseName(const QString & scriptCaseName)
{
    QMutexLocker locker(&m_Mutex);
    m_ScriptCaseNameList.append(scriptCaseName);
    m_bActived = true;
    m_Condition.wakeOne();
}

void JSScriptExcutor::run()
{
    while (!m_bStop)
    {
        m_Mutex.lock();
        QStringList nameList = m_ScriptCaseNameList;
        m_ScriptCaseNameList.clear();
        m_Mutex.unlock();

        //激活次数可能与wait次数不等 故遍历m_ScriptCaseNameList
        for(int i = 0; i<nameList.size(); i++)
        {
            executeScript(nameList[i]);
        }

        m_Mutex.lock();
        if (!m_bActived)
            m_Condition.wait(&m_Mutex);
        m_bActived = false;
        m_Mutex.unlock();
    }
}

void JSScriptExcutor::executeScript(QString scriptCaseName)
{
    QString fileContent;
    if(!ScriptCaseGroupMgr::GetInst()->GetCase(scriptCaseName, fileContent))
    {
        QMessageBox::information(0, "File Error",  QString("cann't open %1 file").arg(scriptCaseName));
        return;
    }

    m_pJSEngine->evaluate(fileContent);

    if(m_pJSEngine->hasUncaughtException()) //执行的脚本是否有错误
    {
        QScriptValue value = m_pJSEngine->uncaughtException();
        QMessageBox::information(0, "Script Exception", value.toString());
        return ;
    }
}

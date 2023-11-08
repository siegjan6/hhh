#include "MDScriptExpertImp.h"
#include "MDScriptEngine.h"
#include "MDScriptContacter.h"
#include <QDir>

CMDScriptExpertImp::CMDScriptExpertImp()
{
    m_contacter = NULL;
    m_pid = "";
    m_lsRunScriptProcess = NULL;
}

/*!
\section 初始化与释放
*/
void CMDScriptExpertImp::setParaBeforeLoad(const QString& pid, const QString& projectPath, CMDScriptContacter* scriptContacter)
{
    m_pid = pid;
    m_projectPath = projectPath;
    m_scriptPath = makePath(projectPath);

    m_contacter = scriptContacter;
}

void CMDScriptExpertImp::load()
{
    createSwitchs(true);
}

void CMDScriptExpertImp::unload()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    lsProjectLoad->LOG_INFO("停止全部脚本引擎");
    stopAll();

    createSwitchs(false);
}

QString CMDScriptExpertImp::makePath(const QString& projectPath)
{
    QString modulePath;
    #ifdef Q_OS_LINUX
    modulePath = projectPath + "/Script/";
    #else
    modulePath = projectPath + "\\Script\\";
    #endif

    QDir dir;
    dir.mkpath(modulePath);
    return modulePath;
}

void CMDScriptExpertImp::createSwitchs(bool create)
{
    if(create)
    {
        QString subFileDir = "ProjectExpert";
        m_lsRunScriptProcess = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "ScriptExpert.RunScriptProcess", "脚本运行流程", subFileDir);
    }
    else
    {
        m_lsRunScriptProcess = NULL;
    }
}

/*!
\section 控制脚本运行；管理脚本引擎
*/
bool CMDScriptExpertImp::runScript(const QString& nameOnly)
{
    QMap<QString, CMDScriptEngine*>::const_iterator iPosition = m_engineMap.find(nameOnly);
    if(m_engineMap.constEnd() != iPosition)
    {
        iPosition.value()->runAgain();
        return true;
    }

    CMDScriptEngine* engine = NULL;
    QList<CMDObjectWrapper*> wrapperList;
    if(m_contacter)
        m_contacter->objectWrappers(wrapperList);

    engine = new CMDScriptEngine;
    if(engine)
    {
        engine->init(false, m_projectPath);
        engine->addObjects(wrapperList);
        connect(engine,SIGNAL(scriptDebug(const QString&,const QString&)),this,SLOT(slotOnScriptDebug(const QString&, const QString&)));
        engine->runFile(jsFileName(nameOnly));

        m_engineMap.insert(nameOnly, engine);
    }
    return (engine != NULL);
}

bool CMDScriptExpertImp::stopScript(const QString& nameOnly)
{
    QMap<QString, CMDScriptEngine*>::const_iterator iPosition = m_engineMap.find(nameOnly);
    if(m_engineMap.constEnd() != iPosition)
    {
        CMDScriptEngine* engine = iPosition.value();
        m_engineMap.remove(nameOnly);

        engine->release();
        delete engine;

        return true;
    }

    return false;
}

void CMDScriptExpertImp::runAll()
{
    QDir qd(m_scriptPath);
    QFileInfoList fiList = qd.entryInfoList(QDir::Files);

    QList<QFileInfo>::const_iterator i;
    for (i = fiList.constBegin(); i != fiList.constEnd(); ++i)
    {
        QString fileName = (*i).fileName();

        if(isValidJSFile(fileName)) // 是有效的 JS 文件吗？
            runScript(fileName);
    }

    // Java 风格的迭代器
//    QListIterator<QString> iPosition(nameOnlyList);
//    while (iPosition.hasNext())
//    {
//        runScript(iPosition.next());
//    }
}

void CMDScriptExpertImp::stopAll()
{
    QMap<QString, CMDScriptEngine*>::const_iterator iPosition = m_engineMap.constBegin();
    while(m_engineMap.constEnd() != iPosition)
    {
        CMDScriptEngine* engine = iPosition.value();
        engine->release();
        delete engine;

        ++ iPosition;
    }
    m_engineMap.clear();
}

QString CMDScriptExpertImp::jsFileName(const QString& nameOnly)
{
    QString fileName = m_scriptPath + nameOnly;
    return fileName;
}

bool CMDScriptExpertImp::isValidJSFile(const QString& fileName)
{
    QString dotJS = fileName.right(3);
    return (0 == dotJS.compare(".js", Qt::CaseSensitivity::CaseInsensitive));
}

void CMDScriptExpertImp::slotOnScriptDebug(const QString& msg, const QString& source/*=""*/)
{
    if(m_lsRunScriptProcess)
        m_lsRunScriptProcess->LOG_INFO_IS(msg, source);
}

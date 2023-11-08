#include "SingleRuntimeApp.h"
#include "mainwindow.h"
#include <QTimer>
#include "../../include/MDSwitchFactory/MDLogSwitchFactory.h"

CSingleRuntimeApp::CSingleRuntimeApp(int &argc, char **argv)
 : CMDSingleApplication(argc, argv)
{
    m_processingClientData = false;
    m_lsRuntime = nullptr;
    m_virtualRunMode = false;
}

CSingleRuntimeApp::~CSingleRuntimeApp()
{
    CMDMObjFactory::deleteAllFactories();
    CMDSysLogExpert::release();
}

bool CSingleRuntimeApp::onResumeSingleApp()
{
    LOG_RUNTIME("重新显示当前的 MindSCADA 运行时实例");
    return CMDSingleApplication::onResumeSingleApp();
}

void CSingleRuntimeApp::onClientDataReceived(const QString& data)
{
   LOG_RUNTIME("收到客户端的数据！");

    QMutexLocker locker(&m_processingClientDataMutex);
    if(m_processingClientData)
    {
       LOG_RUNTIME("正在处理上次的请求，本次数据被忽略！->" + data);
        return;
    }
    m_processingClientData = true;
    locker.unlock();

   LOG_RUNTIME("解析客户端数据 ...");

   QString _data = data.trimmed();
   int index = _data.indexOf(":");
   QString cmdType;
   if(index != -1)
       cmdType = _data.left(index);
   else cmdType = _data;

   if(0 == QString::compare("RUN",  cmdType, Qt::CaseInsensitive))
   {
       QString projectFileName = _data.right(data.length() - index - 1);
       LOG_RUNTIME("客户端要求运行项目->" + projectFileName);

       LOG_RUNTIME("激活应用主窗口");
       activateAppMainWindow();

       // 由于重新加载是在主线程内执行，因此会导致界面不刷新，此处
       // 延迟1秒，可确保主界面显示出来后，再执行项目的重新加载，
       // 避免用户产生误解
       // -----------------------------------------------------------------------
       m_lastProjectFileName = projectFileName;
       m_virtualRunMode = false;
       QTimer::singleShot(1000, this, SLOT(reloadProject()));
   }
   else if(0 == QString::compare("VirtualRUN",  cmdType, Qt::CaseInsensitive))
   {
       QString projectFileName = _data.right(data.length() - index - 1);
       LOG_RUNTIME("客户端要求虚拟化运行项目->" + projectFileName);

       LOG_RUNTIME("激活应用主窗口");
       activateAppMainWindow();

       // 由于重新加载是在主线程内执行，因此会导致界面不刷新，此处
       // 延迟1秒，可确保主界面显示出来后，再执行项目的重新加载，
       // 避免用户产生误解
       // -----------------------------------------------------------------------
       m_lastProjectFileName = projectFileName;
       m_virtualRunMode = true;
       QTimer::singleShot(1000, this, SLOT(reloadProject()));
   }
}

void CSingleRuntimeApp::reloadProject()
{
   LOG_RUNTIME("准备运行客户端要求的项目");
    MainWindow* mainWnd = (MainWindow*)appMainWindow();
    if(mainWnd)
    {
        mainWnd->reloadProject(m_lastProjectFileName, m_virtualRunMode);
    }

    QMutexLocker locker(&m_processingClientDataMutex);
    m_processingClientData = false;
}

void CSingleRuntimeApp::LOG_RUNTIME(const QString& msg)
{
    m_lsRuntime->LOG_INFO(msg);
}

void CSingleRuntimeApp::initSwitchs()
{
    if(!m_lsRuntime)
    {
        QString appPath = QCoreApplication::applicationDirPath();
        m_lsRuntime = CMDLogSwitchFactory::createSwitchWithLogger(appPath, "Runtime", "MindSCADA 运行时");
        m_lsRuntime->turnOn();
    }
}

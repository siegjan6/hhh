#include "../../../include/MDSingleApplication/MDSingleApplication.h"
#include "MDSingleApplicationImp.h"
#include "MDSingleAppClientImp.h"

CMDSingleApplication::CMDSingleApplication(int &argc, char **argv)
 : QApplication(argc, argv)
{
    m_imp = new CMDSingleApplicationImp(this);
}

CMDSingleApplication::~CMDSingleApplication()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 运行单实例
\note 派生于 CMDSingleApplication 的应用都具有一个应用标识，缺省标识是
应用程序名 < QFileInfo(QCoreApplication::applicationFilePath()).fileName(); >，
要改变此缺省值，可在调用 run 时指定，或在调用 run 之前调用 setAppServerName。
*/
bool CMDSingleApplication::run(const QString& appServerName)
{
    return (m_imp ? m_imp->run(appServerName) : false);
}

void CMDSingleApplication::setAppServerName(const QString& name)
{
    if(m_imp)
        m_imp->setAppServerName(name);
}

QString CMDSingleApplication::appServerName()
{
    return (m_imp ? m_imp->appServerName() : "");
}

/*!
\section 管理应用主窗口
*/
void CMDSingleApplication::setAppMainWnd(QWidget *appMainWindow)
{
    if(m_imp)
        m_imp->setAppMainWnd(appMainWindow);
}

QWidget *CMDSingleApplication::appMainWindow()
{
    return (m_imp ? m_imp->appMainWindow() : NULL);
}

void CMDSingleApplication::activateAppMainWindow()
{
    if(m_imp)
        m_imp->activateAppMainWindow();
}

/*!
\section 可重载接口
、 onResumeSingleApp -
    如果已经有一个实例在运行，尝试运行多个实例的操作会
    将第一个实例的主界面显示出来，要改变此缺省行为，
    必须重载  onResumeSingleApp，并返回 true；
、 onClientDataReceived - 需要处理客户端数据时重载此接口。
*/
bool CMDSingleApplication::onResumeSingleApp()
{
    return false;
}

void CMDSingleApplication::onClientDataReceived(const QString& data)
{

}

/*!
 * \brief 单实例运行控制类的客户端类
如果希望控制其他派生于 CMDSingleApplication 的应用，则必须使用
CMDSingleAppClient 类，并通过 init 指定想要控制的应用标识，该标识
必须与应用的标识一致
 */
CMDSingleAppClient::CMDSingleAppClient()
{
    m_imp = new CMDSingleAppClientImp;
}

CMDSingleAppClient::~CMDSingleAppClient()
{
    if(m_imp)
        delete m_imp;
}

void CMDSingleAppClient::init(const QString& appServerName)
{
    if(m_imp)
        m_imp->init(appServerName);
}

void CMDSingleAppClient::release()
{
    if(m_imp)
        m_imp->release();
}

bool CMDSingleAppClient::sendToServer(const QString& data)
{
    return (m_imp ? m_imp->sendToServer(data) : false);
}

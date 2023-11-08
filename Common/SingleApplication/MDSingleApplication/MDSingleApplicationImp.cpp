#include "MDSingleApplicationImp.h"
#include "../../../include/MDSingleApplication/MDSingleApplication.h"

#include <QApplication>
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>
#include <QThread>

#define TIME_OUT                (500)    // 500ms

CMDSingleApplicationImp::CMDSingleApplicationImp(CMDSingleApplication* singleApp)
{
    m_singleApp = singleApp;
    m_localServer = NULL; //! 本地socket Server
    m_appMainWindow = NULL;             //! MainWindow指针
}

CMDSingleApplicationImp::~CMDSingleApplicationImp()
{
    if(m_localServer)
    {
        m_localServer->close();
        delete m_localServer;
    }
    m_localClient.release();
}

/*!
\section 运行单实例
\note 派生于 CMDSingleApplication 的应用都具有一个应用标识，缺省标识是
应用程序名 < QFileInfo(QCoreApplication::applicationFilePath()).fileName(); >，
要改变此缺省值，可在调用 run 时指定，或在调用 run 之前调用 setAppServerName。
\note 通过启动一个端口监听，确保单实例运行。
*/
bool CMDSingleApplicationImp::run(const QString& name)
{
    setAppServerName(name);

    m_localClient.init(appServerName());
    bool isOtherInstanceRunning = m_localClient.tryToConnectToServer();
    if(false == isOtherInstanceRunning)
    {
        startOnlyServerForAllApplications();  // 连接不上，说明没有其他实例，
                                                                        // 启动唯一的服务器，阻止后续的其他实例
    }
    else m_localClient.sendToServer(QString("RESUME"));

    return (false == isOtherInstanceRunning);
}

void CMDSingleApplicationImp::startOnlyServerForAllApplications()
{
    m_localServer = new QLocalServer(this);

    connect(m_localServer, SIGNAL(newConnection()), this, SLOT(newConnectionAccepted()));

    if(!m_localServer->listen(m_serverName))
    {
         // 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
         if(m_localServer->serverError() == QAbstractSocket::AddressInUseError)
         {
             QLocalServer::removeServer(m_serverName); // <-- 重点
             m_localServer->listen(m_serverName); // 再次监听
         }
    }
}

void CMDSingleApplicationImp::setAppServerName(const QString& name)
{
    if(name.isEmpty()) // 没有指定，则取应用程序名作为LocalServer的名字
        m_serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    else
        m_serverName = name;
}

QString CMDSingleApplicationImp::appServerName()
{
    return m_serverName;
}

/*!
\section 管理应用主窗口
*/
void CMDSingleApplicationImp::setAppMainWnd(QWidget *appMainWindow)
{
    m_appMainWindow = appMainWindow;
}

QWidget *CMDSingleApplicationImp::appMainWindow()
{
    return m_appMainWindow;
}

void CMDSingleApplicationImp::activateAppMainWindow()
{
    if(m_appMainWindow)
    {
        m_appMainWindow->hide();
        m_appMainWindow->raise();
        m_appMainWindow->activateWindow();
        m_appMainWindow->showMaximized();
    }
}

/*!
\section 处理客户端连接和数据
*/
void CMDSingleApplicationImp::newConnectionAccepted()
{
    QLocalSocket *clientConnection = m_localServer->nextPendingConnection();
    if(clientConnection)
    {
         connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
         connect(clientConnection, SIGNAL(readyRead()),this, SLOT(readyReadClientData()));

         // TODO：此处的 QLocalSocket 对象指针应该何时释放？
         //delete clientConnection;
    }
}

void CMDSingleApplicationImp::readyReadClientData()
{
    QLocalSocket *clientConnection = static_cast<QLocalSocket *>(sender());
    if (!clientConnection)
        return;

    QTextStream in(clientConnection);
    in.setCodec("UTF-8");
    QString data;
    data = in.readAll();

    handleClientData(data);
}

void CMDSingleApplicationImp::handleClientData(const QString& data)
{
    data.trimmed();
   int index = data.indexOf(":");
   QString cmdType;
   if(index != -1)
       cmdType = data.left(index);
    else cmdType = data;

   if(0 == QString::compare("RESUME",  cmdType, Qt::CaseInsensitive))
   {
       // 如果外部处理了该消息（onResumeSingleApp 返回 true），则内部不做处理
       bool handleOutSide = (m_singleApp ? m_singleApp->onResumeSingleApp() : false);
       if(!handleOutSide)
           activateAppMainWindow();
   }
   else
   {
       if(m_singleApp)
           m_singleApp->onClientDataReceived(data);
   }
}

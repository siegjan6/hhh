/*!

\author dGFuZ3p3

\section 功能

单实例运行控制类的实现类。

*/

#ifndef MDSINGLEAPPLICATIONIMP_H
#define MDSINGLEAPPLICATIONIMP_H

#include <QObject>
#include <QtNetwork/QLocalServer>
#include <QWidget>
#include <QtNetwork/QLocalSocket>

#include "MDSingleAppClientImp.h"

class CMDSingleApplication;
class CMDSingleApplicationImp : public QObject
{
    Q_OBJECT
public:
    CMDSingleApplicationImp(CMDSingleApplication* singleApp);
    ~CMDSingleApplicationImp();
private:
     CMDSingleApplication *m_singleApp;

/*!
\section 运行单实例
\note 派生于 CMDSingleApplication 的应用都具有一个应用标识，缺省标识是
应用程序名 < QFileInfo(QCoreApplication::applicationFilePath()).fileName(); >，
要改变此缺省值，可在调用 run 时指定，或在调用 run 之前调用 setAppServerName。
\note 通过启动一个端口监听，确保单实例运行。
*/
public:
    bool run(const QString& appServerName);
private:
    void startOnlyServerForAllApplications(); //! 启动服务器
private:
    QLocalServer *m_localServer ;// NULL; //! 本地socket Server
    CMDSingleAppClientImp m_localClient;
public:
    void setAppServerName(const QString& name);
    QString appServerName();
private:
    QString m_serverName; //! 应用标识

 /*!
 \section 管理应用主窗口
 */
public:
    void setAppMainWnd(QWidget *appMainWindow);
    QWidget *appMainWindow();
    void activateAppMainWindow();            //! 激活窗口
private:
    QWidget *m_appMainWindow ;// NULL;             //! MainWindow指针

/*!
\section 处理客户端连接和数据
*/
private slots:
     void newConnectionAccepted(); //! 有新连接时触发
     void readyReadClientData();
private:
    void handleClientData(const QString& data);
};

#endif // MDSINGLEAPPLICATIONIMP_H

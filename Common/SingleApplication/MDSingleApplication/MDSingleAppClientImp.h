/*!

\author dGFuZ3p3

\section 功能

单实例运行控制类的客户端类。

*/

#ifndef MDSINGLEAPPCLIENTIMP_H
#define MDSINGLEAPPCLIENTIMP_H

#include <QtNetwork/QLocalSocket>

class CMDSingleAppClientImp
{
public:
    CMDSingleAppClientImp();
    void init(const QString& appServerName);
    void release();
private:
    QString m_serverName;               //! 应用标识
public:
    bool tryToConnectToServer();
    bool sendToServer(const QString& data);
private:
   QLocalSocket m_localSocket;
};

#endif // MDSINGLEAPPCLIENTIMP_H

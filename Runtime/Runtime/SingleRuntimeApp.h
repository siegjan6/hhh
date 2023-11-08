/*!

\author dGFuZ3p3

\section 功能

实现了 MindSCADA - Runtime 以单实例运行；
响应 MindSCADA - Studio 的指令加载指定项目。

*/

#ifndef SINGLERUNTIMEAPP_H
#define SINGLERUNTIMEAPP_H

#include "MDSingleApplication.h"
#include <QMutex>

class CMDLogSwitch;
class CSingleRuntimeApp : public CMDSingleApplication
{
    Q_OBJECT
public:
    CSingleRuntimeApp(int &argc, char **argv);
    virtual ~CSingleRuntimeApp();
public:
    void LOG_RUNTIME(const QString& msg);
    void initSwitchs();
    CMDLogSwitch* m_lsRuntime;
protected:
    QMutex m_processingClientDataMutex;
    bool m_processingClientData;
    QString m_lastProjectFileName;
    bool m_virtualRunMode;
protected slots:
    void reloadProject();
public:
    virtual bool onResumeSingleApp();
    virtual void onClientDataReceived(const QString& data);
};

#endif // SINGLERUNTIMEAPP_H

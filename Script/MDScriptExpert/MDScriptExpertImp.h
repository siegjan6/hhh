/*!

\author dGFuZ3p3

\section 功能

脚本专家类的实现类，实现运行时的项目全局脚本处理。

*/

#ifndef MDSCRIPTEXPERTIMP_H
#define MDSCRIPTEXPERTIMP_H

#include <QString>
#include <QMap>
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include <QObject>

class CMDScriptContacter;
class CMDScriptEngine;
class CMDScriptExpertImp : public QObject
{
    Q_OBJECT
public:
    CMDScriptExpertImp();

/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, const QString& projectPath, CMDScriptContacter* scriptContacter);
    void load();
    void unload();
protected:
    QString makePath(const QString& projectPath);
    QString m_projectPath;
    QString m_scriptPath;
protected:
    CMDScriptContacter* m_contacter;
    QString m_pid;

/*!
\section 控制脚本运行；管理脚本引擎
*/
public:
    bool runScript(const QString& nameOnly);
    bool stopScript(const QString& nameOnly);
    void runAll();
    void stopAll();
    QString jsFileName(const QString& nameOnly);
    bool isValidJSFile(const QString& fileName);
protected:
    QMap<QString, CMDScriptEngine*> m_engineMap;
protected:
    void createSwitchs(bool create);
    CMDLogSwitch* m_lsRunScriptProcess;
private slots:
    void slotOnScriptDebug(const QString& msg, const QString& source="");
};

#endif // MDSCRIPTEXPERTIMP_H

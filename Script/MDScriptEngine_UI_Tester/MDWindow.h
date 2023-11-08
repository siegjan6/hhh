#ifndef MDWINDOW_H
#define MDWINDOW_H

#include <QString>
#include <QSize>
#include "MDScriptEngine.h"

class CMDWindowWrapper;
class CMDWindow
{
public:
    CMDWindow();
    CMDWindow(const QString& name);
    ~CMDWindow();
public:
    QSize m_size;
    QString m_name;
    void setCode(const QString& code);
    QString getCode() const;
    QString m_code;
public:
    void showMsg(const QString& msg);
public:
    void initScript(const CMDObjectWrapperMgr& userWM, const QString& PID);
    void runScript();
    void releaseScript();
    void fireEvent_SizeChanged(int x, int y);
    CMDScriptEngine m_scriptEngine;
    CMDWindowWrapper* m_wrapper;
    CMDObjectWrapperMgr m_wrappersMgr;
};

#endif // MDWINDOW_H

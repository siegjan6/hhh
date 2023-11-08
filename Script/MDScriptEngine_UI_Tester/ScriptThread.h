#ifndef SCRIPTTHREAD_H
#define SCRIPTTHREAD_H

#include <QThread>
#include "MDScriptEngine.h"

class CScriptThread : public QThread
{
    Q_OBJECT
public:
    explicit CScriptThread(QObject *parent = 0);
    ~CScriptThread();
public:
    void load();
    void unload();
    QString getCode();
    void fireEvent();

    //CMDSimpleUser* m_user;
    CMDScriptEngine* m_scriptEngine;
    //CMDUserWrapperExpert* m_userExpert;
protected:
    void run();
};

#endif // SCRIPTTHREAD_H

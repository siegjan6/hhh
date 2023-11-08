#ifndef SCRIPTTHREAD_H
#define SCRIPTTHREAD_H
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QStringList>

class QScriptEngine;
//class JSDataArchivesTestor;
class JSOutputwindow;

class JSScriptExcutor  : public QThread
{
    Q_OBJECT
public:
    static JSScriptExcutor * GetInst();
    void Init();
    QScriptEngine * GetScriptEngine();
    void Destroy();
    //!压入脚本用例名称 此实例在另一个线程中运行对应脚本
    bool PushScriptCaseName(const QString & scriptCaseName);
protected:
    JSScriptExcutor(QObject *parent = 0);
    ~JSScriptExcutor();
    void run() Q_DECL_OVERRIDE;
    void executeScript(QString scriptCaseName);
 private:
    static JSScriptExcutor * m_pExecutor;
    QScriptEngine * m_pJSEngine;
    //JSDataArchivesTestor * m_pJSDATestor;
    JSOutputwindow * m_pJSOutputWin;
    QStringList m_ScriptCaseNameList;
    QMutex m_Mutex;
    bool m_bActived;
    QWaitCondition m_Condition;
    volatile bool m_bStop;
};

#endif // SCRIPTTHREAD_H

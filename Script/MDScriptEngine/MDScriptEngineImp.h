/*!

\author dGFuZ3p3

\section 功能 - 脚本引擎类，实现对Google V8引擎的封装，便于使用。具体功能如下：
、初始化和释放引擎；
、添加对象到引擎，在js代码中通过名字引用对象；
、运行指定的js代码，参考 run(const QString& sourceCode);
、运行指定的js文件，参考 runFile(const QString& jsFile);
、重复运行引擎中的代码，参考 runAgain();
、执行js代码中的函数，实现“事件触发”机制。

\section 线程特性
、引擎接口不是线程安全的，接口都必须在主线程调用。

*/

#ifndef MDSCRIPTENGINEIMP_H
#define MDSCRIPTENGINEIMP_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QScriptEngine>
#include <QMap>
#include <QPair>
#include <QSet>
#include <QFile>


class CMDObjectWrapper;
class CMDObjectWrapperMgr;
class CMDScriptEngine;
class CMDScriptEnginePool;
class CMDScriptEngineImp:public QObject
{
    Q_OBJECT
public:
    CMDScriptEngineImp();
    ~CMDScriptEngineImp();

/*!
\section 引擎的初始化/释放（PID 是引擎分组的标识）
*/
public:
    bool init(bool newIsolate, const QString& PID);
    void release();
    void setIncludeSysPath(QString path);
    void setIncludePrjPath(QString path);
protected:
    QScriptEngine m_qsEngine;
protected:
    QString m_PID;

/*!
\section 添加对象到引擎，在引擎运行期间，对象必须是有效的
*/
public:
    void addObject(CMDObjectWrapper* so);
    void addObjects(const QList<CMDObjectWrapper*>& soList);
    void addObjects(const CMDObjectWrapperMgr& wm);
protected:
    QList<CMDObjectWrapper*> m_soList;
    void regObject();

/*!
\section 运行代码
*/
public:
    bool runFile(const QString& jsFile);
    bool run(const QString& sourceCode, const QString& dbgSource = "");
    bool runDynamically(const QString& tmpCode);
    bool runAgain();
public:
    static QString helperFileCode(const QString& jsFile);
    static QString helperSampleCode();
    QString getCode() const;
protected:
    QString m_source;
    QString m_scriptFileName;

/*!
\section 调用 js 函数
*/
public:
    void callFunc(const QString& funcName, QList<QVariant>& paras);
    QScriptValue toQScriptValue(const QVariant& v);

signals:
    //! Emitted when the running script id changes (also on start/stop)
    void runningScriptIdChanged(const QString& id);
    //! Notification when a script starts running
    void scriptRunning();
    //! Notification when a script has paused running
    void scriptPaused();
    //! Notification when a script has stopped running
    void scriptStopped();
    //! Notification of a script event - warnings, current execution line etc.
    void scriptDebug(const QString& msg, const QString& source="");
    //! Notification of a script event - output line.
    void scriptOutput(const QString&);

private:
    bool runWithIncludeProcessed(const QString& sourceCode, const QString& dbgSource = "");
    bool prepareScript( QString &script, const QString &fileName);
    bool preprocessFile(const QString fileName, QFile &input, QString& output, const QString& scriptDir);
    bool preprocessScript(const QString fileName, const QString &input, QString &output, const QString &scriptDir, int &errLoc );
    void expand(const QString fileName, const QString &input, QString &output, const QString &scriptDir, int &errLoc);

    // Map line numbers of output to <path>:<line>
    int m_outline;
    QMap<int,QPair<QString,int>> m_num2loc;
    QString lookup( int outline );

    // Registry for include files
    QSet<QString> m_includeSet;

    // 脚本引用系统路径
    QString m_scriptIncludeSysPath;
    // 脚本引用项目路径
    QString m_scriptIncludePrjPath;
};

// Migrating an isolate
//class KangarooThread : public v8::internal::Thread
//{
// public:
//  KangarooThread(v8::Isolate* isolate,
//                 v8::Handle<v8::Context> context, int value)
//      : Thread("KangarooThread"),
//        isolate_(isolate), context_(context), value_(value)
//  {
//  }

//  void Run()
//  {
//    {
//      v8::Locker locker(isolate_);
//      v8::Isolate::Scope isolate_scope(isolate_);
//      //CHECK_EQ(isolate_, v8::internal::Isolate::Current());
//      v8::HandleScope scope;
//      v8::Context::Scope context_scope(context_);
//      //Local<Value> v = CompileRun("getValue()");
//      //CHECK(v->IsNumber());
//      //CHECK_EQ(30, static_cast<int>(v->NumberValue()));
//    }
//    {
//      v8::Locker locker(isolate_);
//      v8::Isolate::Scope isolate_scope(isolate_);
//      v8::Context::Scope context_scope(context_);
//      v8::HandleScope scope;
//      //Local<Value> v = CompileRun("getValue()");
//      //CHECK(v->IsNumber());
//      //CHECK_EQ(30, static_cast<int>(v->NumberValue()));
//    }
//    isolate_->Dispose();
//  }

//private:
// v8::Isolate* isolate_;
// Persistent<v8::Context> context_;
// int value_;
//};

#endif // MDSCRIPTENGINEIMP_H

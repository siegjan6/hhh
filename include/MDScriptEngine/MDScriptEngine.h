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

\section 用法
、基本用法：
QList<CMDObjectWrapper*> wrapperList;
// TODO : 准备包装好的对象的链表 wrapperList
// ...
CMDScriptEngine* engine = new CMDScriptEngine;
if(engine)
{
    engine->init(false); // 初始化
    engine->addObjects(wrapperList); // 添加包装好的对象到引擎，必须在 init 之后
    engine->runFile("test.js"); // 运行代码

    engine->release();
    delete engine;
 }
、对象包装：
将 C++ 对象包装为 js 对象，参考示例文件 MDUiWrappers.h 和 MDUiWrappers.cpp；
、事件触发：
（1）仅在特定的引擎做事件触发：
QList<QVariant> paras;
paras.append(QVariant("u1"));
engine->fireEvent("u1", "OnLogin", paras);
上述代码将触发 engine 里如下的js函数：
function u1_OnLogin(userName)
{
}
（2）在所有的关联引擎做事件触发：
QList<QVariant> paras;
paras.append(QVariant("u1"));
CMDScriptEngine::fireEventInPool("u1", "OnLogin", paras);
上述代码将触发关联引擎里如下的js函数：
function u1_OnLogin(userName)
{
}

参数解析：
在 init；fireEventInPool；callFuncInPool 中都有可选参数 PID，
其目的是对引擎进行分组，具有相同 PID 的引擎属于同一组，
fireEventInPool 和 callFuncInPool 按照组来触发引擎；
如果 MindSCADA 运行时可以同时加载多个项目，则要使用项目信息
作为分组依据。

\section 修改历史
2015.5 / dGFuZ3p3 / 创建；
2015.5.21 / dGFuZ3p3 / 功能完成并写注释。
*/

#ifndef MDSCRIPTENGINE_H
#define MDSCRIPTENGINE_H

#include "MDScriptEngine_Global.h"
#include "MDObjectWrapper.h"

//class CMDLogSwitch;
class CMDScriptEngineImp;
class MDSCRIPTENGINESHARED_EXPORT CMDScriptEngine : public QObject
{
    Q_OBJECT
public:
    CMDScriptEngine();
    ~CMDScriptEngine();
/*!
\section 引擎的初始化/释放（PID 是引擎分组的标识）
*/
public:
    bool init(bool newIsolate = false, const QString& PID = "");
    void release();

/*!
\section 添加对象到引擎，在引擎运行期间，对象必须是有效的
*/
public:
    void addObject(CMDObjectWrapper* so);
    void addObjects(const QList<CMDObjectWrapper*>& soList);
    void addObjects(const CMDObjectWrapperMgr& wm);

/*!
\section 运行代码
*/
public:
    bool run(const QString& sourceCode, const QString& dbgSource = "");
    bool runDynamically(const QString& tmpCode);
    bool runFile(const QString& jsFile);
    bool runAgain();
public:
    QString getCode() const;
    static QString helperFileCode(const QString& jsFile);
    static QString helperSampleCode();
//    static CMDLogSwitch* pScriptEngineLogSwitch;

/*!
\section 触发事件（PID 是引擎分组的标识）
*/
public:
    void fireEvent(const QString& objName, const QString& eventName, QList<QVariant>& paras);
    static void fireEventInPool(const QString& objName, const QString& eventName, QList<QVariant>& paras, const QString& PID = "");
public:
    void callFunc(const QString& funcName, QList<QVariant>& paras);
    static void callFuncInPool(const QString& funcName, QList<QVariant>& paras, const QString& PID = "");
signals:
    //! Notification of a script event - warnings, current execution line etc.
    void scriptDebug(const QString& msg, const QString& source="");
private slots:
    void slotOnScriptDebug(const QString& msg, const QString& source="");
protected:
    CMDScriptEngineImp* m_imp;
};

#endif // MDSCRIPTENGINE_H

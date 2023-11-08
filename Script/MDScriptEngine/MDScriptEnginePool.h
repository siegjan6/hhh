/*!

\author dGFuZ3p3

\section 功能

引擎池类及其管理类，在模块内部实现对引擎对象的分组管理；
目的是实现按组来触发引擎运行的 js 函数，实现脚本的“事件触发”功能。

\section 应用场景
在进行事件触发时，有些事件是针对特定引擎的，
例如某个窗口的按钮单击事件，其他窗口并不关心；
有些事件则是全局性的，例如用户登录事件，凡是包含用户登录事件的引擎
都必须被触发（引擎池里的引擎都必须被触发）；
而 MindSCADA 运行时可能包含多个引擎池（同时运行多个项目，则每个项目都
包含一个引擎池），每个引擎池通过 PID 来区分。

\section 线程特性
、引擎接口不是线程安全的，接口都必须在主线程调用。

\section 用法
、引擎在初始化时注册到引擎池：
bool CMDScriptEngineImp::init(bool newIsolate, const QString& PID)
{
    m_PID = PID;
    CMDScriptEnginePoolMgr::regToPool(this, m_PID);
    // ...
}

释放时取消注册：
void CMDScriptEngineImp::release()
{
    CMDScriptEnginePoolMgr::unregFromPool(this, m_PID);
    // ...
}

按照引擎池来触发事件：
void CMDScriptEngine::callFuncInPool(const QString& funcName, QList<QVariant>& paras, const QString& PID)
{
    CMDScriptEnginePoolMgr::callFuncInPool(funcName, paras, PID);
}

\section 修改历史
2015.5 / dGFuZ3p3 / 创建；
2015.5.21 / dGFuZ3p3 / 功能完成并写注释。

*/

#ifndef MDSCRIPTENGINEPOOL_H
#define MDSCRIPTENGINEPOOL_H

#include <QMap>
#include <QVariant>

class CMDScriptEngineImp;
/*!
 * \brief 引擎池类，管理运行的多个脚本引擎
 */
class CMDScriptEnginePool
{
public:
    CMDScriptEnginePool();
    ~CMDScriptEnginePool();
    void regEngine(CMDScriptEngineImp* engine);
    void unregEngine(CMDScriptEngineImp* engine);
    void unregAll();
public:
    //! 对池中的每个引擎调用函数 funcName
    void callFunc(const QString& funcName, QList<QVariant>& paras);
protected:
    QMap<CMDScriptEngineImp*, CMDScriptEngineImp*> m_engineMap;
};

/*!
 * \brief 引擎池类的管理类
 */
class CMDScriptEnginePoolMgr
{
public:
    static CMDScriptEnginePool* regToPool(CMDScriptEngineImp* engine, const QString& PID = "");
    static bool unregFromPool(CMDScriptEngineImp* engine, const QString& PID = "");
    static bool callFuncInPool(const QString& funcName, QList<QVariant>& paras, const QString& PID = "");
public:
    static CMDScriptEnginePool* addPool(const QString& PID = "");
    static CMDScriptEnginePool* findPool(const QString& PID = "");
    static bool deletePool(const QString& PID = "");
    static void deleteAllPools();
protected:
    static QMap<QString, CMDScriptEnginePool*> m_poolMap;

private:
    CMDScriptEnginePoolMgr(){};
public:
    static CMDScriptEnginePoolMgr* instance();
    static void release();
protected:
    static CMDScriptEnginePoolMgr* m_single;
};

#endif // MDSCRIPTENGINEPOOL_H

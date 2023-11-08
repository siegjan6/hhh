/*!

\author dGFuZ3p3

\section 功能

C++对象包装类 CMDObjectWrapper 和辅助管理类 CMDObjectWrapperMgr。

\section 线程特性
、引擎接口不是线程安全的，接口都必须在主线程调用。

\section 用法
、基本用法：
假设有 C++ 类 CMDUser 希望在脚本中使用，则必须从 CMDObjectWrapper 派生一个
包装类，如下代码所示：
class CMDUser;
class CMDUserWrapper : public CMDObjectWrapper
{
public:
    CMDUserWrapper(void* object, const QString& name);
    // ...
};
构造时将 CMDUser 的实例指针和名字传递给包装类。
如何添加到引擎，参考 CMDScriptEngine 的说明。
详细的代码示例，参考示例文件 MDUiWrappers.h 和 MDUiWrappers.cpp。

\section 修改历史
2015.5 / dGFuZ3p3 / 创建；
2015.5.21 / dGFuZ3p3 / 功能完成并写注释。
*/

#ifndef MDOBJECTWRAPPER_H
#define MDOBJECTWRAPPER_H

#include "MDScriptEngine_Global.h"
#include <QObject>
#include <QString>
#include <QVariant>
#include <QList>

class CMDObjectWrapperImp;
/*!
 * \brief C++对象包装类
 */
class MDSCRIPTENGINESHARED_EXPORT CMDObjectWrapper : public QObject
{
    friend class CMDScriptEngineImp;

    Q_OBJECT
public:
    CMDObjectWrapper();
    /*!
     构造函数。

     \param object C++对象实例指针。
     \param name 对象名字，在脚本代码中根据此名字引用对象。
     \returns 无。

     \note 通过此函数将 C++ 对象包装为一个脚本对象。
     */
    CMDObjectWrapper(void* object, const QString& name);
    virtual ~CMDObjectWrapper();
public:
    void* MDObject() const; //! 获取 C++对象实例指针
    const char* name() const; //! 获取 const char* 类型的对象名
    QString QName() const; //! 获取 QString 类型的对象名
public:
    //! 注册对象属性和方法到引擎，必须重载
    virtual void registerPropertyAndMethod(void* templ);
protected:
    CMDObjectWrapperImp* m_imp;
};

class CMDObjectWrapperMgrImp;
/*!
 * \brief C++对象包装类的管理类
 */
class MDSCRIPTENGINESHARED_EXPORT CMDObjectWrapperMgr
{
public:
    CMDObjectWrapperMgr();
    virtual ~CMDObjectWrapperMgr();
public:
    bool addWrapper(CMDObjectWrapper* wrapper);
    CMDObjectWrapper* find(const QString& name);
    void Wrappers(QList<CMDObjectWrapper*>& wrapperList) const;
    void releaseWrappers();
protected:
    CMDObjectWrapperMgrImp* m_imp;
};

#endif // MDOBJECTWRAPPER_H

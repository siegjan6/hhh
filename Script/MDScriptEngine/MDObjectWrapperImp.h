/*!

\author dGFuZ3p3

\section 功能

C++对象包装类 CMDObjectWrapper 的实现类和辅助管理类 CMDObjectWrapperMgr 的实现类。

\section 线程特性
、引擎接口不是线程安全的，接口都必须在主线程调用。

*/

#ifndef MDOBJECTWRAPPERIMP_H
#define MDOBJECTWRAPPERIMP_H

#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QList>
#include <QMap>

class CMDObjectWrapper;
/*!
 * \brief C++对象包装类的实现类
 */
class CMDObjectWrapperImp
{
public:
    CMDObjectWrapperImp(void* object, const QString& name);
    virtual ~CMDObjectWrapperImp();
public:
    void* MDObject() const;
    const char* name() const;
    QString QName() const;
protected:
    void* m_object ;// NULL;
    QString m_name;
};

/*!
 * \brief C++对象包装类的管理类的实现类
 */
class CMDObjectWrapperMgrImp
{
public:
    CMDObjectWrapperMgrImp();
    ~CMDObjectWrapperMgrImp();
public:
    bool addWrapper(CMDObjectWrapper* wrapper);
    CMDObjectWrapper* find(const QString& name);
    void Wrappers(QList<CMDObjectWrapper*>& userWrapperList);
    void releaseWrappers();
protected:
    QMap<QString, CMDObjectWrapper*> m_WrapperMap;
};

#endif // MDOBJECTWRAPPERIMP_H

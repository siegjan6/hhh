#include "MDObjectWrapperImp.h"
#include "MDScriptEngine.h"

/*!
 * \brief C++对象包装类的实现类
 */
CMDObjectWrapperImp::CMDObjectWrapperImp(void* object, const QString& name)
{
    m_object = object;
    m_name = name;
}

CMDObjectWrapperImp::~CMDObjectWrapperImp()
{

}

void* CMDObjectWrapperImp::MDObject() const
{
    return m_object;
}

const char* CMDObjectWrapperImp::name() const
{
    QByteArray ba = m_name.toLatin1();
    return ba.data();
}

QString CMDObjectWrapperImp::QName() const
{
    return m_name;
}

/*!
 * \brief C++对象包装类的管理类的实现类
 */
CMDObjectWrapperMgrImp::CMDObjectWrapperMgrImp()
{
}

CMDObjectWrapperMgrImp::~CMDObjectWrapperMgrImp()
{
    releaseWrappers();
}

bool CMDObjectWrapperMgrImp::addWrapper(CMDObjectWrapper* wrapper)
{
    if(!wrapper)
        return false;

    if(m_WrapperMap.contains(wrapper->QName()))
    {
        delete wrapper;
        return false;
    }

    m_WrapperMap.insert(wrapper->QName(), wrapper);
    return true;
}

CMDObjectWrapper* CMDObjectWrapperMgrImp::find(const QString& name)
{
    QMap<QString, CMDObjectWrapper*>::const_iterator iPosition = m_WrapperMap.find(name);
    return (m_WrapperMap.constEnd() != iPosition ? iPosition.value() : NULL);
}

void CMDObjectWrapperMgrImp::Wrappers(QList<CMDObjectWrapper*>& WrapperList)
{
    QMap<QString, CMDObjectWrapper*>::const_iterator iPosition = m_WrapperMap.constBegin();
    while(m_WrapperMap.constEnd() != iPosition)
    {
        WrapperList.append(iPosition.value());
        ++ iPosition;
    }
}

void CMDObjectWrapperMgrImp::releaseWrappers()
{
    QMap<QString, CMDObjectWrapper*>::const_iterator iPosition = m_WrapperMap.constBegin();
    while(m_WrapperMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_WrapperMap.clear();
}

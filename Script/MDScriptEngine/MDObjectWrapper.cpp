#include "MDObjectWrapper.h"
#include "MDObjectWrapperImp.h"

/*!
 * \brief C++对象包装类
 */
CMDObjectWrapper::CMDObjectWrapper()
{
    m_imp = new CMDObjectWrapperImp(NULL, "");
}

CMDObjectWrapper::CMDObjectWrapper(void* object, const QString& name)
{
    m_imp = new CMDObjectWrapperImp(object, name);
}

CMDObjectWrapper::~CMDObjectWrapper()
{
    if(m_imp)
        delete m_imp;
}

void* CMDObjectWrapper::MDObject() const
{
    return (m_imp ? m_imp->MDObject() : NULL);
}

const char* CMDObjectWrapper::name() const
{
    return (m_imp ? m_imp->name() : NULL);
}

QString CMDObjectWrapper::QName() const
{
    return (m_imp ? m_imp->QName() : "");
}

// 参考 CMDScriptEngineImp::WrapObject
void CMDObjectWrapper::registerPropertyAndMethod(void* templ)
{

}

/*!
 * \brief C++对象包装类的管理类
 */
CMDObjectWrapperMgr::CMDObjectWrapperMgr()
{
    m_imp = new CMDObjectWrapperMgrImp();
}

CMDObjectWrapperMgr::~CMDObjectWrapperMgr()
{
    if(m_imp)
        delete m_imp;
}

bool CMDObjectWrapperMgr::addWrapper(CMDObjectWrapper* wrapper)
{
    return (m_imp ? m_imp->addWrapper(wrapper) : false);
}

void CMDObjectWrapperMgr::Wrappers(QList<CMDObjectWrapper*>& wrapperList) const
{
    if(m_imp)
        m_imp->Wrappers(wrapperList);
}

void CMDObjectWrapperMgr::releaseWrappers()
{
    if(m_imp)
        m_imp->releaseWrappers();
}

CMDObjectWrapper* CMDObjectWrapperMgr::find(const QString& name)
{
    return (m_imp ? m_imp->find(name) : NULL);
}


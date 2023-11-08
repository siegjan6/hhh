#include "MDRuntimeUI.h"
#include "MDRuntimeUIImp.h"

CMDRuntimeUI::CMDRuntimeUI()
{
    m_imp = new CMDRuntimeUIImp;
}

CMDRuntimeUI::~CMDRuntimeUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section property
*/
//根节点名称
QString CMDRuntimeUI::rootName()
{
    return "运行参数";
}

//根节点图标
QIcon* CMDRuntimeUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDRuntimeUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDRuntimeUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDRuntimeUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDRuntimeUI::load()
{
    m_imp->load();
}

//卸载
void CMDRuntimeUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDRuntimeUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDRuntimeUI::collectWords()
{
}


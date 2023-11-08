#include "MDScriptUI.h"
#include "MDScriptUIImp.h"

CMDScriptUI::CMDScriptUI()
{
    m_imp = new CMDScriptUIImp;
}

CMDScriptUI::~CMDScriptUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section property
*/
//根节点名称
QString CMDScriptUI::rootName()
{
    return "全局脚本";
}

//根节点图标
QIcon* CMDScriptUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDScriptUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDScriptUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDScriptUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDScriptUI::load()
{
    m_imp->load();
}

//卸载
void CMDScriptUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDScriptUI::changeLanguage()
{
}

void CMDScriptUI::collectWords()
{
}

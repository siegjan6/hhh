#include "MDEventActionUI.h"
#include "MDEventUIContacterImp.h"
#include "MDEventActionUIImp.h"

CMDEventActionUI::CMDEventActionUI()
{
    m_imp = new CMDEventActionUIImp;
}

CMDEventActionUI::~CMDEventActionUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 访问【事件界面】 和 【动作界面】
*/
CMDEventUI* CMDEventActionUI::eventUI()
{
    return &m_imp->m_eventUI;
}

CMDActionUI* CMDEventActionUI::actionUI()
{
    return &m_imp->m_actionUI;
}

/*!
\section property
*/
//根节点名称
QString CMDEventActionUI::rootName()
{
    return "事件动作";
}

//根节点图标
QIcon* CMDEventActionUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDEventActionUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDEventActionUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
    m_imp->m_eventUI.setRootItem(value);
    m_imp->m_actionUI.setRootItem(value);
}

//设置Studio接口
void CMDEventActionUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;

    m_imp->m_eventUI.setStudioInterface(value);
    m_imp->m_actionUI.setStudioInterface(value);
}
/*!
\section function
*/
//加载
void CMDEventActionUI::load()
{
    m_imp->m_eventUI.load();
    m_imp->m_actionUI.load();
}

//卸载
void CMDEventActionUI::unload()
{
    m_imp->m_eventUI.unload();
    m_imp->m_actionUI.unload();
}

/*!
\section 多语言
*/
void CMDEventActionUI::changeLanguage()
{
    m_imp->m_eventUI.changeLanguage();
    m_imp->m_actionUI.changeLanguage();
}

void CMDEventActionUI::collectWords()
{
    m_imp->m_eventUI.collectWords();
    m_imp->m_actionUI.collectWords();
}

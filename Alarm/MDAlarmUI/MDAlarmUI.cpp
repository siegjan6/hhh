#include "MDAlarmUI.h"
#include "MDAlarmUIImp.h"
#include "MDAlarmConfiger.h"
#include "MDAlarmParaForm.h"

CMDAlarmUI::CMDAlarmUI()
{
    m_imp = new CMDAlarmUIImp;
}

CMDAlarmUI::~CMDAlarmUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 访问【报警等级界面】和【报警条件界面】
*/
CMDAlarmLevelUI* CMDAlarmUI::levelUI()
{
    return &m_imp->m_levelUI;
}

CMDAlarmConditionUI* CMDAlarmUI::conditionUI()
{
    return &m_imp->m_conditionUI;
}

/*!
\section property
*/
//根节点名称
QString CMDAlarmUI::rootName()
{
    return "报警";
}

//根节点图标
QIcon* CMDAlarmUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDAlarmUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDAlarmUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
    m_imp->m_levelUI.setRootItem(value);
    m_imp->m_conditionUI.setRootItem(value);
}

//设置Studio接口
void CMDAlarmUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
    m_imp->m_levelUI.setStudioInterface(value);
    m_imp->m_conditionUI.setStudioInterface(value);
}
/*!
\section function
*/
//加载
void CMDAlarmUI::load()
{
    m_imp->m_levelUI.load();
    m_imp->m_conditionUI.load();
}

//卸载
void CMDAlarmUI::unload()
{
    m_imp->m_levelUI.unload();
    m_imp->m_conditionUI.unload();
}

/*!
\section 多语言
*/
void CMDAlarmUI::changeLanguage()
{
    m_imp->m_levelUI.changeLanguage();
    m_imp->m_conditionUI.changeLanguage();
}

void CMDAlarmUI::collectWords()
{
    m_imp->m_levelUI.collectWords();
    m_imp->m_conditionUI.collectWords();
}

#include "MDAlarmLevelUI.h"
#include "MDAlarmLevelUIImp.h"

CMDAlarmLevelUI::CMDAlarmLevelUI()
{
    m_imp = new CMDAlarmLevelUIImp;
}

CMDAlarmLevelUI::~CMDAlarmLevelUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 从界面选择
*/
bool CMDAlarmLevelUI::select(const QString& source, QString& dest)
{
    return (m_imp ? m_imp->select(source, dest) : false);
}

/*!
\section property
*/
//根节点名称
QString CMDAlarmLevelUI::rootName()
{
    return "报警等级";
}

//根节点图标
QIcon* CMDAlarmLevelUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDAlarmLevelUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDAlarmLevelUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDAlarmLevelUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDAlarmLevelUI::load()
{
    m_imp->load();
}

//卸载
void CMDAlarmLevelUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDAlarmLevelUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDAlarmLevelUI::collectWords()
{
    m_imp->collectWords();
}

#include "MDPowerGroupUI.h"
#include "MDPowerGroupUIImp.h"

CMDPowerGroupUI::CMDPowerGroupUI()
{
    m_imp = new CMDPowerGroupUIImp;
}

CMDPowerGroupUI::~CMDPowerGroupUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化
*/
void CMDPowerGroupUI::setAccountUI(CMDAccountUI* accountUI)
{
    if(m_imp)
        m_imp->setAccountUI(accountUI);
}

/*!
\section 显示对象选择界面
*/
bool CMDPowerGroupUI::selectPowerGroups(const QStringList& sourceList, QStringList& destList)
{
    return (m_imp ? m_imp->selectPowerGroups(sourceList, destList) : false);
}

/*!
\section 创建对象树管理界面，目前用于运行时管理权限组
*/
QWidget* CMDPowerGroupUI::createMgrWidget()
{
    return (m_imp ? m_imp->createMgrWidget() : NULL);
}

/*!
\section property
*/
//根节点名称
QString CMDPowerGroupUI::rootName()
{
    return "权限组";
}

//根节点图标
QIcon* CMDPowerGroupUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDPowerGroupUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDPowerGroupUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDPowerGroupUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDPowerGroupUI::load()
{
    m_imp->load();
}

//卸载
void CMDPowerGroupUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDPowerGroupUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDPowerGroupUI::collectWords()
{
    m_imp->collectWords();
}

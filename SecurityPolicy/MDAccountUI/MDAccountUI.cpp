#include "MDAccountUI.h"
#include "MDAccountUIImp.h"

CMDAccountUI::CMDAccountUI()
{
    m_imp = new CMDAccountUIImp;
}

CMDAccountUI::~CMDAccountUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 显示对象选择界面
*/
bool CMDAccountUI::selectAccounts(const QStringList& sourceList, QStringList& destList)
{
    return (m_imp ? m_imp->selectAccounts(sourceList, destList) : false);
}

/*!
\section 创建对象树管理界面，目前用于运行时管理帐户
*/
QWidget* CMDAccountUI::createMgrWidget()
{
    return (m_imp ? m_imp->createMgrWidget() : NULL);
}

/*!
\section property
*/
//根节点名称
QString CMDAccountUI::rootName()
{
    return "帐户";
}

//根节点图标
QIcon* CMDAccountUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDAccountUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDAccountUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDAccountUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDAccountUI::load()
{
    m_imp->load();
}

//卸载
void CMDAccountUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDAccountUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDAccountUI::collectWords()
{
    m_imp->collectWords();
}

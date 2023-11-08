#include "MDDataArchivesUI.h"
#include "DataArchivesUI.h"

CMDDataArchivesUI::CMDDataArchivesUI()
{
    m_imp = new CDataArchivesUI;
}

CMDDataArchivesUI::~CMDDataArchivesUI()
{
    if(m_imp)
    {
        delete m_imp;
        m_imp = NULL;
    }
}

/*!
\section property
*/
//根节点名称
QString CMDDataArchivesUI::rootName()
{
    return QObject::tr("数据库");
}

//根节点图标
QIcon* CMDDataArchivesUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDDataArchivesUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDDataArchivesUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDDataArchivesUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDDataArchivesUI::load()
{
    m_imp->load();
}

//卸载
void CMDDataArchivesUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDDataArchivesUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDDataArchivesUI::collectWords()
{

}

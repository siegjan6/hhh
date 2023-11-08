#include "MDReportDesigner.h"
#include "Report.h"
CMDReportDesigner::CMDReportDesigner()
{
    m_imp = new CReport;
}

CMDReportDesigner::~CMDReportDesigner()
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
QString CMDReportDesigner::rootName()
{
    return "报表";
}

//根节点图标
QIcon* CMDReportDesigner::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDReportDesigner::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDReportDesigner::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDReportDesigner::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDReportDesigner::load()
{
    m_imp->load();
}

//卸载
void CMDReportDesigner::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDReportDesigner::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDReportDesigner::collectWords()
{
}

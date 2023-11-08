#include "../include/MDDBQueryUI/MDDBQueryUI.h"
//#include "MDDBQueryUI\MDDBQueryUI.h"
#include "DBQueryUI.h"

MDDBQueryUI::MDDBQueryUI()
{
    m_DBQueryUI = new DBQueryUI;
}

MDDBQueryUI::~MDDBQueryUI()
{
    if(m_DBQueryUI)
    {
        delete m_DBQueryUI;
        m_DBQueryUI = NULL;
    }
}

/*!
\section property
*/
//根节点名称
QString MDDBQueryUI::rootName()
{
    return QObject::tr("数据视图");
}

//根节点图标
QIcon *MDDBQueryUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem *MDDBQueryUI::rootItem()
{
    return m_DBQueryUI->m_rootInStudio;
}

void MDDBQueryUI::setRootItem(QTreeWidgetItem *value)
{
     m_DBQueryUI->m_rootInStudio = value;
}

//设置Studio接口
void MDDBQueryUI::setStudioInterface(IStudioInterface *value)
{
    m_DBQueryUI->m_studioInterface = value;
}

//设置Studio接口
void MDDBQueryUI::load()
{
    m_DBQueryUI->load();
}

/*!
\section function
*/
//加载
void MDDBQueryUI::unload()
{
    m_DBQueryUI->unLoad();
}

//卸载
void MDDBQueryUI::changeLanguage()
{
    m_DBQueryUI->changeLanguage();
}

/*!
\section 多语言
*/
void MDDBQueryUI::collectWords()
{

}

#include "MDActionUI.h"
#include "MDActionUIImp.h"

/*!
 * \brief 动作界面类。
 */
CMDActionUI::CMDActionUI()
{
    m_imp = new CMDActionUIImp;
}

CMDActionUI::~CMDActionUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化
*/
void CMDActionUI::setProjectConfiger(CMDProjectConfiger* projectConfiger)
{

}

void CMDActionUI::setContacter(CMDActionUIContacter* contacter)
{
    if(m_imp)
        m_imp->setContacter(contacter);
}

/*!
\section 显示对象选择界面
*/
bool CMDActionUI::selectActions(const QStringList& sourceList, QStringList& destList)
{
    return (m_imp ? m_imp->selectActions(sourceList, destList) : false);
}

/*!
\section property
*/
//根节点名称
QString CMDActionUI::rootName()
{
    return "动作";
}

//根节点图标
QIcon* CMDActionUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDActionUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDActionUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDActionUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDActionUI::load()
{
    m_imp->load();
}

//卸载
void CMDActionUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDActionUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDActionUI::collectWords()
{
    m_imp->collectWords();
}

/*!
 * \brief 动作界面与外界联系的类。
 */
CMDActionUIContacter::CMDActionUIContacter()
{

}

CMDActionUIContacter::~CMDActionUIContacter()
{

}

bool CMDActionUIContacter::onSelectSingleObject(MDActionType type, const QString& source, QString& dest)
{
    return false;
}


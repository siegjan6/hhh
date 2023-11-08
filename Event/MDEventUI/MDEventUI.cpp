#include "MDEventUI.h"
#include "MDEventUIImp.h"

/*!
 * \brief 事件界面类。
 */
CMDEventUI::CMDEventUI()
{
    m_imp = new CMDEventUIImp;
}

CMDEventUI::~CMDEventUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化
*/
void CMDEventUI::setContacter(CMDEventUIContacter* contacter)
{
    if(m_imp)
        m_imp->setContacter(contacter);
}

/*!
\section property
*/
//根节点名称
QString CMDEventUI::rootName()
{
    return "事件";
}

//根节点图标
QIcon* CMDEventUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDEventUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDEventUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDEventUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDEventUI::load()
{
    m_imp->load();
}

//卸载
void CMDEventUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDEventUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDEventUI::collectWords()
{
    m_imp->collectWords();
}

/*!
 * \brief 事件界面与外界联系的类。
 */
CMDEventUIContacter::CMDEventUIContacter()
{

}

CMDEventUIContacter::~CMDEventUIContacter()
{

}

bool CMDEventUIContacter::onSelectActions(const QStringList& sourceList, QStringList& destList)
{
    return false;
}

bool CMDEventUIContacter::onSelectSingleObject(MDEventType type, const QString& source, QString& dest)
{
    return false;
}

#include "MDAlarmConditionUI.h"
#include "MDAlarmConditionUIImp.h"

/*!
 * \brief 报警条件界面类，管理报警条件界面，提供选择报警条件的界面。
 */
CMDAlarmConditionUI::CMDAlarmConditionUI()
{
    m_imp = new CMDAlarmConditionUIImp;
}

CMDAlarmConditionUI::~CMDAlarmConditionUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化
*/

void CMDAlarmConditionUI::setContacter(CMDAlarmConditionUIContacter* contacter)
{
    if(m_imp)
        m_imp->setContacter(contacter);
}

/*!
\section 显示对象选择界面
*/
bool CMDAlarmConditionUI::select(const QStringList& sourceList, QStringList& destList)
{
    return (m_imp ? m_imp->select(sourceList, destList) : false);
}

/*!
\section property
*/
//根节点名称
QString CMDAlarmConditionUI::rootName()
{
    return "报警条件";
}

//根节点图标
QIcon* CMDAlarmConditionUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDAlarmConditionUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDAlarmConditionUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDAlarmConditionUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDAlarmConditionUI::load()
{
    m_imp->load();
}

//卸载
void CMDAlarmConditionUI::unload()
{
    m_imp->unLoad();
}

/*!
\section 多语言
*/
void CMDAlarmConditionUI::changeLanguage()
{
    m_imp->changeLanguage();
}

void CMDAlarmConditionUI::collectWords()
{
    m_imp->collectWords();
}

/*!
 * \brief 报警条件界面与外界联系的类，通过此类可获取【报警等级】和【报警变量】。
 */
CMDAlarmConditionUIContacter::CMDAlarmConditionUIContacter()
{

}

CMDAlarmConditionUIContacter::~CMDAlarmConditionUIContacter()
{

}

bool CMDAlarmConditionUIContacter::onSelectAlarmLevel(const QString& source, QString& dest)
{
    return false;
}

bool CMDAlarmConditionUIContacter::onSelectVariable(const QString& source, QString& dest)
{
    return false;
}

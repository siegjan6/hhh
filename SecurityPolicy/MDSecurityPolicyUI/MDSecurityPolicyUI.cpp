#include "MDSecurityPolicyUI.h"
#include "MDSecurityPolicyUIImp.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDStringResource.h"

CMDSecurityPolicyUI::CMDSecurityPolicyUI()
{
    m_imp = new CMDSecurityPolicyUIImp;
}

CMDSecurityPolicyUI::~CMDSecurityPolicyUI()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化
*/
void CMDSecurityPolicyUI::setConfiger(CMDSecurityPolicyConfiger* configer)
{
    m_imp->m_configer = configer;

    // dGFuZ3p3
//    m_accountUI.setConfiger(configer);
//    m_powerGroupUI.setConfiger(configer->powerGroupConfiger());
}

void CMDSecurityPolicyUI::setSecurityPolicyExpert(CMDSecurityPolicyExpert* spExpert)
{
    if(m_imp)
        m_imp->setSecurityPolicyExpert(spExpert);
}

/*!
\section 显示登录界面、安全验证界面；帐户和权限组管理界面
*/
bool CMDSecurityPolicyUI::login()
{
    return (m_imp ? m_imp->login() : false);
}

bool CMDSecurityPolicyUI::checkSP(CMDActionSPSession* spSession)
{
    return (m_imp ? m_imp->checkSP(spSession) : false);
}

bool CMDSecurityPolicyUI::showMgrDialog()
{
    return (m_imp ? m_imp->showMgrDialog() : false);
}

/*!
\section property
*/
//根节点名称
QString CMDSecurityPolicyUI::rootName()
{
    return "安全策略";
}

//根节点图标
QIcon* CMDSecurityPolicyUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDSecurityPolicyUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDSecurityPolicyUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;

    m_accountUI.setRootItem(value);
    m_powerGroupUI.setRootItem(value);
}

//设置Studio接口
void CMDSecurityPolicyUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;

    m_accountUI.setStudioInterface(value);
    m_powerGroupUI.setStudioInterface(value);
}
/*!
\section function
*/

//加载
void CMDSecurityPolicyUI::load()
{
    m_imp->load();

    m_accountUI.load();
    m_powerGroupUI.load();
    m_powerGroupUI.setAccountUI(&m_accountUI);
}

//卸载
void CMDSecurityPolicyUI::unload()
{
    m_accountUI.unload();
    m_powerGroupUI.unload();
}

/*!
\section 多语言
*/
void CMDSecurityPolicyUI::changeLanguage()
{
    m_imp->changeLanguage();

    m_accountUI.changeLanguage();
    m_powerGroupUI.changeLanguage();
}

void CMDSecurityPolicyUI::collectWords()
{
    m_accountUI.collectWords();
    m_powerGroupUI.collectWords();
}

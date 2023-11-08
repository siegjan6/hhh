#include "MDSecurityPolicyUIImp.h"
#include "../../include/MDProjectConfiger/MDProjectConfiger.h"
#include "../../include/MDAccountConfiger/MDAccountConfiger.h"
#include "../../include/MDSecurityPolicyConfiger/MDSecurityPolicyConfiger.h"
#include "../../include/MDSecurityPolicyExpert/MDSecurityPolicyExpert.h"
#include "MDLoginDialog.h"
#include "MDNSigCheckDialog.h"
#include "MDESigCheckDialog.h"
#include "MDSecurityPolicyMainDialog.h"
#include "MDAccountStrategyMainDialog.h"
#include "MDStringResource.h"

CMDSecurityPolicyUIImp::CMDSecurityPolicyUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_configer = NULL;
    m_spExpert = NULL;
}

CMDSecurityPolicyUIImp::~CMDSecurityPolicyUIImp()
{

}

/*!
\section 初始化
*/
void CMDSecurityPolicyUIImp::load()
{
    buildTree(treeRoot());
}

void CMDSecurityPolicyUIImp::unLoad()
{

}

void CMDSecurityPolicyUIImp::setConfiger(CMDSecurityPolicyConfiger* configer)
{
    m_configer = configer;
}

void CMDSecurityPolicyUIImp::setSecurityPolicyExpert(CMDSecurityPolicyExpert* spExpert)
{
    m_spExpert = spExpert;
}

QTreeWidget *CMDSecurityPolicyUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDSecurityPolicyUIImp::treeRoot()
{
    return m_rootInStudio;
}

void CMDSecurityPolicyUIImp::buildTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDSecurityPolicyUIImp::onMouseDbClick);

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<STRANS(STRING_AS_TEXT), AS_TREEITEM_TYPE);
    item->setIcon(0,QIcon(":/images/file.png"));
    item->setCheckState(1, Qt::Checked);
    item->setData(0, Qt::UserRole, (qulonglong)this);
    treeRoot()->addChild(item);
}

void CMDSecurityPolicyUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
    {
        CMDAccountStrategyMainDialog dlg;
        dlg.m_configer = m_studioInterface->projectConfiger()->securityConfiger()->accountStrategyConfiger();
        dlg.initial(this, NULL, (qint16)0);
        dlg.setFixedHeight(580);
        dlg.exec();
    }
}

/*!
\section 显示登录界面、安全验证界面；帐户和权限组管理界面
*/
bool CMDSecurityPolicyUIImp::login()
{
    bool result = false;
    CMDLoginDialog dlg;
    dlg.initial(m_spExpert);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        result = true;
    }
    return result;
}

bool CMDSecurityPolicyUIImp::checkSP(CMDActionSPSession* spSession)
{
    bool result = false;
    if(MDActionProtectMode::MindSCADA_Sig == spSession->actionSP().protectMode())
    {
        // 动作保护
        if(m_spExpert->isCurrentAccountHasPower(spSession))
        {
            // 具备权限直接通过
            QString resultMsg = spSession->actionSP().actionID().getLogMsg();
            //QString resultMsg = spSession->operatorOutput().resultMsg();
            resultMsg += "[动作保护,无需签名,因为当前账户具备权限]";
            spSession->operatorOutput().setResultMsg(resultMsg);
            spSession->operatorInput().setSigID(m_spExpert->currentAccount());
            m_spExpert->logSPSession(spSession);
            return true;
        }

        //当前账户不具备权限，弹框对此次操作进行权限验证
        CMDNSigCheckDialog dlg;
        dlg.initial(m_spExpert, spSession);
        dlg.exec();
        result = (QDialog::Accepted == dlg.result());
    }
    else if(MDActionProtectMode::Electronic_Sig == spSession->actionSP().protectMode())
    {
        // 电子签名
        CMDESigCheckDialog dlg;
        dlg.initial(m_spExpert, spSession);
        dlg.exec();
        result = (QDialog::Accepted == dlg.result());
    }
    else if(MDActionProtectMode::null_mode == spSession->actionSP().protectMode())
    {
        result = true;
    }
    else;

    return result;
}

bool CMDSecurityPolicyUIImp::showMgrDialog()
{
    bool result = false;
    CMDSecurityPolicyMainDialog dlg;
    dlg.m_configer = m_configer;
    dlg.initial(this, m_configer, (qint16)0);
    dlg.exec();
    result = (QDialog::Accepted == dlg.result());
    return result;
}

/*!
\section 多语言
*/
void CMDSecurityPolicyUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == AS_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_AS_TEXT);
            myItem->setText(0, translated);
        }
    }
}

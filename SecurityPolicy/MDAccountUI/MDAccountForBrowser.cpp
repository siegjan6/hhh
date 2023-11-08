#include "MDAccountForBrowser.h"
#include "MDAccount.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDAccountConfiger.h"
#include "MDActionSP.h"
#include "MDStringResource.h"
#include "MDAccountMainDialog.h"
#include "MDAccountTreeWindow.h"

CMDAccountForBrowser::CMDAccountForBrowser()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("name", tr("名称"));
    m_translateMap.insert("comment", tr("注释"));
    m_translateMap.insert("isEnable", tr("启用"));
    m_translateMap.insert("isLocked", tr("锁定"));
    m_translateMap.insert("isVisible", tr("可见"));
    m_translateMap.insert("GUID_Password", tr("密码"));
    m_translateMap.insert("password", tr("密码"));
    m_translateMap.insert("GUID_SecurityPolicy", tr("安全策略"));
    m_translateMap.insert("sp", tr("安全策略"));

    m_configer = NULL;
    m_ObjTreeWindow = NULL;
}

void CMDAccountForBrowser::setMDObject(CMDAccountProperty_Config* MDObject)
{
    m_MDObjects.clear();
    m_MDObjects.append(MDObject);
}

void CMDAccountForBrowser::setMDObjects(QList<CMDAccountProperty_Config*>& MDObjects)
{
    m_MDObjects.clear();
    foreach (CMDAccountProperty_Config* ap, MDObjects)
        m_MDObjects.append(ap);
}

void CMDAccountForBrowser::setConfiger(CMDSecurityPolicyConfiger* configer)
{
    m_configer = configer;
}

void CMDAccountForBrowser::setObjTreeWindow(CMDAccountTreeWindow* objTreeWindow)
{
    m_ObjTreeWindow = objTreeWindow;
}

/*!
\section 属性
*/
QString CMDAccountForBrowser::name() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->name();
    else
        return "";
}
void CMDAccountForBrowser::setName(const QString& name)
{
    if(1 == m_MDObjects.size())
    {
        if(m_configer->accountConfiger()->find(name))
            return;

        m_MDObjects.first()->setName(name);
    }
}

QString CMDAccountForBrowser::password() const
{
    return "******";
}
void CMDAccountForBrowser::setPassword(const QString& password)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDAccountProperty_Config* firstAP = m_MDObjects.first();
    CMDAccountMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(1);
    dlg.initial(m_ObjTreeWindow, m_configer, firstAP);
    dlg.setWindowTitle(STRANS(STRING_ACCOUNT_TEXT) + " - " + firstAP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDAccountProperty_Config* ap, m_MDObjects)
        {
            if(firstAP != ap)
                ap->setPassword(firstAP->password());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

QString CMDAccountForBrowser::comment() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->comment();
    else
        return "";
}
void CMDAccountForBrowser::setComment(const QString& comment)
{
    foreach (CMDAccountProperty_Config* ap, m_MDObjects)
    {
        ap->setComment(comment);
    }
}

bool CMDAccountForBrowser::isEnable() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isEnable();
    else
        return false;
}
void CMDAccountForBrowser::setEnable(bool enable)
{
    foreach (CMDAccountProperty_Config* ap, m_MDObjects)
    {
        ap->setEnable(enable);
    }
}

bool CMDAccountForBrowser::isLocked() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isLocked();
    else
        return false;
}
void CMDAccountForBrowser::setLocked(bool locked)
{
    foreach (CMDAccountProperty_Config* ap, m_MDObjects)
    {
        ap->setLocked(locked);
    }
}

bool CMDAccountForBrowser::isVisible() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isVisible();
    else
        return false;
}
void CMDAccountForBrowser::setVisible(bool visible)
{
    foreach (CMDAccountProperty_Config* ap, m_MDObjects)
    {
        ap->setVisible(visible);
    }
}

QString CMDAccountForBrowser::spList()
{
    if(1 == m_MDObjects.size())
    {
        QList<CMDActionSP*> enabledList;
        m_MDObjects.first()->spList()->enumEnabled(enabledList);
        QString SPEnable = (enabledList.count() > 0 ? "Yes" : "No");
        return SPEnable;
    }
    else
        return "??";
}
bool CMDAccountForBrowser::setSPList(const QString& spList)
{
    if(m_MDObjects.size() <= 0)
        return false;

    CMDAccountProperty_Config* firstAP = m_MDObjects.first();
    CMDAccountMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstAP);
    dlg.setWindowTitle(STRANS(STRING_ACCOUNT_TEXT) + " - " + firstAP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDAccountProperty_Config* ap, m_MDObjects)
        {
            if(firstAP != ap)
                ap->setSPList(firstAP->spList());
        }
        m_ObjTreeWindow->resetModelData();
        return true;
    }
    return false;
}

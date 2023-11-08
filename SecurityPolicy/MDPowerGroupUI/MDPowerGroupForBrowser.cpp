#include "MDPowerGroupForBrowser.h"
#include "MDPowerGroup.h"
#include "MDPowerGroupConfiger.h"
#include "MDPowerGroupMainDialog.h"
#include "MDPowerGroupTreeWindow.h"
#include "MDActionSP.h"
#include "MDStringResource.h"

CMDPowerGroupForBrowser::CMDPowerGroupForBrowser()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("name", tr("名称"));
    m_translateMap.insert("comment", tr("注释"));
    m_translateMap.insert("isEnable", tr("启用"));
    m_translateMap.insert("isVisible", tr("可见"));
    m_translateMap.insert("GUID_Account", tr("帐户"));
    m_translateMap.insert("accountList", tr("帐户列表"));
    m_translateMap.insert("GUID_SecurityPolicy", tr("安全策略"));
    m_translateMap.insert("sp", tr("安全策略"));

    m_configer = NULL;
    m_ObjTreeWindow = NULL;
}

void CMDPowerGroupForBrowser::setMDObject(CMDPowerGroup* MDObject)
{
    m_MDObjects.clear();
    m_MDObjects.append(MDObject);
}

void CMDPowerGroupForBrowser::setMDObjects(QList<CMDPowerGroup*>& MDObjects)
{
    m_MDObjects.clear();
    foreach (CMDPowerGroup* pg, MDObjects)
        m_MDObjects.append(pg);
}

void CMDPowerGroupForBrowser::setConfiger(CMDPowerGroupConfiger* configer)
{
    m_configer = configer;
}

void CMDPowerGroupForBrowser::setObjTreeWindow(CMDPowerGroupTreeWindow* objTreeWindow)
{
    m_ObjTreeWindow = objTreeWindow;
}

/*!
\section 属性
*/
QString CMDPowerGroupForBrowser::name() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->name();
    else
        return "";
}
void CMDPowerGroupForBrowser::setName(const QString& name)
{
    if(1 == m_MDObjects.size())
    {
        if(m_configer->find(name))
            return;

        m_MDObjects.first()->setName(name);
    }
}

QString CMDPowerGroupForBrowser::accountList() const
{
    if(1 == m_MDObjects.size())
    {
        QString aStringAsList = "";
        const QStringList& aList = m_MDObjects.first()->accountList();
        foreach(QString account, aList)
        {
            aStringAsList += account;
            aStringAsList += ";";
        }
        return aStringAsList;
    }
    else
        return "";
}
void CMDPowerGroupForBrowser::setAccountList(const QString& accountList)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDPowerGroup* firstAP = m_MDObjects.first();
    CMDPowerGroupMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(1);
    dlg.initial(m_ObjTreeWindow, m_configer, firstAP);
    dlg.setWindowTitle(STRANS(STRING_POWER_GROUP_TEXT) + " - " + firstAP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDPowerGroup* ap, m_MDObjects)
        {
            if(firstAP != ap)
                ap->setAccountList(firstAP->accountList());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

QString CMDPowerGroupForBrowser::comment() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->comment();
    else
        return "";
}
void CMDPowerGroupForBrowser::setComment(const QString& comment)
{
    foreach (CMDPowerGroup* ap, m_MDObjects)
    {
        ap->setComment(comment);
    }
}

bool CMDPowerGroupForBrowser::isEnable() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isEnable();
    else
        return false;
}
void CMDPowerGroupForBrowser::setEnable(bool enable)
{
    foreach (CMDPowerGroup* ap, m_MDObjects)
    {
        ap->setEnable(enable);
    }
}

bool CMDPowerGroupForBrowser::isVisible() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isVisible();
    else
        return false;
}
void CMDPowerGroupForBrowser::setVisible(bool visible)
{
    foreach (CMDPowerGroup* ap, m_MDObjects)
    {
        ap->setVisible(visible);
    }
}

QString CMDPowerGroupForBrowser::spList()
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
bool CMDPowerGroupForBrowser::setSPList(const QString& spList)
{
    if(m_MDObjects.size() <= 0)
        return false;

    CMDPowerGroup* firstAP = m_MDObjects.first();
    CMDPowerGroupMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstAP);
    dlg.setWindowTitle(STRANS(STRING_POWER_GROUP_TEXT) + " - " + firstAP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDPowerGroup* ap, m_MDObjects)
        {
            if(firstAP != ap)
                ap->setSPList(firstAP->spList());
        }
        m_ObjTreeWindow->resetModelData();
        return true;
    }
    return false;
}

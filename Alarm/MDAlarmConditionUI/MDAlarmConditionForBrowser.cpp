#include "MDAlarmConditionForBrowser.h"
#include "MDAlarmCondition.h"
#include "MDAlarmConditionConfiger.h"
#include "MDActionSP.h"
#include "MDStringResource.h"
#include "MDAlarmConditionMainDialog.h"
#include "MDAlarmConditionTreeWindow.h"

CMDAlarmConditionForBrowser::CMDAlarmConditionForBrowser()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("name", tr("名称"));
    m_translateMap.insert("comment", tr("注释"));
    m_translateMap.insert("levelName", tr("报警等级"));
    m_translateMap.insert("GUID_Condition", tr("条件"));
    m_translateMap.insert("condition", tr("条件"));
    m_translateMap.insert("GUID_SecurityPolicy", tr("安全策略"));
    m_translateMap.insert("sp", tr("安全策略"));

    m_configer = NULL;
    m_ObjTreeWindow = NULL;
}

void CMDAlarmConditionForBrowser::setMDObject(CMDAlarmCondition* MDObject)
{
    m_MDObjects.clear();
    m_MDObjects.append(MDObject);
}

void CMDAlarmConditionForBrowser::setMDObjects(QList<CMDAlarmCondition*>& MDObjects)
{
    m_MDObjects.clear();
    foreach (CMDAlarmCondition* ap, MDObjects)
        m_MDObjects.append(ap);
}

void CMDAlarmConditionForBrowser::setConfiger(CMDAlarmConditionConfiger* configer)
{
    m_configer = configer;
}

void CMDAlarmConditionForBrowser::setObjTreeWindow(CMDAlarmConditionTreeWindow* objTreeWindow)
{
    m_ObjTreeWindow = objTreeWindow;
}

/*!
\section 属性
*/
QString CMDAlarmConditionForBrowser::name() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->name();
    else
        return "";
}
void CMDAlarmConditionForBrowser::setName(const QString& name)
{
    if(1 == m_MDObjects.size())
    {
        if(m_configer->find(name))
            return;

        m_MDObjects.first()->setName(name);
    }
}

QString CMDAlarmConditionForBrowser::comment() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->comment();
    else
        return "";
}
void CMDAlarmConditionForBrowser::setComment(const QString& comment)
{
    foreach (CMDAlarmCondition* ac, m_MDObjects)
    {
        ac->setComment(comment);
    }
}

QString CMDAlarmConditionForBrowser::levelName() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->levelName();
    else
        return "";
}
void CMDAlarmConditionForBrowser::setLevelName(const QString& levelName)
{
    foreach (CMDAlarmCondition* ac, m_MDObjects)
    {
        ac->setLevelName(levelName);
    }
}

QString CMDAlarmConditionForBrowser::expression() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->expression();
    else
        return "";
}
void CMDAlarmConditionForBrowser::setExpression(const QString& expression)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDAlarmCondition* firstAC = m_MDObjects.first();
    CMDAlarmConditionMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(1);
    dlg.initial(m_ObjTreeWindow, m_configer, firstAC);
    dlg.setWindowTitle(STRANS(STRING_COND_TEXT) + " - " + firstAC->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDAlarmCondition* ac, m_MDObjects)
        {
            if(firstAC != ac)
                ac->setExpression(firstAC->expression());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

QString CMDAlarmConditionForBrowser::spList()
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
bool CMDAlarmConditionForBrowser::setSPList(const QString& spList)
{
    if(m_MDObjects.size() <= 0)
        return false;

    CMDAlarmCondition* firstAC = m_MDObjects.first();
    CMDAlarmConditionMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstAC);
    dlg.setWindowTitle(STRANS(STRING_COND_TEXT) + " - " + firstAC->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDAlarmCondition* ac, m_MDObjects)
        {
            if(firstAC != ac)
                ac->setSPList(firstAC->spList());
        }
        m_ObjTreeWindow->resetModelData();
    }
    return true;
}

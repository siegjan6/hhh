#include "MDAlarmLevelForBrowser.h"
#include "MDAlarmLevel.h"
#include "MDAlarmConfiger.h"
#include "MDAlarmLevelConfiger.h"

#include "MDAlarmLevelMainDialog.h"
#include "MDAlarmLevelColorDialog.h"
#include "MDAlarmLevelTreeWindow.h"
#include "MDStringResource.h"

CMDAlarmLevelForBrowser::CMDAlarmLevelForBrowser()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("name", tr("名称"));
    m_translateMap.insert("comment", tr("注释"));
    m_translateMap.insert("GUID_Color", tr("颜色"));
    m_translateMap.insert("alarmColor", tr("报警颜色"));

    m_configer = NULL;
    m_ObjTreeWindow = NULL;
}

void CMDAlarmLevelForBrowser::setMDObject(CMDAlarmLevel* MDObject)
{
    m_MDObjects.clear();
    m_MDObjects.append(MDObject);
}

void CMDAlarmLevelForBrowser::setMDObjects(QList<CMDAlarmLevel*>& MDObjects)
{
    m_MDObjects.clear();
    foreach (CMDAlarmLevel* ap, MDObjects)
        m_MDObjects.append(ap);
}

void CMDAlarmLevelForBrowser::setConfiger(CMDAlarmConfiger* configer)
{
    m_configer = configer;
}

void CMDAlarmLevelForBrowser::setObjTreeWindow(CMDAlarmLevelTreeWindow* objTreeWindow)
{
    m_ObjTreeWindow = objTreeWindow;
}

/*!
\section 属性
*/
QString CMDAlarmLevelForBrowser::name() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->name();
    else
        return "";
}
void CMDAlarmLevelForBrowser::setName(const QString& name)
{
    if(1 == m_MDObjects.size())
    {
        if(m_configer->levelConfiger()->find(name))
            return;

        m_MDObjects.first()->setName(name);
    }
}

QString CMDAlarmLevelForBrowser::comment() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->comment();
    else
        return "";
}
void CMDAlarmLevelForBrowser::setComment(const QString& comment)
{
    foreach (CMDAlarmLevel* al, m_MDObjects)
    {
        al->setComment(comment);
    }
}

QString CMDAlarmLevelForBrowser::alarmColor() const
{
    return "";
}
void CMDAlarmLevelForBrowser::setAlarmColor(const QString& alarmColor)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDAlarmLevel* firstEP = m_MDObjects.first();
    CMDAlarmLevelMainDialog dlg;
    dlg.setTheOnlyPage(1);
    dlg.initial(m_ObjTreeWindow, m_configer, firstEP);
    dlg.setWindowTitle(STRANS(STRING_LEVEL_TEXT) + " - " + firstEP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDAlarmLevel* ep, m_MDObjects)
        {
            ((CMDAlarmLevel*)ep)->setColor_occur(((CMDAlarmLevel*)firstEP)->color_occur());
            ((CMDAlarmLevel*)ep)->setColor_confirm_end(((CMDAlarmLevel*)firstEP)->color_confirm_end());
            ((CMDAlarmLevel*)ep)->setColor_confirm_not_end(((CMDAlarmLevel*)firstEP)->color_confirm_not_end());
            ((CMDAlarmLevel*)ep)->setColor_end_confirm(((CMDAlarmLevel*)firstEP)->color_end_confirm());
            ((CMDAlarmLevel*)ep)->setColor_end_not_confirm(((CMDAlarmLevel*)firstEP)->color_end_not_confirm());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

//qint32 CMDAlarmLevelForBrowser::delayLength() const
//{
//    return m_MDObject->delayLength();
//}
//void CMDAlarmLevelForBrowser::setDelayLength(qint32 delayLength)
//{
//    m_MDObject->setDelayLength(delayLength);
//}


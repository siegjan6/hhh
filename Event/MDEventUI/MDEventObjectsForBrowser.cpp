#include "MDEventObjectsForBrowser.h"
#include "MDEventConfiger.h"
#include "MDStringResource.h"
#include "MDEventMainDialog.h"
#include "MDEventTreeWindow.h"

CMDEventProperty_ConfigForBrowser::CMDEventProperty_ConfigForBrowser()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("name", tr("名称"));
    m_translateMap.insert("comment", tr("注释"));
    m_translateMap.insert("type", tr("类型"));
    m_translateMap.insert("objName", tr("对象名"));
    m_translateMap.insert("isEnable", tr("启用"));
    m_translateMap.insert("isEnableLog", tr("记录到日志"));
    m_translateMap.insert("expressionString", tr("表达式"));
    m_translateMap.insert("GUID_Action", tr("动作"));
    m_translateMap.insert("actionNameList", tr("动作列表"));

    m_configer = NULL;
    m_ObjTreeWindow = NULL;
}

void CMDEventProperty_ConfigForBrowser::setMDObject(CMDEventProperty_Config* MDObject)
{
    m_MDObjects.clear();
    m_MDObjects.append(MDObject);
}

void CMDEventProperty_ConfigForBrowser::setMDObjects(QList<CMDEventProperty_Config*>& MDObjects)
{
    m_MDObjects.clear();
    foreach (CMDEventProperty_Config* ep, MDObjects)
        m_MDObjects.append(ep);
}

bool CMDEventProperty_ConfigForBrowser::isSameType() const
{
    if(m_MDObjects.size() <= 0)
        return false;

    bool sameType = true;
    MDEventType theType = m_MDObjects.first()->type();
    foreach (CMDEventProperty_Config* ep, m_MDObjects)
    {
        if(theType != ep->type())
        {
            sameType = false;
            break;
        }
    }
    return sameType;
}

void CMDEventProperty_ConfigForBrowser::setConfiger(CMDEventConfiger* configer)
{
    m_configer = configer;
}

void CMDEventProperty_ConfigForBrowser::setObjTreeWindow(CMDEventTreeWindow* objTreeWindow)
{
    m_ObjTreeWindow = objTreeWindow;
}

/*!
\section 属性
*/
QString CMDEventProperty_ConfigForBrowser::name() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->name();
    else
        return "";
}
void CMDEventProperty_ConfigForBrowser::setName(const QString& name)
{
    if(1 == m_MDObjects.size())
    {
        if(m_configer->findEvent(name))
            return;

        m_MDObjects.first()->setName(name);
    }
}

QString CMDEventProperty_ConfigForBrowser::comment() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->comment();
    else
        return "";
}
void CMDEventProperty_ConfigForBrowser::setComment(const QString& comment)
{
    foreach (CMDEventProperty_Config* ep, m_MDObjects)
    {
        ep->setComment(comment);
    }
}

bool CMDEventProperty_ConfigForBrowser::isEnable() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isEnable();
    else
        return false;
}
void CMDEventProperty_ConfigForBrowser::setEnable(bool enable)
{
    foreach (CMDEventProperty_Config* ep, m_MDObjects)
    {
        ep->setEnable(enable);
    }
}

bool CMDEventProperty_ConfigForBrowser::isEnableLog() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isEnableLog();
    else
        return false;
}
void CMDEventProperty_ConfigForBrowser::setEnableLog(bool enable)
{
    foreach (CMDEventProperty_Config* ep, m_MDObjects)
    {
        ep->setEnableLog(enable);
    }
}

QString CMDEventProperty_ConfigForBrowser::type() const
{
    if(m_MDObjects.size() > 0 && isSameType())
        return CMDEventProperty_Config::typeToString(m_MDObjects.first()->type());
    else
        return "";
}

QString CMDEventProperty_ConfigForBrowser::objName() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->objName();
    else
        return "";
}
void CMDEventProperty_ConfigForBrowser::setObjName(const QString& objName)
{
    foreach (CMDEventProperty_Config* ep, m_MDObjects)
    {
        ep->setObjName(objName);
    }
}

QString CMDEventProperty_ConfigForBrowser::expressionString() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->expressionString();
    else
        return "";
}
void CMDEventProperty_ConfigForBrowser::setExpressionString(const QString& expressionString)
{
    foreach (CMDEventProperty_Config* ep, m_MDObjects)
    {
        ep->setExpressionString(expressionString);
    }
}

QString CMDEventProperty_ConfigForBrowser::actionNameList() const
{
    if(m_MDObjects.size() > 0)
    {
        QString aStringAsList = "";
        const QStringList& aList = m_MDObjects.first()->actionNameList();
        foreach(QString action, aList)
        {
            aStringAsList += action;
            aStringAsList += ";";
        }
        return aStringAsList;
    }
    else
        return "??";
}

void CMDEventProperty_ConfigForBrowser::setActionNameList(const QString& actionNameList)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDEventProperty_Config* firstEP = m_MDObjects.first();
    CMDEventMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(1);
    dlg.initial(m_ObjTreeWindow, m_configer, firstEP);
    dlg.setWindowTitle(STRANS(STRING_EVENT_TEXT) + " - " + firstEP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDEventProperty_Config* ep, m_MDObjects)
        {
            if(firstEP != ep)
                ep->setActionNameList(firstEP->actionNameList());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

// 派生类：周期
CEPC_TimeCycleForBrowser::CEPC_TimeCycleForBrowser()
{
    m_translateMap.insert("GUID_TimeCycle", tr("周期"));
    m_translateMap.insert("timeCycle", tr("周期"));
}

QString CEPC_TimeCycleForBrowser::timeCycle() const
{
    if(m_MDObjects.size() > 0)
    {
        quint32 tc = ((CEPC_TimeCycle*)m_MDObjects.first())->timeCycle();
        return QString("%1").arg(tc);
    }
    else
        return "";
}
void CEPC_TimeCycleForBrowser::setTimeCycle(const QString& timeCycle)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDEventProperty_Config* firstEP = m_MDObjects.first();
    CMDEventMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstEP);
    dlg.setWindowTitle(STRANS(STRING_EVENT_TEXT) + " - " + firstEP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDEventProperty_Config* ep, m_MDObjects)
        {
            ((CEPC_TimeCycle*)ep)->setTimeCycle(((CEPC_TimeCycle*)firstEP)->timeCycle());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

// 派生类：变量品质变化
CEPC_VariableQualityChangedForBrowser::CEPC_VariableQualityChangedForBrowser()
{
    m_translateMap.insert("GUID_QualityChanged", tr("品质变化"));
    m_translateMap.insert("rangeCondition", tr("条件"));
}

QString CEPC_VariableQualityChangedForBrowser::rangeCondition() const
{
    if(m_MDObjects.size() > 0)
    {
        return ((CEPC_VariableQualityChanged*)m_MDObjects.first())->condition();
    }
    else
        return "";
}
void CEPC_VariableQualityChangedForBrowser::setRangeCondition(const QString& rangeCondition)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDEventProperty_Config* firstEP = m_MDObjects.first();
    CMDEventMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstEP);
    dlg.setWindowTitle(STRANS(STRING_EVENT_TEXT) + " - " + firstEP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDEventProperty_Config* ep, m_MDObjects)
        {
            ((CEPC_VariableQualityChanged*)ep)->setRangeCondition(((CEPC_VariableQualityChanged*)firstEP)->rangeCondition());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

// 派生类：变量数值变化范围
CEPC_VariableValueChangedInRangeForBrowser::CEPC_VariableValueChangedInRangeForBrowser()
{
    m_translateMap.insert("GUID_ValueChangedInRange", tr("数值变化"));
    m_translateMap.insert("rangeCondition", tr("条件"));
}

QString CEPC_VariableValueChangedInRangeForBrowser::rangeCondition() const
{
    if(m_MDObjects.size() > 0)
    {
        return ((CEPC_VariableValueChangedInRange*)m_MDObjects.first())->condition();
    }
    else
        return "";
}
void CEPC_VariableValueChangedInRangeForBrowser::setRangeCondition(const QString& rangeCondition)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDEventProperty_Config* firstEP = m_MDObjects.first();
    CMDEventMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstEP);
    dlg.setWindowTitle(STRANS(STRING_EVENT_TEXT) + " - " + firstEP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDEventProperty_Config* ep, m_MDObjects)
        {
            ((CEPC_VariableValueChangedInRange*)ep)->setRangeCondition(((CEPC_VariableValueChangedInRange*)firstEP)->rangeCondition());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

// 派生类：写变量
CEPC_VariableValueWriteForBrowser::CEPC_VariableValueWriteForBrowser()
{
    m_translateMap.insert("GUID_WriteVariable", tr("变量写"));
    m_translateMap.insert("checkConditionBeforeOrAfterWrite", tr("检查条件"));
    m_translateMap.insert("rangeCondition", tr("条件"));
}

quint8 CEPC_VariableValueWriteForBrowser::checkConditionBeforeOrAfterWrite() const
{
    if(m_MDObjects.size() > 0)
    {
        return ((CEPC_VariableValueWrite*)m_MDObjects.first())->checkConditionBeforeOrAfterWrite();
    }
    else
        return 0;
}
void CEPC_VariableValueWriteForBrowser::setCheckConditionBeforeOrAfterWrite(quint8 flag)
{
    foreach (CMDEventProperty_Config* ep, m_MDObjects)
    {
        ((CEPC_VariableValueWrite*)ep)->setCheckConditionBeforeOrAfterWrite(flag);
    }
}

QString CEPC_VariableValueWriteForBrowser::rangeCondition() const
{
    if(m_MDObjects.size() > 0)
    {
        return ((CEPC_VariableValueWrite*)m_MDObjects.first())->condition();
    }
    else
        return "";
}
void CEPC_VariableValueWriteForBrowser::setRangeCondition(const QString& rangeCondition)
{
    if(m_MDObjects.size() <= 0)
        return;

    CMDEventProperty_Config* firstEP = m_MDObjects.first();
    CMDEventMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstEP);
    dlg.setWindowTitle(STRANS(STRING_EVENT_TEXT) + " - " + firstEP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDEventProperty_Config* ep, m_MDObjects)
        {
            ((CEPC_VariableValueWrite*)ep)->setRangeCondition(((CEPC_VariableValueWrite*)firstEP)->rangeCondition());
        }
        m_ObjTreeWindow->resetModelData();
    }
}

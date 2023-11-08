#include "MDActionObjectsForBrowser.h"
#include "MDActionConfiger.h"
#include "MDActionSP.h"
#include "MDStringResource.h"
#include "MDActionMainDialog.h"
#include "MDActionTreeWindow.h"

CMDActionProperty_ConfigForBrowser::CMDActionProperty_ConfigForBrowser()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("name", tr("名称"));
    m_translateMap.insert("comment", tr("注释"));
    m_translateMap.insert("actionType", tr("类型"));
    m_translateMap.insert("objName", tr("对象名"));
    m_translateMap.insert("isEnable", tr("启用"));
    m_translateMap.insert("isEnableLog", tr("记录到日志"));
    m_translateMap.insert("isEnableDelay", tr("延迟"));
    m_translateMap.insert("delayLength", tr("延迟时间"));
    m_translateMap.insert("GUID_SecurityPolicy", tr("安全策略"));
    m_translateMap.insert("sp", tr("安全策略"));

    m_configer = NULL;
    m_ObjTreeWindow = NULL;
}

void CMDActionProperty_ConfigForBrowser::setMDObject(CMDActionProperty_Config* MDObject)
{
    m_MDObjects.clear();
    m_MDObjects.append(MDObject);
}

void CMDActionProperty_ConfigForBrowser::setMDObjects(QList<CMDActionProperty_Config*>& MDObjects)
{
    m_MDObjects.clear();
    foreach (CMDActionProperty_Config* ap, MDObjects)
        m_MDObjects.append(ap);
}

bool CMDActionProperty_ConfigForBrowser::isSameType() const
{
    if(m_MDObjects.size() <= 0)
        return false;

    bool sameType = true;
    MDActionType theType = m_MDObjects.first()->actionID().type();
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        if(theType != ap->actionID().type())
        {
            sameType = false;
            break;
        }
    }
    return sameType;
}

void CMDActionProperty_ConfigForBrowser::setConfiger(CMDActionConfiger* configer)
{
    m_configer = configer;
}

void CMDActionProperty_ConfigForBrowser::setObjTreeWindow(CMDActionTreeWindow* objTreeWindow)
{
    m_ObjTreeWindow = objTreeWindow;
}

/*!
\section 属性
*/
QString CMDActionProperty_ConfigForBrowser::name() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->name();
    else
        return "";
}
void CMDActionProperty_ConfigForBrowser::setName(const QString& name)
{
    if(1 == m_MDObjects.size())
    {
        if(m_configer->findAction(name))
            return;

        m_MDObjects.first()->setName(name);
    }
}

QString CMDActionProperty_ConfigForBrowser::comment() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->comment();
    else
        return "";
}
void CMDActionProperty_ConfigForBrowser::setComment(const QString& comment)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ap->setComment(comment);
    }
}

bool CMDActionProperty_ConfigForBrowser::isEnable() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isEnable();
    else
        return false;
}
void CMDActionProperty_ConfigForBrowser::setEnable(bool enable)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ap->setEnable(enable);
    }
}

bool CMDActionProperty_ConfigForBrowser::isEnableLog() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isEnableLog();
    else
        return false;
}
void CMDActionProperty_ConfigForBrowser::setEnableLog(bool enable)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ap->setEnableLog(enable);
    }
}

QString CMDActionProperty_ConfigForBrowser::actionType() const
{
    if(m_MDObjects.size() > 0 && isSameType())
        return CMDActionID::typeToString(m_MDObjects.first()->actionID().type());
    else
        return "";
}

QString CMDActionProperty_ConfigForBrowser::objName() const
{
    if(m_MDObjects.size() > 0)
        return m_MDObjects.first()->actionID().objName();
    else
        return "";
}
void CMDActionProperty_ConfigForBrowser::setObjName(const QString& objName)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ap->actionID().setObjName(objName);
    }
}

bool CMDActionProperty_ConfigForBrowser::isEnableDelay() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->isEnableDelay();
    else
        return false;
}
void CMDActionProperty_ConfigForBrowser::setEnableDelay(bool enable)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ap->setEnableDelay(enable);
    }
}

qint32 CMDActionProperty_ConfigForBrowser::delayLength() const
{
    if(1 == m_MDObjects.size())
        return m_MDObjects.first()->delayLength();
    else
        return 0;
}
void CMDActionProperty_ConfigForBrowser::setDelayLength(qint32 delayLength)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ap->setDelayLength(delayLength);
    }
}

QString CMDActionProperty_ConfigForBrowser::spList()
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
bool CMDActionProperty_ConfigForBrowser::setSPList(const QString& spList)
{
    if(m_MDObjects.size() <= 0)
        return false;

    CMDActionProperty_Config* firstAP = m_MDObjects.first();
    CMDActionMainDialog dlg(m_ObjTreeWindow);
    dlg.setTheOnlyPage(2);
    dlg.initial(m_ObjTreeWindow, m_configer, firstAP);
    dlg.setWindowTitle(STRANS(STRING_ACTION_TEXT) + " - " + firstAP->name());
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        foreach (CMDActionProperty_Config* ap, m_MDObjects)
        {
            if(firstAP != ap)
                ap->setSPList(firstAP->spList());
        }
        m_ObjTreeWindow->resetModelData();
        return true;
    }
    return false;
}

// 派生类：写变量
CAPC_VariableWriteValueForBrowser::CAPC_VariableWriteValueForBrowser()
{
    m_translateMap.insert("GUID_WriteVariable", tr("写变量"));
    m_translateMap.insert("mode", tr("模式"));
    m_translateMap.insert("value", tr("值"));

    m_translateMap.insert("null_mode", tr("无"));
    m_translateMap.insert("mode_fixed", tr("绝对"));
    m_translateMap.insert("mode_relative", tr("相对"));
    m_translateMap.insert("mode_switch_0_1", tr("开关"));
}

CAPC_VariableWriteValueForBrowser::MDWriteMode1 CAPC_VariableWriteValueForBrowser::mode() const
{
    if(1 == m_MDObjects.size())
        return (CAPC_VariableWriteValueForBrowser::MDWriteMode1)((CAPC_VariableWriteValue*)m_MDObjects.first())->mode();
    else
        return CAPC_VariableWriteValueForBrowser::MDWriteMode1::mode_fixed;
}
void CAPC_VariableWriteValueForBrowser::setMode(CAPC_VariableWriteValueForBrowser::MDWriteMode1 mode)
{
    emit modeChanged(mode);

    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ((CAPC_VariableWriteValue*)ap)->setMode((MDWriteMode)mode);
    }
}

QString CAPC_VariableWriteValueForBrowser::value() const
{
    if(1 == m_MDObjects.size())
        return ((CAPC_VariableWriteValue*)m_MDObjects.first())->value().toString();
    else
        return "";
}
void CAPC_VariableWriteValueForBrowser::setValue(const QString& value)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ((CAPC_VariableWriteValue*)ap)->setValue(QVariant(value));
    }
}

// 派生类：窗口动作
CAPC_WindowOperationForBrowser::CAPC_WindowOperationForBrowser()
{
    m_translateMap.insert("GUID_WindowOperation", tr("窗口"));
    m_translateMap.insert("second", tr("第二"));
    m_translateMap.insert("left", tr("左"));
    m_translateMap.insert("top", tr("顶"));
    m_translateMap.insert("width", tr("宽"));
    m_translateMap.insert("height", tr("高"));
}

/*!
\section 属性
*/
QString CAPC_WindowOperationForBrowser::second() const
{
    if(1 == m_MDObjects.size())
        return ((CAPC_WindowOperation*)m_MDObjects.first())->second();
    else
        return "";
}
void CAPC_WindowOperationForBrowser::setSecond(const QString& windowName)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ((CAPC_WindowOperation*)ap)->setSecond(windowName);
    }
}

int CAPC_WindowOperationForBrowser::left() const
{
    if(1 == m_MDObjects.size())
        return ((CAPC_WindowOperation*)m_MDObjects.first())->left();
    else
        return 0;
}
void CAPC_WindowOperationForBrowser::setLeft(int left)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ((CAPC_WindowOperation*)ap)->setLeft(left);
    }
}

int CAPC_WindowOperationForBrowser::top() const
{
    if(1 == m_MDObjects.size())
        return ((CAPC_WindowOperation*)m_MDObjects.first())->top();
    else
        return 0;
}
void CAPC_WindowOperationForBrowser::setTop(int top)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ((CAPC_WindowOperation*)ap)->setTop(top);
    }
}

int CAPC_WindowOperationForBrowser::width() const
{
    if(1 == m_MDObjects.size())
        return ((CAPC_WindowOperation*)m_MDObjects.first())->width();
    else
        return 100;
}
void CAPC_WindowOperationForBrowser::setWidth(int width)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ((CAPC_WindowOperation*)ap)->setWidth(width);
    }
}

int CAPC_WindowOperationForBrowser::height() const
{
    if(1 == m_MDObjects.size())
        return ((CAPC_WindowOperation*)m_MDObjects.first())->height();
    else
        return 100;
}
void CAPC_WindowOperationForBrowser::setHeight(int height)
{
    foreach (CMDActionProperty_Config* ap, m_MDObjects)
    {
        ((CAPC_WindowOperation*)ap)->setHeight(height);
    }
}

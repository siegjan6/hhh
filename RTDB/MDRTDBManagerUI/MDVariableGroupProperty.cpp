#include <QMessageBox>

#include "MDVariableGroupProperty.h"
#include "IMDVariableGroup.h"
#include "IMDTreeItem.h"
#include "MDMultiLanguage.h"
#include "MDRTDBVariablesWidget.h"
CMDVariableGroupProperty::CMDVariableGroupProperty()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("Name", tr("变量组名"));
    m_translateMap.insert("Discription", tr("描述"));

    m_localName= "";
    m_VariableGroup = NULL;
    m_parentWidget = NULL;
}


CMDVariableGroupProperty::~CMDVariableGroupProperty()
{
    m_translateMap.clear();
}
void CMDVariableGroupProperty::setParentWidget(CMDRTDBVariablesWidget* parentWidget)
{
    m_parentWidget = parentWidget;
}

bool CMDVariableGroupProperty::isSystemVariableGroup()
{
    if(m_localName=="SystemVariableGroup")
    {
        return true;
    }
    return false;
}
void CMDVariableGroupProperty::copy(IMDVariableGroup *otherVarGroup)
{
    if(otherVarGroup== NULL)
    {
        return;
    }
    m_localName= otherVarGroup->localName();
    m_description= otherVarGroup->description();
    m_VariableGroup = otherVarGroup;
}
//!Group名
QString CMDVariableGroupProperty::localName()
{
    return m_localName;
}

void CMDVariableGroupProperty::setLocalName(QString name)
{
    if(isSystemVariableGroup())
        return;

    if(0!=QString::compare(m_localName, name,Qt::CaseSensitive))
    {
        //检查重复性
        IMDVariableGroup* parentGroup = (IMDVariableGroup*)m_VariableGroup->parentItem(MDTreeItemType::VariableGroup);
        if(NULL!=parentGroup)
        {
            if(parentGroup->getChildVariableGroup(name) != NULL)
            {
                promptErrorMsg(tr("变量组名重复"));
                return;
            }
        }

        m_localName = name;
        m_VariableGroup->setLocalName(name);
        m_parentWidget->refreshItemProp((qulonglong)m_VariableGroup);
    }
}
//!组描述
void CMDVariableGroupProperty::setDescription(QString description)
{
    m_description  = description;
    m_VariableGroup->setDescription(description);
    m_parentWidget->refreshItemProp(0);
}

QString CMDVariableGroupProperty::description()
{
  return m_description;
}

void CMDVariableGroupProperty::promptErrorMsg(const QString& errMsg)
{
    QMessageBox box;
    box.setText(errMsg);
    box.setWindowTitle(tr("警告"));
    box.setStandardButtons(QMessageBox::Ok);

    CMDMultiLanguage::instance()->translateWidget(&box);
    box.exec();
}

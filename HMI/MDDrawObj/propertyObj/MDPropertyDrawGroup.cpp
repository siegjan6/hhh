#include "MDPropertyDrawGroup.h"
#include "MDDrawGroup.h"
#include "CustomPropertyDialog.h"
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawGroup::CMDPropertyDrawGroup(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("customProperty", tr("自定义属性"));
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawGroup::setCustomProperty(const QString&)
{
    QList<CMDCustomProperty>* oldValue = new QList<CMDCustomProperty>();
    *oldValue = *group()->customPropertys();

    CCustomPropertyDialog dlg(container());
    dlg.init(group());
    if (dlg.exec() == QDialog::Accepted)
    {
        QList<CMDCustomProperty>* newValue = new QList<CMDCustomProperty>();
        *newValue = *group()->customPropertys();

        undo()->push(new CMDDrawGroupCommand(
            group(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawGroupId::CustomProperty));

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawGroup::customProperty()
{
    QString str;
    return str.setNum(group()->customPropertys()->count());
}
//////////////////////////////////////////////////////////////////////////
CMDDrawGroup *CMDPropertyDrawGroup::group()
{
    return dynamic_cast<CMDDrawGroup*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawGroupVector::CMDPropertyDrawGroupVector(CMDDrawObj *parent) :
    CMDPropertyDrawVector(parent)
{
    insertTranslateMap("customProperty", tr("自定义属性"));
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawGroupVector::setCustomProperty(const QString&)
{
    QList<CMDCustomProperty>* oldValue = new QList<CMDCustomProperty>();
    *oldValue = *group()->customPropertys();

    CCustomPropertyDialog dlg(container());
    dlg.init(group());
    if (dlg.exec() == QDialog::Accepted)
    {
        QList<CMDCustomProperty>* newValue = new QList<CMDCustomProperty>();
        *newValue = *group()->customPropertys();

        undo()->push(new CMDDrawGroupCommand(
            group(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawGroupId::CustomProperty));

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawGroupVector::customProperty()
{
    QString str;
    return str.setNum(group()->customPropertys()->count());
}
//////////////////////////////////////////////////////////////////////////
CMDDrawGroup *CMDPropertyDrawGroupVector::group()
{
    return dynamic_cast<CMDDrawGroup*>(m_object);
}
//////////////////////////////////////////////////////////////////////////





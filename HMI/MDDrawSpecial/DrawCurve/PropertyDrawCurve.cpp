#include "PropertyDrawCurve.h"
#include "MDDrawCurve.h"
//////////////////////////////////////////////////////////////////////////
CPropertyDrawCurve::CPropertyDrawCurve(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("config",tr("配置"));
}
//////////////////////////////////////////////////////////////////////////
CMDDrawCurve *CPropertyDrawCurve::object()
{
    return dynamic_cast<CMDDrawCurve*>(m_object);
}
QString CPropertyDrawCurve::GUID_Appearance() const
{
    return QString("Appearance");
}

QString CPropertyDrawCurve::DILconfig() const
{
    return QString();
}

void CPropertyDrawCurve::setDILconfig(QString)
{

    //1 - QDialog::Accepted
    //0 - QDialog::Rejected
    if(object()->setProperty() == QDialog::Accepted)
        setModify(true);
}

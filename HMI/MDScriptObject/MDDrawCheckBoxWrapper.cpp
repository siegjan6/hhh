#include "MDDrawCheckBoxWrapper.h"
#include "MDDrawCheckBox.h"

CMDDrawCheckBoxWrapper::CMDDrawCheckBoxWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object, name)
{

}

bool CMDDrawCheckBoxWrapper::isChecked()
{
    return object()->checked();
}

void CMDDrawCheckBoxWrapper::setChecked(bool value)
{
    object()->setChecked(value);
}

QVariant CMDDrawCheckBoxWrapper::text()
{
    return QVariant(object()->getScriptText());
}

void CMDDrawCheckBoxWrapper::setText(const QVariant& value)
{
    object()->setScriptText(value.toString());
}

CMDDrawCheckBox *CMDDrawCheckBoxWrapper::object()
{
    return reinterpret_cast<CMDDrawCheckBox*>(MDObject());
}

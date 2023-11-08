#include "MDDrawRadioButtonWrapper.h"
#include "MDDrawRadioButton.h"

CMDDrawRadioButtonWrapper::CMDDrawRadioButtonWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object,name)
{

}

bool CMDDrawRadioButtonWrapper::isChecked()
{
    return object()->checked();
}

void CMDDrawRadioButtonWrapper::setChecked(bool check)
{
    object()->setChecked(check);
}

QVariant CMDDrawRadioButtonWrapper::text()
{
    return QVariant(object()->text());
}

void CMDDrawRadioButtonWrapper::setText(const QVariant &value)
{
    object()->setText(value.toString());
}

CMDDrawRadioButton *CMDDrawRadioButtonWrapper::object()
{
    return reinterpret_cast<CMDDrawRadioButton*>(MDObject());
}

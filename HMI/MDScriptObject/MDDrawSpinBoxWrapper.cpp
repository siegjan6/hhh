#include "MDDrawSpinBoxWrapper.h"
#include "MDDrawSpinBox.h"

CMDDrawSpinBoxWrapper::CMDDrawSpinBoxWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object, name)
{

}

int  CMDDrawSpinBoxWrapper::maxiMum()
{
    return object()->maximum();
}

void CMDDrawSpinBoxWrapper::setMaximum(int value)
{
    object()->setMaximum(value);
}

int CMDDrawSpinBoxWrapper::miniMum()
{
    return object()->minimum();
}

void CMDDrawSpinBoxWrapper::setMinimum(int value)
{
    object()->setMinimum(value);
}

int CMDDrawSpinBoxWrapper::currentValue()
{
    return object()->getCurValue();
}

void CMDDrawSpinBoxWrapper::setCurrentValue(int value)
{
    object()->setCurrentValue(value);
}

CMDDrawSpinBox *CMDDrawSpinBoxWrapper::object()
{
    return reinterpret_cast<CMDDrawSpinBox*>(MDObject());
}

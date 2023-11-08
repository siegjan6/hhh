#include "CustomEvent.h"

CCustomEvent::CCustomEvent(QEvent::Type type, int fromModule):QEvent(type)
{
    QEvent::registerEventType(type);
    m_fromModule = fromModule;
}

int  CCustomEvent::fromModule()
{
    return m_fromModule;
}

QVariant CCustomEvent::param()
{
    return m_param;
}
void CCustomEvent::setParam(QVariant param)
{
    m_param = param;
}

QVariant CCustomEvent::value()
{
    return m_value;
}

void CCustomEvent::setValue(QVariant value)
{
    m_value = value;
}

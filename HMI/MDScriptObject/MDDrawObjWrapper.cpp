#include "MDDrawObjWrapper.h"
#include "MDDrawObj.h"
#include "MDDrawCurve.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawObjWrapper::CMDDrawObjWrapper(void *object, const QString &name)
    :CMDObjectWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObjWrapper::setLeft(qreal value)
{
    QRectF rf = object()->rect();
    rf.moveLeft(value);
    object()->setDynamicRect(rf);
}
qreal CMDDrawObjWrapper::left()
{
    return object()->rect().left();
}
void CMDDrawObjWrapper::setTop(qreal value)
{
    QRectF rf = object()->rect();
    rf.moveTop(value);
    object()->setDynamicRect(rf);
}
qreal CMDDrawObjWrapper::top()
{
    return object()->rect().top();
}
void CMDDrawObjWrapper::setWidth(qreal value)
{
    QRectF rf = object()->rect();
    rf.setWidth(value);
    object()->setDynamicRect(rf);
}
qreal CMDDrawObjWrapper::width()
{
    return object()->rect().width();
}
void CMDDrawObjWrapper::setHeight(qreal value)
{
    QRectF rf = object()->rect();
    rf.setHeight(value);
    object()->setDynamicRect(rf);
}
qreal CMDDrawObjWrapper::height()
{
    return object()->rect().height();
}
void CMDDrawObjWrapper::setVisible(bool value)
{
    object()->setVisible(value);
}
bool CMDDrawObjWrapper::visible()
{
    return object()->visible();
}
void CMDDrawObjWrapper::setExpression(const QString &name, const QString &expression)
{
    object()->setScriptExpression(name, expression);
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawObjWrapper::object()
{
    return reinterpret_cast<CMDDrawObj*>(MDObject());
}


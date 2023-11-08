#include "MDDrawVectorWrapper.h"
#include "MDDrawVector.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawVectorWrapper::CMDDrawVectorWrapper(void *object, const QString &name)
    :CMDDrawObjWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVectorWrapper::setAngle(qreal value)
{
    object()->setDynamicRotateAngle(value);
}
qreal CMDDrawVectorWrapper::angle()
{
    return object()->rotateAngle();
}
//////////////////////////////////////////////////////////////////////////
CMDDrawVector *CMDDrawVectorWrapper::object()
{
    return reinterpret_cast<CMDDrawVector*>(MDObject());
}

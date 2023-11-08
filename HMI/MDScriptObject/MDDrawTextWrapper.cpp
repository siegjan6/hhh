#include "MDDrawTextWrapper.h"
#include "MDDrawText.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawTextWrapper::CMDDrawTextWrapper(void *object, const QString &name)
    :CMDDrawVectorWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawTextWrapper::setText(const QVariant &value)
{
    object()->setText(value.toString());
}
QVariant CMDDrawTextWrapper::text()
{
    return QVariant(object()->text());
}
//////////////////////////////////////////////////////////////////////////
CMDDrawText *CMDDrawTextWrapper::object()
{
    return reinterpret_cast<CMDDrawText*>(MDObject());
}





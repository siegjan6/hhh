#include "DrawDataArchivesWrapper.h"
#include "MDDrawDataArchives.h"

CDrawDataArchivesWrapper::CDrawDataArchivesWrapper(void *object, const QString &name)
    :CMDDrawObjWrapper(object, name)
{
}

void CDrawDataArchivesWrapper::query()
{
    CMDDrawDataArchives *obj = (CMDDrawDataArchives*)MDObject();
    obj->query();
}

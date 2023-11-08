#include "DrawProcDockerWrapper.h"
#include "MDDrawProcDocker.h"

//////////////////////////////////////////////////////////////////////////
CDrawProcDockerWrapper::CDrawProcDockerWrapper(void *object, const QString &name)
    :CMDObjectWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
void CDrawProcDockerWrapper::setCmd(const QString& cmd)
{
    CMDDrawProcDocker* obj = (CMDDrawProcDocker*)MDObject();
    obj->setCmd(cmd);
}
//////////////////////////////////////////////////////////////////////////
QString CDrawProcDockerWrapper::getCmd()
{
    CMDDrawProcDocker* obj = (CMDDrawProcDocker*)MDObject();
    return obj->getCmd();
}
//////////////////////////////////////////////////////////////////////////
void CDrawProcDockerWrapper::run(const QString& cmd)
{
    CMDDrawProcDocker* obj = (CMDDrawProcDocker*)MDObject();
    obj->run(cmd);
}


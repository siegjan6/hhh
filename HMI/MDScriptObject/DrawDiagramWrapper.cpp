#include "DrawDiagramWrapper.h"
#include "MDDrawDiagram.h"

CDrawDiagramWrapper::CDrawDiagramWrapper(void *object, const QString &name)
 :CMDObjectWrapper(object, name)
{
}

void CDrawDiagramWrapper::setString1(const QString &string1)
{
    CMDDrawDiagram* obj = (CMDDrawDiagram*)MDObject();
    obj->setString1(string1);
}

void CDrawDiagramWrapper::setString2(const QString &string2)
{
    CMDDrawDiagram* obj =  (CMDDrawDiagram*)MDObject();
    obj->setString2(string2);
}

void CDrawDiagramWrapper::setString3(const QString &string3)
{

    CMDDrawDiagram* obj =  (CMDDrawDiagram*)MDObject();
    obj->setString3(string3);
}

void CDrawDiagramWrapper::setValue(const QString &valueString)
{
    CMDDrawDiagram* obj =  (CMDDrawDiagram*)MDObject();
    obj->setValue(valueString);
}

//////////////////////////////////////////////////////////////////////////


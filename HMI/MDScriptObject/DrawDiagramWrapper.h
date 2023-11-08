#ifndef DRAWDIAGRAMWRAPPER_H
#define DRAWDIAGRAMWRAPPER_H
#include "MDDrawObjWrapper.h"
class CMDDrawDiagram;

class CDrawDiagramWrapper : public CMDObjectWrapper
{
public:
    CDrawDiagramWrapper(void* object, const QString& name);
    virtual ~CDrawDiagramWrapper(){}

public Q_SLOTS:
    void setString1(const QString &string1);
    void setString2(const QString &string2);
    void setString3(const QString &string3);
    void setValue(const QString &valueString);

    //////////////////////////////////////////////////////////////////////////

};

#endif // DRAWDIAGRAMWRAPPER_H

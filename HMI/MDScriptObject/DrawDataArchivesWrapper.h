#ifndef DRAWDATAARCHIVESWRAPPER_H
#define DRAWDATAARCHIVESWRAPPER_H

#include "MDDrawObjWrapper.h"

class CMDDrawDataArchives;

class CDrawDataArchivesWrapper : public CMDDrawObjWrapper
{
    Q_OBJECT
public:
    CDrawDataArchivesWrapper(void* object, const QString& name);
    virtual ~CDrawDataArchivesWrapper(){}

public Q_SLOTS:
    void query();
};
#endif // DRAWDATAARCHIVESWRAPPER_H

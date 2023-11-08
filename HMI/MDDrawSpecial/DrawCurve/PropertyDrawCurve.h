#ifndef PROPERTYDRAWCURVE_H
#define PROPERTYDRAWCURVE_H
//////////////////////////////////////////////////////////////////////////
#include "MDPropertyDrawObj.h"

class CMDDrawCurve;
//////////////////////////////////////////////////////////////////////////
class CPropertyDrawCurve : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString DILconfig READ DILconfig WRITE setDILconfig)

public:
    CPropertyDrawCurve(CMDDrawObj *parent);
    virtual ~CPropertyDrawCurve(){}

public:
    QString GUID_Appearance() const;
    QString DILconfig() const;
    void setDILconfig(QString);
protected:
    CMDDrawCurve* object();
};

#endif // PROPERTYDRAWCurve_H

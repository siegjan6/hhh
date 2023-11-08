/*!
\author aGRw 2016.08.17
\section 联合控件属性类
*/
#ifndef PROPERTYDRAWCOMBINE_H
#define PROPERTYDRAWCOMBINE_H

#include "MDPropertyDrawVector.h"
#include "MDDrawCombine.h"

class CPropertyDrawCombine : public CMDPropertyDrawVector
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(bool connectLine READ connectLine WRITE setConnectLine)
    Q_PROPERTY(Qt::FillRule fillMode READ fillMode WRITE setFillMode)
public:
    CPropertyDrawCombine(CMDDrawObj* parent);
    virtual ~CPropertyDrawCombine(){}
//////////////////////////////////////////////////////////////////////////
public:
    void setConnectLine(bool value);
    bool connectLine();
    void setFillMode(Qt::FillRule value);
    Qt::FillRule fillMode();
//////////////////////////////////////////////////////////////////////////
protected:
    CMDDrawCombine *combine();
/*!
\section batch edit
*/
private:
    void setBatchConnectLine(bool value);
    void setBatchFillMode(Qt::FillRule value);
};


#endif // PROPERTYDRAWCOMBINE_H













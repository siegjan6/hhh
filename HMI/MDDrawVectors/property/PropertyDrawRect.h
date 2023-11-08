/*!
\author aGRw
\section 矩形控件属性类
*/
#ifndef PROPERTYDRAWRECT_H
#define PROPERTYDRAWRECT_H

#include "MDPropertyDrawVector.h"
#include "MDDrawRect.h"

class MDDRAWVECTORSSHARED_EXPORT CPropertyDrawRect : public CMDPropertyDrawVector
{
public:
    Q_OBJECT

    Q_PROPERTY(QString GUID_Appearance)
//////////////////////////////////////////////////////////////////////////
public:
    CPropertyDrawRect(CMDDrawObj* parent);
    virtual ~CPropertyDrawRect(){}
/*!
\section private function
*/
private:
    CMDDrawRect* object();
};

#endif // PROPERTYDRAWRECT_H

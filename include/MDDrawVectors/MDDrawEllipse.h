/*!
\author aGRw 2015.05.18
\section 圆形控件
*/
#ifndef DRAWELLIPSE_H
#define DRAWELLIPSE_H

#if defined(MDDRAWVECTORS_LIBRARY)
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDDrawVector.h"


class MDDRAWVECTORSSHARED_EXPORT CMDDrawEllipse  : public CMDDrawVector
{
public:
    CMDDrawEllipse();
    virtual ~CMDDrawEllipse(){}
/*!
\section virtual property
*/
public:
    DrawType type();
    QString className(){return "CMDDrawEllipse";}
    bool canCombine(){return true;}
/*!
\section protected function
*/
protected:
    CMDDrawObj *createSelf();
    void onGeneratePath();
//////////////////////////////////////////////////////////////////////////
};

#endif // DRAWELLIPSE_H






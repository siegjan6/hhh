/*!
\author aGRw
\section 圆弧控件
*/
#ifndef MDDRAWARC_H
#define MDDRAWARC_H

#include "MDDrawBasicArc.h"

class MDDRAWVECTORSSHARED_EXPORT CMDDrawArc : public CMDDrawBasicArc
{
public:
    CMDDrawArc();
    virtual ~CMDDrawArc(){}
/*!
\section virtual property
*/
public:
    DrawType type();
    QString className(){return "CMDDrawArc";}

/*!
\section protected function
*/
protected:
    CMDDrawObj *createSelf();
    void onGeneratePath();
//////////////////////////////////////////////////////////////////////////
};

#endif // MDDRAWARC_H


/*!
\author aGRw
\section 圆切控件
*/
#ifndef MDDRAWSHEAR_H
#define MDDRAWSHEAR_H

#include "MDDrawBasicArc.h"

class MDDRAWVECTORSSHARED_EXPORT CMDDrawShear : public CMDDrawBasicArc
{
public:
    CMDDrawShear();
    virtual ~CMDDrawShear(){}
/*!
\section virtual property
*/
public:
    DrawType type() ;
    QString className(){return "CMDDrawShear";}
/*!
\section protected function
*/
protected:
    CMDDrawObj *createSelf() ;
    void onGeneratePath() ;


};

#endif // MDDRAWSHEAR_H



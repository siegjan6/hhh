/*!
\author aGRw
\section 扇形控件
*/
#ifndef MDDRAWSECTOR_H
#define MDDRAWSECTOR_H

#include "MDDrawBasicArc.h"

class MDDRAWVECTORSSHARED_EXPORT CMDDrawSector : public CMDDrawBasicArc
{
public:
    CMDDrawSector();
    virtual ~CMDDrawSector(){}
/*!
\section virtual property
*/
public:
    DrawType type() ;
    QString className(){return "CMDDrawSector";}

/*!
\section protected function
*/
protected:
    CMDDrawObj *createSelf() ;
    void onGeneratePath() ;

};


#endif // MDDRAWSECTOR_H



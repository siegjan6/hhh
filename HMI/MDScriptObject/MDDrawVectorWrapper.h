/*!
\author aGRw 2015.05.21
\section 矢量控件，脚本封装类
*/
#ifndef MDDRAWVECTORWRAPPER_H
#define MDDRAWVECTORWRAPPER_H

#include "MDDrawObjWrapper.h"

class CMDDrawVector;

class MDHMIFORMSHARED_EXPORT CMDDrawVectorWrapper : public CMDDrawObjWrapper
{
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle)
public:
    CMDDrawVectorWrapper(void* object, const QString& name);
    virtual ~CMDDrawVectorWrapper(){}
/*!
\section property
*/
public Q_SLOTS:
    void setAngle(qreal value);
    qreal angle();
public:
    CMDDrawVector* object();

};


#endif // MDDRAWVECTORWRAPPER_H


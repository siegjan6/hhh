/*!
\author aGRw
\section 圆弧基类属性类
*/
#ifndef MDPROPERTYDRAWBASICARC_H
#define MDPROPERTYDRAWBASICARC_H

#include "MDPropertyDrawVector.h"
#include "MDDrawBasicArc.h"

class MDDRAWVECTORSSHARED_EXPORT CMDPropertyDrawBasicArc : public CMDPropertyDrawVector
{
public:
    Q_OBJECT

    Q_PROPERTY(QString GUID_Layout)
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle)
    Q_PROPERTY(qreal spanAngle READ spanAngle WRITE setSpanAngle)
//////////////////////////////////////////////////////////////////////////
public:
    CMDPropertyDrawBasicArc(CMDDrawObj* parent);
    virtual ~CMDPropertyDrawBasicArc(){}

/*!
\section property
*/
public:
    void setStartAngle(qreal value);
    qreal startAngle();
    void setSpanAngle(qreal value);
    qreal spanAngle();

/*!
\section private function
*/
private:
    CMDDrawBasicArc* object();
/*!
\section batch edit
*/
private:
    void setBatchStartAngle(qreal value);
    void setBatchSpanAngle(qreal value);
};

#endif // MDPROPERTYDRAWBASICARC_H

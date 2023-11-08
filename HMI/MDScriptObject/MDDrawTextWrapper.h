/*!
\author aGRw 2015.05.21
\section 文本控件，脚本封装类
*/
#ifndef MDDRAWTEXTWRAPPER_H
#define MDDRAWTEXTWRAPPER_H

#include "MDDrawVectorWrapper.h"

class CMDDrawText;

class MDHMIFORMSHARED_EXPORT CMDDrawTextWrapper : public CMDDrawVectorWrapper
{
    Q_OBJECT
    Q_PROPERTY(QVariant text READ text WRITE setText)

public:
    CMDDrawTextWrapper(void* object, const QString& name);
    virtual ~CMDDrawTextWrapper(){}
/*!
\section property
*/
public Q_SLOTS:
    void setText(const QVariant& value);
    QVariant text();
public:
    CMDDrawText* object();
};


#endif // MDDRAWTEXTWRAPPER_H


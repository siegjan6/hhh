/*!
\author Y2hhaWhq

\section 功能

微调按钮控件脚本类
*/
#ifndef MDDRAWSPINBOXWRAPPER_H
#define MDDRAWSPINBOXWRAPPER_H

#include "MDDrawWidgetWrapper.h"
class CMDDrawSpinBox;

class MDHMIFORMSHARED_EXPORT CMDDrawSpinBoxWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(int maxiMum READ maxiMum WRITE setMaximum)
    Q_PROPERTY(int miniMum READ miniMum WRITE setMinimum)
    Q_PROPERTY(int currentValue READ currentValue WRITE setCurrentValue)

public:
    CMDDrawSpinBoxWrapper(void* object, const QString& name);
    virtual ~CMDDrawSpinBoxWrapper(){}

//! 属性
public Q_SLOTS:
     int maxiMum();
     void setMaximum(int value);
     int miniMum();
     void setMinimum(int value);
     int currentValue();
     void setCurrentValue(int value);

public:
    CMDDrawSpinBox* object();
};

#endif // MDDRAWSPINBOXWRAPPER_H

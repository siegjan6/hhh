/*!
\author bWVpeGw=

\section 功能

微调按钮控件属性类
*/
#ifndef MDPROPERTYDRAWSPINBOX_H
#define MDPROPERTYDRAWSPINBOX_H

#include "MDPropertyDrawObj.h"
#include "MDDrawSpinBox.h"
#include "MDPropertyDrawBasicWidget.h"
class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawSpinBox:public CMDPropertyDrawBasicWidget
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int initialValue READ initialValue WRITE setInitialValue)
/*!
\section 属性函数
*/
public:
    //! 设置单步值
    void setSingleStep(int value);
    int singleStep();
    //! 设置最大值
    void setMaximum(int value);
    int maximum();
    //! 设置最小值
    void setMinimum(int value);
    int minimum();
    //! 设置初始值
    void setInitialValue(int value);
    int initialValue();

public:
    CMDPropertyDrawSpinBox(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawSpinBox(){}

protected:
    CMDDrawSpinBox* widget();
/*!
\section 设置属性值
*/
public:
    void resetAttribute();

/*!
\section batch edit
*/
private:
    void setBatchSingleStep(int value);
    void setBatchMaximum(int value);
    void setBatchMinimum(int value);
    void setBatchInitialValue(int value);
};

#endif // MDPROPERTYDRAWSPINBOX_H


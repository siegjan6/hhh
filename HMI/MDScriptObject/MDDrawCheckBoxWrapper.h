/*!
\author Y2hhaWhq

\section 功能

复选框控件脚本类
*/
#ifndef MDDRAWCHECKBOXWRAPPER_H
#define MDDRAWCHECKBOXWRAPPER_H

#include "MDDrawWidgetWrapper.h"
class CMDDrawCheckBox;

class MDHMIFORMSHARED_EXPORT CMDDrawCheckBoxWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(bool isChecked READ isChecked WRITE setChecked)
    Q_PROPERTY(QVariant text READ text WRITE setText)

public:
    CMDDrawCheckBoxWrapper(void* object, const QString& name);
    virtual ~CMDDrawCheckBoxWrapper(){}

 //! 属性
public Q_SLOTS:
    //! 选中状态
    bool isChecked();
    void setChecked(bool value);
    //! 文本
     QVariant text();
     void setText(const QVariant &value);

public:
    CMDDrawCheckBox* object();
};

#endif // MDDRAWCHECKBOXWRAPPER_H

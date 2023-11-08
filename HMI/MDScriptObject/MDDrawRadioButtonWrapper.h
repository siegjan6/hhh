/*!
\author Y2hhaWhq

\section 功能

单选按钮控件脚本类
*/
#ifndef MDDRAWRADIOBUTTONWRAPPER_H
#define MDDRAWRADIOBUTTONWRAPPER_H

#include "MDDrawWidgetWrapper.h"
class CMDDrawRadioButton;

class MDHMIFORMSHARED_EXPORT CMDDrawRadioButtonWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(bool isChecked READ isChecked WRITE setChecked)
    Q_PROPERTY(QVariant text READ text WRITE setText)

public:
    CMDDrawRadioButtonWrapper(void* object, const QString& name);
    virtual ~CMDDrawRadioButtonWrapper(){}

//！ 脚本属性
public Q_SLOTS:
    //! 选中状态
    bool isChecked();
    void setChecked(bool check);
    //! 文本
    QVariant text();
    void setText(const QVariant &value);

public:
    CMDDrawRadioButton* object();
};

#endif // MDDRAWRADIOBUTTONWRAPPER_H

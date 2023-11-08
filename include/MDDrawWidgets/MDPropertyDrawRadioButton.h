/*!
\author bWVpeGw=

\section 功能

单选按钮控件属性类
*/
#ifndef MDPROPERTYDRAWRADIOBUTTON_H
#define MDPROPERTYDRAWRADIOBUTTON_H

#include "MDPropertyDrawObj.h"
#include "MDDrawRadioButton.h"
#include "MDPropertyDrawBasicWidget.h"
class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawRadioButton : public CMDPropertyDrawBasicWidget
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Action)
//    Q_PROPERTY(bool checked READ checked WRITE setChecked)
    Q_PROPERTY(int groupId READ groupId WRITE setGroupId)
    Q_PROPERTY(qreal checkValue READ checkValue WRITE setCheckValue)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString text READ text WRITE setText)

/*!
\section 属性函数
*/
public:
    //! 文本
    void setText(const QString& value);
    QString text();
    //! 选中状态
//    void setChecked(bool value);
//    bool checked();
    //! 分组
    void setGroupId(int value);
    int groupId();
    //! 选中后设置数值
    void setCheckValue(qreal value);
    qreal checkValue();

public:
    CMDPropertyDrawRadioButton(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawRadioButton(){}

protected:
    CMDDrawRadioButton *widget();

/*!
\section 设置属性值
*/
public:
    void resetAttribute();

/*!
\section batch edit
*/
private:
    void setBatchText(const QString& value);
    void setBatchChecked(bool value);
    void setBatchGroupId(int value);
    void setBatchCheckValue(qreal value);
};

#endif  //MDPROPERTYDRAWRADIOBUTTON_H

/*!
\author bWVpeGw=

\section 功能

复选框控件属性类
*/
#ifndef MDPROPERTYDRAWCHECKBOX_H
#define MDPROPERTYDRAWCHECKBOX_H

#include "MDPropertyDrawObj.h"
#include "MDDrawCheckBox.h"
#include "MDPropertyDrawBasicWidget.h"

class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawCheckBox : public CMDPropertyDrawBasicWidget
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool checked READ checked WRITE setChecked)

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
    void setChecked(bool value);
    bool checked();

public:
    CMDPropertyDrawCheckBox(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawCheckBox(){}

protected:
    CMDDrawCheckBox* widget();
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
};

#endif // MDPROPERTYDRAWCHECKBOX_H


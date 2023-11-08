/*!
\author bWVpeGw=

\section 功能

windows控件属性管理基类
*/

#ifndef MDPROPERTYDRAWBASICWIDGET_H
#define MDPROPERTYDRAWBASICWIDGET_H

#include "MDPropertyDrawObj.h"
#include "MDDrawBasicWidget.h"

class  MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawBasicWidget : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool enable READ enable WRITE setEnable)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor backColor READ backColor WRITE setBackColor)
    Q_PROPERTY(QColor disableBackColor READ disableBackColor WRITE setDisableBackColor )
    Q_PROPERTY(QFont font READ font WRITE setFont)

public:
    CMDPropertyDrawBasicWidget(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawBasicWidget(){}

public:
    enum class ColorType{ Red ,Green, Blue, Alpha, All};
public:
    /*!
    \section  可用
    */
    void setEnable(bool value);
    bool enable();
    /*!
    \section  文本色
    */
    void setTextColor(const QColor& value);
    QColor textColor();
    /*!
    \section  背景色
    */
    void setBackColor(const QColor& value);
    QColor backColor();
    /*!
    \section  disable时的背景色
    */
    void setDisableBackColor(const QColor& value);
    QColor disableBackColor();
    /*!
    \section  字体
    */
    void setFont(const QFont& value);
    QFont font();

protected:
        CMDDrawBasicWidget *widget();
private:
    IDrawEnvrionment* m_environment;

/*!
\section batch edit
*/

protected:
    virtual void resetBatchProperty(CPropertyBrowser* browser);
private:
    void setBatchEnable(bool value);
    void setBatchTextColor(const QColor& value);
    void setBatchBackColor(const QColor& value);
    void setBatchDisableBackColor(const QColor& value);
    void setBatchFont(const QFont& value);
    CMDPropertyDrawBasicWidget::ColorType getColorChanged(const QColor &c1, const QColor &c2);
    QColor getChangedColor(QColor value, QColor c, ColorType type);
};

#endif // MDPROPERTYDRAWBASICWIDGET_H

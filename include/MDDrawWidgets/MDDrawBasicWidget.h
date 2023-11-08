/*!
\author bWVpeGw=

\section 功能

windows 窗口控件基类
*/
#ifndef MDDRAWBASICWIDGET_H
#define MDDRAWBASICWIDGET_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>
#include <QFrame>
#include <QJsonObject>
#include "MDDrawWidget.h"

class CDrawBasicWidgetData;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawBasicWidget : public CMDDrawWidget
{
public:
    CMDDrawBasicWidget();
    virtual ~CMDDrawBasicWidget();

private:
    CDrawBasicWidgetData* m_data;
/*!
\section  属性设置函数
*/
public:
    /*!
    \section  字体色
    */
    virtual void setTextColor(const QColor& value);
    QColor textColor();
    /*!
    \section  背景色
    */
    virtual void setBackColor(const QColor& value);
    QColor backColor();
    /*!
    \section  disable时的背景色
    */
    virtual void setDisableBackColor(const QColor& value);
    QColor disableBackColor();
    /*!
    \section  字体设置
    */
    virtual void setFont(const QFont* value);
    QFont *font();
    /*!
    \section  是否可用
    */
     void setEnable(bool value);
     bool enable();
    virtual bool finalEnabled();
/*!
\section  初始化
*/
public:
    void onInitialization();
/*!
\section  序列化
*/
public:
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
/*!
\section  颜色设置函数
*/
private:
    void resetColor();
    void resetBackColor(QColor& backColor);
    void resetTextColor(QColor& textColor);

/*!
\section batch edit
*/
    public:
         virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
    protected:
        //初始化类信息
        virtual void initClassInfo(QStringList &classInfo);

public:
    QString getBasicWidgetColor();
};

#endif // MDDRAWBASICWIDGET_H


/*!
\author Y2hhaWhq

\section 功能

windows窗口控件基类数据
*/
#ifndef DRAWBASICWIDGETDATA_H
#define DRAWBASICWIDGETDATA_H

#include <QWidget>
#include <QFrame>
#include <QJsonObject>


class CDrawBasicWidgetData
{
public:
    CDrawBasicWidgetData();
    ~CDrawBasicWidgetData(){}
public:
    QColor m_textColor;  //!  字体色
    QColor m_backColor;  //!背景色
    QColor m_disableBackColor;  //!disable时的背景色
    QFont m_font;  //!字体
    bool m_enable;  //!是否可用

/*!
\section  序列化
*/
public:
    void operator= (const CDrawBasicWidgetData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWBASICWIDGETDATA_H


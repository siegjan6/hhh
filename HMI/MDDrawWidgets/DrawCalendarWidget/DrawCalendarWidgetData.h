/*!
\author Y2hhaWhq

\section 功能

日历控件数据类
*/
#ifndef DRAWCALENDARWIDGETDATA_H
#define DRAWCALENDARWIDGETDATA_H

#include <QJsonObject>
#include "DrawObjDefine.h"
#include "CalendarForm.h"
class CMDDrawCalendarWidgetEvent;       //! 日历事件类

class CDrawCalendarWidgetData
{
public:
    CDrawCalendarWidgetData();
    ~CDrawCalendarWidgetData(){}
public:
    CCalendarForm* m_widget;            //! 日历控件
    CMDDrawCalendarWidgetEvent* m_event;

    bool m_weekNum;                     //! 显示周数
    bool m_currentDateHighlight;        //! 高亮当天日期
    bool m_backToToday;                 //! 显示回到今天
    bool m_showToday;                   //! 显示当天日期
    bool m_gridVisible;                 //! 显示网格
    Qt::DayOfWeek m_firstDayofWeek;     //! 每周以周日开始
    QDate m_maxDate;
    QDate m_minDate;
    QDateTime m_year;
    QDateTime m_date;

/*!
\section  序列化
*/
public:
    void operator = (const CDrawCalendarWidgetData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWCALENDARWIDGETDATA_H



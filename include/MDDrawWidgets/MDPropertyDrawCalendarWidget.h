/*!
\author bWVpeGw=

\section 功能

日历控件属性类
*/
#ifndef MDPROPERTYDRAWCALENDARWIDGET_H
#define MDPROPERTYDRAWCALENDARWIDGET_H

#include "MDPropertyDrawObj.h"
#include "MDDrawCalendarWidget.h"

class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawCalendarWidget : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool enable READ enable WRITE setEnable)
    Q_PROPERTY(bool backToToday READ backToToday WRITE setBackToToday)
    Q_PROPERTY(bool currentDate READ currentDate WRITE setCurrentDate)
    Q_PROPERTY(bool currentDateHighlight READ currentDateHighlight WRITE setCurrentDateHighlight)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(bool weekNum READ weekNum WRITE setWeekNum)
    Q_PROPERTY(bool gridVisible READ gridVisible WRITE setGridVisible)
    Q_PROPERTY(Qt::DayOfWeek firstDayofWeek READ firstDayofWeek WRITE setFirstDayofWeek)
/*!
\section 属性函数
*/
public:
    //! 显示周数
    void setWeekNum(bool value);
    bool weekNum();
    //! 圈定当天日期
    void setCurrentDateHighlight(bool value);
    bool currentDateHighlight();
    //! 是否显示回到今天
    void setBackToToday(bool value);
    bool backToToday();
    //! 是否显示当天日期
    void setCurrentDate(bool value);
    bool currentDate();
    //! 显示网格
    void setGridVisible(bool value);
    bool gridVisible();
    //! 每周开始于星期几
    void setFirstDayofWeek(Qt::DayOfWeek value);
    Qt::DayOfWeek firstDayofWeek();
    //! 是否可用
    void setEnable(bool value);
    bool enable();

public:
    CMDPropertyDrawCalendarWidget(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawCalendarWidget(){}

protected:
    CMDDrawCalendarWidget* widget();

/*!
\section 设置属性值
*/
public:
    void resetAttribute();

/*!
\section batch edit
*/
private:
    void setBatchWeekNum(bool value);
    void setBatchCurrentDateHighlight(bool value);
    void setBatchBackToToday(bool value);
    void setBatchCurrentDate(bool value);
    void setBatchGridVisible(bool value);
    void setBatchFirstDayofWeek(Qt::DayOfWeek value);
    void setBatchEnable(bool value);
};

#endif // MDPROPERTYDRAWCALENDARWIDGET_H


/*!
\author bWVpeGw=

\section 功能

日历控件类
*/
#ifndef MDDRAWCALENDARWIDGET_H
#define MDDRAWCALENDARWIDGET_H
#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include "MDDrawBasicWidget.h"
#include "../MDDrawWidgets/DrawCalendarWidget/CalendarForm.h"

class CDrawCalendarWidgetData; //! 日历数据类

class  MDDRAWWIDGETSSHARED_EXPORT CMDDrawCalendarWidget : public CMDDrawBasicWidget
{
public:
    CMDDrawCalendarWidget();
    virtual ~CMDDrawCalendarWidget();

private:
    CDrawCalendarWidgetData* m_data;

/*!
\section 属性函数
*/
public:
    CCalendarForm *widget();                    //! 控件
    void setWeekNum(bool value);                //! 显示周数
    bool weekNum();
    void setCurrentDateHighlight(bool value);   //! 高亮当天日期
    bool currentDateHighlight();
    void setBackToToday(bool value);            //! 是否显示回到今天
    bool backToToday();
    void setCurrentDate(bool value);            //! 是否显示当天日期
    bool currentDate();
    void setGridVisible(bool value);            //! 显示网格
    bool gridVisible();
    void setFirstDayofWeek(Qt::DayOfWeek value);//! 每周开始于星期几
    Qt::DayOfWeek firstDayofWeek();

/*!
\section 脚本属性函数, 读写年月日
*/
public:
    void setYear(int year);
    int getYear();
    void setMonth(int month);
    int getMonth();
    void setDay(int day);
    int getDay();
    void setScriptDate(QDate datetime);         //! 读写年月日
    QDate getScriptDate();
    void setMaxDate(QDate date);                //! 日期上限
    QDate maxDate();
    void setMinDate(QDate date);                //! 日期下限
    QDate minDate();

/*!
\section  控件对象属性
*/
public:
    DrawType type();                            //! 返回控件类型名
    QString className(){return "CMDDrawCalendarWidget";}
    CMDPropertyDrawObj *createPropertyDrawObj();//! 返回属性框对象
    void onInitialization();                    //! 初始化
protected:
    CMDDrawObj *createSelf();                   //! 创建自身

/*!
\section 序列化
*/
public:
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

/*!
\section 绑定相关变量
*/
public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);
    void writeVariable();

    bool writeVarEnabled();
    void setWriteVarEnabled(bool writeVarEnabled);

private:
    bool m_writeVarEnabled;                     //! 可写入变量

/*!
\section batch edit
*/
public:
   virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
   //初始化类信息
   virtual void initClassInfo(QStringList &classInfo);
};
#endif // MDDRAWCALENDARWIDGET_H


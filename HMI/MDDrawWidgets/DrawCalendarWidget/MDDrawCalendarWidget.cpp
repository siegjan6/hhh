#include <QDateEdit>
#include <QDate>
#include <QColor>
#include <QTextCharFormat>
#include "MDDrawCalendarWidget.h"
#include "DrawCalendarWidgetData.h"
#include "MDPropertyDrawCalendarWidget.h"
#include "MDDrawCalendarWidgetEvent.h"

CMDDrawCalendarWidget::CMDDrawCalendarWidget()
{
    m_data = new CDrawCalendarWidgetData;
    m_data->m_widget = new CCalendarForm;
    m_data->m_event = new CMDDrawCalendarWidgetEvent();
    setWidget(m_data->m_widget,m_data->m_event);
    m_writeVarEnabled = true;
}

CMDDrawCalendarWidget::~CMDDrawCalendarWidget()
{
    delete m_data;
}

CCalendarForm *CMDDrawCalendarWidget::widget()
{
    return m_data->m_widget;
}

/*!
\section 属性函数
*/
void CMDDrawCalendarWidget::setWeekNum(bool value)
{
    m_data->m_weekNum = value;
    if(!m_data->m_weekNum)
        m_data->m_widget->m_calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    else
        m_data->m_widget->m_calendar->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
}

bool CMDDrawCalendarWidget::weekNum()
{
    return m_data->m_weekNum;
}

void CMDDrawCalendarWidget::setCurrentDateHighlight(bool value)
{
    m_data->m_currentDateHighlight = value;
    QDate tdate=QDate::currentDate();
    QTextCharFormat todayFormat;
    if(value)
    {
        todayFormat.setBackground(QColor(85,170,255));
        todayFormat.setFontPointSize(18);
        todayFormat.setFontWeight(15);
        QPen pen;
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(3);
        pen.setBrush(Qt::red);
        pen.setColor(Qt::red);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        todayFormat.setTextOutline(pen);
        m_data->m_widget->m_calendar->setDateTextFormat(tdate,todayFormat);
    }
}

bool CMDDrawCalendarWidget::currentDateHighlight()
{
    return m_data->m_currentDateHighlight;
}

void CMDDrawCalendarWidget::setBackToToday(bool value)
{
    m_data->m_backToToday = value;
    m_data->m_widget->m_button->setVisible(value);
}

bool CMDDrawCalendarWidget::backToToday()
{
    return m_data->m_backToToday;
}

void CMDDrawCalendarWidget::setCurrentDate(bool value)
{
    m_data->m_showToday = value;
    m_data->m_widget->m_label->setVisible(value);
}

bool CMDDrawCalendarWidget::currentDate()
{
    return m_data->m_showToday;
}

void CMDDrawCalendarWidget::setGridVisible(bool value)
{
    m_data->m_gridVisible = value;
    m_data->m_widget->m_calendar->setGridVisible(value);
}

bool CMDDrawCalendarWidget::gridVisible()
{
    return m_data->m_gridVisible;
}

void CMDDrawCalendarWidget::setFirstDayofWeek(Qt::DayOfWeek value)
{
    m_data->m_firstDayofWeek = value;
    m_data->m_widget->m_calendar->setFirstDayOfWeek(value);
}

Qt::DayOfWeek CMDDrawCalendarWidget::firstDayofWeek()
{
    return m_data->m_firstDayofWeek;
}

/*!
\section 脚本属性函数, 读写年月日
*/
void CMDDrawCalendarWidget::setYear(int year)
{
    int month,day;
    month = m_data->m_widget->m_calendar->selectedDate().month();
    day = m_data->m_widget->m_calendar->selectedDate().day();
    m_data->m_widget->m_calendar->setSelectedDate(QDate(year,month,day));
}

int CMDDrawCalendarWidget::getYear()
{
    int num;
    num = m_data->m_widget->m_calendar->selectedDate().year();
    return num;
}

void CMDDrawCalendarWidget::setMonth(int month)
{
    int year,day;
    if(month>0 && month<13)
    {

        year = m_data->m_widget->m_calendar->selectedDate().year();
        day = m_data->m_widget->m_calendar->selectedDate().day();
        m_data->m_widget->m_calendar->setSelectedDate(QDate(year,month,day));
    }
}

int CMDDrawCalendarWidget::getMonth()
{
    int num;
    num = m_data->m_widget->m_calendar->selectedDate().month();
    return num;
}

void CMDDrawCalendarWidget::setDay(int day)
{
    int year,month;
    if(day>0 && day<32)
    {
        year = m_data->m_widget->m_calendar->selectedDate().year();
        month = m_data->m_widget->m_calendar->selectedDate().month();
        m_data->m_widget->m_calendar->setSelectedDate(QDate(year,month,day));
    }
}

int CMDDrawCalendarWidget::getDay()
{
    int num;
    num = m_data->m_widget->m_calendar->selectedDate().day();
    return num;
}

void CMDDrawCalendarWidget::setScriptDate(QDate date)
{
    m_data->m_widget->m_calendar->setSelectedDate(date);
}

QDate CMDDrawCalendarWidget::getScriptDate()
{
    return m_data->m_widget->m_calendar->selectedDate();
}

void CMDDrawCalendarWidget::setMaxDate(QDate date)
{
    m_data->m_maxDate = date;
    m_data->m_widget->m_calendar->setMaximumDate(date);
}

QDate CMDDrawCalendarWidget::maxDate()
{
    return m_data->m_maxDate;
}

void CMDDrawCalendarWidget::setMinDate(QDate date)
{
    m_data->m_minDate = date;
    m_data->m_widget->m_calendar->setMinimumDate(date);
}

QDate CMDDrawCalendarWidget::minDate()
{
    return m_data->m_minDate;
}

/*!
\section 控件对象属性
*/
DrawType CMDDrawCalendarWidget::type()
{
    return DrawType::Calendar;
}

CMDPropertyDrawObj *CMDDrawCalendarWidget::createPropertyDrawObj()
{
    return new CMDPropertyDrawCalendarWidget(this);
}

void CMDDrawCalendarWidget::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();
    //    CMDDrawBasicWidget::setBackColor(Qt::gray);
    m_data->m_widget->setStyleSheet(QString(
                                        "QCalendarWidget QToolButton{color: red; background-color:gray; width: 150px;}  QWidget { alternate-background-color: lightgray;} QAbstractItemView:enabled {color: black;} QAbstractItemView:disabled { color: gray}"));
    if(!widget())
        return;
    m_data->m_widget->m_calendar->setFirstDayOfWeek(m_data->m_firstDayofWeek);
    m_data->m_widget->m_calendar->setGridVisible(m_data->m_gridVisible);
    m_data->m_widget->m_calendar->setSelectedDate(QDate::currentDate());
    m_data->m_widget->m_button->setVisible(m_data->m_backToToday);
    m_data->m_widget->m_label->setVisible(m_data->m_showToday);
    if(!m_data->m_weekNum)
        m_data->m_widget->m_calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    else
        m_data->m_widget->m_calendar->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);


    if(m_data->m_currentDateHighlight){
        QDate tdate=QDate::currentDate();
        QTextCharFormat todayFormat;
        todayFormat.setBackground(QColor(85,170,255));
        todayFormat.setFontPointSize(18);
        todayFormat.setFontWeight(15);
        QPen pen;
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(3);
        pen.setBrush(Qt::red);
        pen.setColor(Qt::red);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        todayFormat.setTextOutline(pen);
        m_data->m_widget->m_calendar->setDateTextFormat(tdate,todayFormat);
    }
}

CMDDrawObj *CMDDrawCalendarWidget::createSelf()
{
    return new CMDDrawCalendarWidget();
}
/*!
\section   序列化
*/

CMDDrawObj *CMDDrawCalendarWidget::clone()
{
    CMDDrawCalendarWidget* obj = static_cast<CMDDrawCalendarWidget*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawCalendarWidget::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawCalendarWidget",js);
}

void CMDDrawCalendarWidget::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawCalendarWidget").toObject();
    m_data->deserialize(js);
}

QList<QStringList> CMDDrawCalendarWidget::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawCalendar.date",       QObject::tr("日期")});
    return list;
}

void CMDDrawCalendarWidget::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0: //! 日期
        m_writeVarEnabled = false;
        setScriptDate(QDate::fromString(exp->stringValue(10,"yyyy-MM-dd"),"yyyy-MM-dd"));
       m_writeVarEnabled = true;
        break;
    default:
        break;
    }
}

void CMDDrawCalendarWidget::writeVariable()
{
    if (parent() == NULL)
        return;
    if (isStudioMode())
        return;

    QString textExpression = expManager()->list()->last()->lowerExpression();

    if (textExpression.count() == 0)
        return;

    parent()->framework()->setVariableValue(textExpression,m_data->m_widget->m_calendar->selectedDate());

    setScriptDate(parent()->framework()->variableValue(textExpression).toDate());
}

bool CMDDrawCalendarWidget::writeVarEnabled()
{
    return m_writeVarEnabled;
}

void CMDDrawCalendarWidget::setWriteVarEnabled(bool writeVarEnabled)
{
    m_writeVarEnabled = writeVarEnabled;
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawCalendarWidget::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawCalendarWidget", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawCalendarWidget(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawCalendarWidget::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawCalendarWidget");
}
//////////////////////////////////////////////////////////////////////////


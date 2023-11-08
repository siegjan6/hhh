#include <QDateTimeEdit>
#include <QDateTime>
#include "MDDrawDateTimeEdit.h"
#include "DrawDateTimeEditData.h"
#include "MDDrawDateTimeEditEvent.h"
#include "MDPropertyDrawDateTimeEdit.h"

CMDDrawDateTimeEdit::CMDDrawDateTimeEdit()
{
    m_data = new CDrawDateTimeEditData;
    m_data->m_widget = new QDateTimeEdit(QDateTime::currentDateTime());
    m_data->m_event = new CMDDrawDateTimeEditEvent();
    setWidget(m_data->m_widget,m_data->m_event);
    m_writeVarEnabled = true;
}

CMDDrawDateTimeEdit::~CMDDrawDateTimeEdit()
{
    delete m_data;
}

QDateTimeEdit *CMDDrawDateTimeEdit::widget()
{
    return m_data->m_widget;
}

/*!
\section 属性函数
*/
void CMDDrawDateTimeEdit::setShowType(int value)
{
    m_data->m_showType = value;

    switch (value)
    {
    case 0:
        m_data->m_widget->setDisplayFormat("yyyy/MM/dd");  //! 短日期型
        break;
    case 1:
        m_data->m_widget->setDisplayFormat("yyyy年MM月dd日");  //! 长日期型
        break;
    case 2:
//        m_data->m_widget->setTime(QTime::currentTime());
        m_data->m_widget->setDisplayFormat("hh:mm:ss");  //! 时间型
        break;
    }
}

int CMDDrawDateTimeEdit::showType()
{
    return m_data->m_showType;
}

void CMDDrawDateTimeEdit::setCalendarPop(bool value)
{
    m_data->m_calendarPop = value;
    m_data->m_widget->setCalendarPopup(value);
}

bool CMDDrawDateTimeEdit::calendarPop()
{
    return m_data->m_calendarPop;
}

/*!
\section 脚本应用函数, 读写年月日时分秒
*/
void CMDDrawDateTimeEdit::setYear(int year)
{
    int month,day;
    if(m_data->m_showType == 0 || m_data->m_showType == 1)
    {
        month = m_data->m_widget->date().month();
        day = m_data->m_widget->date().day();
        m_data->m_widget->setDate(QDate(year,month,day));
    }
}

int CMDDrawDateTimeEdit::getYear()
{
    int num;
    if(m_data->m_showType == 0 || m_data->m_showType == 1)
    {
        num = m_data->m_widget->date().year();
        return num;
    }
    else
        return 0;
}

void CMDDrawDateTimeEdit::setMonth(int month)
{
    int year,day;
    if(m_data->m_showType == 0 || m_data->m_showType == 1)
    {
        if(month>0 && month<13)
        {
            year = m_data->m_widget->date().year();
            day = m_data->m_widget->date().day();
            m_data->m_widget->setDate(QDate(year,month,day));
        }
    }
}

int CMDDrawDateTimeEdit::getMonth()
{
    int num;
    if(m_data->m_showType == 0 || m_data->m_showType == 1)
    {
        num = m_data->m_widget->date().month();
        return num;
    }
    else
        return 0;
}

void CMDDrawDateTimeEdit::setDay(int day)
{
    int year,month;
    if(m_data->m_showType == 0 || m_data->m_showType == 1)
    {
        if(day>0 && day<32)
        {

            year = m_data->m_widget->date().year();
            month = m_data->m_widget->date().month();
            m_data->m_widget->setDate(QDate(year,month,day));
        }
    }
}

int CMDDrawDateTimeEdit::getDay()
{
    int num;
    if(m_data->m_showType == 0 || m_data->m_showType == 1)
    {
        num = m_data->m_widget->date().day();
        return num;
    }
    else
        return 0;
}

void CMDDrawDateTimeEdit::setHour(int hour)
{
    int minute,second;
    if(m_data->m_showType == 2)
    {
        if(hour>-1 && hour<25)
        {
            minute = m_data->m_widget->time().minute();
            second = m_data->m_widget->time().second();
            m_data->m_widget->setTime(QTime(hour,minute,second));
        }
    }

}

int CMDDrawDateTimeEdit::getHour()
{
    int num;
    if(m_data->m_showType == 2)
    {
        num = m_data->m_widget->time().hour();
        return num;
    }
    else
        return 0;
}

void CMDDrawDateTimeEdit::setMinute(int minute)
{
    int hour,second;
    if(m_data->m_showType == 2)
    {
        if(minute>-1 && minute<60)
        {

            hour = m_data->m_widget->time().hour();
            second = m_data->m_widget->time().second();
            m_data->m_widget->setTime(QTime(hour,minute,second));
        }
    }
}

int CMDDrawDateTimeEdit::getMinute()
{
    int num;
    if(m_data->m_showType == 2)
    {
        num = m_data->m_widget->time().minute();
        return num;
    }
    else
        return 0;
}

void CMDDrawDateTimeEdit::setSecond(int second)
{
    int hour,minute;
    if(m_data->m_showType == 2)
    {
        if(second>-1 && second<60)
        {
            hour = m_data->m_widget->time().hour();
            minute = m_data->m_widget->time().minute();
            m_data->m_widget->setTime(QTime(hour,minute,second));
        }
    }
}

int CMDDrawDateTimeEdit::getSecond()
{
    int num;
    if(m_data->m_showType == 2)
    {
        num = m_data->m_widget->time().second();
        return num;
    }
    else
        return 0;
}

void CMDDrawDateTimeEdit::setTime(QDateTime time)
{
    m_data->m_widget->setDateTime(time);
}

QDateTime CMDDrawDateTimeEdit::getTime()
{
    QDateTime time;
    time = m_data->m_widget->dateTime();
    return time;
}

void CMDDrawDateTimeEdit::setMaxDateTime(QDateTime datetime)
{
    m_data->m_maxDateTime = datetime;
    m_data->m_widget->setMaximumDateTime(datetime);
}

QDateTime CMDDrawDateTimeEdit::maxDateTime()
{
    return m_data->m_maxDateTime;
}

void CMDDrawDateTimeEdit::setMinDateTime(QDateTime datetime)
{
    m_data->m_minDateTime = datetime;
    m_data->m_widget->setMinimumDateTime(datetime);
}

QDateTime CMDDrawDateTimeEdit::minDateTime()
{
    return m_data->m_minDateTime;
}

/*!
\section 控件对象属性
*/
DrawType CMDDrawDateTimeEdit::type()
{
    return DrawType::DateTime;
}

CMDPropertyDrawObj *CMDDrawDateTimeEdit::createPropertyDrawObj()
{
    return new CMDPropertyDrawDateTimeEdit(this);
}

void CMDDrawDateTimeEdit::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();

    if(!widget())
        return;
    switch (m_data->m_showType) {
    case 0:
        m_data->m_widget->setDisplayFormat("yyyy/MM/dd");   //! 短日期型
        break;
    case 1:
        m_data->m_widget->setDisplayFormat("yyyy年MM月dd日");//! 长日期型
        break;
    case 2:
        m_data->m_widget->setTime(QTime::currentTime());
        m_data->m_widget->setDisplayFormat("hh:mm:ss");     //! 时间型
        break;
    }
    m_data->m_widget->setCalendarPopup(m_data->m_calendarPop);

    QLineEdit *lineEdit = widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly);
    if(lineEdit)
    {
        widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly)->setStyleSheet(getBasicWidgetColor());
    }
    widget()->setStyleSheet(CMDDrawBasicWidget::getBasicWidgetColor());
}

CMDDrawObj *CMDDrawDateTimeEdit::createSelf()
{
    return new CMDDrawDateTimeEdit();
}
/*!
\section 序列化
*/
CMDDrawObj *CMDDrawDateTimeEdit::clone()
{
    CMDDrawDateTimeEdit* obj = static_cast<CMDDrawDateTimeEdit*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawDateTimeEdit::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawDateTimeEdit",js);
}

void CMDDrawDateTimeEdit::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawDateTimeEdit").toObject();
    m_data->deserialize(js);
}

QList<QStringList> CMDDrawDateTimeEdit::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawdatetime.date",       QObject::tr("日期")});
    return list;
}
void CMDDrawDateTimeEdit::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0:     //! date
        m_writeVarEnabled = false;
        setTime(QDateTime::fromString(exp->stringValue(10,"yyyy-MM-dd hh:mm:ss"),"yyyy-MM-dd hh:mm:ss"));
        m_writeVarEnabled = true;
        break;
    default:
        break;
    }
}
void CMDDrawDateTimeEdit::writeVariable()
{
    if (parent() == NULL)
        return;
    if (isStudioMode())
        return;

    QString textExpression = expManager()->list()->last()->lowerExpression();

    if (textExpression.count() == 0)
        return;

    parent()->framework()->setVariableValue(textExpression,m_data->m_widget->dateTime());

    setTime(parent()->framework()->variableValue(textExpression).toDateTime());
}

bool CMDDrawDateTimeEdit::writeVarEnabled()
{
    return m_writeVarEnabled;
}

void CMDDrawDateTimeEdit::setWriteVarEnabled(bool writeVarEnabled)
{
    m_writeVarEnabled = writeVarEnabled;
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawDateTimeEdit::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawDateTimeEdit", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawDateTimeEdit(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawDateTimeEdit::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawDateTimeEdit");
}

void CMDDrawDateTimeEdit::setBackColor(const QColor &value)
{
    CMDDrawBasicWidget::setBackColor(value);

    QLineEdit *lineEdit = widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly);
    if(lineEdit)
    {
        widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly)->setStyleSheet(getBasicWidgetColor());
    }
    widget()->setStyleSheet(getBasicWidgetColor());
}

void CMDDrawDateTimeEdit::setDisableBackColor(const QColor &value)
{
    CMDDrawBasicWidget::setDisableBackColor(value);

    QLineEdit *lineEdit = widget()->findChild<QLineEdit *>("qt_spinbox_lineedit", Qt::FindDirectChildrenOnly);
    if(lineEdit)
    {
        lineEdit->setStyleSheet(getBasicWidgetColor());
    }
    widget()->setStyleSheet(getBasicWidgetColor());
}
//////////////////////////////////////////////////////////////////////////

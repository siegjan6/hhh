#include "MDPropertyDrawCalendarWidget.h"
#include "MDDrawCalendarWidget.h"
#include "undo/DrawCalendarWidgetCommand.h"
/*!
\section 属性函数
*/
void CMDPropertyDrawCalendarWidget::setWeekNum(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchWeekNum(value);
    else
        undo()->push(new DrawCalendarWidgetCommand(
                         widget(),widget()->weekNum(),value,(int)DrawCalendarWidgetId::WeekNum));

    setModify(true);
}

bool CMDPropertyDrawCalendarWidget::weekNum()
{
    if(widget())
        return widget()->weekNum();
    return false;
}

void CMDPropertyDrawCalendarWidget::setCurrentDateHighlight(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchCurrentDateHighlight(value);
    else
        undo()->push(new DrawCalendarWidgetCommand(
                         widget(),widget()->currentDateHighlight(),value,(int)DrawCalendarWidgetId::CurrentDateHighlight));

    setModify(true);
}

bool CMDPropertyDrawCalendarWidget::currentDateHighlight()
{
    if(widget())
        return widget()->currentDateHighlight();
    return false;
}

void CMDPropertyDrawCalendarWidget::setBackToToday(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchBackToToday(value);
    else
        undo()->push(new DrawCalendarWidgetCommand(
                         widget(),widget()->backToToday(),value,(int)DrawCalendarWidgetId::BackToToday));

    setModify(true);
}

bool CMDPropertyDrawCalendarWidget::backToToday()
{
    if(widget())
        return widget()->backToToday();
    return false;
}

void CMDPropertyDrawCalendarWidget::setCurrentDate(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchCurrentDate(value);
    else
        undo()->push(new DrawCalendarWidgetCommand(
                         widget(),widget()->currentDate(),value,(int)DrawCalendarWidgetId::CurrentDate));

    setModify(true);
}

bool CMDPropertyDrawCalendarWidget::currentDate()
{
    if(widget())
        return widget()->currentDate();
    return false;
}

void CMDPropertyDrawCalendarWidget::setGridVisible(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchGridVisible(value);
    else
        undo()->push(new DrawCalendarWidgetCommand(
                         widget(),widget()->gridVisible(),value,(int)DrawCalendarWidgetId::GridVisible));

    setModify(true);
}

bool CMDPropertyDrawCalendarWidget::gridVisible()
{
    if(widget())
        return widget()->gridVisible();
    return false;
}

void CMDPropertyDrawCalendarWidget::setFirstDayofWeek(Qt::DayOfWeek value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchFirstDayofWeek(value);
    else
        undo()->push(new DrawCalendarWidgetCommand(
                         widget(),widget()->firstDayofWeek(),value,(int)DrawCalendarWidgetId::FirstDayofWeek));

    setModify(true);
}

Qt::DayOfWeek CMDPropertyDrawCalendarWidget::firstDayofWeek()
{
    if(widget())
        return widget()->firstDayofWeek();
    return Qt::DayOfWeek::Sunday;
}

void CMDPropertyDrawCalendarWidget::setEnable(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchEnable(value);
    else
        undo()->push(new DrawCalendarWidgetCommand(
                         widget(),widget()->enable(),value,(int)DrawCalendarWidgetId::Enable));

    setModify(true);
}

bool CMDPropertyDrawCalendarWidget::enable()
{
    if(widget())
        return widget()->enable();
    return false;
}

CMDPropertyDrawCalendarWidget::CMDPropertyDrawCalendarWidget(CMDDrawObj *parent):
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("enable",tr("可用"));
    insertTranslateMap("backToToday",tr("显示回到今天"));
    insertTranslateMap("currentDate",tr("显示当天日期"));
    insertTranslateMap("currentDateHighlight",tr("高亮当天日期"));
    insertTranslateMap("weekNum",tr("显示周数"));
    insertTranslateMap("gridVisible",tr("显示网格"));
    insertTranslateMap("firstDayofWeek",tr("每周开始于星期"));
}

CMDDrawCalendarWidget *CMDPropertyDrawCalendarWidget::widget()
{
    return dynamic_cast<CMDDrawCalendarWidget*>(m_object);
}

void CMDPropertyDrawCalendarWidget::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}


//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawCalendarWidget::setBatchWeekNum(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCalendarWidget* v = dynamic_cast<CMDDrawCalendarWidget*>(o);
        undo()->push(new DrawCalendarWidgetCommand(
                         v,v->weekNum(),value,(int)DrawCalendarWidgetId::WeekNum));
    }

    undo()->endMacro();
}

void CMDPropertyDrawCalendarWidget::setBatchCurrentDateHighlight(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCalendarWidget* v = dynamic_cast<CMDDrawCalendarWidget*>(o);
        undo()->push(new DrawCalendarWidgetCommand(
                         v,v->currentDateHighlight(),value,(int)DrawCalendarWidgetId::CurrentDateHighlight));
    }

    undo()->endMacro();
}

void CMDPropertyDrawCalendarWidget::setBatchBackToToday(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCalendarWidget* v = dynamic_cast<CMDDrawCalendarWidget*>(o);
        undo()->push(new DrawCalendarWidgetCommand(
                         v,v->backToToday(),value,(int)DrawCalendarWidgetId::BackToToday));
    }

    undo()->endMacro();
}

void CMDPropertyDrawCalendarWidget::setBatchCurrentDate(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCalendarWidget* v = dynamic_cast<CMDDrawCalendarWidget*>(o);
        undo()->push(new DrawCalendarWidgetCommand(
                         v,v->currentDate(),value,(int)DrawCalendarWidgetId::CurrentDate));
    }

    undo()->endMacro();
}

void CMDPropertyDrawCalendarWidget::setBatchGridVisible(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCalendarWidget* v = dynamic_cast<CMDDrawCalendarWidget*>(o);
        undo()->push(new DrawCalendarWidgetCommand(
                         v,v->gridVisible(),value,(int)DrawCalendarWidgetId::GridVisible));
    }

    undo()->endMacro();
}

void CMDPropertyDrawCalendarWidget::setBatchFirstDayofWeek(Qt::DayOfWeek value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCalendarWidget* v = dynamic_cast<CMDDrawCalendarWidget*>(o);
        undo()->push(new DrawCalendarWidgetCommand(
                         v,v->firstDayofWeek(),value,(int)DrawCalendarWidgetId::FirstDayofWeek));
    }

    undo()->endMacro();
}

void CMDPropertyDrawCalendarWidget::setBatchEnable(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCalendarWidget* v = dynamic_cast<CMDDrawCalendarWidget*>(o);
        undo()->push(new DrawCalendarWidgetCommand(
                         v,v->enable(),value,(int)DrawCalendarWidgetId::Enable));
    }

    undo()->endMacro();
}
//////////////////////////////////////////////////////////////////////////

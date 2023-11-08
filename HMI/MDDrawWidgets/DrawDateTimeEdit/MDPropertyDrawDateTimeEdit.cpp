#include "MDPropertyDrawDateTimeEdit.h"
#include "MDDrawDateTimeEdit.h"
#include "undo/DrawDateTimeCommand.h"

void CMDPropertyDrawDateTimeEdit::setShowType(PropertyDateTimeStyle value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchShowType(value);
    else
    {
        int factValue = 0;
        switch (value) {
        case PropertyDateTimeStyle::shortDate:
            factValue = 0;
            break;
        case PropertyDateTimeStyle::longDate:
            factValue = 1;
            break;
        case PropertyDateTimeStyle::time:
            factValue = 2;
            break;
        default:
            break;
        }
        undo()->push(new DrawDateTimeCommand(
                         widget(),widget()->showType(),factValue,(int)DrawDateTimeId::ShowType));
    }
    setModify(true);
}

CMDPropertyDrawDateTimeEdit::PropertyDateTimeStyle CMDPropertyDrawDateTimeEdit::showType()
{
    if(widget())
    {
        int dtStyle = widget()->showType();
        return (PropertyDateTimeStyle)dtStyle;
    }
    return PropertyDateTimeStyle::shortDate;
}

void CMDPropertyDrawDateTimeEdit::setCalendarPop(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchCalendarPop(value);
    else
        undo()->push(new DrawDateTimeCommand(
                         widget(),widget()->calendarPop(),value,(int)DrawDateTimeId::CalendarPop));

    setModify(true);
}

bool CMDPropertyDrawDateTimeEdit::calendarPop()
{
    if(widget())
        return widget()->calendarPop();
    return true;
}

CMDPropertyDrawDateTimeEdit::CMDPropertyDrawDateTimeEdit(CMDDrawObj *parent):
    CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap("calendarPop",tr("下拉月历"));
    insertTranslateMap("showType",tr("类型"));
    insertTranslateMap("longDate",tr("长日期型"));
    insertTranslateMap( "shortDate",tr("短日期型"));
    insertTranslateMap("time" ,tr("时间型"));
}

CMDDrawDateTimeEdit *CMDPropertyDrawDateTimeEdit::widget()
{
    return dynamic_cast<CMDDrawDateTimeEdit*>(m_object);
}

void CMDPropertyDrawDateTimeEdit::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}

//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawDateTimeEdit::setBatchShowType(CMDPropertyDrawDateTimeEdit::PropertyDateTimeStyle value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawDateTimeEdit* v = dynamic_cast<CMDDrawDateTimeEdit*>(o);
        int factValue = 0;
        switch (value) {
        case PropertyDateTimeStyle::shortDate:
            factValue = 0;
            break;
        case PropertyDateTimeStyle::longDate:
            factValue = 1;
            break;
        case PropertyDateTimeStyle::time:
            factValue = 2;
            break;
        default:
            break;
        }
        undo()->push(new DrawDateTimeCommand(
                         v,v->showType(),factValue,(int)DrawDateTimeId::ShowType));
    }

    undo()->endMacro();
}

void CMDPropertyDrawDateTimeEdit::setBatchCalendarPop(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawDateTimeEdit* v = dynamic_cast<CMDDrawDateTimeEdit*>(o);
        undo()->push(new DrawDateTimeCommand(
                         v,v->calendarPop(),value,(int)DrawDateTimeId::CalendarPop));
    }

    undo()->endMacro();
}
//////////////////////////////////////////////////////////////////////////

#include <QKeyEvent>
#include "MDDrawCalendarWidgetEvent.h"
#include "MDDrawCalendarWidget.h"
#include "DrawCalendarWidgetData.h"
#include "CalendarForm.h"

CMDDrawCalendarWidgetEvent::CMDDrawCalendarWidgetEvent()
{
}
/*!
\section  绑定日期变化事件
*/
void CMDDrawCalendarWidgetEvent::connectEvent()
{
    if (parent()->parent() == NULL)
        return;
    if (parent()->parent()->framework()->isStudioMode())
        return;
    CCalendarForm* sender = static_cast<CCalendarForm*>(parent()->widget());
    connect(sender->calendar(),&QCalendarWidget::clicked,this,&CMDDrawCalendarWidgetEvent::dateChanged);
    sender->installEventFilter(this);
}

void CMDDrawCalendarWidgetEvent::dateChanged(const QDate &date)
{
    QList<QVariant> paras;
    paras.append(QDateTime(date).toMSecsSinceEpoch());
    parent()->parent()->run()->fireEvent(parent()->name(), "onDateChanged", paras);

    writeVariable();
}

void CMDDrawCalendarWidgetEvent::writeVariable()
{
    CMDDrawCalendarWidget* w = dynamic_cast<CMDDrawCalendarWidget*>(parent());
    if (w != NULL)
    {
        if(w->writeVarEnabled())
        {
            w->writeVariable();
        }
    }
}

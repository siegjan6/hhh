#include <QEvent>
#include <QKeyEvent>
#include "MDDrawDateTimeEditEvent.h"
#include "MDDrawDateTimeEdit.h"
#include "DrawDateTimeEditData.h"

CMDDrawDateTimeEditEvent::CMDDrawDateTimeEditEvent()
{

}
/*!
\section 绑定时间、日期变化事件
*/
void CMDDrawDateTimeEditEvent::connectEvent()
{
   CMDDrawBasicWidgetEvent::connectEvent();

    if (parent()->parent() == NULL)
        return;
    if (parent()->parent()->framework()->isStudioMode())
        return;
    QDateTimeEdit* sender = static_cast<QDateTimeEdit*>(parent()->widget());
    connect(sender,&QDateTimeEdit::dateTimeChanged,this,&CMDDrawDateTimeEditEvent::dateTimeChanged);
}

void CMDDrawDateTimeEditEvent::dateTimeChanged(const QDateTime &datetime)
{
    //! 脚本
    QList<QVariant> paras;
    paras.append(datetime.toMSecsSinceEpoch());
    parent()->parent()->run()->fireEvent(parent()->name(), "onDateTimeChanged", paras);
    writeVariable();
}

void CMDDrawDateTimeEditEvent::writeVariable()
{
    CMDDrawDateTimeEdit* w = dynamic_cast<CMDDrawDateTimeEdit*>(parent());
    if(w != NULL)
    {
        if(w->writeVarEnabled())
        {
            w->writeVariable();
        }
    }
}

#include "MDDrawSpinBox.h"
#include "MDDrawSpinBoxEvent.h"
#include "DrawSpinBoxData.h"

CMDDrawSpinBoxEvent::CMDDrawSpinBoxEvent()
{
}
/*!
\section 绑定变量改变事件
*/
void CMDDrawSpinBoxEvent::connectEvent()
{
    CMDDrawBasicWidgetEvent::connectEvent();

    if (parent()->parent() == NULL)
        return;
    if (parent()->parent()->framework()->isStudioMode())
        return;
    QSpinBox* sender = static_cast<QSpinBox*>(parent()->widget());
    connect(sender,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void CMDDrawSpinBoxEvent::valueChanged(int value)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(value));
    parent()->parent()->run()->fireEvent(parent()->name(), "onValueChanged", paras);
    writeVariable();
}

void CMDDrawSpinBoxEvent::writeVariable()
{
    CMDDrawSpinBox* w = dynamic_cast<CMDDrawSpinBox*>(parent());
    if(w != NULL)
    {
        if(w->writeVarEnabled())
        {
            w->writeVariable();
        }
    }
}


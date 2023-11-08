#include "MDDrawCheckBoxEvent.h"
#include "MDDrawCheckBox.h"
#include "DrawCheckBoxData.h"
#include <QMouseEvent>
#include <QApplication>

CMDDrawCheckBoxEvent::CMDDrawCheckBoxEvent()
{

}

/*!
\section  绑定选中状态改变事件
*/
void CMDDrawCheckBoxEvent::connectEvent()
{
    CMDDrawBasicWidgetEvent::connectEvent();

    if (parent()->parent() == NULL)
        return;
    if (parent()->parent()->framework()->isStudioMode())
        return;
    QCheckBox* sender = static_cast<QCheckBox*>(parent()->widget());
    connect(sender,&QCheckBox::stateChanged,this,&CMDDrawCheckBoxEvent::checkChanged);
}

void CMDDrawCheckBoxEvent::checkChanged(int value)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(value));
    parent()->parent()->run()->fireEvent(parent()->name(), "onCheckChanged", paras);
    writeVariable();
}

/*!
\section  绑定鼠标事件
*/
bool CMDDrawCheckBoxEvent::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != parent()->widget())
        return false;

    if (parent()->parent() != NULL)
    {
        if (parent()->isStudioMode())
        {
            switch (event->type()) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease:
            case QEvent::MouseMove:
                sendMouseEvent(event);
                break;
            case QEvent::ContextMenu:
                if (parent()->widget()->contextMenuPolicy() == Qt::DefaultContextMenu)
                {
                    QMainWindow* window = dynamic_cast<QMainWindow*>(parent()->parent());
                    QApplication::sendEvent(window, static_cast<QContextMenuEvent *>(event));
                }
                break;
            default:
                break;
            }
        }
    }

    return CMDDrawBasicWidgetEvent::eventFilter(watched, event);
}

void CMDDrawCheckBoxEvent::sendMouseEvent(QEvent *event)
{
    QMouseEvent* m = static_cast<QMouseEvent*>(event);
    int x = parent()->widget()->x();
    int y = parent()->widget()->y();
    QPointF localPos(m->localPos().x() + x, m->localPos().y() + y);
    QMouseEvent mouseEvent(m->type(), localPos, m->button(), m->buttons(), m->modifiers());
    QMainWindow* window = dynamic_cast<QMainWindow*>(parent()->parent());
    QApplication::sendEvent(window, &mouseEvent);
}

void CMDDrawCheckBoxEvent::writeVariable()
{
    CMDDrawCheckBox* w = dynamic_cast<CMDDrawCheckBox*>(parent());
    if (w != NULL)
    {
        if(w->writeVarEnabled())
        {
            w->writeVariable();
        }
    }
}


#include <QMouseEvent>
#include <QApplication>
#include "MDDrawRadioButtonEvent.h"
#include "MDDrawRadioButton.h"
#include "DrawRadioButtonData.h"

CMDDrawRadioButtonEvent::CMDDrawRadioButtonEvent()
{

}

/*!
\section  绑定选中状态事件
*/
void CMDDrawRadioButtonEvent::connectEvent()
{
    CMDDrawBasicWidgetEvent::connectEvent();

    if (parent()->parent() == NULL)
        return;
    if (parent()->parent()->framework()->isStudioMode())
        return;
    QRadioButton* sender = static_cast<QRadioButton*>(parent()->widget());
    connect(sender, &QRadioButton::toggled,this, &CMDDrawRadioButtonEvent::checkChanged);
}
void CMDDrawRadioButtonEvent::checkChanged(bool value)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(value));
    parent()->parent()->run()->fireEvent(parent()->name(), "onCheckChanged", paras);

    dynamic_cast<CMDDrawRadioButton*>(parent())->checkChanged(value);
}
/*!
\section  绑定鼠标事件
*/
bool CMDDrawRadioButtonEvent::eventFilter(QObject *watched, QEvent *event)
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

void CMDDrawRadioButtonEvent::sendMouseEvent(QEvent *event)
{
    QMouseEvent* m = static_cast<QMouseEvent*>(event);
    int x = parent()->widget()->x();
    int y = parent()->widget()->y();
    QPointF localPos(m->localPos().x() + x, m->localPos().y() + y);
    QMouseEvent mouseEvent(m->type(), localPos, m->button(), m->buttons(), m->modifiers());
    QMainWindow* window = dynamic_cast<QMainWindow*>(parent()->parent());
    QApplication::sendEvent(window, &mouseEvent);
}


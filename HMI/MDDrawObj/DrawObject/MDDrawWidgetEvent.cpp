#include <QEvent>
#include <QMouseEvent>
#include "MDDrawWidgetEvent.h"
#include "MDDrawWidget.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawWidgetEvent::CMDDrawWidgetEvent()
{
    m_parent = NULL;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawWidget *CMDDrawWidgetEvent::parent()
{
    return m_parent;
}
void CMDDrawWidgetEvent::setParent(CMDDrawWidget *parent)
{
    Q_ASSERT(parent != NULL);

    m_parent = parent;
    m_parent->widget()->installEventFilter(this);
}
//////////////////////////////////////////////////////////////////////////
bool CMDDrawWidgetEvent::eventFilter(QObject *watched, QEvent *event)
{
    if (m_parent->isStudioMode())
        return false;
    if (watched != m_parent->widget())
        return false;

    switch (event->type()) {
    case QEvent::MouseButtonPress:{
        QMouseEvent* m = static_cast<QMouseEvent*>(event);
        m_parent->onMouseDown(m->button(), m->pos().x(), m->pos().y());
    }
        break;
    case QEvent::MouseButtonRelease:{
        QMouseEvent* m = static_cast<QMouseEvent*>(event);
        m_parent->onMouseUp(m->button(), m->pos().x(), m->pos().y());
    }
        break;
    case QEvent::Enter:
            m_parent->parent()->run()->loadWidgetEnterEvent(m_parent);
        break;
    case QEvent::Leave:
            m_parent->parent()->run()->loadWidgetLeaveEvent(m_parent);
        break;
    default:
        break;
    }

    return QObject::eventFilter(watched, event);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

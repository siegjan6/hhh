#include "MDDrawBasicWidgetEvent.h"
#include "MDDrawBasicWidget.h"
CMDDrawBasicWidgetEvent::CMDDrawBasicWidgetEvent()
{
}

void CMDDrawBasicWidgetEvent::connectEvent()
{
    if(parent()->parent() == NULL)
        return;
    if(parent()->parent()->framework()->isStudioMode())
        return;
    QWidget* sender = static_cast<QWidget*>(parent()->widget());
    sender->installEventFilter(this);
}

bool CMDDrawBasicWidgetEvent::eventFilter(QObject *watched, QEvent *event)
{
    if (parent()->isStudioMode())
        return false;
    if(watched != parent()->widget())
        return false;

    switch (event->type()) {
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        onMouseDown(e->x(),e->y());
    }
        break;
    case QEvent::MouseButtonRelease:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        onMouseUp(e->x(),e->y());
    }
        break;
    case QEvent::MouseMove:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        onMouseMove(e->x(),e->y());
    }
        break;
    case QEvent::MouseButtonDblClick:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        onMouseDoubleClick(e->x(),e->y());
    }
        break;
    case QEvent::FocusIn:
        onGotFocus();
        break;
    case QEvent::FocusOut:
        onLostFocus();
        break;
    case QEvent::KeyPress:
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int uKey = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(uKey);
        if(key == Qt::Key_unknown)
            return false;
        //获取修饰键(Ctrl,Alt,Shift)的状态
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        int mod = modifierTypes(modifiers);
        onKeyDown(uKey,mod);
    }
        break;
    case QEvent::KeyRelease:
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int uKey = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(uKey);
        if(key == Qt::Key_unknown)
            return false;
        //获取修饰键(Ctrl,Alt,Shift)的状态
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        int mod = modifierTypes(modifiers);
        onKeyUp(uKey,mod);
    }
        break;
        break;
    default:
        break;
    }
    return CMDDrawWidgetEvent::eventFilter(watched,event);
}

void CMDDrawBasicWidgetEvent::onMouseDown(int x, int y)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(x));
    paras.append(QVariant(y));
    parent()->parent()->run()->fireEvent(parent()->name(), "onMouseDown", paras);
}

void CMDDrawBasicWidgetEvent::onMouseUp(int x, int y)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(x));
    paras.append(QVariant(y));
    parent()->parent()->run()->fireEvent(parent()->name(), "onMouseUp", paras);
}

void CMDDrawBasicWidgetEvent::onMouseMove(int x, int y)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(x));
    paras.append(QVariant(y));
    parent()->parent()->run()->fireEvent(parent()->name(), "onMouseMove", paras);
}

void CMDDrawBasicWidgetEvent::onMouseDoubleClick(int x, int y)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(x));
    paras.append(QVariant(y));
    parent()->parent()->run()->fireEvent(parent()->name(), "onMouseDoubleClick", paras);
}

void CMDDrawBasicWidgetEvent::onGotFocus()
{
    //script
    QList<QVariant> paras;
    parent()->parent()->run()->fireEvent(parent()->name(), "onGotFocus", paras);
}

void CMDDrawBasicWidgetEvent::onLostFocus()
{
    //script
    QList<QVariant> paras;
    parent()->parent()->run()->fireEvent(parent()->name(), "onLostFocus", paras);
}

void CMDDrawBasicWidgetEvent::onKeyDown(int key,int isModifiers)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(key));
    paras.append(QVariant(isModifiers));
    parent()->parent()->run()->fireEvent(parent()->name(), "onKeyDown", paras);
}

void CMDDrawBasicWidgetEvent::onKeyUp(int key, int isModifiers)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(key));
    paras.append(QVariant(isModifiers));
    parent()->parent()->run()->fireEvent(parent()->name(), "onKeyUp", paras);
}

int CMDDrawBasicWidgetEvent::modifierTypes(Qt::KeyboardModifiers modifiers)
{
    int shift = (modifiers & Qt::ShiftModifier) ? 1 : 0;
    int ctrl = (modifiers & Qt::ControlModifier) ? 2 : 0;
    int alt = (modifiers & Qt::AltModifier) ? 4 : 0;
    int mod = shift | ctrl | alt;
    return mod;
}

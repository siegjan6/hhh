#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include "FormObjectContainer.h"
#include "PreviewDialog.h"
#include "MDMultiLanguage.h"
//////////////////////////////////////////////////////////////////////////
CFormObjectContainer::CFormObjectContainer(QWidget *parent) :
    QWidget(parent)
{
    m_currentObj = NULL;
    m_operateType = MoveType::None;
    m_autoJoin = true;
    m_isTransparent = false;
    m_container = NULL;
    QRect screen = QApplication::desktop()->screenGeometry();
    setFrameSize(screen.size());

    setMouseTracking(true);
}
CFormObjectContainer::~CFormObjectContainer()
{
    clear();
}
//////////////////////////////////////////////////////////////////////////
void CFormObjectContainer::setFrameSize(const QSize &value)
{
    m_frameSize = value;

    QSize s = m_frameSize;
    s.setWidth(s.width() + 200);
    s.setHeight(s.height() + 200);
    setFixedSize(s);
}
void CFormObjectContainer::setCurrentObj(CFormObject *value)
{
    m_currentObj = value;
    emit rectChanged();
}
//////////////////////////////////////////////////////////////////////////
void CFormObjectContainer::appendForm(CMDHMIForm *form)
{
    CFormObject* obj = new CFormObject(form);
    m_list.append(obj);
    setCurrentObj(obj);

    update();
}
CFormObject *CFormObjectContainer::findOpened(const QString &fullName)
{
    for(int i = 0; i < m_list.count(); i++)
    {
        CFormObject* obj = m_list.at(i);
        if(obj->fullName().compare(fullName, Qt::CaseInsensitive) == 0)
            return obj;
    }

    return NULL;
}
void CFormObjectContainer::saveForms()
{
    for(int i = 0; i < m_list.count(); i++)
    {
        CFormObject* obj = m_list.at(i);
        if(!obj->hasChanged())
            continue;
        m_container->saveForm(obj);
    }
}
void CFormObjectContainer::select(CFormObject *obj)
{
    if(m_list.removeOne(obj))
    {
        m_list.append(obj);
        obj->setIsVisible(true);
        setCurrentObj(obj);
        update();
    }
}
void CFormObjectContainer::onKeyPress(QKeyEvent *e)
{
    if(m_currentObj == NULL)
        return;
    QRect r = m_currentObj->rect();

    switch (e->key())
    {
    case Qt::Key_Up://up
        if (e->modifiers() & Qt::ShiftModifier)
            r.setHeight(r.height() - 1);
        else
            r.moveTop(r.y() - 1);
        break;
    case Qt::Key_Down://down
        if (e->modifiers() & Qt::ShiftModifier)
            r.setHeight(r.height() + 1);
        else
            r.moveTop(r.y() + 1);
        break;
    case Qt::Key_Left://left
        if (e->modifiers() & Qt::ShiftModifier)
            r.setWidth(r.width() - 1);
        else
            r.moveLeft(r.x() - 1);
        break;
    case Qt::Key_Right://right
        if (e->modifiers() & Qt::ShiftModifier)
            r.setWidth(r.width() + 1);
        else
            r.moveLeft(r.x() + 1);
        break;
    default:
        break;
    }

    switch (e->key())
    {
    case Qt::Key_Up://up
    case Qt::Key_Down://down
    case Qt::Key_Left://left
    case Qt::Key_Right://right
        setRect(r);
        update();
        break;
    default:
        break;
    }
}
void CFormObjectContainer::onClose(QCloseEvent *e)
{
    bool needSave = false;
    for(int i = 0; i < m_list.count(); i++)
    {
        if(m_list.at(i)->hasChanged())
        {
            needSave = true;
            break;
        }
    }

    if(needSave)
    {
        QMessageBox box(this);
        box.setText(tr("是否保存？"));
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        if(box.exec() == QMessageBox::Ok)
        {
            for(int i = 0; i < m_list.count(); i++)
            {
                CFormObject* o = m_list.at(i);
                if(o->hasChanged())
                    m_container->saveForm(o);
            }
        }
    }

    clear();
}

void CFormObjectContainer::clear()
{
    qDeleteAll(m_list.begin(), m_list.end());
    m_list.clear();
    m_currentObj = NULL;
    emit rectChanged();
}
//////////////////////////////////////////////////////////////////////////
void CFormObjectContainer::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    //background
    p.fillRect(QRect(0, 0, frameSize().width()+500, frameSize().height()+500), Qt::white);

    QRect frame(QRect(0, 0, frameSize().width()+2, frameSize().height()+2));
    p.setClipRect(frame);

    //background
    p.fillRect(frame, QColor(220, 220, 220));

    //object
    if(m_isTransparent)
        p.setOpacity(0.6);
    for(int i = 0; i < m_list.count(); i++)
    {
        CFormObject* obj = m_list.at(i);
        if(obj->isVisible())
            obj->paint(&p);
    }
    p.setOpacity(1);

    //draw frame
    if(m_currentObj != NULL)
    {
        QPen pen(Qt::blue);
        pen.setWidth(2);
        p.setPen(pen);
        p.drawRect(m_currentObj->rect());
    }

    //background
    p.setPen(Qt::yellow);
    p.drawRect(QRect(0, 0, frameSize().width(), frameSize().height()));
}
void CFormObjectContainer::mouseMoveEvent(QMouseEvent *e)
{
    resetCursor(getOperateType(m_currentObj, e->pos()));
    if(m_operateType == MoveType::None)
        return;

    mouseOperate(e->pos());
    update();
}
void CFormObjectContainer::mousePressEvent(QMouseEvent *e)
{
    m_operateType = getOperateType(m_currentObj, e->pos());
    if(m_operateType == MoveType::None)
    {
        selectObject(e->pos());
        m_operateType = getOperateType(m_currentObj, e->pos());
    }
    resetCursor(m_operateType);

    if(m_operateType != MoveType::None)
    {
        m_mouseDownPoint = e->pos();
        m_backupRect = m_currentObj->rect();
    }

    update();
}
void CFormObjectContainer::mouseReleaseEvent(QMouseEvent *e)
{
    m_operateType = MoveType::None;
    update();
}
void CFormObjectContainer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(m_currentObj != NULL)
    {
        m_currentObj->setIsVisible(false);
        setCurrentObj(NULL);
        update();
    }
}
//////////////////////////////////////////////////////////////////////////
CFormObjectContainer::MoveType CFormObjectContainer::getOperateType(CFormObject *obj, QPoint p)
{
    if(obj == NULL)
        return MoveType::None;

    const int w = 5;
    QRect r = obj->rect();
    QRect top(r.left(), r.top() - w, r.width(), w*2);
    QRect bottom(r.left(), r.bottom() - w, r.width(), w*2);
    QRect left(r.left() - w, r.top(), w*2, r.height());
    QRect right(r.right() - w, r.top(), w*2, r.height());

    if(right.contains(p))
        return MoveType::Right;
    if(bottom.contains(p))
        return MoveType::Bottom;
    if(left.contains(p))
        return MoveType::Left;
    if(top.contains(p))
        return MoveType::Top;
    if(r.contains(p))
        return MoveType::Move;

    return MoveType::None;
}
void CFormObjectContainer::resetCursor(CFormObjectContainer::MoveType type)
{
    switch (type)
    {
    case MoveType::Left:
    case MoveType::Right:
        setCursor(Qt::SizeHorCursor);
        break;
    case MoveType::Top:
    case MoveType::Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case MoveType::Move:
        setCursor(Qt::SizeAllCursor);
        break;
    case MoveType::None:
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}
void CFormObjectContainer::selectObject(const QPoint &p)
{
    for(int i = m_list.count() - 1; i >= 0; i--)
    {
        CFormObject* obj = m_list.at(i);
        if(obj->isVisible())
        {
            if(obj->rect().contains(p))
            {
                select(obj);
                return;
            }
        }
    }
    setCurrentObj(NULL);
    update();
}
void CFormObjectContainer::mouseOperate(const QPoint &currentPos)
{
    if(m_currentObj == NULL)
        return;

    int xOff = currentPos.x() - m_mouseDownPoint.x();
    int yOff = currentPos.y() - m_mouseDownPoint.y();      
    QRect r = m_backupRect;
    int x = r.left();
    int y = r.top();
    int w = r.width();
    int h = r.height();

    switch (m_operateType)
    {
    case MoveType::Left:
        r.moveLeft(x + xOff);
        r.setWidth(w - xOff);
        break;
    case MoveType::Right:
        r.setWidth(w + xOff);
        break;
    case MoveType::Top:
        r.moveTop(y + yOff);
        r.setHeight(h - yOff);
        break;
    case MoveType::Bottom:
        r.setHeight(h + yOff);
        break;
    case MoveType::Move:
        r.moveTopLeft(QPoint(x + xOff, y + yOff));
        break;
    default:
        break;
    }

    joinRect(m_currentObj, r, m_operateType);
    setRect(r);
}
void CFormObjectContainer::joinRect(CFormObject *obj, QRect &rect, MoveType type)
{
    if(!m_autoJoin)
        return;

    const int Interval = 20;

    QRect r = rect;
    int x = r.left();
    int y = r.top();
    int w = r.width();
    int h = r.height();
    int right = x + w;
    int bottom = y + h;
    int xJoin = 0;
    int yJoin = 0;
    int xOff = 9999;
    int yOff = 9999;

    int count = m_list.count();
    //count+1个比较，包括所有窗体对象和边框
    for(int i = 0; i <= count; i++)
    {
        //frame&move,left,top
        int xObj = 0;
        int yObj = 0;

        if(i == count)
        {
            //frame&right,bottom
            if(type == MoveType::Right || type == MoveType::Bottom)
            {
                xObj= frameSize().width();
                yObj = frameSize().height();
            }
        }
        else
        {
            CFormObject* o = m_list.at(i);
            if((!o->isVisible()) || o == obj)
                continue;

            if(type == MoveType::Right || type == MoveType::Bottom)//form&right,bottom
            {
                xObj = o->rect().left();
                yObj = o->rect().top();
            }
            else//form&move,left,top
            {
                //不能直接用right(),有一个像素的误差
                xObj = o->rect().left() + o->rect().width();
                yObj = o->rect().top() + o->rect().height();
            }
        }

        //move,left,top
        int xObjOff = x - xObj;
        int yObjOff = y - yObj;
        //right,bottom
        if(type == MoveType::Right || type == MoveType::Bottom)
        {
            xObjOff = right - xObj;
            yObjOff = bottom - yObj;
        }

        if(abs(xObjOff) < abs(xOff))
        {
            xJoin = xObj;
            xOff = xObjOff;
        }
        if(abs(yObjOff) < abs(yOff))
        {
            yJoin = yObj;
            yOff = yObjOff;
        }
    }

    if(type == MoveType::Move)
    {
        if(abs(xOff) < Interval)
            r.moveLeft(xJoin);
        if(abs(yOff) < Interval)
            r.moveTop(yJoin);
    }
    else if(type == MoveType::Left)
    {
        if(abs(xOff) < Interval)
        {
            r.moveLeft(xJoin);
            r.setWidth(w + xOff);
        }
    }
    else if(type == MoveType::Top)
    {
        if(abs(yOff) < Interval)
        {
            r.moveTop(yJoin);
            r.setHeight(h + yOff);
        }
    }
    else if(type == MoveType::Right)
    {
        if(abs(xOff) < Interval)
            r.setWidth(w - xOff);
    }
    else if(type == MoveType::Bottom)
    {
        if(abs(yOff) < Interval)
            r.setHeight(h - yOff);
    }

    rect = r;
}
void CFormObjectContainer::setRect(const QRect &rect)
{
    m_currentObj->setRect(rect);
    emit rectChanged();
}
//////////////////////////////////////////////////////////////////////////

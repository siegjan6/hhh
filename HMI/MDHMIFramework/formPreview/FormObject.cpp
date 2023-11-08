#include "FormObject.h"
//////////////////////////////////////////////////////////////////////////
CFormObject::CFormObject(CMDHMIForm *form)
{
    m_hasChanged = false;
    m_isVisible = true;

    init(form);
}
//////////////////////////////////////////////////////////////////////////
void CFormObject::setRect(const QRect &rect)
{
    if(m_rect == rect)
        return;

    m_rect = rect;

    const int Min = 5;
    if(m_rect.width() < Min)
        m_rect.setWidth(Min);
    if(m_rect.height() < Min)
        m_rect.setHeight(Min);
    m_rect.normalized();

    m_hasChanged = true;
}
//////////////////////////////////////////////////////////////////////////
void CFormObject::paint(QPainter *p)
{
    QRect r = m_rect.adjusted(0, 0, 1, 1);
    p->setClipRect(r);
    p->fillRect(m_rect, Qt::gray);
    p->drawPixmap(m_rect.left(), m_rect.top(), m_image);
    p->setPen(Qt::green);
    p->drawRect(m_rect);
    p->setClipRect(QRect(), Qt::NoClip);
}
//////////////////////////////////////////////////////////////////////////
void CFormObject::init(CMDHMIForm *form)
{
    m_rect = form->rect();
    m_fullName = form->common()->fullName();
    m_image = QPixmap(m_rect.width(), m_rect.height());

    QPainter p;
    p.begin(&m_image);
    p.setRenderHint(QPainter::Antialiasing, true);
    form->studio()->drawPreview(&p);
    p.end();
}
//////////////////////////////////////////////////////////////////////////

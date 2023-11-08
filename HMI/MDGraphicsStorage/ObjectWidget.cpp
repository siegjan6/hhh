#include <QWidget>
#include "ObjectWidget.h"

CObjectWidget::CObjectWidget(const QString& imagePath, QWidget *parent):
    QWidget(parent)
{
    m_manager = NULL;
    m_container = NULL;
    m_imagePath = imagePath;

    setMouseTracking(true);
}
CObjectWidget::~CObjectWidget()
{
    if (m_manager != NULL)
        delete m_manager;
}
//////////////////////////////////////////////////////////////////////////
void CObjectWidget::changePath(const QString& path, IHMIForm* container)
{
    m_container = container;

    if (m_manager != NULL)
        delete m_manager;

    m_manager = new CObjectManager(path, m_imagePath, this);
    QSize size = static_cast<QWidget*>(parent())->size();
    calculateSize(size.width(), size.height());
    update();
}
void CObjectWidget::calculateSize(int scrollAreaWidth, int scrollAreaHeight)
{
    if (m_manager == NULL)
        return;

    QSize size =  m_manager->calculateSize(scrollAreaWidth - 20, scrollAreaHeight - 20);
    setFixedSize(size);
}
//////////////////////////////////////////////////////////////////////////
void CObjectWidget::paintEvent(QPaintEvent *e)
{
    if (m_manager == NULL)
        return;

    QPainter p(this);
    m_manager->paint(&p);
}
void CObjectWidget::mousePressEvent(QMouseEvent *e)
{
    if (m_manager == NULL)
        return;

    m_manager->selectObject(e->pos().x(), e->pos().y());
}
void CObjectWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_manager == NULL)
        return;
    if (m_container == NULL)
        return;

    CMDDrawObj* obj = m_manager->currentObj();
    if (obj == NULL)
        return;

    QRectF bound = obj->bound();
    QRectF rf = obj->rect();

    rf.moveLeft(rf.x() + 100 - bound.x());
    rf.moveTop(rf.y() + 100 - bound.y());
    obj->setRect(rf);

    m_container->studio()->pasteObject(obj);
}
//////////////////////////////////////////////////////////////////////////

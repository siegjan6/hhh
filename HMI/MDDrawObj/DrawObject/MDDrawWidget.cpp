#include <QDebug>
#include "MDDrawWidget.h"
#include "DrawWidgetData.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawWidget::CMDDrawWidget()
{
    m_data = new CDrawWidgetData;
}
CMDDrawWidget::~CMDDrawWidget()
{
    if (m_data->m_event != NULL)
        delete m_data->m_event;
    delete m_data;
}
void CMDDrawWidget::initCreate()
{
    CMDDrawObj::initCreate();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawWidget::setWidget(QWidget *value, CMDDrawWidgetEvent* event/* = NULL*/)
{
    Q_ASSERT(value != NULL);
    m_data->m_widget = value;
    m_data->m_widget->setMouseTracking(true);

    if (event == NULL)
        event = new CMDDrawWidgetEvent();
    m_data->m_event = event;
    m_data->m_event->setParent(this);
}
void CMDDrawWidget::setWidgetDock(QWidget *value, CMDDrawWidgetEvent* event/* = NULL*/)
{
    Q_ASSERT(value != NULL);
    m_data->m_widget = value;
    m_data->m_widget->setMouseTracking(true);

    if (event == NULL)
        event = new CMDDrawWidgetEvent();
    m_data->m_event = event;
    m_data->m_event->setParent(this);
}
QWidget *CMDDrawWidget::widget()
{
    return m_data->m_widget;
}
CMDDrawWidgetEvent *CMDDrawWidget::event()
{
    return m_data->m_event;
}
bool CMDDrawWidget::finalEnabled()
{
    return !finalLocked();
}
//////////////////////////////////////////////////////////////////////////
bool CMDDrawWidget::isVector()
{
    return false;
}
bool CMDDrawWidget::isWidget()
{
    return true;
}
void CMDDrawWidget::updateFormScale()
{
    onGeneratePath();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawWidget::serialize(QJsonObject &json)
{
    CMDDrawObj::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawWidget", js);
}
void CMDDrawWidget::deserialize(const QJsonObject &json)
{
    CMDDrawObj::deserialize(json);

    QJsonObject js = json.value("DrawWidget").toObject();
    m_data->deserialize(js);
}
CMDDrawObj *CMDDrawWidget::clone()
{
    CMDDrawWidget* obj = static_cast<CMDDrawWidget*>(CMDDrawObj::clone());
    *obj->m_data = *m_data;
    return obj;
}
void CMDDrawWidget::onGeneratePath()
{
    CMDDrawObj::onGeneratePath();

    QRectF rf = rect();
    qreal scaleW = 1;
    qreal scaleH = 1;
    if (parent() != NULL)
    {
        scaleW = parent()->common()->formScale().x();
        scaleH = parent()->common()->formScale().y();
    }

    rf.moveLeft(rf.left()*scaleW);
    rf.moveTop(rf.top()*scaleH);
    rf.setWidth(rf.width()*scaleW);
    rf.setHeight(rf.height()*scaleH);
    m_data->m_widget->setGeometry(rf.toRect());
}
void CMDDrawWidget::updateVisible()
{
    m_data->m_widget->setVisible(finalVisible());
}
void CMDDrawWidget::updateEnabled()
{
    m_data->m_widget->setEnabled(finalEnabled());
}
void CMDDrawWidget::top()
{
    m_data->m_widget->raise();
}
void CMDDrawWidget::last()
{
    m_data->m_widget->lower();
}
void CMDDrawWidget::onPrint(QPainter *p)
{
    QPixmap pix = m_data->m_widget->grab(QRect(QPoint(0 ,0), rect().size().toSize()));
    p->drawPixmap(rect().topLeft().toPoint(), pix);
}
void CMDDrawWidget::onInitialization()
{
    Q_ASSERT(m_data->m_widget != NULL);

    m_data->m_widget->setParent(dynamic_cast<QWidget*>(parent()));
    bool visible = (m_data->m_widget->parent() != NULL && finalVisible());
    m_data->m_widget->setVisible(visible);
    if(isStudioMode())
        m_data->m_widget->setEnabled(false);
    else
        m_data->m_widget->setEnabled(finalEnabled());

    if (m_data->m_event != NULL)
        m_data->m_event->connectEvent();

    //tooltip
    m_data->m_widget->setToolTip(toolTip());
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawWidget::onMouseDown(Qt::MouseButton /*button*/, qreal x, qreal /*y*/)
{
    QString str;
    str.setNum(x);
    qDebug() << "Widget Down:" + str;
}
void CMDDrawWidget::onMouseUp(Qt::MouseButton /*button*/, qreal /*x*/, qreal /*y*/)
{
//    QString str;
//    str.setNum(x);
//    qDebug() << "Widget Up:" + str;
}
void CMDDrawWidget::onMouseMove(Qt::MouseButton /*button*/, qreal /*x*/, qreal /*y*/)
{
//    QString str;
//    str.setNum(x);
    //    qDebug() << "Widget Move:" + str;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawWidget::undoAdd()
{
    widget()->setParent(dynamic_cast<QWidget*>(parent()));
    widget()->show();
}
void CMDDrawWidget::undoDelete()
{
    widget()->setParent(NULL);
}
//////////////////////////////////////////////////////////////////////////


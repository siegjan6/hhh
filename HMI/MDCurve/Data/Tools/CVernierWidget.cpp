#include "CVernierWidget.h"
#include "CLabelItem.h"
#include "CVernier.h"
#include "CFind.h"

#include <math.h>
#include <QPainter>
#include <QPushButton>

CVernierWidget::CVernierWidget(QWidget *parent) :
    QWidget(parent),
    m_fontColor(Qt::black)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setWindowOpacity(0.7);
}

CVernierWidget::~CVernierWidget()
{

}

void CVernierWidget::paintEvent(QPaintEvent *)
{
    if(m_labelList.isEmpty())
        return;
    QPainter p(this);
    p.setFont(m_itemFont);
    for(int i = 0; i < m_labelList.count();i++){
        CLabelItem *item = m_labelList.at(i);
        item->setFont(m_itemFont);
        item->draw(&p);
    }
}
QColor CVernierWidget::fontColor() const
{
    return m_fontColor;
}

void CVernierWidget::setFontColor(const QColor &fontColor)
{
    m_fontColor = fontColor;
}

CVernierWidget *CVernierWidget::clone() const
{
    CVernierWidget *vernierWidget = new CVernierWidget;
    return vernierWidget;
}

QFont CVernierWidget::itemFont() const
{
    return m_itemFont;
}

void CVernierWidget::setItemFont(const QFont &itemFont)
{
    m_itemFont = itemFont;
}

QSizeF CVernierWidget::widgetSize() const
{
    return m_widgetSize;
}

void CVernierWidget::setWidgetSize(const QSizeF &widgetSize)
{
    m_widgetSize = widgetSize;
    if(m_widgetSize.isValid()){
        QSize size;
        size.setWidth(ceil(m_widgetSize.width()));
        size.setHeight(ceil(m_widgetSize.height()));
        setFixedSize(size);
    }
}

QList<CLabelItem *> CVernierWidget::labelList() const
{
    return m_labelList;
}

void CVernierWidget::setLabelList(const QList<CLabelItem *> &labelList)
{
    m_labelList = labelList;
}



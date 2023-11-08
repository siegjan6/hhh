
#include "DrawItem.h"

//!  获取弧度
qreal getRadian(qreal theta)
{
    return theta * (qreal)3.14159265358979323846 / 180.0F;
}
//!  获取角度
qreal getAngle(qreal radian)
{
    return (qreal)(radian * 180.0F / 3.14159265358979323846);
}


///////////////////////////////////////////////////////////////////////////
CDrawItem::CDrawItem()
{
    m_g = NULL;
    m_r = QRectF(-50,-50,100,100);
    m_d = NULL;
    m_layer = NULL;
    m_rAct = m_r;
    m_shortLen = m_r.width();
    m_nIndex = QFontDatabase::addApplicationFont(":/led/DS-DIGI.TTF");

}

CDrawItem::~CDrawItem()
{
}
/////////////////////////////////////////////////////////////////
/*!
\section 获取画刷以及填充形式
*/
QBrush* CDrawItem::createBrush(const CBackColor& backColor,const QRectF& rect,const QPainterPath* path,bool bRotate)
{
    QBrush* b = NULL;
    QRectF re = rect;

    switch(backColor.m_gradientStyle)
    {
    case gsSolid:
    {
        b = new QBrush(QColor(backColor.m_color1));
        break;
    }
    case gs1:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.y()),QPointF(re.width()+re.x(),re.y()));
        lineargradient.setColorAt(0.1,backColor.m_color1);
        lineargradient.setColorAt(1,backColor.m_color2);
        b = new QBrush(lineargradient);
        break;
    }
    case gs2:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.y()),QPointF(re.x(),re.height()+re.y()));
        lineargradient.setColorAt(0.1,backColor.m_color1);
        lineargradient.setColorAt(1,backColor.m_color2);
        b = new QBrush(lineargradient);
        break;
    }
    case gs3:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.height()+re.y()),QPointF(re.x()+re.width(),re.y()));
        lineargradient.setColorAt(0.1,backColor.m_color2);
        lineargradient.setColorAt(1,backColor.m_color1);
        b = new QBrush(lineargradient);
        break;
    }
    case gs4:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.y()),QPointF(re.x()+re.width(),re.y()+re.height()));
        lineargradient.setColorAt(0.1,backColor.m_color1);
        lineargradient.setColorAt(1,backColor.m_color2);
        b = new QBrush(lineargradient);
        break;
    }
    case gs5:
    {
        qreal re1 = re.width()>=re.height()?re.width():re.height();
        QRadialGradient radialgradient(re.width()/2+re.x(),re.height()/2+re.y(),re1/2,re.width()/2+re.x(),re.height()/2+re.y());
        radialgradient.setColorAt(0,backColor.m_color2);
        radialgradient.setColorAt(0.05,backColor.m_color2);
        radialgradient.setColorAt(1,backColor.m_color1);
        b = new QBrush(radialgradient);
        break;
    }
    case gs6:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.y()),QPointF(re.x()+re.width(),re.y()));
        lineargradient.setColorAt(0.1,backColor.m_color1);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(1,backColor.m_color1);
        b = new QBrush(lineargradient);
        break;
    }
    case gs7:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.y()),QPointF(re.x(),re.y()+re.height()));
        lineargradient.setColorAt(0.1,backColor.m_color1);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(1,backColor.m_color1);
        b = new QBrush(lineargradient);
        break;
    }
    case gs8:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.y()+re.height()),QPointF(re.x()+re.width(),re.y()));
        lineargradient.setColorAt(0.1,backColor.m_color1);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(1,backColor.m_color1);
        b = new QBrush(lineargradient);
        break;
    }
    case gs9:
    {
        QLinearGradient lineargradient(QPointF(re.x(),re.y()),QPointF(re.x()+re.width(),re.y()+re.height()));
        lineargradient.setColorAt(0.1,backColor.m_color1);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(0.5,backColor.m_color2);
        lineargradient.setColorAt(1,backColor.m_color1);
        b = new QBrush(lineargradient);
        break;
    }
    case gsNull:
     {
        b = new QBrush(Qt::NoBrush);
        break;
    }
    default:
        break;
    }
    return b;
}
////////////////////////////////////////////////////////////////////////////////
/*!
\section 获取画笔以及画笔绘制线的风格
*/
QPen* CDrawItem::createPen(CEdgeLine& edgeLine)
{
    QPen* p = NULL;

    switch(edgeLine.m_lineStyle)
    {
    case lsSolid:
    {
        p = new QPen(edgeLine.m_color, qreal(edgeLine.getWidth()));
        p->setStyle(Qt::SolidLine);
        break;
    }
    case lsDash:
    {
        p = new QPen(edgeLine.m_color, qreal(edgeLine.getWidth()));
        p->setStyle(Qt::DashLine);
        break;
    }
    case lsDot:
    {
        p = new QPen(edgeLine.m_color, qreal(edgeLine.getWidth()));
        p->setStyle(Qt::DotLine);
        break;
    }
    case lsDashDot:
    {
        p = new QPen(edgeLine.m_color, qreal(edgeLine.getWidth()));
        p->setStyle(Qt::DashDotLine);
        break;
    }
    case lsDashDotDot:
    {
        p = new QPen(edgeLine.m_color, qreal(edgeLine.getWidth()));
        p->setStyle(Qt::DashDotDotLine);
        break;
    }
    case lsNull:
    {
        p = new QPen(Qt::NoPen);
         break;
    }
    default:
        break;
    }
    return p;
}
/////////////////////////////////////////////////////////////////////////////
void CDrawItem::setPath(QPainterPath &path, const QRectF &rf, qint32 style)
{
    QPolygon polygon;
    switch(style)
        {
        case 0:
            path.addRect(rf);
            break;
        case 1:
            {
              polygon.setPoints(4,(qint32)(rf.x()+rf.width()*0.5),(qint32)rf.y(),
                                 (qint32)(rf.x()+rf.width()),(qint32)(rf.y()+rf.height()),
                                 (qint32)rf.x(),(qint32)(rf.y()+rf.height()),
                                 (qint32)(rf.x()+rf.width()*0.5),(qint32)rf.y());

               path.addPolygon(polygon);
            }
            break;
        case 2:
            {
               polygon.setPoints(4,(qint32)rf.x(),(qint32)rf.y(),(qint32)(rf.x()+rf.width()),
                                 (qint32)rf.y(),(qint32)(rf.x()+rf.width()*0.5),
                                 (qint32)(rf.y()+rf.height()),
                                 (qint32)rf.x(),(qint32)rf.y());

               path.addPolygon(polygon);
            }
            break;
        case 3:
            {
                polygon.setPoints(5,(qint32)(rf.x()+rf.width()*0.25),(qint32)rf.y(),
                                  (qint32)(rf.x()+rf.width()*0.75),(qint32)rf.y(),
                                  (qint32)(rf.x()+rf.width()),(qint32)(rf.y()+rf.height()),
                                  (qint32)rf.x(),(qint32)(rf.y()+rf.height()),
                                  (qint32)(rf.x()+rf.width()*0.25),(qint32)rf.y());
                path.addPolygon(polygon);
            }
            break;
        case 4:
            {
               polygon.setPoints(5,(qint32)rf.x(),(qint32)rf.y(),
                                 (qint32)(rf.x()+rf.width()),(qint32)rf.y(),
                                 (qint32)(rf.x()+rf.width()*0.75),(qint32)(rf.y()+rf.height()),
                                 (qint32)(rf.x()+rf.width()*0.25),(qint32)(rf.y()+rf.height()),
                                 (qint32)rf.x(),(qint32)rf.y());
               path.addPolygon(polygon);
            }
            break;
        case 5:
            {
               polygon.setPoints(5,(qint32)(rf.x()+rf.width()*0.5),(qint32)rf.y(),
                                 (qint32)(rf.x()+rf.width()),(qint32)(rf.y()+rf.height()*0.5),
                                 (qint32)(rf.x()+rf.width()*0.5),(qint32)(rf.y()+rf.height()),
                                 (qint32)rf.x(),(qint32)(rf.y()+rf.height()*0.5),
                                 (qint32)(rf.x()+rf.width()*0.5),(qint32)rf.y());
               path.addPolygon(polygon);
            }
            break;
        case 6:
            {
                qreal len = rf.height() - rf.width() / 2;

                if (len > 0)
                {
                    QPointF ps[] = {QPointF(rf.x(), rf.y()),
                        QPointF(rf.x() + rf.width(), rf.y()),
                        QPointF(rf.x() + rf.width(), rf.y() + len),
                        QPointF(rf.x(), rf.y() + len)
                    };
                    path.moveTo(ps[0].x(),ps[0].y());
                    path.lineTo(ps[3].x(),ps[3].y());
                    path.lineTo(ps[2].x(),ps[2].y());
                    path.lineTo(ps[1].x(),ps[1].y());
                    QRectF r(rf.x(), rf.y() + rf.height() - rf.width(),  rf.width(), rf.width());
                    path.arcMoveTo(rf.x(), rf.y() + rf.height() - rf.width(),  rf.width(), rf.width(),180);
                    path.arcTo(r,180,180);
                }
                else
                {
                    QRectF r(rf.x(), rf.y() - rf.height(),  rf.width(), rf.height() * 2);
                    path.arcMoveTo(rf.x(), rf.y() - rf.height(),  rf.width(), rf.height() * 2,180);
                    path.arcTo(r,180,180);

                }
            }
            break;
        case 7:
            {
                qreal len = rf.height() - rf.width() / 2;

                if (len > 0)
                {
                    QPointF ps[] = {QPointF(rf.x(), rf.y() + rf.width() / 2),
                        QPointF(rf.x() + rf.width(), rf.y() + rf.width() / 2),
                        QPointF(rf.x() + rf.width(), rf.y() + rf.height()),
                        QPointF(rf.x(), rf.y() + rf.height())
                    };
                    path.moveTo(ps[0].x(),ps[0].y());
                    path.lineTo(ps[3].x(),ps[3].y());
                    path.lineTo(ps[2].x(),ps[2].y());
                    path.lineTo(ps[1].x(),ps[1].y());
                    QRectF r(rf.x(), rf.y(),  rf.width(), rf.width());
                    path.arcMoveTo(rf.x(), rf.y(),  rf.width(), rf.width(),0);
                    path.arcTo(r,0,180);
                }
                else
                {
                    QRectF r(rf.x(), rf.y(),  rf.width(), rf.height() * 2);
                    path.arcMoveTo(r,0);
                    path.arcTo(r,0,180);
                }

            }
            break;
        case 8:
            {
                qreal len = rf.height() - rf.width();

                if (len > 0)
                {
                    QPointF ps[] = {QPointF(rf.x(), rf.y() + rf.width() / 2),
                        QPointF(rf.x()+ rf.width(), rf.y() + rf.width() / 2),
                        QPointF(rf.x() + rf.width(), rf.y() + rf.width() / 2 + len),
                        QPointF(rf.x(), rf.y()+ rf.width() / 2 + len)
                    };
                    path.moveTo(ps[1].x(), ps[1].y());
                    path.lineTo(ps[2].x(), ps[2].y());

                    QRectF r(rf.x(), rf.y() + len,  rf.width(), rf.width());
                    path.arcMoveTo(r,180);
                    path.arcTo(r,180,180);

                    QRectF r1(rf.x(), rf.y(),  rf.width(), rf.width());
                    path.arcTo(r1,0,180);

                    path.moveTo(ps[3].x(), ps[3].y());
                    path.lineTo(ps[0].x(), ps[0].y());
                }

                else
                {
                   QRectF r(rf.x(), rf.y(),  rf.width(), rf.height());
                   path.addEllipse(r);
                }
            }
            break;
        default:
            break;
        }
}
////////////////////////////////////////////////////////////////////////////
void CDrawItem::setLayer(CDataLayer* layer)
{
    m_layer = layer;
}
void CDrawItem::setRectAct(const QRectF& rAct)
{
    m_rAct = rAct;
    m_shortLen = (m_rAct.width()< m_rAct.height()) ? m_rAct.width() : m_rAct.height();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘制仪表某一组件
*/
void CDrawItem::draw(QPainter* painter, const QRectF& rect, CData* data, QRegion* moveRgn, QTransform* m)
{
    m_g = painter;
    m_r = rect;
    m_d = data;

    switch(data->getDrawType())
    {
    case dtCap:
        drawCap();
        break;
    case dtNeedle:
        drawNeedle(moveRgn, m);
        break;
    case dtRange:
        drawRange();
        break;
    case dtScaleText:
        drawScaleText();
        break;
    case dtScale:
        drawScale();
        break;
    case dtScaleBack:
        drawScaleBack();
        break;
    case dtBack:
        drawBack();
        break;
    case dtHalfBack:
        drawHalfBack();
        break;
    case dtLabel:
        drawLabel();
        break;
    case dtLed:
        drawLed();
        break;
    case dtTemperature:
        drawTemperature(moveRgn, m);
        break;
    case dtRNeedle:
        drawRNeedle(moveRgn, m);
        break;
    case dtRMarker:
        drawRMarker(moveRgn, m);
        break;
    case dtRRange:
        drawRRange();
        break;
    case dtRScaleText:
        drawRScaleText();
        break;
    case dtRScale:
        drawRScale();
        break;
    case dtRScaleBack:
        drawRScaleBack();
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawCap()
{
    CDataCap* cap = (CDataCap*)m_d;

    qreal shift = m_r.width() * cap->getRadius() / 50;
    QRectF rf(-(shift / 2), -(shift / 2), shift, shift);


    QPainterPath path;
    path.addEllipse(rf);
    QPen* pen = createPen(cap->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        m_g->drawEllipse(rf);
        delete pen;
    }

    QBrush* brush = createBrush(cap->m_backColor, rf);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        m_g->fillPath(path,*brush);
        delete brush;

        if (cap->m_style==1 && cap->m_backColor.m_gradientStyle!=gsSolid && cap->m_backColor.m_gradientStyle!=gsNull)
        {
            qint32 a1 = cap->m_backColor.m_color1.alpha();
            qint32 r1 = cap->m_backColor.m_color1.red();
            qint32 g1 = cap->m_backColor.m_color1.green();
            qint32 b1 = cap->m_backColor.m_color1.blue();

            qint32 a2 = cap->m_backColor.m_color2.alpha();
            qint32 r2 = cap->m_backColor.m_color2.red();
            qint32 g2 = cap->m_backColor.m_color2.green();
            qint32 b2 = cap->m_backColor.m_color2.blue();

            qint32 a = (qint32)(a1 + a2) / 2;
            qint32 r = (qint32)(((r1 * a1 / 255.0) + (r2 * a2 / 255.0)) / 2);
            qint32 g = (qint32)(((g1 * a1 / 255.0) + (g2 * a2 / 255.0)) / 2);
            qint32 b = (qint32)(((b1 * a1 / 255.0) + (b2 * a2 / 255.0)) / 2);
            QColor color = QColor(r,g,b,a);

            m_g->setBrush(color);
            m_g->setPen(Qt::NoPen);
            qreal shift1 = shift / 2;
            QRectF rf1(-(shift1 / 2), -(shift1 / 2), shift1, shift1);
            m_g->drawEllipse(rf1);
        }
    }
    m_g->setBrush(Qt::NoBrush);
    m_g->setPen(Qt::NoPen);
}
///////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawNeedle(QRegion* moveRgn, QTransform* m)
{
    CDataNeedle* needle = (CDataNeedle*)m_d;

    //!  指针动画效果
    double value = 0;
    qint32 step = needle->m_currentStep;
    if (step > 0)
    {
        double rate = 1.0f - step * step / (double)((step + 1) * (step + 1)) ;
        value = needle->m_currentValue + (needle->m_value - needle->m_currentValue) * rate;
    }
    else
        value = needle->m_value;

    //!  值限制
    double min = m_layer->getMinValue();
    double max = min + m_layer->getValueRange();
    value = (value >= min) ? value : min;
    value = (value <= max) ? value : max ;

    //!  计算角度
    qreal angle = qreal((value - min) / m_layer->getValueRange());
    angle *= m_layer->getAngleRange();
    angle += m_layer->getStartAngle();

    qreal width = m_r.width() * needle->getWidth() / 100;
    qreal len = m_r.width() * needle->getLen() / 100;
    qreal radius = m_r.width() * needle->getRadius() / 100;

    QRectF rf(-width / 2, radius - len, width, len);

    QPainterPath path;

    //!  计算绘图区域
    if (moveRgn != NULL)
    {
        QRectF rgnR = rf;
        qreal x,y,w,h;
        x = rgnR.x() - 1;
        y = rgnR.y() - 1;
        w = rgnR.width() + 2;
        h = rgnR.height() + 2;
        QRectF rect(x,y,w,h);
        rgnR = rect;
        QRegion rgn(rgnR.x(),rgnR.y(),rgnR.width(),rgnR.height());
        m->rotate(angle);
        moveRgn->united(rgn);
        return;
    }

    needle->m_currentValue = value;
    m_g->rotate(angle);

    QBrush* brush = createBrush(needle->m_backColor, rf, &path);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        delete brush;
    }

    QPen* pen = createPen(needle->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        delete pen;
    }

    switch(needle->m_style)
    {
    case 0:
        {
            QPointF ps[] = {QPointF(rf.x(), rf.y()),
            QPointF(rf.x() + rf.width(), rf.y()),
            QPointF(rf.x() + rf.width() / 2, rf.y() + rf.height())
        };

            m_g->drawPolygon(ps,3);

            break;
         }
    case 1:
       {
            path.addRect(rf);
            m_g->drawPath(path);
            break;
        }
    case 2:
        {
                QPointF ps[] = {QPointF(rf.x(), rf.y()),
                QPointF(rf.x() + rf.width(), rf.y()),
                QPointF(qreal(rf.x() + rf.width() * 0.8), qreal(rf.y() + rf.height() * 0.8)),
                QPointF(qreal(rf.x() + rf.width() / 2), rf.y() + rf.height()),
                QPointF(qreal(rf.x() + rf.width() * 0.2), qreal(rf.y() + rf.height() * 0.8))
            };
            m_g->drawPolygon(ps,5);
            break;
        }
    case 3:
        {
            QPointF ps[] = {QPointF(rf.x(), rf.y()),
                QPointF(rf.x() + rf.width(), rf.y()),
                QPointF(qreal(rf.x() + rf.width() * 0.75), rf.y() + rf.height()),
                QPointF(qreal(rf.x() + rf.width() * 0.25), rf.y() + rf.height())
            };
        m_g->drawPolygon(ps,4);
        break;
        }
    case 4:
        {
            qreal len = rf.height() - rf.width() / 2;

            if (len > 0)
            {
                QPointF ps[] = {QPointF(rf.x(), rf.y()),
                    QPointF(rf.x() + rf.width(), rf.y()),
                    QPointF(rf.x() + rf.width(), rf.y() + len),
                    QPointF(rf.x(), rf.y() + len)
                };
                m_g->drawPolygon(ps,4);
                path.moveTo(ps[0].x(), ps[0].y());
                QRectF r(rf.x(), rf.y() + rf.height() - rf.width(),  rf.width(), rf.width());
                path.arcTo(r, 180, 180);
                m_g->drawPath(path);

            }
            else
            {
                QRectF r(rf.x(), rf.y() - rf.height(),  rf.width(), rf.height() * 2);
                path.arcTo(r, 180, 180);
                path.closeSubpath();
                m_g->drawPath(path);
            }

            break;
        }
    default:
        break;

    }
    m_g->setBrush(Qt::NoBrush);
    m_g->setPen(Qt::NoPen);
    m_g->rotate(-angle);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawRange()
{
    CDataRange* range = (CDataRange*)m_d;

    //!  取值范围限制
    double layerMin = m_layer->getMinValue();
    double layerMax = layerMin + m_layer->getValueRange();
    double min = range->getMinValue();
    min = (min >= layerMin) ? min : layerMin;

    if (min >= layerMax)
        return;
    double max = min + range->getValueRange();
    max = (max <= layerMax) ? max : layerMax;

    qreal radius = m_r.width() * range->getRadius() / 100;
    qreal width = m_r.width() * range->getWidth() / 100;

    //!  获取角度
    qreal stAngle = qreal((min - layerMin) / m_layer->getValueRange() * m_layer->getAngleRange());
    stAngle += m_layer->getStartAngle();

    qreal sweepAngle = qreal((max - min) / m_layer->getValueRange() * m_layer->getAngleRange());

    qreal offset1 = width / 2 * (range->m_arrangement -1);
    QRectF r(-radius - offset1, -radius - offset1, (radius + offset1) *2, (radius + offset1) *2);

    QPainterPath path;
    qreal offset2 = width /2;
    QRectF r1(r.x() - offset2, r.y() - offset2, r.width() + offset2 * 2, r.height() + offset2 * 2);
    QRectF r2(r.x() + offset2, r.y() + offset2, r.width() - offset2 * 2, r.height() - offset2 * 2);

    path.arcMoveTo(r1, -(stAngle + 90));
    path.arcTo(r1, -(stAngle + 90), -sweepAngle);
    path.arcTo(r2, -(stAngle + 90 + sweepAngle), sweepAngle);
    path.closeSubpath();

    QRectF rectf;
    rectf = path.boundingRect();

    QPen* pen = createPen(range->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        delete pen;
    }

    QBrush* brush = createBrush(range->m_backColor, rectf, &path);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        delete brush;
    }

    m_g->drawPath(path);
    m_g->setBrush(Qt::NoBrush);
    m_g->setPen(Qt::NoPen);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawScale()
{
    CDataScale* scale = (CDataScale*)m_d;

    qreal width = m_r.width() * scale->getWidth() / 100;
    qreal len = m_r.width() * scale->getLen() / 100;
    qreal radius = m_r.width() * scale->getRadius() / 100;
    qint32 count = m_layer->getScaleCount();
    qreal interval = m_layer->getAngleRange() / (count - 1.0f);
    qint32 subCount = scale->getSubCount();
    qreal subInterval = interval / (subCount + 1);

    QRectF rf1,rf2;
    QRectF rf(-width / 2, radius - len, width, len);
    switch(scale->m_arrangement)
    {
    case 0:
        break;
    case 1:
        rf1 = QRectF(rf.x(),radius - len / 2,rf.width(),rf.height());
        rf = rf1;
        break;
    case 2:
        rf2 = QRectF(rf.x(),radius,rf.width(),rf.height());
        rf = rf2;
        break;
    default:
        break;
    }

    QPainterPath path;
    switch(scale->m_style)
    {
    case 0:
        {

         QPolygon polygon;
         polygon.setPoints(5,(qint32)(rf.x() + rf.width()*0.25), (qint32)rf.y(),(qint32)(rf.x() + rf.width()*0.75), (qint32)rf.y(),
                           (qint32)(rf.x()+ rf.width()),(qint32)(rf.y() + rf.height()),
                           (qint32)rf.x(),(qint32)(rf.y() + rf.height()),(qint32)(rf.x() + rf.width()*0.25), (qint32)rf.y());
         path.addPolygon(polygon);

        }
        break;
    case 1:
        {
            QPolygon polygon;
            polygon.setPoints(4,(qint32)rf.x(),(qint32)rf.y(),(qint32)(rf.x()+rf.width()),(qint32)rf.y(),
                              (qint32)(rf.x()+rf.width()*0.5),(qint32)(rf.y()+rf.height()),(qint32)rf.x(),(qint32)rf.y());
            path.addPolygon(polygon);

        }
        break;
    case 2:
        path.addRect(rf);
        break;
    case 3:
        {

            QPolygon polygon;
            polygon.setPoints(4,(qint32)(rf.x()+rf.width()*0.5),(qint32)rf.y(),(qint32)(rf.x()+rf.width()),(qint32)(rf.y()+rf.height()),
                          (qint32)rf.x(),(qint32)(rf.y()+rf.height()),(qint32)(rf.x()+rf.width()*0.5),(qint32)rf.y());
            path.addPolygon(polygon);

        }
        break;
    case 4:
    {
        qreal len = rf.height() - rf.width() / 2;

        if (len > 0)
        {
            QPointF ps[] = {QPointF(rf.x(), rf.y() + rf.width() / 2),
                QPointF(rf.x() + rf.width(), rf.y() + rf.width() / 2),
                QPointF(rf.x() + rf.width(), rf.y() + rf.height()),
                QPointF(rf.x(), rf.y() + rf.height())
            };
            path.moveTo(ps[0].x(),ps[0].y());
            path.lineTo(ps[3].x(),ps[3].y());
            path.lineTo(ps[2].x(),ps[2].y());
            path.lineTo(ps[1].x(),ps[1].y());
            QRectF r(rf.x(), rf.y(),  rf.width(), rf.width());
            path.arcMoveTo(rf.x(), rf.y(),  rf.width(), rf.width(),0);
            path.arcTo(r, 0, 180);

        }
        else
        {
            QRectF r(rf.x(), rf.y(),  rf.width(), rf.height() * 2);
            path.arcMoveTo(rf.x(), rf.y(),  rf.width(), rf.height() * 2,0);
            path.arcTo(r, 0, 180);

        }
    }
        break;
    case 5:
    {
        qreal len = rf.height() - rf.width() / 2;

        if (len > 0)
        {
            QPointF ps[] = {QPointF(rf.x(), rf.y()),
                QPointF(rf.x() + rf.width(), rf.y()),
                QPointF(rf.x() + rf.width(), rf.y() + len),
                QPointF(rf.x(), rf.y() + len)
            };
            path.moveTo(ps[2].x(),ps[2].y());
            path.lineTo(ps[1].x(),ps[1].y());
            path.lineTo(ps[0].x(),ps[0].y());
            path.lineTo(ps[3].x(),ps[3].y());

            QRectF r(rf.x(), rf.y() + rf.height() - rf.width(),  rf.width(), rf.width());
            path.arcMoveTo(rf.x(), rf.y() + rf.height() - rf.width(),  rf.width(), rf.width(),180);
            path.arcTo(r,180,180);
        }
        else
        {
            QRectF r(rf.x(), rf.y() - rf.height(),  rf.width(), rf.height() * 2);
            path.arcMoveTo(r,180);
            path.arcTo(r,180,180);

        }
    }
        break;
    case 6:
        {
            qreal len = rf.height() - rf.width();

            if (len > 0)
            {
                QPointF ps[] = {QPointF(rf.x(), rf.y() + rf.width() / 2),
                    QPointF(rf.x() + rf.width(), rf.y() + rf.width() / 2),
                    QPointF(rf.x() + rf.width(), rf.y() + rf.width() / 2 + len),
                    QPointF(rf.x(), rf.y() + rf.width() / 2 + len)
                };

                path.moveTo(ps[1].x(), ps[1].y());
                path.lineTo(ps[2].x(), ps[2].y());

                QRectF r(rf.x(), rf.y() + len,  rf.width(), rf.width());
                path.arcMoveTo(r,180);
                path.arcTo(r,180,180);

                QRectF r1(rf.x(), rf.y(),  rf.width(), rf.width());
                path.arcTo(r1,0,180);

                path.moveTo(ps[3].x(), ps[3].y());
                path.lineTo(ps[0].x(), ps[0].y());
            }
            else
            {
                QRectF r(rf.x(), rf.y(),  rf.width(), rf.height());
                path.addEllipse(r);

            }
        }

        break;
    case 7:
        {
            QPolygon polygon;
            polygon.setPoints(5,(qint32)(rf.x() + rf.width() * 0.5),(qint32)rf.y(),(qint32)(rf.x() + rf.width()),
                             (qint32)(rf.y() + rf.height() * 0.5), (qint32)(rf.x() + rf.width() * 0.5),
                              (qint32)(rf.y() + rf.height()),(qint32)rf.x(),(qint32)(rf.y() + rf.height() * 0.5),
                              (qint32)(rf.x() + rf.width() * 0.5),(qint32)rf.y());
            path.addPolygon(polygon);
        }
        break;
    default:
        break;
    }

    m_g->rotate(qreal(m_layer->getStartAngle()));
    QBrush* brush = createBrush(scale->m_backColor, rf, &path);
    if (brush != NULL)
    {
        for (qint32 i = 0; i < count; i++)
        {
            if (scale->m_isSubScale)
            {
                if (i == count - 1)
                    break;

                m_g->rotate(subInterval);

                for (qint32 j = 0; j < subCount; j++)
                {

                    m_g->setBrush(*brush);
                    m_g->rotate(subInterval);

                }
            }
            else
            {
                m_g->setBrush(*brush);

                m_g->rotate(interval);
            }
        }

        if (scale->m_isSubScale)
            m_g->rotate(qreal(-m_layer->getAngleRange()));
        else
            m_g->rotate(qreal(-m_layer->getAngleRange() - interval));
    }

    QPen* pen = createPen(scale->m_edgeLine);
    if (pen != NULL)
    {
        for (qint32 i = 0; i < count; i++)
        {
            if (scale->m_isSubScale)
            {
                if (i == count - 1)
                    break;

                m_g->rotate(subInterval);

                for (qint32 j = 0; j < subCount; j++)
                {
                    m_g->setPen(*pen);
                    m_g->fillPath(path,*brush);
                    m_g->drawPath(path);

                    m_g->rotate(subInterval);
                }
            }
            else
            {
                m_g->setPen(*pen);
                m_g->fillPath(path,*brush);
                m_g->drawPath(path);

                m_g->rotate(interval);
            }
        }

        if (scale->m_isSubScale)
            m_g->rotate(qreal(-m_layer->getAngleRange()));
        else
            m_g->rotate(qreal(-m_layer->getAngleRange() - interval));

        delete pen;
    }

    m_g->rotate(qreal(-m_layer->getStartAngle()));
    m_g->setBrush(Qt::NoBrush);
    m_g->setPen(Qt::NoPen);

}
////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawScaleBack()
{
    CDataScaleBack* sBack = (CDataScaleBack*)m_d;

    qreal stAngle = qreal(m_layer->getStartAngle());
    qreal sweepAngle = qreal(m_layer->getAngleRange());
    qreal width = m_r.width() * sBack->getWidth() / 100;
    qreal radius = m_r.width() * sBack->getRadius() / 100;

    qreal offset1 = width / 2 * (sBack->m_arrangement - 1);
    QRectF r(-radius - offset1, -radius - offset1, (radius + offset1) * 2, (radius + offset1) * 2);
    QPainterPath path;
    qreal offset2 = width / 2;
    QRectF r1(r.x() - offset2, r.y() - offset2, r.width() + offset2 * 2, r.height() + offset2 * 2);
    path.arcMoveTo(r1, -(stAngle + 90));
    path.arcTo(r1, -(stAngle + 90), -sweepAngle);
    QRectF r2(r.x() + offset2, r.y() + offset2, r.width() - offset2 * 2, r.height() - offset2 * 2);
    path.arcTo(r2, -(stAngle + 90 + sweepAngle), sweepAngle);
    path.closeSubpath();

    QBrush* brush = createBrush(sBack->m_backColor, r, &path);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        delete brush;
    }

    QPen* pen = createPen(sBack->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        m_g->drawPath(path);
        delete pen;
    }
    m_g->setBrush(Qt::NoBrush);
    m_g->setPen(Qt::NoPen);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawScaleText()
{
    CDataScaleText* text = (CDataScaleText*)m_d;

    qreal size = qreal(m_r.width() * text->getSize() / 100);
    qreal radius = m_r.width() * text->getRadius() / 100;
    QRectF rect(m_r.width()/2-radius,m_r.height()/2-radius,2*radius,2*radius);

    qint32 count = m_layer->getScaleCount();
    qreal startAngle = qreal(m_layer->getStartAngle());
    qreal intervalAngle = m_layer->getAngleRange() / (count - 1.0f);
    double minValue = m_layer->getMinValue();
    double intervalValue = m_layer->getValueRange() / (count - 1.0f);
    qint32 decimal = text->getDecimals();

    QString s;
    m_g->setPen(QPen(QColor(text->m_color)));
    QFont font;
    if( text->m_isBold)
    {
        font.setBold(true);
    }

    font.setPointSizeF(size);
    font.setFamily(text->m_name);
    m_g->setFont(font);

    for(qint32 i = 0; i < count; i++)
    {
        qreal value = qreal(minValue + intervalValue * i);

        //!  值为0时，不显示-0.00
        qreal v = value;
        for (qint32 j = 0; j < decimal; j++)
            v *= 10;

        if (qint32(v) == 0)
            s = "0";

        qreal angle = startAngle + intervalAngle * i;
        angle = getRadian(angle);

        QSizeF offset = m_g->boundingRect(rect,QString::number(value,'d',decimal)).size();
        qreal r = radius;

        switch(text->m_style / 4)
        {
        case 0:
        {
            qreal w,h;
            w =  offset.width();
            h =  offset.height();
            w = -w / 2;
            h = -h / 2;
            QSizeF size(w,h);
            offset = size;
            break;
        }
        case 1:
        {
            qreal w,h;
            w =  offset.width();
            h =  offset.height();
            w = w / 4;
            h = h / 4;
            QSizeF size(w,h);
            offset = size;
            break;
        }
        case 2:
        {
            qreal w,h;
            w =  offset.width();
            h =  offset.height();
            w = w / 2;
            h = h / 2;
            QSizeF size(w,h);
            offset = size;
            break;
        }
        default:
            break;
        }

        switch(text->m_style % 4)
        {
        case 0:
            {
              QPointF pf(-qSin(angle)*(radius+offset.width()), qCos(angle)*(radius+offset.height()));
              if(text->m_style == 0)
              {
                  pf.setX(pf.x() + offset.width());
                  pf.setY(pf.y() - offset.height()+offset.height()/3);
              }
              if(text->m_style == 4)
              {
                  pf.setX(pf.x() - offset.width()*2);
                  pf.setY(pf.y() + offset.height());
              }
              if(text->m_style == 8)
              {
                  pf.setX(pf.x() - offset.width());
                  pf.setY(pf.y() + offset.height()/2);
              }


              m_g->drawText(pf,QString::number(value,'d',decimal));
        }

            break;
        case 1:
            {
                if (i == 0)
                {
                    m_g->rotate(startAngle + 180);
                }

                r += offset.height();
                QPointF pf(0,r);
                if(text->m_style == 1)
                {
                    pf.setX(pf.x() + offset.width());
                    pf.setY(pf.y() - offset.height()+offset.height()/3);
                }
                if(text->m_style == 5)
                {
                    pf.setX(pf.x() - offset.width()*2);
                    pf.setY(pf.y() + offset.height());
                }
                if(text->m_style == 9)
                {
                    pf.setX(pf.x() - offset.width());
                    pf.setY(pf.y() + offset.height()/2);
                }

                qreal actAngle = startAngle + intervalAngle * i;
                actAngle = (actAngle < 360) ? actAngle : actAngle - 360;
                if (actAngle < 90 || actAngle >= 270)
                {
                    m_g->rotate(-180);
                    m_g->drawText(pf,QString::number(value,'d',decimal));
                    m_g->rotate(180);
                }
                else
                   {
                    QPointF pf(0,-r);
                    if(text->m_style == 1)
                    {
                        pf.setX(pf.x() + offset.width());
                        pf.setY(pf.y() - offset.height()+offset.height()/3);
                    }
                    if(text->m_style == 5)
                    {
                        pf.setX(pf.x() - offset.width()*2);
                        pf.setY(pf.y() + offset.height());
                    }
                    if(text->m_style == 9)
                    {
                        pf.setX(pf.x() - offset.width());
                        pf.setY(pf.y() + offset.height()/2);
                    }
                    m_g->drawText(pf,QString::number(value,'d',decimal));
                    }

                 if (i < (count - 1))
                    m_g->rotate(intervalAngle);
                 else
                    m_g->rotate(-startAngle - m_layer->getAngleRange() - 180);
            }
            break;
        case 2:
            {
                if (i == 0)
                    m_g->rotate(startAngle + 180);

                r += offset.height();
                QPointF pf(0,-r);
                if(text->m_style == 2)
                {
                    pf.setX(pf.x() + offset.width());
                    pf.setY(pf.y() - offset.height());
                }
                if(text->m_style == 6)
                {
                    pf.setX(pf.x() - offset.width()*2);
                    pf.setY(pf.y() + offset.height());
                }

                if(text->m_style == 10)
                {
                    pf.setX(pf.x() - offset.width());
                    pf.setY(pf.y() + offset.height()/2);
                }

                m_g->drawText(pf,QString::number(value,'d',decimal));

                if (i < (count - 1))
                    m_g->rotate(intervalAngle);
                else
                    m_g->rotate(-startAngle - m_layer->getAngleRange() - 180);
            }
            break;
        case 3:
            {
                if (i == 0)
                    m_g->rotate(startAngle);

                r += offset.height();
                QPointF pf(0,r);
                if(text->m_style == 3)
                {
                    pf.setX(pf.x() + offset.width());
                    pf.setY(pf.y() - offset.height());
                }
                if(text->m_style == 7)
                {
                    pf.setX(pf.x() - offset.width()*2);
                    pf.setY(pf.y() + offset.height());
                }
                if(text->m_style == 11)
                {
                    pf.setX(pf.x() - offset.width());
                    pf.setY(pf.y() + offset.height()/2);
                }

                m_g->drawText(pf,QString::number(value,'d',decimal));

                if (i < (count - 1))
                    m_g->rotate(intervalAngle);
                else
                    m_g->rotate(-startAngle - m_layer->getAngleRange());
            }
            break;
        default:
            break;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawBack()
{
    CDataBack* back = (CDataBack*)m_d;

    bool isSquare = (back->m_style < 3);
    qreal rate = 0.15f;
    qreal width = m_r.width() * back->getEdgeWidth() / 100;
    QRectF rf(-m_r.width() / 2, -m_r.height() / 2, m_r.width(), m_r.height());

    if (!isSquare)
    {
        width = qreal(m_shortLen * back->getEdgeWidth() / 100);
        rf = QRectF(-m_rAct.width() / 2, -m_rAct.height() / 2, m_rAct.width(), m_rAct.height());
    }

    QPainterPath pathBack;
    QPainterPath pathBack1;
    QRectF r = rf;
    if (back->m_style % 3 != 0)
    {
        qreal x,y,w,h;
        x = r.x()+width;
        y = r.y()+width;
        w = r.width()-2*width;
        h = r.height()-2*width;
        QRectF rect(x,y,w,h);
        r = rect;

    }
    QRectF rAct = r;
    qreal x1,y1,wt1,h1;
    x1 = rAct.x()+1;
    y1 = rAct.y()+1;
    wt1 = rAct.width()-2;
    h1 = rAct.height()-2;
    QRectF rect1(x1,y1,wt1,h1);
    rAct = rect1;

    switch(back->m_style) {
    case 0:
    case 1:
    case 2:
        pathBack.addEllipse(r);
        pathBack1.addEllipse(rAct);
        break;
    case 3:
    case 4:
    case 5:
        pathBack.addRect(r);
        pathBack1.addRect(rAct);
        break;
    case 6:
    case 7:
    case 8:
        {
            qreal w = rate * r.width();
            if (!isSquare)
            {
                qreal len = (r.width() < r.height()) ? r.height() : r.height();
                w = rate * len;
            }

            pathBack.arcMoveTo(r.x() + r.width() - w, r.y(), w, w, 0);
            pathBack.arcTo(r.x() + r.width() - w, r.y(), w, w, 0, 90);
            pathBack.arcTo(r.x(), r.y(), w, w, 90, 90);
            pathBack.arcTo(r.x(), r.y() + r.height() - w, w, w, 180, 90);
            pathBack.arcTo(r.x() + r.width() - w, r.y() + r.height() - w, w, w, 270, 90);
            pathBack.closeSubpath();

            pathBack1.arcMoveTo(rAct.x() + rAct.width() - w, rAct.y(), w, w, 0);
            pathBack1.arcTo(rAct.x() + rAct.width() - w, rAct.y(), w, w, 0, 90);
            pathBack1.arcTo(rAct.x(), rAct.y(), w, w, 90, 90);
            pathBack1.arcTo(rAct.x(), rAct.y() + rAct.height() - w, w, w, 180, 90);
            pathBack1.arcTo(rAct.x() + rAct.width() - w, rAct.y() + rAct.height() - w, w, w, 270, 90);
            pathBack1.closeSubpath();
        }
        break;
    default:
        break;
    }

    QPainterPath pathEdge,pathEdgeBackup, pathEdge1, pathEdgeBackup1;
    if (back->m_style % 3 == 1)
        pathEdge.addPath(pathBack1);
    else if (back->m_style % 3 == 2)
        pathEdge.addPath(pathBack);
    switch(back->m_style)
    {
    case 1:
    case 2:
        pathEdgeBackup.addEllipse(rf);
        break;
    case 4:
    case 5:
        pathEdgeBackup.addRect(rf);
        break;
    case 7:
    case 8:
        {
            qreal w = rate * rf.width();

            if (!isSquare)
            {
                qreal len = (rf.width() < rf.height()) ? rf.width() : rf.height();
                w = rate * len;
            }

            pathEdgeBackup.arcMoveTo(rf.x() + rf.width() - w, rf.y(), w, w, 0);
            pathEdgeBackup.arcTo(rf.x() + rf.width() - w, rf.y(), w, w, 0, 90);
            pathEdgeBackup.arcTo(rf.x(), rf.y(), w, w, 90, 90);
            pathEdgeBackup.arcTo(rf.x(), rf.y() + rf.height() - w, w, w, 180, 90);
            pathEdgeBackup.arcTo(rf.x() + rf.width() - w, rf.y() + rf.height() - w, w, w, 270, 90);
            pathEdgeBackup.closeSubpath();
        }
        break;
    default:
        break;
    }
    pathEdge.addPath(pathEdgeBackup);

    //!  style edgeback1
    pathEdge1.addPath(pathBack1);
    QRectF r1 = r;
    qreal w1 = qreal(width * 0.33);
    qreal x2,y2,wt2,h2;
    x2 = r1.x()-w1;
    y2 = r1.y()-w1;
    wt2 = r1.width()+2*w1;
    h2 = r1.height()+2*w1;
    QRectF rect2(x2,y2,wt2,h2);
    r1 = rect2;

    switch(back->m_style)
    {
    case 2:
        pathEdgeBackup1.addEllipse(r1);
        break;
    case 5:
        pathEdgeBackup1.addRect(r1);
        break;
    case 8:
        {
            qreal w = qreal(0.15 * r1.width());

            if (!isSquare)
            {
                qreal len = (r1.width() < r1.height()) ? r1.width() : r1.height();
                w = qreal(0.15 * len);
            }

            pathEdgeBackup1.arcMoveTo(r1.x() + r1.width() - w, r1.y(), w, w, 0);
            pathEdgeBackup1.arcTo(r1.x() + r1.width() - w, r1.y(), w, w, 0, 90);
            pathEdgeBackup1.arcTo(r1.x(), r1.y(), w, w, 90, 90);
            pathEdgeBackup1.arcTo(r1.x(), r1.y() + r1.height() - w, w, w, 180, 90);
            pathEdgeBackup1.arcTo(r1.x() + r1.width() - w, r1.y() + r1.height() - w, w, w, 270, 90);

            pathEdgeBackup1.closeSubpath();
        }
        break;
    default:
        break;
    }
    pathEdge1.addPath(pathEdgeBackup1);

    //!  style edgeBackLine
    QPainterPath pathEdgeLine;//
    switch(back->m_style / 3)
    {
    case 0:
        pathEdgeLine.addEllipse(rf);
        break;
    case 1:
        pathEdgeLine.addRect(rf);
        break;
    case 2:
        {
            qreal w = rate * rf.width();

            if (!isSquare)
            {
                qreal len = (rf.width() < rf.height()) ? rf.width() : rf.height();
                w = rate * len;
            }
            pathEdgeLine.arcMoveTo(rf.x() + rf.width() - w, rf.y(), w, w, 0);
            pathEdgeLine.arcTo(rf.x() + rf.width() - w, rf.y(), w, w, 0, 90);
            pathEdgeLine.arcTo(rf.x(), rf.y(), w, w, 90, 90);
            pathEdgeLine.arcTo(rf.x(), rf.y() + rf.height() - w, w, w, 180, 90);
            pathEdgeLine.arcTo(rf.x() + rf.width() - w, rf.y() + rf.height() - w, w, w, 270, 90);
            pathEdgeLine.closeSubpath();
        }
        break;
    default:
        break;
    }
    m_g->setPen(Qt::NoPen);

    QBrush* brushBack = createBrush(back->m_backColor, rf, &pathBack);
    if (brushBack != NULL)
     {
         m_g->setBrush(*brushBack);
         m_g->drawPath(pathBack);
         delete brushBack;
     }

    if (back->m_style % 3 != 0)
     {
        QBrush* brushEdge = createBrush(back->m_edgeColor, rf, &pathEdgeBackup);
        if (brushEdge != NULL)
          {
             m_g->setBrush(*brushEdge);
             m_g->drawPath(pathEdge);
             delete brushEdge;
          }

// !  edgeColor1
     if (back->m_style % 3 == 2)
      {
         CBackColor bColor = back->m_edgeColor;
         QColor c = bColor.m_color1;
         bColor.m_color1 = bColor.m_color2;
         bColor.m_color2 = c;

         QBrush* brushEdge1 = createBrush(bColor, r1, &pathEdge1);
         if (brushEdge1 != NULL)
         {
            m_g->setBrush(*brushEdge1);
            m_g->drawPath(pathEdge1);
            delete brushEdge1;
         }
       }
     }

    QPen* pen = createPen(back->m_edgeLine);
     if (pen != NULL)
       {
           m_g->setPen(*pen);
           m_g->setBrush(Qt::NoBrush);
           m_g->drawPath(pathEdgeLine);
           delete pen;
       }

    m_g->setBrush(Qt::NoBrush);
    m_g->setPen(Qt::NoPen);

}
/////////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawHalfBack()
{
    CDataHalfBack* half = (CDataHalfBack*)m_d;

    qreal rate = 0.12f;
    qreal width = m_r.width() * half->getEdgeWidth() / 100;
    QRectF rf(-m_r.width() / 2, -m_r.height() / 2, m_r.width(), m_r.height());

    QPainterPath pathBack;
    QRectF rBack1(qreal(rf.width() * (0.5 - rate * 2)), qreal(-rf.width() * rate), qreal(rf.width() * rate * 2), qreal(rf.width() * rate * 2));
    QRectF rBack2(rf.x(), rBack1.y(), rBack1.width(), rBack1.height());
    pathBack.arcMoveTo(rf, 180);
    pathBack.arcTo(rf, 180, 180);
    pathBack.arcTo(rBack1, 0, 90);
    pathBack.arcTo(rBack2, 90, 90);


    QPainterPath pathEdge, pathInEdge;
    QRectF r = rf;
    qreal x,y,w,h;
    x = r.x()+width;
    y = r.y()+width;
    w = r.width() - width*2;
    h = r.height() - width*2;
    QRectF rect(x,y,w,h);
    r = rect;
    qreal radius = rf.width() * rate - width;
    QRectF rEdge1(qreal(r.width() * 0.5 - radius * 2), -radius, radius * 2, radius * 2);
    QRectF rEdge2(r.x(), rEdge1.y(), rEdge1.width(), rEdge1.height());
    if (half->m_style == 1 || half->m_style == 2)
    {
        pathEdge.addPath(pathBack);
        pathEdge.arcMoveTo(r, 180);
        pathEdge.arcTo(r, 180, 180);
        if (radius > 0)
        {
            pathEdge.arcTo(rEdge1, 0, 90);
            pathEdge.arcTo(rEdge2, 90, 90);
        }

        pathInEdge.arcTo(r, 180, 180);
        if (radius > 0)
        {
            pathInEdge.arcTo(rEdge1, 0, 90);
            pathInEdge.arcTo(rEdge2, 90, 90);
        }
    }

    //!  style edgeback1
    QRectF r1 = r;
    qreal w1 = qreal(width * 0.33);
    qreal x1,y1,wt1,h1;
    x1 = r1.x() - w1;
    y1 = r1.y() - w1;
    wt1 = r1.width() + w1*2;
    h1 = r1.height() + w1*2;
    QRectF rect1(x1,y1,wt1,h1);
    r1 = rect1;
    qreal radius1 = rf.width() * rate - (width - w1);
    QRectF rInEdge1(qreal(r1.width() * 0.5 - radius1 * 2), -radius1, radius1 * 2, radius1 * 2);
    QRectF rInEdge2(r1.x(), rInEdge1.y(), rInEdge1.width(), rInEdge1.height());
    if (half->m_style == 2)
    {
        pathInEdge.arcTo(r1, 180, 180);

        if (radius1 > 0)
        {
            pathInEdge.arcTo(rInEdge1, 0, 90);
            pathInEdge.arcTo(rInEdge2, 90, 90);
        }
    }

    //!  arrangement
    qreal angle = (qreal)(half->m_arrangement *90 +180);

    //!  rotate
    m_g->rotate(angle);

    QRectF rb(rf.x(), rf.y(), rf.width(), qreal(rf.height() * (0.5 + rate)));
    m_g->setPen(Qt::NoPen);

    QBrush* brushBack = createBrush(half->m_backColor, rb, &pathBack);
    if (brushBack != NULL)
    {
        m_g->setBrush(*brushBack);
        m_g->drawPath(pathBack);
        delete brushBack;
    }
    //!  edgeColor
    if (half->m_style == 1 || half->m_style == 2)
    {
        QBrush* brushEdge = createBrush(half->m_edgeColor, rb, &pathEdge);
        if (brushEdge != NULL)
        {
            m_g->setBrush(*brushEdge);
            m_g->drawPath(pathEdge);
            delete brushEdge;
        }

        //!  edgeColor1
        if (half->m_style == 2)
        {
            CBackColor bColor = half->m_edgeColor;
            QColor c = bColor.m_color1;
            bColor.m_color1 = bColor.m_color2;
            bColor.m_color2 = c;
            QBrush* brushEdge1 = createBrush(bColor, r1, &pathInEdge);
            if (brushEdge1 != NULL)
            {
                m_g->setBrush(*brushEdge1);
                m_g->drawPath(pathInEdge);
                delete brushEdge1;
            }
        }
    }
    //!  edgeLine
    QPen* pen = createPen(half->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        m_g->setBrush(Qt::NoBrush);
        m_g->drawPath(pathBack);
        delete pen;
    }
    //!  restore rotate
    m_g->rotate(-angle);
    m_g->setBrush(Qt::NoBrush);
    m_g->setPen(Qt::NoPen);
}
//////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawLabel()
{
    CDataLabel* label = (CDataLabel*)m_d;

    qreal size = qreal(m_shortLen * label->getSize() / 100 * 0.8);

    m_g->setPen(QPen(QColor(label->m_color)));
    QFont font;
    if( label->m_isBold)
    {
        font.setBold(true);
    }

    font.setPointSizeF(size);
    font.setFamily(label->m_name);
    m_g->setFont(font);

    qreal x = (label->m_pos.x() - 50) / 100 * m_rAct.width();
    qreal y = (label->m_pos.y() - 50) / 100 * m_rAct.height();
    QPointF pf(x,y);

    m_g->drawText(pf,label->m_showText);

}
///////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawLed()
{
    CDataLed* led = (CDataLed*)m_d;
    double value = led->m_value;
    qreal size = qreal(m_shortLen * led->getSize() / 100);
    qint32 decimal = led->getDecimals();
    m_g->setPen(QPen(QColor(led->m_color)));
    if (m_nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(m_nIndex));
        if (strList.count() > 0)
        {
          QFont fontThis(strList.at(0));
          fontThis.setPointSizeF(size);
          m_g->setFont(fontThis);
        }
   }

    qreal x = (led->m_pos.x() - 50) / 100 * m_rAct.width();
    qreal y = (led->m_pos.y() - 50) / 100 * m_rAct.height();
    QPointF pf(x,y);
    m_g->drawText(pf,QString::number(value,'d',decimal));
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CDrawItem::drawTemperature(QRegion* moveRgn, QTransform* m)
{
    CDataTemperature* tem = (CDataTemperature*)m_d;

    qreal w1 = qreal(m_rAct.width() * tem->getWidth1() / 100);
    qreal w2 = qreal(m_rAct.width() * tem->getWidth2() / 100);
    w2 = (w1 > w2) ? w1 : w2;
    if (w2 == 0)
        return;
    qreal h1 = qreal(m_rAct.height() * tem->getHeight1() / 100);
    qreal h2 = qreal(m_rAct.height() * tem->getHeight2() / 100);
    qreal offset = qreal(m_rAct.width() * tem->getOffset() / 100);
    qreal arrangement = w1 / 2 * (tem->m_arrangement - 1);

    //!  指针动画效果
    double value = tem->m_value;
    qint32 step = tem->m_currentStep;
    if (step > 0)
    {
        double rate = 1.0f - step * step / (double)((step + 1) * (step + 1)) ;
        value = tem->m_currentValue + (tem->m_value - tem->m_currentValue) * rate;
    }

    //!   值限制
    double min = m_layer->getMinValue();
    double max = min + m_layer->getValueRange();
    value = (value >= min) ? value : min;
    value = (value <= max) ? value : max;

    //!  计算指针长度
    qreal len = qreal((value - min) / m_layer->getValueRange());
    len *= m_rAct.height();

    //!  计算圆形数据
    double radian = asin(w1 / w2);
    qreal angle = getAngle((qreal)radian);
    qreal cenOffset = (qreal)((w2 / 2) * (1 - cos(radian)));
    QRectF rEllipse(-w2/2+offset+arrangement, m_rAct.height()/2+h2-cenOffset, w2, w2);


    //!  总尺寸
    QRectF rf(rEllipse.x(), -(m_rAct.height()/2+h1), w2, h1 + m_rAct.height()+h2+w2-cenOffset);
    //!  指针尺寸
    QRectF rf1(rEllipse.x(), m_rAct.height()/2-len, w2, len+h2+w2-cenOffset);

    QPainterPath path, path1;
    qreal x1 = -w1 / 2 + offset + arrangement;
    qreal x2 = w1 / 2 + offset + arrangement;
    qreal y1 = h2 + m_rAct.height() / 2;
    qreal y2 = -h1 - m_rAct.height() / 2;
    qreal y3 = -len + m_rAct.height() / 2;

    path.arcMoveTo(rEllipse, 90 + angle);
    path.arcTo(rEllipse, 90 + angle, 360 - angle * 2);
    path.moveTo(x2, y1);
    path.lineTo(x2, y2);
    path.lineTo(x1, y2);
    path.lineTo(x1, y1);


    path1.arcMoveTo(rEllipse, 90 + angle);
    path1.arcTo(rEllipse, 90 + angle, 360 - angle * 2);
    path1.moveTo(x2, y1);
    path1.lineTo(x2, y3);
    path1.lineTo(x1, y3);
    path1.lineTo(x1, y1);


    //!  计算绘图区域
    if (moveRgn != NULL)
    {
        QRectF rgnR = rf;
        qreal x,y,w,h;
        x = rgnR.x() - 1;
        y = rgnR.y() - 1;
        w = rgnR.width() + 2;
        h = rgnR.height() + 2;
        QRectF rect(x,y,w,h);
        rgnR = rect;
        QRegion rgn(rgnR.x(),rgnR.y(),rgnR.width(),rgnR.height());
        moveRgn->united(rgn);
        return;
    }

    tem->m_currentValue = value;
    //!  edgeline
    QPen* pen = createPen(tem->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        delete pen;
    }

    //!  backColor
    QBrush* brushBack = createBrush(tem->m_backColor, rf, &path);
    if (brushBack != NULL)
    {
        m_g->setBrush(*brushBack);
        m_g->drawPath(path);
        delete brushBack;
    }

    //!  temColor
    QBrush* brush = createBrush(tem->m_temColor, rf1, &path1);
    if (brush != NULL)
    {

        m_g->setBrush(*brush);
        m_g->fillPath(path1,*brush);
        delete brush;
    }
}

////////////////////////////////////////////////////////////////////
//!   绘制直表组件
void CDrawItem::drawRNeedle(QRegion *moveRgn, QTransform *m)
{
    CRulerNeedle* needle = (CRulerNeedle*)m_d;

    qreal height = qreal(m_shortLen * needle->getWidth() / 100);
    qreal width = m_layer->m_isHori ? m_rAct.width() : m_rAct.height();
    qreal offset = qreal(-m_shortLen * needle->getOffset() / 100);
    qreal arrangement = height / 2 * (needle->m_arrangement - 2);

    //!  指针动画效果
    double value = needle->m_value;
    qint32 step = needle->m_currentStep;
    if (step > 0)
    {
        double rate = 1.0f - step * step / (double)((step + 1) * (step + 1)) ;
        value = needle->m_currentValue + (needle->m_value - needle->m_currentValue) * rate;
    }

    //!  值限制
    double min = m_layer->getMinValue();
    double max = min + m_layer->getValueRange();
    value = (value >= min) ? value : min;
    value = (value <= max) ? value : max;

    //!  计算指针长度
    qreal len = qreal((value - min) / m_layer->getValueRange());
    len *= width;

    QRectF rf;
    if (m_layer->m_isHori)
    {
        qreal x = rf.x() - width / 2;
        qreal y = offset + arrangement;
        qreal wid = len;
        qreal hgt = height;
        rf = QRectF(x, y, wid, hgt);
    }
    else
    {
        qreal x = offset + arrangement;
        qreal y = rf.y() - width / 2 + (width - len);
        qreal wid = height;
        qreal hgt = len;
        rf = QRectF(x, y, wid, hgt);
    }

    //!  计算绘图区域
    if (moveRgn != NULL)
    {
        return;
    }

    needle->m_currentValue = value;

    //!  backColor
    QBrush* brush = createBrush(needle->m_backColor, rf);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
    }

    //!  edgeline
    QPen* pen = createPen(needle->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        m_g->drawRect(rf);
    }
    delete brush;
    delete pen;
}

void CDrawItem::drawRMarker(QRegion *moveRgn, QTransform *m)
{
    //!  此组件只绘制水平，垂直需要旋转
    CRulerMarker* marker = (CRulerMarker*)m_d;

    qreal width = qreal(m_shortLen * marker->getWidth() / 100);
    qreal height = qreal(m_shortLen * marker->getLen() / 100);
    qreal offset = qreal(-m_shortLen * marker->getOffset() / 100);
    qreal arrangement = height / 2 * (marker->m_arrangement - 2);
    qreal len = m_layer->m_isHori ? m_rAct.width() : m_rAct.height();

    //!  指针动画效果
    double value = marker->m_value;
    qint32 step = marker->m_currentStep;
    if (step > 0)
    {
        double rate = 1.0f - step * step / (double)((step + 1) * (step + 1)) ;
        value = marker->m_currentValue + (marker->m_value - marker->m_currentValue) * rate;
    }

    //!  值限制
    double min = m_layer->getMinValue();
    double max = min + m_layer->getValueRange();
    value = (value >= min) ? value : min;
    value = (value <= max) ? value : max;

    //!  计算指示块位置
    qreal x = qreal((value - min) / m_layer->getValueRange());
    x = x * len - width / 2 - len / 2;

    QRectF rf(x, offset + arrangement, width, height);

    //!  计算绘图区域
    if (moveRgn != NULL)
    {
        QRectF rgnR = rf;
        qreal x = rgnR.x() - 1;
        qreal y = rgnR.y() - 1;
        qreal wid = rgnR.width() + 2;
        qreal hgt = rgnR.height() + 2;
        rgnR = QRectF(x,y,wid,hgt);
        QRegion rgn;
        if (!m_layer->m_isHori)
            m->rotate(270);
        moveRgn->united(rgn);
        return;
    }

    marker->m_currentValue = value;

    //!  style
    QPainterPath path;
    setPath(path, rf, marker->m_style);

    //!  if vertical,rotate
    if (!m_layer->m_isHori)
        m_g->rotate(270);

    //!  backColor
    QBrush* brush = createBrush(marker->m_backColor, rf, &path, !m_layer->m_isHori);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        m_g->fillPath(path,*brush);
        delete brush;
    }

    //!  edgeline
    QPen* pen = createPen(marker->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        m_g->drawPath(path);
        delete pen;
    }

    //!  if vertical,restore rotate
    if (!m_layer->m_isHori)
        m_g->rotate(-270);

}

void CDrawItem::drawRRange()
{
    //!  此组件只绘制水平，垂直需要旋转Graphics
    CRulerRange* range = (CRulerRange*)m_d;

    qreal height = qreal(m_shortLen * range->getWidth() / 100);
    qreal offset = qreal(-m_shortLen * range->getOffset() / 100);
    qreal len = m_layer->m_isHori ? m_rAct.width() : m_rAct.height();
    qreal width = qreal(len * range->getPercentRange() / 100);
    qreal x = qreal(-len / 2 + len * range->getMinPercent() / 100);
    qreal y = offset + height / 2 * (range->m_arrangement - 2);
    qreal interval = qreal(height * 0.5);
    //! bezier
    qreal x1Rate = (qreal)(width * 0.4);
    qreal y1Rate = (qreal)(interval * 0.1);
    qreal x2Rate = (qreal)(width * 0.8);
    qreal y2Rate = (qreal)(interval * 0.4);

    QRectF rf(x, y, width, height);
    QPointF ps[] = {QPointF(x, y),
        QPointF(x + width, y),
        QPointF(x + width, y + height),
        QPointF(x, y + height)
    };
    QPointF pb[2];
    QPainterPath path;
    //!  style
    switch(range->m_style)
    {
    case 0:
    {
        ps[0] = QPointF(x, y + interval);
        pb[0] = QPointF(ps[0].x() + x1Rate, ps[0].y() - y1Rate);
        pb[1] = QPointF(ps[0].x() + x2Rate, ps[0].y() - y2Rate);

        path.moveTo(ps[0]);
        path.cubicTo(ps[0], pb[1],  ps[1]);
        path.moveTo(ps[1]);
        path.lineTo(ps[2]);
        path.lineTo(ps[3]);
        path.lineTo(ps[0]);
     }

        break;
    case 1:
    {
        ps[1] = QPointF(x + width, y + interval);
        pb[0] = QPointF(ps[1].x() - x1Rate, ps[1].y() - y1Rate);
        pb[1] = QPointF(ps[1].x() - x2Rate, ps[1].y() - y2Rate);

        path.moveTo(ps[0]);
        path.cubicTo(ps[0], pb[1],  ps[1]);
        path.moveTo(ps[1]);
        path.lineTo(ps[2]);
        path.lineTo(ps[3]);
        path.lineTo(ps[0]);
    }
        break;
    case 2:
    {
        ps[3] = QPointF(x, y + height - interval);
        pb[0] = QPointF(ps[3].x() + x1Rate, ps[3].y() + y1Rate);
        pb[1] = QPointF(ps[3].x() + x2Rate, ps[3].y() + y2Rate);

        path.moveTo(ps[0]);
        path.lineTo(ps[1]);
        path.lineTo(ps[2]);
        path.cubicTo(ps[2], pb[1],  ps[3]);
        path.lineTo(ps[0]);
    }

        break;
    case 3:
    {
        ps[2] = QPointF(x + width, y + height -interval);
        pb[0] = QPointF(ps[2].x() - x1Rate, ps[2].y() + y1Rate);
        pb[1] = QPointF(ps[2].x() - x2Rate, ps[2].y() + y2Rate);

        path.moveTo(ps[0]);
        path.lineTo(ps[1]);
        path.lineTo(ps[2]);
        path.cubicTo(ps[2],  pb[1], ps[3]);
        path.lineTo(ps[0]);
    }
        break;
    case 4:
    {
        QPolygon  polygon;
        polygon.setPoints(5,(qint32)x,(qint32)y,
                          (qint32)(x + width),(qint32)y,
                          (qint32)(x + width),(qint32)(y+height),
                          (qint32)x,(qint32)(y+height),
                          (qint32)x,(qint32)y);
        path.addPolygon(polygon);
    }
        break;
    default:
        break;
    }

    if (!m_layer->m_isHori)
        m_g->rotate(270);

    //!   backColor
    QBrush* brush = createBrush(range->m_backColor, rf, &path, !m_layer->m_isHori);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        m_g->fillPath(path,*brush);
        delete brush;
    }

    //!   edgeline
    QPen* pen = createPen(range->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        m_g->drawPath(path);
        delete pen;
    }

    if (!m_layer->m_isHori)
        m_g->rotate(-270);
}

void CDrawItem::drawRScaleText()
{
    CRulerScaleText* text = (CRulerScaleText*)m_d;

    qreal size = qreal(m_shortLen * text->getSize() / 100);
    qreal offset = qreal(-m_shortLen * text->getOffset() / 100);
    qreal len = m_layer->m_isHori ? m_rAct.width() : m_rAct.height();
    qint32 count = m_layer->getScaleCount();
    double minValue = m_layer->getMinValue();
    double intervalValue = m_layer->getValueRange() / (count - 1.0f);
    qreal interval = len / (count - 1);
    qint32 decimal = text->getDecimals();
    qreal arrangement = size * (2 - text->m_arrangement);

    m_g->setPen(QPen(QColor(text->m_color)));

    //!  decimals
    QFont font;
    if( text->m_isBold)
    {
        font.setBold(true);
    }
    font.setPointSizeF(size);
    font.setFamily(text->m_name);
    m_g->setFont(font);
    QString  s;
    for (qint32 i = 0; i < count; i++)
    {
        double value = minValue  + intervalValue * i;
        //!  值为0时，不显示-0.00
        qreal v = (qreal)value;
        for (qint32 j = 0; j < decimal; j++)
            v *= 10;

        if (qint32(v) == 0)
            s = "0";

        qint32 wid = QString::number(value,'d',decimal).size()*size;

        qreal x, y;
        if (m_layer->m_isHori)
        {
            x = -len / 2 + interval * i - wid / 3;
            y = offset - arrangement;
        }
        else
        {
            QFontMetrics fm(font);
            QString str;
            double value1 = minValue  + intervalValue * (count-1);
            str.setNum(value1);
            qint32 fontwidth = fm.width(str);
            if(text->m_arrangement == 2)
            {
                x = offset;
                y = len / 2 - interval * i + size / 2;
            }
            else if(text->m_arrangement == 0)
            {
                if(i == count-1)
                {
                    x = offset;
                   y = len / 2 - interval * i + size / 2;
                }
                else
                {
                    str.setNum(value);
                    qint32 width = fm.width(str);
                    x = offset+fontwidth-width;
                    y = len / 2 - interval * i + size / 2;
                }
            }
            else
            {
                if(i == count-1)
                {
                    x = offset;
                    y = len / 2 - interval * i + size / 2;
                }
                else
                {
                    str.setNum(value);
                    qint32 width = fm.width(str);
                    x = offset+fontwidth/2-width/2;
                    y = len / 2 - interval * i + size / 2;
                }

            }

        }
        QPointF pf = QPointF(x,y);
        m_g->drawText(pf,QString::number(value,'d',decimal));
    }

}

void CDrawItem::drawRScale()
{
    //!  此组件只绘制水平，垂直需要旋转
    CRulerScale* scale = (CRulerScale*)m_d;

    qint32 count = m_layer->getScaleCount();
    qint32 subCount = scale->getSubCount();

    qreal width = qreal(m_shortLen * scale->getWidth() / 100);
    qreal height = qreal(m_shortLen * scale->getLen() / 100);
    qreal offset = qreal(-m_shortLen * scale->getOffset() / 100);
    qreal len = m_layer->m_isHori ? m_rAct.width() : m_rAct.height();
    qreal arrangement = height / 2 * (scale->m_arrangement - 2);
    qreal interval = len / (count - 1.0f);
    qreal subInterval = interval / (subCount + 1);

    QRectF rf(-width / 2, arrangement + offset, width, height);

    //!  style
    QPainterPath path;
    setPath(path, rf, scale->m_style);

    //!  if vertical,rotate
    if (!m_layer->m_isHori)
        m_g->rotate(270);

    //!  backColor
    QBrush* brush = createBrush(scale->m_backColor, rf, &path, !m_layer->m_isHori);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        m_g->translate(-len / 2, 0);

        for (qint32 i = 0; i < count; i++)
        {
            if (scale->m_isSubScale)
            {
                if (i == count - 1)
                    break;

                m_g->translate(subInterval, 0);

                for (qint32 j = 0; j < subCount; j++)
                {
                    m_g->fillPath(path,*brush);
                    m_g->translate(subInterval, 0);
                }
            }
            else
            {
                m_g->fillPath(path,*brush);
                m_g->translate(interval, 0);
            }
        }

        if (scale->m_isSubScale)
            m_g->translate(-len / 2, 0);
        else
            m_g->translate(-len / 2 - interval, 0);

        delete brush;
    }

    //!  edgeline
    QPen* pen = createPen(scale->m_edgeLine);
    if (pen != NULL)
    {
        m_g->translate(-len / 2, 0);
        m_g->setPen(*pen);
        for (qint32 i = 0; i < count; i++)
        {
            if (scale->m_isSubScale)
            {
                if (i == count - 1)
                    break;

                m_g->translate(subInterval, 0);

                for (qint32 j = 0; j < subCount; j++)
                {
                    m_g->drawPath(path);
                m_g->translate(subInterval, 0);
                }
            }
            else
            {
                m_g->drawPath(path);
                m_g->translate(interval, 0);
            }
        }

        if (scale->m_isSubScale)
            m_g->translate(-len / 2, 0);
        else
            m_g->translate(-len / 2 - interval, 0);

        delete pen;
    }

    //!  if vertical,restore rotate
    if (!m_layer->m_isHori)
        m_g->rotate(-270);
}

void CDrawItem::drawRScaleBack()
{
    CRulerScaleBack* sBack = (CRulerScaleBack*)m_d;

    QRectF rf;
    qreal width = qreal(m_shortLen * sBack->getWidth() / 100);
    qreal len = qreal(width / 2  * (sBack->m_arrangement - 2));
    qreal offset = qreal(-m_shortLen * sBack->getOffset() / 100);

    if (m_layer->m_isHori)		//!  水平
    {

        rf = QRectF(-m_rAct.width() / 2,offset + len,m_rAct.width(),width);
    }
    else						//!  垂直
    {
        rf = QRectF(offset + len,-m_rAct.height()/2,width,m_rAct.height());
    }

    //!  backColor
    QBrush* brush = createBrush(sBack->m_backColor, rf);
    if (brush != NULL)
    {
        m_g->setBrush(*brush);
        m_g->fillRect(rf,*brush);
        delete brush;
    }

    //!  edgeline
    QPen* pen = createPen(sBack->m_edgeLine);
    if (pen != NULL)
    {
        m_g->setPen(*pen);
        m_g->drawRect(rf);
        delete pen;
    }
}

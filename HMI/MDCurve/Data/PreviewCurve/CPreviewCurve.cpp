#include "CPreviewCurve.h"
#include "CCurveObject.h"
#include "CCurveObjectManager.h"
#include "CPreviewCurveData.h"
#include "CXAxis.h"
#include "CYAxis.h"
#include "CXAxisManager.h"
#include <QDebug>
//#include <sys/time.h>
#include <time.h>

CPreviewCurve::CPreviewCurve()
{
    m_previewData = new CPreviewCurveData;
    m_startScale = 0;
    m_endScale = 1;
    m_previewPoint.clear();
}

CPreviewCurve::~CPreviewCurve()
{
    if(m_previewData)
    {
        delete m_previewData;
        m_previewData = NULL;
    }
}

CPreviewCurveData *CPreviewCurve::previewData()
{
    return m_previewData;
}

void CPreviewCurve::setMoveLength(const double &length)
{
    switch(m_moveType)
    {
    case moveType::moveStart:
        startMove(length);
        break;
    case moveType::moveEnd:
        endMove(length);
        break;
    case moveType::move:
    {
        dragMove(length);
    }
        break;
    case moveType::moveStartRoll:
        break;
    case moveType::moveEndRoll:
        break;
    default:
        break;
    }

    emit updatePreviewArea();
}

void CPreviewCurve::setMoveType(int type)
{
    switch(type)
    {
    case 0:
        m_moveType = moveType::moveStart;
        break;
    case 1:
        m_moveType = moveType::moveEnd;
        break;
    case 2:
        m_moveType = moveType::move;
        break;
    case 3:
    {
        m_moveType = moveType::moveStartRoll;
    }
        break;
    case 4:
    {
        m_moveType = moveType::moveEndRoll;
    }
        break;
    default:
        break;
    }
}

void CPreviewCurve::setIsStudioMode(bool isStudio)
{
    m_isStudioMode = isStudio;
}

void CPreviewCurve::setIsReal(bool isReal)
{
    m_isReal = isReal;
}

QRectF CPreviewCurve::previewArea() const
{
    return m_previewArea;
}

void CPreviewCurve::setPreviewArea(const QRectF &area)
{
    m_previewArea = area;
}

QRectF CPreviewCurve::curveArea() const
{
    return m_curveArea;
}

void CPreviewCurve::setCurveArea(const QRectF &area)
{
    m_curveArea = area;
}

void CPreviewCurve::setCurves(CCurveObjectManager *curves)
{
    m_curves = curves;
}

CCurveObjectManager *CPreviewCurve::curves()
{
    return m_curves;
}

QRectF CPreviewCurve::startMoveRect() const
{
    return m_startMoveRect;
}

QRectF CPreviewCurve::endMoveRect() const
{
    return m_endMoveRect;
}

QRectF CPreviewCurve::dragMoveRect() const
{
    return m_dragMoveRect;
}

QRectF CPreviewCurve::startRollMoveRect() const
{
    return m_startRollRect;
}

QRectF CPreviewCurve::endRollMoveRect() const
{
    return m_endRollRect;
}

void CPreviewCurve::draw(QPainter *p)
{
    if(!m_previewData->isPreviewVisible())
        return;

    drawBackGround(p);
    if(m_isStudioMode && m_curves->list().count() == 0)
    {
        drawMoveArea(p);
    }
    else
    {
        drawCurve(p);
        drawMoveArea(p);
    }
}

void CPreviewCurve::calculate()
{
    if(!m_previewData->isPreviewVisible())
        return;

    m_rollBackRect = QRectF(m_previewArea.x(),m_previewArea.y()+m_previewArea.height()-22,
                        m_previewArea.width(),20);
    m_rollRect = QRectF(m_previewArea.x()+20,m_previewArea.y()+m_previewArea.height()-22,
                        m_previewArea.width()-40,20);

    m_startRollRect = QRectF(m_rollBackRect.x(),m_rollBackRect.y(),20,20);
    m_endRollRect = QRectF(m_rollBackRect.x()+m_rollBackRect.width()-20,m_rollBackRect.y(),20,20);
    m_previewBackRect = QRectF(m_rollBackRect.x(),m_rollBackRect.y()+m_rollBackRect.height()-57,m_rollBackRect.width(),35);
    m_previewCurveRect = m_previewBackRect.adjusted(20,0,-20,0);

    if(m_isStudioMode)
    {
        m_moveRect = QRectF(m_previewCurveRect.x(),m_previewCurveRect.y(),50,m_previewCurveRect.height());
        m_startMoveRect = QRectF(m_moveRect.x()-6,m_moveRect.y()+5,12,25);
        m_endMoveRect = QRectF(m_moveRect.x()+m_moveRect.width()-6,m_moveRect.y()+5,12,25);
        m_dragMoveRect = QRectF(m_moveRect.x(),m_moveRect.y()+m_moveRect.height()+2,
                                m_moveRect.width(),20);
    }
    else
    {
        double min_x = m_previewCurveRect.x() + m_previewCurveRect.width()*startScale();
        double max_x = m_previewCurveRect.x() + m_previewCurveRect.width()*endScale();
        m_moveRect = QRectF(min_x,m_previewCurveRect.y(),max_x - min_x,m_previewCurveRect.height());
        m_startMoveRect = QRectF(m_moveRect.x()-6,m_moveRect.y()+5,12,25);
        m_endMoveRect = QRectF(m_moveRect.x()+m_moveRect.width()-6,m_moveRect.y()+5,12,25);
        m_dragMoveRect = QRectF(m_moveRect.x(),m_moveRect.y()+m_moveRect.height()+2,
                                m_moveRect.width(),20);
    }

    calculatePreviewPoint();
}

void CPreviewCurve::calculatePreviewPoint()
{
    if(m_curves->list().count() == 0 || m_previewData->boundedCurve().isEmpty()
            ||m_previewData->isCurveVisible() == false)
        return;
    CCurveObject *boundCurve = NULL;
    for(int i = 0; i< m_curves->list().count(); i++)
    {
        CCurveObject *curve = m_curves->list().at(i);
        if(curve->name() == m_previewData->boundedCurve())
        {
            boundCurve = curve;
            break;
        }
    }

    m_previewPoint.clear();
    CCurveData previewData = boundCurve->previewData();
    if(previewData.timeList().isEmpty())
        return;
    CYAxis *yAxis = boundCurve->yAxis();
    CXAxis *xAxis = boundCurve->xAxis();

    double yMaxValue;
    double yMinValue;
    if(!yAxis->isNumeric())
    {
        yMaxValue = yAxis->maxPercent();
        yMinValue = yAxis->minPercent();
    }
    else
    {
        yMaxValue = yAxis->originMax();
        yMinValue = yAxis->originMin();
    }

    double xWidth = m_curveArea.width();
    qint64 valueLength = xAxis->totalLength();

    double yHeight = m_curveArea.height();
    double yValueRange = yMaxValue - yMinValue;
    QDateTime timeMark = xAxis->timeMark();
    QDateTime endTime = xAxis->originEndTime();

    QDateTime startTime = endTime.addSecs(-valueLength);
    qint64 time = startTime.secsTo(timeMark);

    QList<CTimeValuePoint> list;
    list = previewData.timeList();

    for(int i = 0; i < list.count();++i)
    {
        CTimeValuePoint timePoint = list.at(i);
        qint64 timeConst = timeMark.secsTo(timePoint.xTime());

        double tx = xWidth *timeConst / valueLength;
        double cx = xWidth*time/valueLength;
        tx += cx;

        double value = timePoint.y() - yMinValue;
        double cy =  -yHeight/yValueRange*value;
        m_previewPoint.append(QPointF(tx,cy).toPoint());
    }

    qreal xdiffer = m_previewCurveRect.width() / m_curveArea.width();
    qreal ydiffer = m_previewCurveRect.height() / m_curveArea.height();
    for(int i = 0; i<m_previewPoint.count();++i)
    {
        QPoint point =QPoint(m_previewPoint.at(i).x()*xdiffer,m_previewPoint.at(i).y()*ydiffer);
        m_previewPoint.removeAt(i);
        m_previewPoint.insert(i,point);
    }
}

void CPreviewCurve::drawBackGround(QPainter *painter)
{
    //! 滚动条背景区域
    painter->setPen(Qt::NoPen);
    QPainterPath rollBackpath;
    rollBackpath.addRect(m_rollBackRect);
    CMDBrushManager rollBackBrush;
    rollBackBrush = m_previewData->rollBackBrush();
    rollBackBrush.generate(m_rollBackRect);
    rollBackBrush.draw(painter,m_rollBackRect,&rollBackpath);

    //! 左右点击移动区域

    QPainterPath rollStartpath;
    rollStartpath.addRect(m_startRollRect);
    CMDBrushManager rollStartBrush;
    rollStartBrush = m_previewData->rollStartBackBrush();
    rollStartBrush.generate(m_startRollRect);
    rollStartBrush.draw(painter,m_startRollRect,&rollStartpath);

    QPainterPath rollEndpath;
    rollEndpath.addRect(m_endRollRect);
    CMDBrushManager rollEndBrush;
    rollEndBrush = m_previewData->rollEndBackBrush();
    rollEndBrush.generate(m_endRollRect);
    rollEndBrush.draw(painter,m_endRollRect,&rollEndpath);

    painter->setPen(1);
    painter->drawLine(m_startRollRect.x()+20,m_startRollRect.y(),m_startRollRect.x()+20,m_startRollRect.y()+20);
    painter->drawLine(m_startRollRect.x()+20,m_startRollRect.y()+20,m_startRollRect.x(),m_startRollRect.y()+20);

    painter->drawLine(m_endRollRect.x()+20,m_endRollRect.y(),m_endRollRect.x()+20,m_endRollRect.y()+20);
    painter->drawLine(m_endRollRect.x()+20,m_endRollRect.y()+20,m_endRollRect.x(),m_endRollRect.y()+20);
    //! 绘制左右箭头
    QRectF leftArrows = m_startRollRect.adjusted(6,6,-8,-8);
    QPen penStart;
    penStart.setColor(m_previewData->rollStartArrowsColor());
    penStart.setWidth(3);
    painter->setPen(penStart);
    painter->drawLine(leftArrows.x()+leftArrows.width(),leftArrows.y(),
                      leftArrows.x(),leftArrows.y()+leftArrows.width()/2);
    painter->drawLine(leftArrows.x(),leftArrows.y()+leftArrows.width()/2,
                      leftArrows.x()+leftArrows.width(),leftArrows.y()+leftArrows.height());

    QPen penEnd;
    penEnd.setColor(m_previewData->rollEndArrowsColor());
    penEnd.setWidth(3);
    painter->setPen(penEnd);
    QRectF rightArrows = m_endRollRect.adjusted(6,6,-8,-8);
    painter->drawLine(rightArrows.x(),rightArrows.y(),
                      rightArrows.x()+rightArrows.width(),rightArrows.y()+rightArrows.width()/2);
    painter->drawLine(rightArrows.x()+rightArrows.width(),rightArrows.y()+rightArrows.width()/2,
                      rightArrows.x(),rightArrows.y()+rightArrows.height());

    //! 曲线预览区域
    QPainterPath path;
    path.addRect(m_previewBackRect);
    CMDBrushManager backBrush;
    backBrush = m_previewData->previewBackBrush();
    backBrush.generate(m_previewBackRect);
    backBrush.draw(painter,m_previewBackRect,&path);

    QPen pen2;
    pen2.setColor(QColor(0,0,0,150));
    painter->setBrush(Qt::NoBrush);
    painter->setPen(pen2);
    painter->drawLine(m_previewCurveRect.x(),m_previewCurveRect.y()+m_previewCurveRect.height(),
                      m_previewCurveRect.x()+m_previewCurveRect.width(),m_previewCurveRect.y()+m_previewCurveRect.height());
    painter->drawLine(m_previewCurveRect.x(),m_previewCurveRect.y(),
                      m_previewCurveRect.x() + m_previewCurveRect.width(),m_previewCurveRect.y());

    //! 预览区域刻度
    int mainScale = m_previewData->mainBisectrix();
    double internal = m_previewCurveRect.width() / mainScale;
    int subScale = m_previewData->subBisectrix();
    double subInteral = internal/subScale;
    int scaleHeight = m_previewData->subBisectrixHeight();
    for(int i = 0; i <= mainScale;i++)
    {
        painter->setPen(m_previewData->mainBisectrixColor());
        painter->drawLine(m_previewCurveRect.x()+i*internal,m_previewCurveRect.y(),
                          m_previewCurveRect.x()+i*internal,m_previewCurveRect.y()+m_previewCurveRect.height());

        for(int j = 1;j < subScale;j++)
        {
            if(i != 0)
            {
                painter->setPen(m_previewData->subBisectrixColor());
                painter->drawLine(m_previewCurveRect.x()+i*internal - j*subInteral,m_previewCurveRect.y()+35 - scaleHeight,
                                  m_previewCurveRect.x()+i*internal- j*subInteral,m_previewCurveRect.y()+35);
            }
        }
    }
}

void CPreviewCurve::drawMoveArea(QPainter *painter)
{
    //! 预览曲线显示区域
    QPainterPath path;
    path.addRect(m_moveRect);
    CMDBrushManager moveBrush;
    moveBrush = m_previewData->previewHightLightBrush();
    moveBrush.generate(m_moveRect);
    moveBrush.draw(painter,m_moveRect,&path);

    //! 左移拖动区域
    painter->setPen(m_previewData->startLineMarkColor());
    painter->drawLine(m_moveRect.x(),m_moveRect.y(),
                      m_moveRect.x(),m_moveRect.y()+m_moveRect.height());
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_previewData->startBackColor());
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->drawRoundedRect(m_startMoveRect,5,5);
    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->setPen(m_previewData->startScaleColor());
    painter->drawLine(m_startMoveRect.x()+4,m_startMoveRect.y()+5,
                      m_startMoveRect.x()+4,m_startMoveRect.y()+20);
    painter->drawLine(m_startMoveRect.x()+8,m_startMoveRect.y()+5,
                      m_startMoveRect.x()+8,m_startMoveRect.y()+20);

    //! 右移拖动区域
    painter->setPen(m_previewData->endLineMarkColor());
    painter->drawLine(m_moveRect.x()+m_moveRect.width(),m_moveRect.y(),
                      m_moveRect.x()+m_moveRect.width(),m_moveRect.y()+m_moveRect.height());
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_previewData->endBackColor());
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->drawRoundedRect(m_endMoveRect,5,5);
    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->setPen(m_previewData->endScaleColor());
    painter->drawLine(m_endMoveRect.x()+4,m_endMoveRect.y()+5,
                      m_endMoveRect.x()+4,m_endMoveRect.y()+20);
    painter->drawLine(m_endMoveRect.x()+8,m_endMoveRect.y()+5,
                      m_endMoveRect.x()+8,m_endMoveRect.y()+20);

    //! 左右拖动区域
    painter->setPen(Qt::NoPen);
    QPainterPath dragPath;
    dragPath.addRect(m_dragMoveRect);
    CMDBrushManager dragBrush;
    dragBrush = m_previewData->rollMoveBackBrush();
    dragBrush.generate(m_dragMoveRect);
    dragBrush.draw(painter,m_dragMoveRect,&dragPath);

    QPen scalePen;
    scalePen.setColor(m_previewData->rollScaleColor());
    scalePen.setWidth(1);
    painter->setPen(scalePen);
    painter->drawLine(m_dragMoveRect.x()+m_dragMoveRect.width()/2,m_dragMoveRect.y()+4,
                      m_dragMoveRect.x()+m_dragMoveRect.width()/2,m_dragMoveRect.y()+16);

    painter->drawLine(m_dragMoveRect.x()+m_dragMoveRect.width()/2-4,m_dragMoveRect.y()+6,
                      m_dragMoveRect.x()+m_dragMoveRect.width()/2-4,m_dragMoveRect.y()+14);
    painter->drawLine(m_dragMoveRect.x()+m_dragMoveRect.width()/2+4,m_dragMoveRect.y()+6,
                      m_dragMoveRect.x()+m_dragMoveRect.width()/2+4,m_dragMoveRect.y()+14);

    painter->setPen(1);
    painter->drawLine(m_dragMoveRect.x()+m_dragMoveRect.width(),m_dragMoveRect.y(),
                      m_dragMoveRect.x()+m_dragMoveRect.width(),m_dragMoveRect.y()+m_dragMoveRect.height());

}

void CPreviewCurve::drawCurve(QPainter *p)
{
    p->save();
    p->setClipRect(m_previewCurveRect);
    p->translate(m_previewCurveRect.x(),m_previewCurveRect.y() + m_previewCurveRect.height());
    if(m_curves->list().count() == 0 || m_previewData->boundedCurve().isEmpty()
            ||m_previewData->isCurveVisible() == false)
        return;

    CCurveObject *curve = NULL;
    for(int i = 0;i <m_curves->list().count(); ++i)
    {
        if(m_previewData->boundedCurve() == m_curves->list().at(i)->name())
            curve = m_curves->list().at(i);
    }
    QColor color = curve->curveColor();
    p->setPen(color);
    for(int i = 0;i<m_previewPoint.count()-1;++i)
    {
        p->drawLine(m_previewPoint.at(i),m_previewPoint.at(i+1));
    }
    p->restore();

}

void CPreviewCurve::startRollMove()
{
    double start = startScale();
    double end = endScale();
    double moveScale;

    if(m_moveRect.x() - m_previewCurveRect.x()>= 5)
    {
        moveScale = 5 / m_previewCurveRect.width();
    }
    else
    {
        double lengthR = m_moveRect.x() - m_previewCurveRect.x();
        moveScale = lengthR / m_previewCurveRect.width();
    }

    start = start - moveScale;
    end = end - moveScale;

    int min_x = m_previewCurveRect.x() + m_previewCurveRect.width()*start;
    int max_x = m_previewCurveRect.x() + m_previewCurveRect.width()*end;
    m_moveRect = QRectF(min_x,m_previewCurveRect.y(),max_x - min_x,m_previewCurveRect.height());
    m_startMoveRect = QRectF(m_moveRect.x()-6,m_moveRect.y()+5,12,25);
    m_endMoveRect = QRectF(m_moveRect.x()+m_moveRect.width()-6,m_moveRect.y()+5,12,25);
    m_dragMoveRect = QRectF(m_moveRect.x(),m_moveRect.y()+m_moveRect.height()+2,
                            m_moveRect.width(),20);
    if(m_moveRect.x() >= m_previewCurveRect.x())
    {
        setScale(start,end);
        if(m_xManager)
        {
            for(int i = 0; i<m_xManager->list().count();++i)
            {
                CXAxis *xAxis = m_xManager->list().at(i);
                xAxis->setScale(start,end);
            }
        }
    }
    emit updatePreviewArea();
}

void CPreviewCurve::endRollMove()
{
    double start = startScale();
    double end = endScale();
    double moveScale;

    if(m_previewCurveRect.x()+m_previewCurveRect.width()-m_moveRect.x() -m_moveRect.width() >= 5)
    {
        moveScale = 5 / m_previewCurveRect.width();
    }
    else
    {
        double lengthR = m_previewCurveRect.x()+m_previewCurveRect.width()-m_moveRect.x() -m_moveRect.width();
        moveScale = lengthR / m_previewCurveRect.width();
    }

    start = start + moveScale;
    end = end + moveScale;

    int min_x = m_previewCurveRect.x() + m_previewCurveRect.width()*start;
    int max_x = m_previewCurveRect.x() + m_previewCurveRect.width()*end;
    m_moveRect = QRectF(min_x,m_previewCurveRect.y(),max_x - min_x,m_previewCurveRect.height());
    m_startMoveRect = QRectF(m_moveRect.x()-6,m_moveRect.y()+5,12,25);
    m_endMoveRect = QRectF(m_moveRect.x()+m_moveRect.width()-6,m_moveRect.y()+5,12,25);
    m_dragMoveRect = QRectF(m_moveRect.x(),m_moveRect.y()+m_moveRect.height()+2,
                            m_moveRect.width(),20);
    if(m_moveRect.x() + m_moveRect.width() <= m_previewCurveRect.x() +m_previewCurveRect.width())
    {
        setScale(start,end);
        if(m_xManager)
        {
            for(int i = 0; i<m_xManager->list().count();++i)
            {
                CXAxis *xAxis = m_xManager->list().at(i);
                xAxis->setScale(start,end);
            }
        }
    }
    emit updatePreviewArea();
}

void CPreviewCurve::startMove(const double &dx)
{
    double start = startScale();
    double end = endScale();
    double moveScale = dx / m_previewCurveRect.width();

    start = start + moveScale;
    if(start <= 0)
        start = 0;
    if(start > end)
        return;

    int min_x = m_previewCurveRect.x() + m_previewCurveRect.width()*start;
    int max_x = m_previewCurveRect.x() + m_previewCurveRect.width()*end;
    m_moveRect = QRectF(min_x,m_previewCurveRect.y(),max_x - min_x,m_previewCurveRect.height());
    m_startMoveRect = QRectF(m_moveRect.x()-6,m_moveRect.y()+5,12,25);
    m_endMoveRect = QRectF(m_moveRect.x()+m_moveRect.width()-6,m_moveRect.y()+5,12,25);
    m_dragMoveRect = QRectF(m_moveRect.x(),m_moveRect.y()+m_moveRect.height()+2,
                            m_moveRect.width(),20);
    if(m_moveRect.x() >= m_previewCurveRect.x() && m_moveRect.x() <= m_endMoveRect.x())
    {
        setScale(start,end);
        if(m_xManager)
        {
            for(int i = 0; i<m_xManager->list().count();++i)
            {
                CXAxis *xAxis = m_xManager->list().at(i);
                xAxis->setScale(start,end);
            }
        }
    }
}

void CPreviewCurve::endMove(const double &dx)
{
    double start = startScale();
    double end = endScale();
    double moveScale = dx / m_previewCurveRect.width();

    end = end + moveScale;
    if(end >= 1)
        end = 1;
    if(start > end)
        return;

    int min_x = m_previewCurveRect.x() + m_previewCurveRect.width()*start;
    int max_x = m_previewCurveRect.x() + m_previewCurveRect.width()*end;
    m_moveRect = QRectF(min_x,m_previewCurveRect.y(),max_x - min_x,m_previewCurveRect.height());
    m_startMoveRect = QRectF(m_moveRect.x()-6,m_moveRect.y()+5,12,25);
    m_endMoveRect = QRectF(m_moveRect.x()+m_moveRect.width()-6,m_moveRect.y()+5,12,25);
    m_dragMoveRect = QRectF(m_moveRect.x(),m_moveRect.y()+m_moveRect.height()+2,
                            m_moveRect.width(),20);
    if(m_moveRect.x()+m_moveRect.width() <= m_previewCurveRect.x()+m_previewCurveRect.width()
            && m_moveRect.x() + m_moveRect.width() >= m_startMoveRect.x())
    {
        setScale(start,end);
        if(m_xManager)
        {
            for(int i = 0; i<m_xManager->list().count();++i)
            {
                CXAxis *xAxis = m_xManager->list().at(i);
                xAxis->setScale(start,end);
            }
        }
    }
}

void CPreviewCurve::dragMove(const double &dx)
{
    double length = dx;
    double preStart = startScale();
    double preEnd = endScale();
    if((preStart == 0&& length <0)|| (preEnd == 1 && length >0))
        setScale(preStart,preEnd);
    double moveScale = length / m_previewCurveRect.width();
    double start = preStart + moveScale;
    double end = preEnd + moveScale;

    if(start <= 0)
    {
        start = 0;
        end = preEnd - preStart;
    }
    if(end >= 1)
    {
         end = 1;
         start = 1 - (preEnd - preStart);
    }

    if(start > end)
        return;

    double min_x = m_previewCurveRect.x() + m_previewCurveRect.width()*start;
    double max_x = m_previewCurveRect.x() + m_previewCurveRect.width()*end;
    m_moveRect = QRectF(min_x,m_previewCurveRect.y(),max_x - min_x,m_previewCurveRect.height());
    m_startMoveRect = QRectF(m_moveRect.x()-6,m_moveRect.y()+5,12,25);
    m_endMoveRect = QRectF(m_moveRect.x()+m_moveRect.width()-6,m_moveRect.y()+5,12,25);
    m_dragMoveRect = QRectF(m_moveRect.x(),m_moveRect.y()+m_moveRect.height()+2,
                            m_moveRect.width(),20);
    if(m_moveRect.x()+m_moveRect.width() <= m_previewCurveRect.x()+m_previewCurveRect.width()
            && m_moveRect.x()  >= m_previewCurveRect.x())
    {
        setScale(start,end);
        if(m_xManager)
        {
            for(int i = 0; i<m_xManager->list().count();++i)
            {
                CXAxis *xAxis = m_xManager->list().at(i);
                xAxis->setScale(start,end);
            }
        }
    }
}

void CPreviewCurve::getBrushes(QList<CMDBrushManager *> &list)
{
    m_previewData->getBrushes(list);
}

void CPreviewCurve::setScale(double start, double end)
{
    setStartScale(start);
    setEndScale(end);
}

void CPreviewCurve::setStartScale(double start)
{
   m_startScale = start;
}

void CPreviewCurve::setEndScale(double end)
{
    m_endScale = end;
}

double CPreviewCurve::startScale()
{
    return m_startScale;
}

double CPreviewCurve::endScale()
{
    return m_endScale;
}

void CPreviewCurve::setXManager(CXAxisManager *xManager)
{
    m_xManager = xManager;
}

CPreviewCurve &CPreviewCurve::operator =(const CPreviewCurve &other)
{
    if(&other == this)
        return *this;

    *this->m_previewData = *other.m_previewData;
    return *this;
}

void CPreviewCurve::serialize(QJsonObject &json)
{
    m_previewData->serialize(json);
}

void CPreviewCurve::deserialize(const QJsonObject &json)
{
    m_previewData->deserialize(json);
}

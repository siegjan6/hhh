#include <QtCore/qmath.h>
#include <QDebug>
#include <QPen>
#include <math.h>

#include "MDCalculation.h"
//////////////////////////////////////////////////////////////////////////
QTransform CMDCalculation::m_transform;
QTransform CMDCalculation::m_funTransform;
//////////////////////////////////////////////////////////////////////////
QPointF CMDCalculation::invertPos(const QTransform& matrix, const QPointF& point)
{
	if (matrix.isIdentity() || !matrix.isInvertible())
		return point;

	QTransform m = matrix.inverted();
	return m.map(point);
}
void CMDCalculation::calcMatrix(const QRectF& data, IDataVector* dataV, QTransform& matrix, QPointF& rotatePointPos)
{
	matrix.reset();

	QRectF rf(data);
    rf = scaleRect(dataV->scalePoint(), dataV->xScale(), dataV->yScale(), rf);
    QPointF scalePoint(rf.x() + rf.width()*dataV->scalePoint().x(),
        rf.y() + rf.height()*dataV->scalePoint().y());
    QPointF rectCenter = rf.center();
	//计算旋转中心点
    QPointF origCenter(rf.width()*(dataV->rotatePoint().x() - 0.5),
        rf.height()*(dataV->rotatePoint().y() - 0.5));
    rotatePointPos = shearPoint(origCenter, dataV->shear()) + rectCenter;

	//本段代码次序不能改变
	//Matrix后面的操作先执行，切记
	//旋转
	matrix.translate(rotatePointPos.x(), rotatePointPos.y());
    matrix.rotate(dataV->rotateAngle());
	matrix.translate(-rotatePointPos.x(), -rotatePointPos.y());
	//倾斜
	matrix.translate(rectCenter.x(), rectCenter.y());
    matrix.shear(dataV->shear(), 0);
	matrix.translate(-rectCenter.x(), -rectCenter.y());
	//缩放
	matrix.translate(scalePoint.x(), scalePoint.y());
    matrix.scale(dataV->xScale(), dataV->yScale());
	matrix.translate(-scalePoint.x(), -scalePoint.y());
	//垂直水平翻转
    if (dataV->isFlipX() || dataV->isFlipY())
	{
        int x = dataV->isFlipX() ? -1 : 1;
        int y = dataV->isFlipY() ? -1 : 1;
		matrix.translate(rectCenter.x(), rectCenter.y());
        matrix.scale(x, y);
		matrix.translate(-rectCenter.x(), -rectCenter.y());
	}
}
QPointF CMDCalculation::calcFixPoint(const QRectF& data, IDataVector* dataV, const QPointF& fixRate, bool isBound/* = false*/)
{
	m_funTransform.reset();
    QPointF point(0, 0);
	calcMatrix(data, dataV, m_funTransform, point);

	QRectF rf = data;
	QPointF fixPos(rf.x() + rf.width() * fixRate.x(), rf.y() + rf.height() * fixRate.y());

	if (isBound)
	{
		rf = m_funTransform.mapRect(rf);
		fixPos = QPointF(rf.x() + rf.width() * fixRate.x(), rf.y() + rf.height() * fixRate.y());
		return fixPos;
	}

	return m_funTransform.map(fixPos);
}
QPointF CMDCalculation::calcMatrixPoint(const QRectF& data, IDataVector* dataV, const QPointF& point)
 {
	 m_funTransform.reset();
	 QPointF center;
	 calcMatrix(data, dataV, m_funTransform, center);
	 return m_funTransform.map(point);
 }
void CMDCalculation::boundMoveData(const QRectF& data, IDataVector* dataV, qreal wOff, qreal hOff, qreal& angle, qreal& shear, QRectF& rect)
{
	QPointF pf;
	QPolygonF ps(data);
	calcMatrix(data, dataV, m_funTransform, pf);
	ps = m_funTransform.map(ps);

	//反转Flip
    flipRectPos(ps, dataV->isFlipX(), dataV->isFlipY());
	QRectF bound = ps.boundingRect();
	qreal newW = bound.width() + wOff;
	qreal scaleW = newW / bound.width();
	qreal newH = bound.height() + hOff;
	qreal scaleH = newH / bound.height();

	for (int i = 0; i < ps.count(); i++)
	{
		ps[i].setX((ps[i].x() - bound.x()) * scaleW + bound.x());
		ps[i].setY((ps[i].y() - bound.y()) * scaleH + bound.y());
	}

	//计算旋转角度
	angle = qAtan2(ps.at(1).y() - ps.at(0).y(),
		ps.at(1).x() - ps.at(0).x());
	angle = angle * 180 / PI;

	//获取反旋转后的数据
	bound = ps.boundingRect();
	QPointF center = bound.center();
	m_funTransform.reset();
	m_funTransform.translate(center.x(), center.y());
	m_funTransform.rotate(-angle);
	m_funTransform.translate(-center.x(), -center.y());
	ps = m_funTransform.map(ps);
	
	//计算倾斜
	qreal h = ps[3].y() - ps[0].y();
	qreal wShear = ps[3].x() - ps[0].x();
	shear = wShear / h;

	//获取反倾斜后的数据
	rect = QRectF(ps[0].x() + wShear/2, ps[0].y(), ps[1].x() - ps[0].x(), h);
}
void CMDCalculation::matrixData(const QRectF& data, IDataVector* dataV, const QTransform& matrix, QRectF& rect)
{
    qreal angle = dataV->rotateAngle();
    qreal shear = dataV->shear();
    QPolygonF ps(data);
    ps = matrix.map(ps);
	QRectF bound = ps.boundingRect();

	//反转Flip
    flipRectPos(ps, dataV->isFlipX(), dataV->isFlipY());
	//计算旋转角度
	angle = qAtan2(ps[1].y() - ps[0].y(), ps[1].x() - ps[0].x());
	angle = angle * 180 / PI;

	//获取反旋转后的数据
	QPointF center = bound.center();
	m_funTransform.reset();
	m_funTransform.translate(center.x(), center.y());
	m_funTransform.rotate(-angle);
	m_funTransform.translate(-center.x(), -center.y());
    ps = m_funTransform.map(ps);

	//计算倾斜
	qreal h = ps[3].y() - ps[0].y();
	qreal wShear = ps[3].x() - ps[0].x();
	shear = wShear / h;
	//获取反倾斜后的数据
	rect = QRectF(ps[0].x() + wShear/2, ps[0].y(), ps[1].x() - ps[0].x(), h);

	//获取rect偏移
    QPointF origCenter(rect.width() * (dataV->rotatePoint().x() - 0.5),
        rect.height() * (dataV->rotatePoint().y() - 0.5));
	QPointF rotateCenter = shearPoint(origCenter, shear);
	QPointF offset = rotatePos(QPointF(0, 0), rotateCenter, angle);
	qreal xOff = center.x() - (rect.x() + rect.width()/2 + offset.x());
	qreal yOff = center.y() - (rect.y() + rect.height()/2 + offset.y());
	rect.translate(xOff, yOff);

    dataV->setRotateAngle(angle);
    dataV->setShear(shear);
}
QPointF CMDCalculation::rotatePoint(const QTransform& matrix, const QPointF& point, const QRectF& rect)
{
	QPointF pf = invertPos(matrix, point);
	return QPointF((pf.x() - rect.x()) / rect.width(),
		(pf.y() - rect.y()) / rect.height());
}
qreal CMDCalculation::shear(ICalcData *data, IDataVector *dataV, const QPointF& point, int pos)
{
    qreal x1 = rotatePos(data->mousePos(), dataV->rotatePointPos(), -dataV->rotateAngle()).x();
    qreal x2 = rotatePos(point, dataV->rotatePointPos(), -dataV->rotateAngle()).x();
    qreal xShear = (x2 - x1) * 2 / data->data()->rect().height();

	if (pos == 0)
		xShear = -xShear;
    if (dataV->isFlipY())
		xShear = -xShear;

    return dataV->shear() + xShear;
}
void CMDCalculation::frameOffset(ICalcData *data, const QPointF& point, int pos,
    qreal& xOff, qreal& yOff, qreal& wOff, qreal& hOff, qreal orthoTan)
{
    QPointF begin = data->mousePos();
	QPointF end = point;
    if (!data->matrix()->isIdentity())
	{
        begin = invertPos(*data->matrix(), begin);
        end = invertPos(*data->matrix(), end);
	}

	offset(begin, end, pos, xOff, yOff, wOff, hOff, orthoTan);
    limitOffset(data->data()->rect(), xOff, yOff, wOff, hOff);
}
qreal CMDCalculation::rotateAngle(ICalcData *data, const QPointF& point, const QPointF& center)
{
    qreal angle1 = qAtan2(data->mousePos().y() - center.y(), data->mousePos().x() - center.x());
	qreal angle2 = qAtan2(point.y() - center.y(), point.x() - center.x());
	qreal angle = (angle2 - angle1) * 180 / PI;

    return data->dataVector()->rotateAngle() + angle;
}
QPointF CMDCalculation::rotateAtOffset(ICalcData *data, const QPointF& center, qreal angle)
{
    QPolygonF ps(data->data()->rect());
    ps = data->matrix()->map(ps);
	QPointF rectCenter = (ps[0] + ps[2]) / 2;

    return rotatePos(rectCenter, center, angle) - rectCenter;
}
QPointF CMDCalculation::fixPointRate(qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
	QPointF p(0.5, 0.5);
	if (xOff == 0 && yOff == 0)                       //TopLeft
		p = QPointF(0, 0);
	else if ((xOff + wOff) == 0 && yOff == 0)        //TopRight
		p = QPointF(1, 0);
	else if (xOff == 0 && (yOff + hOff) == 0)       //BottomLeft
		p = QPointF(0, 1);
	else if ((xOff + wOff) == 0 && (yOff + hOff) == 0)//BottomRight
		p = QPointF(1, 1);

	return p;
}
qreal CMDCalculation::calcOrthoTan(const QRectF& data, int pos)
{
	if (pos == 0 || pos == 2 || pos == 5 || pos == 7)
	{
		qreal tan = data.height() / data.width();
		if (pos == 2 || pos == 5)
			tan = -tan;
		return tan;
	}

	return 0;
}
QRectF CMDCalculation::offsetRect(const QRectF& rect, qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
	return rect.adjusted(xOff, yOff, xOff + wOff, yOff + hOff);
}
//////////////////////////////////////////////////////////////////////////
QPointF CMDCalculation::shearPoint(const QPointF& point, qreal shear)
{
	if (shear == 0)
		return point;

	m_transform.reset();
	m_transform.shear(shear, 0);
	return m_transform.map(point);
}
QPointF CMDCalculation::rotatePos(const QPointF& point, const QPointF& center, qreal angle)
{
    if (angle == 0)
        return point;

    QPointF p = point - center;
	m_transform.reset();
	m_transform.rotate(angle);
    return m_transform.map(p) + center;
}
QRectF CMDCalculation::scaleRect(const QPointF& center, qreal xScale, qreal yScale, QRectF& rect)
{
	//无缩放，返回原值
	if (xScale == 1 && yScale == 1)
		return rect;

    QPointF centerPos(rect.x()+rect.width()*center.x(), rect.y()+rect.height()*center.y());

	rect.setWidth(rect.width()*xScale);
	rect.setHeight(rect.height()*yScale);
    rect.moveTo(centerPos.x() + (rect.x() - centerPos.x())*xScale,
                centerPos.y() + (rect.y() - centerPos.y())*yScale);

	return rect;
}
void CMDCalculation::flipRectPos(QPolygonF& poly, bool isFlipX, bool isFlipY)
{
    Q_ASSERT(poly.count() == 5);
	
	QPointF change;
	if (isFlipX)
	{
		change = poly[0];
		poly[0] = poly[1];
		poly[1] = change;
		change = poly[3];
		poly[3] = poly[2];
		poly[2] = change;
        poly[4] = poly[0];
	}
	if (isFlipY)
	{
		change = poly[0];
		poly[0] = poly[3];
		poly[3] = change;
		change = poly[1];
		poly[1] = poly[2];
		poly[2] = change;
        poly[4] = poly[0];
	}
}
void CMDCalculation::offset(const QPointF& begin, const QPointF& end, int pos,
	qreal& xOff, qreal& yOff, qreal& wOff, qreal& hOff, qreal orthoTan)
{
	xOff = 0;
	yOff = 0;
	wOff = 0;
	hOff = 0;
	qreal xOffset = end.x() - begin.x();
	qreal yOffset = end.y() - begin.y();

	if (orthoTan != 0)
		yOffset = xOffset * orthoTan;

	switch (pos)
	{
	case 0:
		xOff = xOffset;
		yOff = yOffset;
		wOff = -xOff;
		hOff = -yOff;
		break;
	case 1:
		yOff = yOffset;
		hOff = -yOffset;
		break;
	case 2:
		yOff = yOffset;
		wOff = xOffset;
		hOff = -yOffset;
		break;
	case 3:
		xOff = xOffset;
		wOff = -xOffset;
		break;
	case 4:
		wOff = xOffset;
		break;
	case 5:
		xOff = xOffset;
		wOff = -xOffset;
		hOff = yOffset;
		break;
	case 6:
		hOff = yOffset;
		break;
	case 7:
		wOff = xOffset;
		hOff = yOffset;
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
QRectF CMDCalculation::limitRect(QRectF& value)
{
	const qreal min = 0.001;
	const qreal max = 10000;
	qreal x = value.x();
	qreal y = value.y();
	qreal w = value.width();
	qreal h = value.height();

	x = (x > -max)  ? x : -max;
	x = (x < max) ? x : max;
	y = (y > -max) ? y : -max;
	y = (y < max) ? y : max;
	w = (w > min) ? w : min;
	w = (w < max) ? w : max;
	h = (h > min) ? h : min;
	h = (h < max) ? h : max;

	value = QRectF(x, y, w, h);
	return value;
}
qreal CMDCalculation::limitAngle(qreal& value)
{
	//限制在[0,360)
	int v = (int)value;
	qreal deci = value - v;
	value = v % 360 + deci;
	if (value < 0)
		value += 360;

	return value;
}
void CMDCalculation::limitOffset(const QRectF& rect, qreal& xOff, qreal& yOff, qreal& wOff, qreal& hOff)
{
	const qreal min = 1;
	qreal w = rect.width();
	qreal h = rect.height();

	if (w - xOff < min)
		xOff = w - min;
	if (h - yOff < min)
		yOff = h - min;
	if (w + wOff < min)
		wOff = min - w;
	if (h + hOff < min)
		hOff = min - h;
}
void CMDCalculation::limitShear(qreal& value)
{
	//范围[-5,5]
	const int max = 5;
	const int min = -max;

	value = (value >= min) ? value : min;
	value = (value <= max) ? value : max;
}
void CMDCalculation::limitScale(qreal& value)
{
	//范围[0.1, 10]
	const qreal max = 10;
	const qreal min = 1 / max;
	value = (value >= min) ? value : min;
	value = (value <= max) ? value : max;

}
void CMDCalculation::limitScalePoint(QPointF& value)
{
	const int max = 10;
	const int min = -max;
	qreal x = value.x();
	qreal y = value.y();
	value.setX((x >= min) ? x : min);
	value.setX((x <= max) ? x : max);
	value.setY((y >= min) ? y : min);
	value.setY((y <= max) ? y : max);
}
//////////////////////////////////////////////////////////////////////////



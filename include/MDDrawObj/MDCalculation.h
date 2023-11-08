/*!
\author aGRw 2014.05.14
\section 矩阵计算类
*/
#ifndef MDCALCULATION_H
#define MDCALCULATION_H

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QPainterPath>
#include <QTransform>

#include "IDrawData.h"

const qreal PI = 3.1415926535898;

class MDDRAWOBJSHARED_EXPORT CMDCalculation
{
public:
    //!获取矩阵逆转换后的坐标
    static QPointF invertPos(const QTransform& matrix, const QPointF&  point);
    //!生成矩阵
    static void calcMatrix(const QRectF& data, IDataVector* dataV, QTransform& matrix, QPointF& rotatePointPos);
    //!计算特定比例点坐标 fixRate 特定点比例系数 isBound 是否是矩形边框特定点
    static QPointF calcFixPoint(const QRectF& data, IDataVector* dataV, const QPointF& fixRate, bool isBound = false);
    //!计算矩阵转换后的点
    static QPointF calcMatrixPoint(const QRectF& data, IDataVector* dataV, const QPointF& point);
    //!根据控件形状获取其矩阵旋转角度，倾斜和实际大小。边框移动调用此函数
    static void boundMoveData(const QRectF& data, IDataVector* dataV, qreal wOff, qreal hOff, qreal& angle, qreal& shear, QRectF& rect);
    //!根据实际矩阵获取旋转角度，倾斜和实际大小。解组时调用此函数
    static void matrixData(const QRectF& data, IDataVector* dataV, const QTransform& matrix, QRectF& rect);
    //!获取旋转点，比例系数
    static QPointF rotatePoint(const QTransform& matrix, const QPointF& point, const QRectF& rect);
    //!获取倾斜值
    static qreal shear(ICalcData* data, IDataVector* dataV, const QPointF& point, int pos);
    //!获取边框移动后的偏移量
    static void frameOffset(ICalcData* data, const QPointF& point, int pos,
        qreal& xOff, qreal& yOff, qreal& wOff, qreal& hOff, qreal orthoTan);
    //!获取旋转角
    static qreal rotateAngle(ICalcData* data, const QPointF& point, const QPointF& center);
    //!获取成组旋转的偏移
    static QPointF rotateAtOffset(ICalcData* data, const QPointF& center, qreal angle);
    static QPointF fixPointRate(qreal xOff, qreal yOff, qreal wOff, qreal hOff);
    //!正交模式下，获取Rect的宽高比例值
    static qreal calcOrthoTan(const QRectF& data, int pos);
    //!获取移动后的Rect
    static QRectF offsetRect(const QRectF& rect, qreal xOff, qreal yOff, qreal wOff, qreal hOff);
    //!获取边框移动后的偏移量
    static void offset(const QPointF& begin, const QPointF& end, int pos,
        qreal& xOff, qreal& yOff, qreal& wOff, qreal& hOff, qreal orthoTan);
private:
    //!获取倾斜后的点位置
    static QPointF shearPoint(const QPointF& point, qreal shear);
    //!获取旋转后的坐标
    static QPointF rotatePos(const QPointF& point, const QPointF& center, qreal angle);
    //!获取缩放后的Rect
    static QRectF scaleRect(const QPointF& center, qreal xScale, qreal yScale, QRectF& rect);
    //!获取反转后的矩形数据
    static void flipRectPos(QPolygonF& poly, bool isFlipX, bool isFlipY);

    static QTransform m_transform;
    static QTransform m_funTransform;
/*!
\section limit
*/
public:
    static QRectF limitRect(QRectF& value);
    static qreal limitAngle(qreal& value);
    static void limitOffset(const QRectF& rect, qreal& xOff, qreal& yOff, qreal& wOff, qreal& hOff);
    static void limitShear(qreal& value);
    static void limitScale(qreal& value);
    static void limitScalePoint(QPointF& value);
};

#endif // MDCALCULATION_H


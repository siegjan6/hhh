/*!
author Y2hvdWps 2016.11.09
section QJsonValue和QVariant之间互相转换
*/
#ifndef MDJSONCONVERT_H
#define MDJSONCONVERT_H

#include "Utility.h"
#include <QJsonObject>
#include <QVariant>
#include <QGradient>

class UTILITYSHARED_EXPORT CMDJsonConvert
{
protected:
    CMDJsonConvert();

public:
    //! QRect
    static QJsonValue toJson(const QRect& rect);
    //! 转换失败返回空的QRect,可以使用isNull()函数做判断,其他类型同理.
    static QRect toQRect(const QJsonValue& jsonValue);
    //! QRectF
    static QJsonValue toJson(const QRectF& rectF);
    static QRectF toQRectF(const QJsonValue& jsonValue);
    //! QPoint
    static QJsonValue toJson(const QPoint& point);
    static QPoint toQPoint(const QJsonValue& jsonValue);
    //! QPointF
    static QJsonValue toJson(const QPointF& pointF);
    static QPointF toQPointF(const QJsonValue& jsonValue);
    //! QFont
    static QJsonValue toJson(const QFont& font);
    static QFont toQFont(const QJsonValue& jsonValue);
    //! QPen
    static QJsonValue toJson(const QPen& pen);
    static QPen toQPen(const QJsonValue& jsonValue);
    //! QBrush
    static QJsonValue toJson(const QBrush& brush);
    static QBrush toQBrush(const QJsonValue& jsonValue);

protected:
    //! QGradient
    static QJsonValue toJson(const QGradient& gradient);
    static QBrush gradientBrush(const QJsonValue& jsonValue);
    static void setGradientProperty(QGradient& gradient, QJsonObject& obj, QGradientStops& stops);
};

#endif // MDJSONCONVERT_H

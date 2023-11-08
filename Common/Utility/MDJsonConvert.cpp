#include "MDJsonConvert.h"
#include <QJsonArray>
#include <QRect>
#include <QFont>
#include <QPen>
#include <QBrush>


CMDJsonConvert::CMDJsonConvert()
{
}

QJsonValue CMDJsonConvert::toJson(const QRect &rect)
{
    QJsonObject json;
    json.insert("Left", rect.left());
    json.insert("Top", rect.top());
    json.insert("Right", rect.right());
    json.insert("Bottom", rect.bottom());
    return QJsonValue(json);
}
QRect CMDJsonConvert::toQRect(const QJsonValue &jsonValue)
{
    if( !jsonValue.isObject() )
        return QRect();
    QJsonObject obj = jsonValue.toObject();

    QRect rect;
    QJsonValue tmpValue = obj.value("Left");
    if( tmpValue.type() == QJsonValue::Undefined )
        return QRect();
    rect.setLeft(tmpValue.toInt());
    rect.setTop(obj.value("Top").toInt());
    rect.setRight(obj.value("Right").toInt());
    rect.setBottom(obj.value("Bottom").toInt());

    return rect;
}

QJsonValue CMDJsonConvert::toJson(const QRectF &rectF)
{
    QJsonObject json;
    json.insert("Left", rectF.left());
    json.insert("Top", rectF.top());
    json.insert("Right", rectF.right());
    json.insert("Bottom", rectF.bottom());
    return QJsonValue(json);
}

QRectF CMDJsonConvert::toQRectF(const QJsonValue &jsonValue)
{
    if( !jsonValue.isObject() )
        return QRectF();
    QJsonObject obj = jsonValue.toObject();

    QRectF rectF;
    QJsonValue tmpValue = obj.value("Left");
    if( tmpValue.type() == QJsonValue::Undefined )
        return QRectF();
    rectF.setLeft(tmpValue.toDouble());
    rectF.setTop(obj.value("Top").toDouble());
    rectF.setRight(obj.value("Right").toDouble());
    rectF.setBottom(obj.value("Bottom").toDouble());

    return rectF;
}

QJsonValue CMDJsonConvert::toJson(const QPoint &point)
{
    QJsonObject json;
    json.insert("X", point.x());
    json.insert("Y", point.y());
    return QJsonValue(json);
}

QPoint CMDJsonConvert::toQPoint(const QJsonValue &jsonValue)
{
    if( !jsonValue.isObject() )
        return QPoint();
    QJsonObject obj = jsonValue.toObject();

    QPoint point;
    QJsonValue tmpValue = obj.value("X");
    if( tmpValue.type() == QJsonValue::Undefined )
        return QPoint();
    point.setX(tmpValue.toInt());
    point.setY(obj.value("Y").toInt());

    return point;
}

QJsonValue CMDJsonConvert::toJson(const QPointF &pointF)
{
    QJsonObject json;
    json.insert("X", pointF.x());
    json.insert("Y", pointF.y());
    return QJsonValue(json);
}

QPointF CMDJsonConvert::toQPointF(const QJsonValue &jsonValue)
{
    if( !jsonValue.isObject() )
        return QPointF();
    QJsonObject obj = jsonValue.toObject();

    QPointF pointF;
    QJsonValue tmpValue = obj.value("X");
    if( tmpValue.type() == QJsonValue::Undefined )
        return QPointF();
    pointF.setX(tmpValue.toDouble());
    pointF.setY(obj.value("Y").toDouble());

    return pointF;
}

QJsonValue CMDJsonConvert::toJson(const QFont &font)
{
    QJsonObject json;
    json.insert("Family", font.family());
    json.insert("PointSize", font.pointSize());
    json.insert("Weight", font.weight());
    json.insert("Italic", font.italic());

    json.insert("OverLine", font.overline());
    json.insert("UnderLine", font.underline());
    json.insert("StrikeOut", font.strikeOut());
    json.insert("PixelSize", font.pixelSize());
    json.insert("StyleStrategy", font.styleStrategy());
    json.insert("StyleHint", font.styleHint());
    json.insert("Stretch", font.stretch());
    json.insert("Capitalization", font.capitalization());
    json.insert("HintingPreference", font.hintingPreference());
    json.insert("Kerning", font.kerning());
    json.insert("WordSpacing", font.wordSpacing());
    json.insert("Style", font.style());

    return QJsonValue(json);
}

QFont CMDJsonConvert::toQFont(const QJsonValue &jsonValue)
{
    if( !jsonValue.isObject() )
        return QFont();
    QJsonObject obj = jsonValue.toObject();


    QJsonValue tmpValue = obj.value("Family");
    if( tmpValue.type() == QJsonValue::Undefined )
        return QFont();

    QString familyname = tmpValue.toString();
    int ptsize = obj.value("PointSize").toInt();
    int weight = obj.value("Weight").toInt();
    bool italic = obj.value("Italic").toBool();
    QFont font(familyname, ptsize, weight, italic);

    font.setOverline(obj.value("OverLine").toBool());
    font.setUnderline(obj.value("UnderLine").toBool());
    font.setStrikeOut(obj.value("StrikeOut").toBool());
    qint32 pixelsize = obj.value("PixelSize").toInt();
    if( pixelsize > 0 )
        font.setPixelSize(pixelsize);
    font.setStyleStrategy((QFont::StyleStrategy)obj.value("StyleStrategy").toInt());
    font.setStyleHint((QFont::StyleHint)obj.value("StyleHint").toInt(), font.styleStrategy());
    font.setStretch(obj.value("Stretch").toInt());
    font.setCapitalization((QFont::Capitalization)obj.value("Capitalization").toInt());
    font.setHintingPreference((QFont::HintingPreference)obj.value("HintingPreference").toInt());
    font.setKerning(obj.value("Kerning").toBool());
    font.setWordSpacing(obj.value("WordSpacing").toDouble());
    font.setStyle((QFont::Style)obj.value("Style").toInt());

    return font;
}

QJsonValue CMDJsonConvert::toJson(const QPen &pen)
{
    QJsonObject json;
    json.insert("Color", (double)pen.color().rgba());
    json.insert("WidthF", pen.widthF());
    json.insert("PenStyle", pen.style());
    json.insert("CapStyle", pen.capStyle());
    json.insert("JoinStyle", pen.joinStyle());
    json.insert("Cosmetic", pen.isCosmetic());
    json.insert("MiterLimit", pen.miterLimit());
    json.insert("DashOffset", double(pen.dashOffset()));
    //! dashpattern
    if( pen.style() == Qt::CustomDashLine )
    {
        QJsonArray dashArray;
        const QVector<qreal>& pattern = pen.dashPattern();
        for (int i = 0; i < pattern.size(); ++i)
            dashArray.append(double(pattern.at(i)));
        json.insert("DashArray", dashArray);
    }

    return QJsonValue(json);
}

QPen CMDJsonConvert::toQPen(const QJsonValue &jsonValue)
{
    if( !jsonValue.isObject() )
        return QPen();
    QJsonObject obj = jsonValue.toObject();
    QJsonValue tmpValue = obj.value("PenStyle");
    if( tmpValue.type() == QJsonValue::Undefined )
        return QPen();

    QPen pen((Qt::PenStyle)tmpValue.toInt());
    pen.setCapStyle((Qt::PenCapStyle)obj.value("CapStyle").toInt());
    pen.setJoinStyle((Qt::PenJoinStyle)obj.value("JoinStyle").toInt());
    pen.setWidthF(obj.value("WidthF").toDouble());
    pen.setColor(obj.value("Color").toDouble());
    pen.setCosmetic(obj.value("Cosmetic").toBool());
    pen.setMiterLimit(obj.value("MiterLimit").toDouble());
    pen.setDashOffset(obj.value("DashOffset").toDouble());

    //! dashpattern
    QVector<qreal> pattern;
    const QJsonArray& dashArray = obj.value("DashArray").toArray(QJsonArray());
    for (int i = 0; i < dashArray.size(); ++i) {
        pattern.append(dashArray.at(i).toDouble());
    }
    if( pattern.size() > 0 )
        pen.setDashPattern(pattern);


    return pen;
}

QJsonValue CMDJsonConvert::toJson(const QBrush &brush)
{
    QJsonObject json;
    json.insert("BrushStyle", brush.style());
    json.insert("Color", (double)brush.color().rgba());
    //! Gradient
    const QGradient* gradient = brush.gradient();
    if( gradient && gradient->type() != QGradient::NoGradient )
        json.insert("Gradient", toJson(*gradient));
    //! Transform
    QJsonArray transArray;
    const QTransform& trans = brush.transform();
    transArray.append(double(trans.m11()));
    transArray.append(double(trans.m12()));
    transArray.append(double(trans.m13()));
    transArray.append(double(trans.m21()));
    transArray.append(double(trans.m22()));
    transArray.append(double(trans.m23()));
    transArray.append(double(trans.m31()));
    transArray.append(double(trans.m32()));
    transArray.append(double(trans.m33()));
    json.insert("Transform", transArray);

    return QJsonValue(json);
}



QBrush CMDJsonConvert::toQBrush(const QJsonValue &jsonValue)
{
    if( !jsonValue.isObject() )
        return QBrush();
    QJsonObject obj = jsonValue.toObject();
    QJsonValue tmpValue = obj.value("BrushStyle");
    if( tmpValue.type() == QJsonValue::Undefined )
        return QBrush();

    QBrush brush;
    Qt::BrushStyle style = (Qt::BrushStyle)tmpValue.toInt();

    if (style == Qt::TexturePattern) {
    //! 暂时不处理材质画刷
        return QBrush();
    } else if (style == Qt::LinearGradientPattern
               || style == Qt::RadialGradientPattern
               || style == Qt::ConicalGradientPattern)
    {
        //! Gradient
        QJsonValue gradientValue = obj.value("Gradient");
        if( gradientValue.type() != QJsonValue::Undefined )
            brush = gradientBrush(gradientValue);
    }
    else{
        //! Color
        QBrush stylebrush(QColor(QRgb(obj.value("Color").toDouble())), style);
        brush = stylebrush;
    }

    //! transform
    const QJsonArray& transArray = obj.value("Transform").toArray();
    brush.setTransform(QTransform(transArray[0].toDouble(),
                                  transArray[1].toDouble(),
                                  transArray[2].toDouble(),
                                  transArray[3].toDouble(),
                                  transArray[4].toDouble(),
                                  transArray[5].toDouble(),
                                  transArray[6].toDouble(),
                                  transArray[7].toDouble(),
                                  transArray[8].toDouble()));


    return brush;
}

QJsonValue CMDJsonConvert::toJson(const QGradient &gradient)
{
    QJsonObject json;

    json.insert("Type", gradient.type());
    json.insert("Spread", gradient.spread());
    json.insert("CoordinateMode", gradient.coordinateMode());
    json.insert("InterpolationMode", gradient.interpolationMode());

    if (gradient.type() == QGradient::LinearGradient) {
        json.insert("Start", toJson(static_cast<const QLinearGradient&>(gradient).start()));
        json.insert("FinalStop", toJson(static_cast<const QLinearGradient&>(gradient).finalStop()));
    } else if (gradient.type() == QGradient::RadialGradient) {
        json.insert("Center", toJson(static_cast<const QRadialGradient&>(gradient).center()));
        json.insert("FocalPoint", toJson(static_cast<const QRadialGradient&>(gradient).focalPoint()));
        json.insert("Radius", (double) static_cast<const QRadialGradient&>(gradient).radius());
    } else { // type == Conical
        json.insert("Center", toJson(static_cast<const QConicalGradient&>(gradient).center()));
        json.insert("Angle", (double) static_cast<const QConicalGradient&>(gradient).angle());
    }
    //! Stops
    QJsonArray stopArray;
    const QGradientStops& stops = gradient.stops();
    for (int i = 0; i < stops.size(); ++i) {
        const QGradientStop &stop = stops.at(i);
        stopArray.append(double(stop.first));
        stopArray.append(double(stop.second.rgba()));
    }
    json.insert("StopArray", stopArray);

    return QJsonValue(json);
}

void CMDJsonConvert::setGradientProperty(QGradient& gradient, QJsonObject& obj, QGradientStops& stops)
{
    gradient.setSpread((QGradient::Spread)obj.value("Spread").toInt());
    gradient.setCoordinateMode((QGradient::CoordinateMode)obj.value("CoordinateMode").toInt());
    gradient.setInterpolationMode((QGradient::InterpolationMode)obj.value("InterpolationMode").toInt());
    gradient.setStops(stops);
}

QBrush CMDJsonConvert::gradientBrush(const QJsonValue& jsonValue)
{
    if( !jsonValue.isObject() )
        return QBrush();
    QJsonObject obj = jsonValue.toObject();

    //! stops
    QJsonArray stopArray = obj.value("StopArray").toArray();
    QGradientStops stops;
    for (int i = 0; i < stopArray.size(); i+=2) {
        stops.append( QPair<qreal, QColor>(stopArray[i].toDouble(), QColor(QRgb(stopArray[i+1].toDouble()))) );
    }

    QGradient::Type type = (QGradient::Type)obj.value("Type").toInt();
    if (type == QGradient::LinearGradient) {
        QPointF p1, p2;
        p1 = toQPointF(obj.value("Start"));
        p2 = toQPointF(obj.value("FinalStop"));
        QLinearGradient lg(p1, p2);
        setGradientProperty(lg, obj, stops);
        return QBrush (lg);
    }else if (type == QGradient::RadialGradient) {
        QPointF center, focal;
        double radius;
        center = toQPointF(obj.value("Center"));
        focal = toQPointF(obj.value("FocalPoint"));
        radius = obj.value("Radius").toDouble();
        QRadialGradient rg(center, radius, focal);
        setGradientProperty(rg, obj, stops);
        return QBrush (rg);
    } else {
        QPointF center;
        double angle;
        center = toQPointF(obj.value("Center"));
        angle = obj.value("Angle").toDouble();
        QConicalGradient cg(center, angle);
        setGradientProperty(cg, obj, stops);
        return QBrush (cg);
    }

    return QBrush();
}



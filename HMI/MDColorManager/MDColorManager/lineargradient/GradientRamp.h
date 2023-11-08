#ifndef MYLINEARCHILD_H
#define MYLINEARCHILD_H
#include <qcolor.h>
#include <QLinearGradient>
#include <qrect.h>
#include <qpainter.h>
#include <qbitmap.h>
#include <qpainterpath.h>
#include "define.h"
class GradientRamp:public QSize
{
public:
    GradientRamp();
    GradientRamp(const QColor& clr,const qreal& scale);
    GradientRamp(const QGradientStop& colorfloat);
    virtual ~GradientRamp();
    void Draw(QPainter *p, const QRectF &rt);
    void setSelected(bool bSelect);
    bool selected() const;
    void setColor(const QColor& clr);
    QColor color();
    void setScale(qreal f);
    qreal scale() const;
    QGradientStop m_gradientStop;
private:
    bool m_select;
};

#endif // MYLINEARCHILD_H

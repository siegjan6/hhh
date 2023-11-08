#ifndef GRADIENTRAMPLUMP_H
#define GRADIENTRAMPLUMP_H

#include <QWidget>
#include <qlist.h>
#include <QMouseEvent>
#include "lineargradient/GradientRamp.h"
#include "define.h"

class GradientRampLump : public QWidget
{
    Q_OBJECT
public:
    explicit GradientRampLump(QWidget *parent = 0);

//数据操作
public:
    QList<GradientRamp> m_datas;
    void setGradientStops(QGradientStops gradientStops);
    QGradientStops gradientStops();

    void append(const QColor& clr,const qreal& scale);
    void append(GradientRamp &ramp);
    void remove(GradientRamp &g);
    void remove(const QColor& clr,const qreal& scale);
    void setfullSelecte(bool bSelect);

    //绘图
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *event) ;
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);

    int selectIndex() const;
    void setSelectIndex(int index);
    GradientRamp* current();
    bool containsChild(const GradientRamp& g,const QPointF &p);
    int contains(const QPointF& p);

    QLinearGradient* lineargradient();
    QLinearGradient* _linearBr;

    QRectF clientRect();
signals:
    void colorChange(const QColor& clr);
    void gradientStopsChange(const QGradientStops& stops);
public slots:
    void setCurrentColor(const QColor& clr);
};

#endif // GRADIENTRAMPLUMP_H

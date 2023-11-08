#ifndef COLOREXAMPLE_H
#define COLOREXAMPLE_H

#include <QWidget>
#include "MDPenManager.h"

class ColorExample : public QWidget
{
    Q_OBJECT
public:
    explicit ColorExample(QWidget *parent = 0);
    ~ColorExample();
    virtual void paintEvent(QPaintEvent *);
    bool m_isPen;
public slots:
    void setColor(const QColor &clr);
    void setAngle(int angle);
    void setStops(QGradientStops stops);
    void setType( CMDBrushManager::BrushType type);
    void setCapStyle(Qt::PenCapStyle capStyle);
    void setJoinStyle(Qt::PenJoinStyle joinStyle);
    void setPenStyle(Qt::PenStyle penStyle);
    void setWidth(int width);

private:
    CMDPenManager* m_penMgr;
    CMDBrushManager* m_brushManager;
};

#endif // COLOREXAMPLE_H

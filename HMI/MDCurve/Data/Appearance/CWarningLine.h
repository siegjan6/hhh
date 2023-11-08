/*!
\author eHVr

\section 功能

警戒线类,基本属性:
    \a value
    \a visible
    \a yPoint
    \a startPoint
    \a endPoint
*/
#ifndef CWARNINGLINE_H
#define CWARNINGLINE_H

#include "CBisectrixLine.h"

class CWarningLine : public CBisectrixLine
{
public:
    CWarningLine();
    virtual ~CWarningLine();
/*!
\section 基本属性
*/
public:
    double value() const;
    void   setValue(double value);

    bool visible() const;
    void setVisible(bool visible);

    //! 绘制点的Y坐标
    double yPoint() const;
    void setYPoint(double yPoint);

    void setLinePoint(const QPointF &start,const QPointF &end);
private:
    double     m_value;       //! 警戒值
    bool       m_visible;     //! 是否可见
    double     m_yPoint;      //! 坐标
    QPointF    m_startPoint;  //! 线起点
    QPointF    m_endPoint;    //! 线终点

public:
    void draw(QPainter *p);
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // CWARNINGLINE_H

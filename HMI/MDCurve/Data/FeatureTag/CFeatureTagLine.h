/*!
\author eHVr
\section 功能
    标签线
*/
#ifndef CFEATURETAGLINE_H
#define CFEATURETAGLINE_H

#include "CBisectrixLine.h"

class CFeatureTagLine : public CBisectrixLine
{
public:
    CFeatureTagLine();
    virtual ~CFeatureTagLine();

    bool visible() const;
    void setVisible(bool visible);

    void appendLinePoints(const QPointF &p1,const QPointF &p2);
    void clearLinePoints();

    void draw(QPainter *p);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    bool m_visible;       //! 是否可见

    QVector<QPointF> m_linePoints;
};

#endif // CFEATURETAGLINE_H

/*!
\author aGRw
\section 工具类
*/
#ifndef TOOL_H
#define TOOL_H

#include "QPointF"
#include "QRect"

class CTool
{
public:
    static QPointF gridPointF(const QPointF& point);
    static QRectF gridRect(const QRect& rect);

};

#endif // TOOL_H



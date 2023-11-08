/*!

\author bWVpeGw=

\section 功能

管理【线条计算模块接口类.
    提供公共接口给外部调用】。

*/

#ifndef CLINEREGION_H
#define CLINEREGION_H
#include<qpoint.h>
#include <qpainter.h>

class CLineRegion
{
public:
    CLineRegion();
    ~CLineRegion();
    bool contains(const QPoint &point);
    void setRegion(QPainterPath *path, qreal width);
private:
    QRegion m_rgn;
};


#endif // CLINEREGION_H


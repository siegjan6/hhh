/*!

\author bWVpeGw=

\section 功能

管理【贝塞尔曲线控件实现类，继承自CMDDrawNode基类.
    用来绘制贝塞尔曲线】。

*/

#ifndef MDDRAWBEZIER_H
#define MDDRAWBEZIER_H

#include "MDDrawNode.h"

class MDDRAWNODESHARED_EXPORT CMDDrawBezier : public CMDDrawNode
{
public:
    CMDDrawBezier();
    virtual ~CMDDrawBezier();

    DrawType type();
    QString className(){return "CMDDrawBezier";}
    CMDDrawObj *createSelf();

    void drawPath(QPainter *p);
    void generateNodePath(QPainterPath* path);

    void drawLine(QPainter* p);
    static QList<QStringList> expNames();
};

#endif // MDDRAWBEZIER_H

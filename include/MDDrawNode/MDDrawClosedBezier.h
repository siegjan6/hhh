/*!

\author bWVpeGw=

\section 功能

管理【闭合贝塞尔曲线控件实现类，继承自CMDDrawNode基类.
    用来绘制闭合贝塞尔曲线】。

*/


#ifndef MDDRAWCLOSEDBEZIER_H
#define MDDRAWCLOSEDBEZIER_H

#include "MDDrawNode.h"

class MDDRAWNODESHARED_EXPORT CMDDrawClosedBezier : public CMDDrawNode
{
public:
    CMDDrawClosedBezier();
    virtual ~CMDDrawClosedBezier();

    DrawType type();
    QString className(){return "CMDDrawClosedBezier";}
    CMDDrawObj *createSelf();

    void drawPath(QPainter *p);
    void generateNodePath(QPainterPath* path) ;

    bool isVisible(const QPointF &point);

    void drawLine(QPainter* p);

    static QList<QStringList> expNames();
    bool canCombine(){return true;}
};

#endif // MDDRAWCLOSEDBEZIER_H

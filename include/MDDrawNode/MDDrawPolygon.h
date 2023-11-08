/*!

\author bWVpeGw=

\section 功能

管理【多边形控件实现类，继承自CMDDrawNode基类.
    用来绘制多边形】。

*/

#ifndef MDDRAWPOLYGON_H
#define MDDRAWPOLYGON_H

#include "MDDrawNode.h"

class MDDRAWNODESHARED_EXPORT CMDDrawPolygon: public CMDDrawNode
{
public:
    CMDDrawPolygon();
    virtual ~CMDDrawPolygon();

    DrawType type() ;
    QString className(){return "CMDDrawPolygon";}
    CMDDrawObj *createSelf() ;

    void drawPath(QPainter* p) ;
    void generateNodePath(QPainterPath* path) ;
    void onDelete(int pos) ;

    void drawLine(QPainter* p);
    bool isVisible(const QPointF &point);

    static QList<QStringList> expNames();
    bool canCombine(){return true;}
};

#endif // MDDRAWPOLYGON_H

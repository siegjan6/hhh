/*!

\author bWVpeGw=

\section 功能

管理【折线控件实现类，继承自CMDDrawNode基类.
    用来绘制折线】。

*/


#ifndef MDDRAWFOLDLINE_H
#define MDDRAWFOLDLINE_H

#include "MDDrawNode.h"

class MDDRAWNODESHARED_EXPORT CMDDrawFoldLine: public CMDDrawNode
{
public:
    CMDDrawFoldLine();
    virtual ~CMDDrawFoldLine();

    DrawType type();
    QString className(){return "CMDDrawFoldLine";}
    CMDDrawObj *createSelf();

    void drawPath(QPainter* p);
    void generateNodePath(QPainterPath* path);

    void drawLine(QPainter* p);
};

#endif // MDDRAWFOLDLINE_H

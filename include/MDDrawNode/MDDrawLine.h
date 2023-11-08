/*!

\author bWVpeGw=

\section 功能

管理【直线控件实现类，继承自CMDDrawNode基类.
    用来绘制直线】。

*/


#ifndef MDDRAWLINE_H
#define MDDRAWLINE_H

#include "MDDrawFoldLine.h"

class MDDRAWNODESHARED_EXPORT CMDDrawLine : public CMDDrawFoldLine
{
public:
    CMDDrawLine();
    virtual ~CMDDrawLine();

    DrawType type() ;
    QString className(){return "CMDDrawLine";}
    CMDDrawObj *createSelf();
    CMDPropertyDrawObj *createPropertyDrawObj();

    void drawPath(QPainter *p);
    void generateNodePath(QPainterPath* path);

    bool createFinish();

    void drawLine(QPainter* p);

    static QList<QStringList> expNames();
};

#endif // MDDRAWLINE_H

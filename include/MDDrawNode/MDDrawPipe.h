/*!

\author bWVpeGw=

\section 功能

管理【管道控件实现类，继承自CMDDrawNode基类.
    用来绘制管道】。

*/

#ifndef CMDDRAWPIPE_H
#define CMDDRAWPIPE_H

#include "MDDrawNode.h"

class MDDRAWNODESHARED_EXPORT CMDDrawPipe: public CMDDrawNode
{
public:
    CMDDrawPipe();
    virtual ~CMDDrawPipe();

    DrawType type();
    QString className(){return "CMDDrawPipe";}
    CMDDrawObj *createSelf();
    CMDPropertyDrawObj *createPropertyDrawObj();

    void drawPath(QPainter* p);
    void generateNodePath(QPainterPath* path);

    void drawLine(QPainter* p);
    static QList<QStringList> expNames();
};

#endif // CMDDRAWPIPE_H

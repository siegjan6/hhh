/*!
author Y2hvdWps 2016.11.01
section RecipeGridView的风格类
详细说明
    对绘制UI的部分细节特殊处理
*/
#ifndef RECIPEGRIDSTYLE_H
#define RECIPEGRIDSTYLE_H

#include <QProxyStyle>

class CRecipeGridStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit CRecipeGridStyle(QStyle* style=0);
    void drawPrimitive(PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0) const;

signals:

public slots:

};

#endif // RECIPEGRIDSTYLE_H
